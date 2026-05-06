// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// LayerRaytracer — closed-form Snell raytracer through 1-D layered SVPs
// -----------------------------------------------------------------------------
// Traces per-beam ray polylines through a SoundVelocityProfile and returns
// world-frame XYZ at user-requested one-way travel times (or sample numbers).
//
// The SoundVelocityProfile depths are in **absolute world coordinates**
// (e.g. metres below the sea surface). Each ray is launched at the absolute
// depth carried by the TX pose at TX time (``tx_poses[0].z``) using the
// SVP-interpolated sound speed at that depth as the Snell invariant
// ``p = sin(theta) / c(launch_depth)``. The SVP must cover the launch depth
// (otherwise the beam returns NaN). No internal rebasing of the SVP is
// performed; if you want to use a different surface sound speed you must
// pre-blend that into the SVP yourself before construction.
//
// Per-layer formulas for ray parameter p = sin(theta_i) / c_i (Snell invariant):
//
//   constant-gradient layer (c(z) = c_i + g_i * (z - z_i), g_i != 0):
//     dx_i = (cos(theta_i) - cos(theta_{i+1})) / (p * g_i)
//     dt_i = (1/g_i) * ln( (c_{i+1}/c_i) * (1 + cos(theta_i))
//                                          / (1 + cos(theta_{i+1})) )
//
//   iso-velocity layer (g_i ≈ 0):
//     dx_i = (z_{i+1} - z_cur) * tan(theta_i)
//     dt_i = (z_{i+1} - z_cur) / (c_i * cos(theta_i))
//
// Knot anchoring is exact: at each requested travel time we close-form
// integrate the partial layer (solving for the exit sound speed via a
// quadratic in c), so floating-point drift never accumulates across knots.
//
// World-frame transform: launch directions are rotated by the per-knot TX
// pose; horizontal displacement (dx) is applied in the rotated direction;
// position translation between TX and RX poses is linearly interpolated
// across the round-trip and added to the world-frame point.
// -----------------------------------------------------------------------------

#pragma once

#include ".docstrings/layerraytracer.doc.hpp"

#include "soundvelocityprofile.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>

#include <themachinethatgoesping/navigation/datastructures/geolocation.hpp>
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Closed-form Snell raytracer through a 1-D layered SVP.
 *
 * Output is a [K+1, n_beams, 3] world-frame xyz tensor that plugs directly
 * into BeamSampleGeometryPiecewise::from_layer_xyz.
 */
class LayerRaytracer
{
  private:
    SoundVelocityProfile _svp;

  public:
    LayerRaytracer() = default;
    explicit LayerRaytracer(SoundVelocityProfile svp)
        : _svp(std::move(svp))
    {
    }

    bool operator==(const LayerRaytracer& other) const { return _svp == other._svp; }

    const SoundVelocityProfile& get_svp() const { return _svp; }
    void                        set_svp(SoundVelocityProfile svp) { _svp = std::move(svp); }

