// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// SoundVelocityProfile — 1-D layered sound velocity profile c(z)
// -----------------------------------------------------------------------------
// Stores depth/sound-speed knots and precomputes per-layer constants used by
// the LayerRaytracer for closed-form Snell raytracing.
//
// Layer i covers z ∈ [z_i, z_{i+1}] with linear gradient:
//   c(z) = c_i + g_i * (z - z_i),       g_i = (c_{i+1} - c_i) / (z_{i+1} - z_i)
//
// Iso-velocity layers (g_i ≈ 0) are flagged so the raytracer can use the
// straight-ray formulas instead of the constant-gradient ones.
// -----------------------------------------------------------------------------

#pragma once

#include ".docstrings/soundvelocityprofile.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief 1-D depth-dependent sound velocity profile with layered analytic
 *        precomputations for use by the LayerRaytracer.
 */
class SoundVelocityProfile
{
  private:
    xt::xtensor<float, 1> _z; ///< [L+1] depths (m), monotonically increasing, z[0] >= 0
    xt::xtensor<float, 1> _c; ///< [L+1] sound speeds (m/s), corresponding to _z

    // Precomputed per-layer constants  (size L)
    xt::xtensor<float, 1> _g;     ///< gradient (c_{i+1}-c_i)/(z_{i+1}-z_i) per layer
    xt::xtensor<float, 1> _inv_g; ///< 1/g per layer (0.0f for iso-velocity layers)
    xt::xtensor<bool, 1>  _iso;   ///< iso-velocity flag per layer (|g| < eps)

    static constexpr float ISO_EPS = 1e-6f; ///< |dc/dz| below which a layer counts as iso-velocity

  public:
    SoundVelocityProfile() = default;

    /**
     * @brief Construct from depth/speed tables.
     * @param z depths (m), monotonically increasing
     * @param c sound speeds (m/s)
     */
    SoundVelocityProfile(xt::xtensor<float, 1> z, xt::xtensor<float, 1> c)
    {
        set(std::move(z), std::move(c));
    }

    /// Convenience: constant SVP from surface to z_max.
    static SoundVelocityProfile uniform(float c, float z_max = 12000.f)
    {
        xt::xtensor<float, 1> zs = { 0.f, z_max };
        xt::xtensor<float, 1> cs = { c, c };
        return SoundVelocityProfile(std::move(zs), std::move(cs));
    }

    bool operator==(const SoundVelocityProfile& other) const
    {
        return _z == other._z && _c == other._c;
    }

    void set(xt::xtensor<float, 1> z, xt::xtensor<float, 1> c)
    {
        if (z.size() != c.size())
            throw std::runtime_error(fmt::format(
                "SoundVelocityProfile: z and c must have same size ({} vs {})", z.size(), c.size()));
        if (z.size() < 2)
            throw std::runtime_error("SoundVelocityProfile: need at least 2 layer knots");
        for (size_t i = 1; i < z.size(); ++i)
        {
            if (!(z.unchecked(i) > z.unchecked(i - 1)))
                throw std::runtime_error("SoundVelocityProfile: z must be strictly increasing");
            if (!(c.unchecked(i) > 0.f) || !(c.unchecked(i - 1) > 0.f))
                throw std::runtime_error("SoundVelocityProfile: c must be positive");
        }
        _z = std::move(z);
        _c = std::move(c);
        recompute_layer_constants_();
    }

    // --- accessors ---
    const xt::xtensor<float, 1>& get_z() const { return _z; }
    const xt::xtensor<float, 1>& get_c() const { return _c; }
    const xt::xtensor<float, 1>& get_g() const { return _g; }
    const xt::xtensor<float, 1>& get_inv_g() const { return _inv_g; }
    const xt::xtensor<bool, 1>&  get_iso() const { return _iso; }
    size_t                       get_n_layers() const { return _z.size() - 1; }

    /// Sound speed at depth z (linear interp inside layers, clamped at ends).
    float get_sound_speed(float z) const
    {
        if (_z.size() < 2)
            throw std::runtime_error("SoundVelocityProfile: not initialized");
        if (z <= _z.unchecked(0))
            return _c.unchecked(0);
        const size_t L = _z.size();
        if (z >= _z.unchecked(L - 1))
            return _c.unchecked(L - 1);
        // binary search for layer
        size_t lo = 0, hi = L - 1;
        while (hi - lo > 1)
        {
            size_t mid = (lo + hi) / 2;
            (z < _z.unchecked(mid) ? hi : lo) = mid;
        }
        const float z0 = _z.unchecked(lo);
        const float c0 = _c.unchecked(lo);
        return c0 + _g.unchecked(lo) * (z - z0);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "SoundVelocityProfile", float_precision, superscript_exponents);
        printer.register_container("z", _z, "m");
        printer.register_container("c", _c, "m/s");
        printer.register_section("layers (precomputed)");
        printer.register_value("n_layers", get_n_layers());
        return printer;
    }

  public:
    static SoundVelocityProfile from_stream(std::istream& is)
    {
        size_t n = 0;
        is.read(reinterpret_cast<char*>(&n), sizeof(size_t));
        SoundVelocityProfile svp;
        svp._z = xt::xtensor<float, 1>::from_shape({ n });
        svp._c = xt::xtensor<float, 1>::from_shape({ n });
        is.read(reinterpret_cast<char*>(svp._z.data()), sizeof(float) * n);
        is.read(reinterpret_cast<char*>(svp._c.data()), sizeof(float) * n);
        if (n >= 2)
            svp.recompute_layer_constants_();
        return svp;
    }

    void to_stream(std::ostream& os) const
    {
        size_t n = _z.size();
        os.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(_z.data()), sizeof(float) * n);
        os.write(reinterpret_cast<const char*>(_c.data()), sizeof(float) * n);
    }

  private:
    void recompute_layer_constants_()
    {
        const size_t L = _z.size() - 1;
        _g    = xt::xtensor<float, 1>::from_shape({ L });
        _inv_g = xt::xtensor<float, 1>::from_shape({ L });
        _iso  = xt::xtensor<bool, 1>::from_shape({ L });
        for (size_t i = 0; i < L; ++i)
        {
            const float dz = _z.unchecked(i + 1) - _z.unchecked(i);
            const float dc = _c.unchecked(i + 1) - _c.unchecked(i);
            const float g  = dc / dz;
            _g.unchecked(i)    = g;
            _iso.unchecked(i)  = std::abs(g) < ISO_EPS;
            _inv_g.unchecked(i) = _iso.unchecked(i) ? 0.f : 1.f / g;
        }
    }

  public:
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SoundVelocityProfile)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