    /**
     * @brief Trace beams to the given one-way travel times.
     *
     * @param launch_dirs   [n_beams, 3] unit vectors in vehicle/transducer frame
     *                      (forward, starboard, down). Need NOT be normalized;
     *                      will be normalized internally. dz must be > 0
     *                      (downward), otherwise the beam gets NaN output.
     * @param knot_times    [K+1] one-way travel times since TX (seconds),
     *                      monotonically increasing, knot_times[0] >= 0.
     * @param tx_poses      length K+1, world-frame TX pose at each knot
     *                      (yaw/pitch/roll + xyz). xyz is the TX origin used
     *                      for that knot's leg.
     * @param rx_poses      length K+1, world-frame RX pose at each knot. Used
     *                      only for translation interpolation across the
     *                      round-trip. May be equal to tx_poses for the
     *                      single-pose case.
     * @param mp_cores      number of OpenMP threads; 1 = serial.
     * @return [K+1, n_beams, 3] world-frame xyz (positive z down), in the
     *         same units as the poses' xyz (typically metres).
     */
    xt::xtensor<float, 3> trace_at_times(
        const xt::xtensor<float, 2>&                                 launch_dirs,
        const xt::xtensor<float, 1>&                                 knot_times,
        const std::vector<navigation::datastructures::Geolocation>&  tx_poses,
        const std::vector<navigation::datastructures::Geolocation>&  rx_poses,
        int                                                          mp_cores = 1) const
    {
        if (launch_dirs.shape(1) != 3)
            throw std::runtime_error("LayerRaytracer.trace_at_times: launch_dirs must be [n_beams, 3]");

        const size_t n_beams = launch_dirs.shape(0);
        const size_t K1      = knot_times.size();
        if (K1 < 1)
            throw std::runtime_error("LayerRaytracer.trace_at_times: need at least 1 knot time");
        if (tx_poses.size() != K1 || rx_poses.size() != K1)
            throw std::runtime_error(fmt::format(
                "LayerRaytracer.trace_at_times: tx_poses ({}) and rx_poses ({}) must match knot_times ({})",
                tx_poses.size(),
                rx_poses.size(),
                K1));
        for (size_t k = 1; k < K1; ++k)
            if (!(knot_times.unchecked(k) >= knot_times.unchecked(k - 1)))
                throw std::runtime_error("LayerRaytracer.trace_at_times: knot_times must be monotone");

        if (_svp.get_number_of_layers() == 0)
            throw std::runtime_error("LayerRaytracer.trace_at_times: SVP not initialized");

        auto out = xt::xtensor<float, 3>::from_shape({ K1, n_beams, size_t(3) });

        // Precompute per-knot TX rotation quaternions (vehicle->world)
        std::vector<Eigen::Quaternion<float>> tx_q(K1);
        for (size_t k = 0; k < K1; ++k)
            tx_q[k] = tools::rotationfunctions::quaternion_from_ypr<float>(
                tx_poses[k].yaw, tx_poses[k].pitch, tx_poses[k].roll, true);

        // Absolute launch depth (m) of every ray: the TX pose's z at TX time.
        // The SVP is in absolute world depth, so we start ray integration at
        // this depth in whichever layer contains it.
        const float  launch_depth = tx_poses[0].z;
        const auto&  svp_z        = _svp.get_depths_in_meters();
        const size_t L            = _svp.get_number_of_layers();
        const float  z_top        = svp_z.unchecked(0);
        const float  z_bot        = svp_z.unchecked(L);

        const int threads = std::max(1, mp_cores);

#pragma omp parallel for if (threads > 1) num_threads(threads) schedule(static)
        for (long bidx = 0; bidx < (long)n_beams; ++bidx)
        {
            const size_t b   = (size_t)bidx;
            float        dx0 = launch_dirs.unchecked(b, 0);
            float        dy0 = launch_dirs.unchecked(b, 1);
            float        dz0 = launch_dirs.unchecked(b, 2);
            const float  norm = std::sqrt(dx0 * dx0 + dy0 * dy0 + dz0 * dz0);
            if (!(norm > 0.f))
            {
                for (size_t k = 0; k < K1; ++k)
                {
                    out(k, b, 0) = std::nanf("");
                    out(k, b, 1) = std::nanf("");
                    out(k, b, 2) = std::nanf("");
                }
                continue;
            }
            dx0 /= norm;
            dy0 /= norm;
            dz0 /= norm;

            // SVP must cover the launch depth.
            if (!(launch_depth >= z_top) || !(launch_depth <= z_bot))
            {
                for (size_t k = 0; k < K1; ++k)
                {
                    out(k, b, 0) = std::nanf("");
                    out(k, b, 1) = std::nanf("");
                    out(k, b, 2) = std::nanf("");
                }
                continue;
            }

            // sin(theta) wrt vertical = sqrt(1 - dz^2)  (use horizontal magnitude)
            const float sin_theta0 = std::sqrt(std::max(0.f, 1.f - dz0 * dz0));
            const float c0         = _svp.get_sound_speed(launch_depth);
            const float p          = sin_theta0 / c0; // Snell invariant
            // unit horizontal direction in vehicle frame
            float hx_v = 0.f, hy_v = 0.f;
            if (sin_theta0 > 1e-12f)
            {
                hx_v = dx0 / sin_theta0;
                hy_v = dy0 / sin_theta0;
            }

            // Walk layers in absolute depth, accumulate (z, t, x_horizontal)
            // starting from (launch_depth, 0, 0).
            const auto& zs    = _svp.get_depths_in_meters();
            const auto& cs    = _svp.get_sound_speeds_in_meters_per_second();
            const auto& gs    = _svp.get_sound_speed_gradients_in_per_second();
            const auto& invg  = _svp.get_inverse_sound_speed_gradients_in_seconds();
            const auto& iso   = _svp.get_isovelocity_flags();

            // current state along ray (downward integration)
            double t_cur     = 0.0;
            double x_cur     = 0.0;       // horizontal range from launch
            double z_cur     = (double)launch_depth;
            double c_cur     = (double)c0;
            double cos_cur   = std::sqrt(std::max(0.0, 1.0 - (double)p * (double)p * c_cur * c_cur));

            // Find the layer index that contains launch_depth (z_cur).
            // Layer i covers [zs[i], zs[i+1]); a depth exactly at zs[i+1] is
            // counted as the start of layer i+1.
            size_t layer = 0;
            {
                size_t lo = 0, hi = L;
                while (hi - lo > 1)
                {
                    size_t mid = (lo + hi) / 2;
                    (z_cur < (double)zs.unchecked(mid) ? hi : lo) = mid;
                }
                layer = lo;
                // If launch_depth == z_bot exactly, we're at the bottom of the
                // last layer with no layers left to traverse downward.
            }
            // pre-compute end-of-layer (dt, dx, c_next, cos_next, z_next).
            // Uses (z_cur, c_cur, cos_cur) so it correctly handles a partial
            // first layer where we start mid-layer at launch_depth.
            auto step_full_layer = [&](size_t i, double& dt_out, double& dx_out,
                                        double& c_next_out, double& cos_next_out,
                                        double& z_next_out) -> bool
            {
                const double c_next = (double)cs.unchecked(i + 1);
                const double pcn    = (double)p * c_next;
                if (pcn >= 1.0)
                    return false; // ray turns inside this layer; not handled here
                const double cos_next = std::sqrt(std::max(0.0, 1.0 - pcn * pcn));
                const double z_next   = (double)zs.unchecked(i + 1);
                if (iso.unchecked(i))
                {
                    const double dz = z_next - z_cur;
                    if (cos_cur < 1e-12) return false;
                    dt_out = dz / (c_cur * cos_cur);
                    dx_out = dz * (double)p * c_cur / cos_cur; // = dz * tan(theta)
                }
                else
                {
                    const double g = (double)gs.unchecked(i);
                    dt_out = std::log((c_next / c_cur) * ((1.0 + cos_cur) / (1.0 + cos_next))) / g;
                    dx_out = (cos_cur - cos_next) / ((double)p * g);
                }
                c_next_out   = c_next;
                cos_next_out = cos_next;
                z_next_out   = z_next;
                return true;
            };

            for (size_t k = 0; k < K1; ++k)
            {
                const double t_target = (double)knot_times.unchecked(k);
                bool         turning  = false;

                while (layer < L)
                {
                    double dt_lay, dx_lay, c_next, cos_next, z_next;
                    if (!step_full_layer(layer, dt_lay, dx_lay, c_next, cos_next, z_next))
                    {
                        turning = true;
                        break;
                    }
                    if (t_cur + dt_lay >= t_target)
                    {
                        // partial layer up to t_target
                        const double dt_part = t_target - t_cur;
                        if (iso.unchecked(layer))
                        {
                            const double ds  = c_cur * dt_part;
                            const double dz  = ds * cos_cur;
                            const double dxp = ds * (double)p * c_cur; // = ds*sin = dz*tan
                            t_cur += dt_part;
                            x_cur += dxp;
                            z_cur += dz;
                            // c_cur, cos_cur unchanged
                        }
                        else
                        {
                            const double g = (double)gs.unchecked(layer);
                            // closed-form invert: c_partial = 2A / (1 + (A*p)^2),
                            //   A = exp(g*dt_part) * c_cur / (1 + cos_cur)
                            const double A = std::exp(g * dt_part) * c_cur / (1.0 + cos_cur);
                            const double Ap = A * (double)p;
                            const double c_part = 2.0 * A / (1.0 + Ap * Ap);
                            const double pc     = (double)p * c_part;
                            const double cos_part = std::sqrt(std::max(0.0, 1.0 - pc * pc));
                            const double dxp = (cos_cur - cos_part) / ((double)p * g);
                            t_cur += dt_part;
                            x_cur += dxp;
                            z_cur += (c_part - c_cur) / g;
                            c_cur   = c_part;
                            cos_cur = cos_part;
                        }
                        break; // landed on knot; do NOT advance layer
                    }
                    // full layer step
                    t_cur += dt_lay;
                    x_cur += dx_lay;
                    z_cur = z_next;
                    c_cur = c_next;
                    cos_cur = cos_next;
                    ++layer;
                }
                if (layer >= L && t_cur < t_target)
                    turning = true; // ran out of profile before reaching knot

                if (turning)
                {
                    out(k, b, 0) = std::nanf("");
                    out(k, b, 1) = std::nanf("");
                    out(k, b, 2) = std::nanf("");
                    continue;
                }

                // Vehicle-frame ray endpoint relative to the TX origin.
                // Convention: launch_depth is the absolute depth of the TX
                // origin at TX time; lz is the depth advance below it.
                // (hx_v, hy_v) is the unit horizontal direction in vehicle frame.
                const float lx = (float)(hx_v * x_cur);
                const float ly = (float)(hy_v * x_cur);
                const float lz = (float)(z_cur - (double)launch_depth);
                // Rotate vehicle-frame offset into world frame using TX pose's ypr.
                auto rotated = tools::rotationfunctions::rotateXYZ<float>(tx_q[k], lx, ly, lz);
                // Translation: depth = mid(tx, rx) + rotated_z. Horizontal x/y are
                // expressed relative to the TX-origin (no nav northing/easting in
                // plain Geolocation); user can add absolute horizontal offsets.
                const auto& tp = tx_poses[k];
                const auto& rp = rx_poses[k];
                out(k, b, 0) = rotated[0];
                out(k, b, 1) = rotated[1];
                out(k, b, 2) = rotated[2] + 0.5f * (tp.z + rp.z);
            }
        }

        return out;
    }

    /**
     * @brief Convenience overload: same TX and RX pose at each knot.
     */
    xt::xtensor<float, 3> trace_at_times(
        const xt::xtensor<float, 2>&                                 launch_dirs,
        const xt::xtensor<float, 1>&                                 knot_times,
        const std::vector<navigation::datastructures::Geolocation>&  poses,
        int                                                          mp_cores = 1) const
    {
        return trace_at_times(launch_dirs, knot_times, poses, poses, mp_cores);
    }

    /**
     * @brief Convert per-beam (tilt, crosstrack) angles in degrees to vehicle-frame
     *        unit launch directions (forward, starboard, down).
     *
     *   tilt_deg:        positive bow-up about the starboard axis (i.e. positive
     *                    tilt -> beam points forward).
     *   crosstrack_deg:  beam pointing angle from nadir, positive towards
     *                    starboard.
     *
     *   dx = sin(tilt) * cos(crosstrack)   (forward)
     *   dy = cos(tilt) * sin(crosstrack)   (starboard)
     *   dz = cos(tilt) * cos(crosstrack)   (down)
     *
     * Caller-side launch-direction maths should not be done elsewhere; route all
     * tracing through this method or its trace_at_angles overload.
     */
    static xt::xtensor<float, 2> launch_dirs_from_angles(
        const xt::xtensor<float, 1>& tilt_deg,
        const xt::xtensor<float, 1>& crosstrack_deg)
    {
        if (tilt_deg.size() != crosstrack_deg.size())
            throw std::runtime_error(fmt::format(
                "LayerRaytracer.launch_dirs_from_angles: tilt_deg ({}) and "
                "crosstrack_deg ({}) must have the same length",
                tilt_deg.size(),
                crosstrack_deg.size()));

        const size_t n_beams = tilt_deg.size();
        auto         out     = xt::xtensor<float, 2>::from_shape({ n_beams, size_t(3) });
        constexpr float deg2rad = float(M_PI) / 180.f;

        for (size_t b = 0; b < n_beams; ++b)
        {
            const float t  = tilt_deg.unchecked(b) * deg2rad;
            const float c  = crosstrack_deg.unchecked(b) * deg2rad;
            const float ct = std::cos(t);
            const float st = std::sin(t);
            const float cc = std::cos(c);
            const float sc = std::sin(c);
            out(b, 0) = st * cc; // forward
            out(b, 1) = ct * sc; // starboard
            out(b, 2) = ct * cc; // down
        }
        return out;
    }

    /**
     * @brief Trace beams given per-beam (tilt, crosstrack) angles in degrees.
     *        See ::launch_dirs_from_angles for the angle convention.
     */
    xt::xtensor<float, 3> trace_at_angles(
        const xt::xtensor<float, 1>&                                 tilt_deg,
        const xt::xtensor<float, 1>&                                 crosstrack_deg,
        const xt::xtensor<float, 1>&                                 knot_times,
        const std::vector<navigation::datastructures::Geolocation>&  tx_poses,
        const std::vector<navigation::datastructures::Geolocation>&  rx_poses,
        int                                                          mp_cores = 1) const
    {
        return trace_at_times(
            launch_dirs_from_angles(tilt_deg, crosstrack_deg),
            knot_times,
            tx_poses,
            rx_poses,
            mp_cores);
    }

    /**
     * @brief Convenience overload: same TX and RX pose at each knot.
     */
    xt::xtensor<float, 3> trace_at_angles(
        const xt::xtensor<float, 1>&                                 tilt_deg,
        const xt::xtensor<float, 1>&                                 crosstrack_deg,
        const xt::xtensor<float, 1>&                                 knot_times,
        const std::vector<navigation::datastructures::Geolocation>&  poses,
        int                                                          mp_cores = 1) const
    {
        return trace_at_times(
            launch_dirs_from_angles(tilt_deg, crosstrack_deg),
            knot_times,
            poses,
            poses,
            mp_cores);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "LayerRaytracer", float_precision, superscript_exponents);
        printer.append(_svp.__printer__(float_precision, superscript_exponents));
        return printer;
    }

  public:
    static LayerRaytracer from_stream(std::istream& is)
    {
        LayerRaytracer rt;
        rt._svp = SoundVelocityProfile::from_stream(is);
        return rt;
    }
    void to_stream(std::ostream& os) const { _svp.to_stream(os); }

    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(LayerRaytracer)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
