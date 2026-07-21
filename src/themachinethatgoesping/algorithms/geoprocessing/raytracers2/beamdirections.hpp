// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// BeamDirections / compute_beam_directions
// -----------------------------------------------------------------------------
// Analytic beam-pointing calculation for a (multi-sector, Mills-cross) multibeam.
// For every beam it computes the pointing direction from the transmit- and
// receive-array installation, the vessel attitude at transmit and at receive, and
// the electronic transmit/receive steering angles. The resulting per-beam
// direction defines the launch geometry a raytracer (e.g. trace_beam) integrates
// through the water column.
//
// Method (Beaudoin, Hughes Clarke & Bartlett 2004, "Application of Surface Sound
// Speed Measurements in Post-Processing for Multi-Sector Multibeam Echosounders",
// Int. Hydrographic Review 5(3):26-31): the transmit and receive arrays are two
// independently mounted linear arrays (a Mills cross). After installation and
// attitude each array defines a world-frame long axis. The transmit and receive
// steering angles fix the projection of the beam onto the respective array axis;
// the beam direction is the (downward) intersection of the two "fans". Here that
// intersection is solved directly in quaternion-rotated axes, so the array
// non-orthogonality is handled exactly rather than through a small-angle plane
// construction.
//
// Frame and sign conventions (ping standard = Kongsberg, matching
// rotationfunctions::quaternion_from_ypr and the geoprocessing xyz):
//   * Vehicle frame: x = forward, y = starboard, z = down (right-handed).
//   * Orientation (yaw, pitch, roll): the body->world rotation
//     R = Rz(yaw) * Ry(pitch) * Rx(roll); yaw about z (down), pitch about y
//     (starboard), roll about x (forward).
//   * transmit array long axis = forward (1,0,0); transmit steering is a fore-aft
//     tilt, positive forward.
//   * receive array long axis = starboard (0,1,0); receive steering is
//     across-track, positive to PORT (paper and Kongsberg convention).
//   * reference heading: the heading (deg) the output is expressed relative to.
//
// Reverse-mounted arrays need no special treatment: a reverse mount is simply an
// installation orientation with a ~180 deg yaw. The quaternion rotation of the
// array long axis carries the reversed steering sign through automatically, so no
// manual "subtract 180 / flip sign" corrections are applied.
//
// Output: per beam a ship-referenced unit pointing vector (x = forward,
// y = starboard, z = down, reference heading removed). From it the signed beam
// pointing angle (the athwartships launch angle for trace_beam) and the beam
// azimuth (the fore-aft rotation that lifts a 2-D trace back into 3-D) are
// derived on access.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/beamdirections.doc.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include <fmt/format.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Per-beam pointing directions of a multibeam swath.
 *
 * Stores one ship-referenced unit pointing vector per beam (x = forward,
 * y = starboard, z = down, reference heading removed). From each direction the
 * class derives, on access:
 *   * the signed beam pointing angle (the athwartships launch angle handed to
 *     trace_beam: 0 deg = nadir/down, +90 deg = horizontal to port, -90 deg =
 *     horizontal to starboard),
 *   * the beam azimuth (the fore-aft rotation about the vertical/down axis that
 *     lifts a 2-D trace (horizontal_offset, depth) back into ship-frame xyz),
 *   * the unsigned beam take-off angle from nadir.
 *
 * A traced beam point is lifted back into the ship frame by rotating it about the
 * vertical (down) axis by the beam azimuth:
 *   x_forward   = -horizontal_offset * sin(beam_azimuth)
 *   y_starboard =  horizontal_offset * cos(beam_azimuth)
 *   z_down      =  depth
 * i.e. xyz = R_down(beam_azimuth) * (0, horizontal_offset, depth).
 */
class BeamDirections
{
    // Ship-referenced unit pointing vectors, shape [n_beams, 3] = (forward, starboard, down).
    xt::xtensor<float, 2> _directions;

  public:
    /// @brief Construct an empty BeamDirections (no beams stored).
    BeamDirections() = default;

    /**
     * @brief Construct from per-beam unit pointing vectors.
     * @param directions [n_beams, 3] tensor of (forward, starboard, down) components.
     * @throws std::runtime_error if the second dimension is not 3.
     */
    explicit BeamDirections(xt::xtensor<float, 2> directions) { set(std::move(directions)); }

    /// @brief Equality comparison.
    bool operator==(const BeamDirections& other) const { return _directions == other._directions; }

    /**
     * @brief Set the per-beam unit pointing vectors.
     * @param directions [n_beams, 3] tensor of (forward, starboard, down) components.
     * @throws std::runtime_error if the second dimension is not 3.
     */
    void set(xt::xtensor<float, 2> directions)
    {
        if (directions.shape(1) != 3)
            throw std::runtime_error(fmt::format(
                "BeamDirections: directions must have shape [n_beams, 3], got [{}, {}]",
                directions.shape(0),
                directions.shape(1)));
        _directions = std::move(directions);
    }

    /// @brief Number of beams stored.
    size_t get_number_of_beams() const { return _directions.shape(0); }

    /// @brief Per-beam ship-referenced unit pointing vectors [n_beams, 3] = (forward, starboard, down).
    const xt::xtensor<float, 2>& get_directions() const { return _directions; }

    /**
     * @brief Ship-referenced unit pointing vector (forward, starboard, down) of a single beam.
     *
     * Convenience accessor for feeding one beam's direction as the concentric
     * initial guess into trace_bistatic_beam.
     * @param beam_index index of the beam.
     */
    std::array<float, 3> get_beam_direction(size_t beam_index) const
    {
        return { _directions(beam_index, 0),
                 _directions(beam_index, 1),
                 _directions(beam_index, 2) };
    }

    /**
     * @brief Signed beam pointing angle (deg): the athwartships launch angle for trace_beam.
     *
     * 0 deg is nadir (straight down), +90 deg is horizontal to port, -90 deg is
     * horizontal to starboard. Its magnitude equals the take-off angle from nadir;
     * its sign follows the port (+) / starboard (-) side of the beam. Combined with
     * get_beam_azimuth_angles_in_degrees() it reconstructs the full 3-D direction:
     *   d = R_down(beam_azimuth) * (0, -sin(beam_pointing), cos(beam_pointing)).
     */
    xt::xtensor<float, 1> get_beam_pointing_angles_in_degrees() const
    {
        constexpr float rad_to_deg = float(180.0 / M_PI);
        const auto      forward    = xt::view(_directions, xt::all(), 0);
        const auto      starboard  = xt::view(_directions, xt::all(), 1);
        const auto      down       = xt::view(_directions, xt::all(), 2);

        const auto horizontal = xt::sqrt(forward * forward + starboard * starboard);
        const auto take_off   = xt::atan2(horizontal, down); // [0, pi] from nadir
        // starboard side (y > 0) -> negative pointing angle
        return xt::eval(xt::where(starboard > 0.f, -take_off, take_off) * rad_to_deg);
    }

    /**
     * @brief Beam azimuth (deg): the fore-aft rotation about the vertical (down) axis.
     *
     * Principal value in (-90, 90]; 0 deg means the beam lies in the athwartships
     * plane (no fore-aft component). Used to lift a 2-D trace back into 3-D:
     *   xyz = R_down(beam_azimuth) * (0, horizontal_offset, depth).
     */
    xt::xtensor<float, 1> get_beam_azimuth_angles_in_degrees() const
    {
        constexpr float pi         = float(M_PI);
        constexpr float rad_to_deg = float(180.0 / M_PI);
        const auto      forward    = xt::view(_directions, xt::all(), 0);
        const auto      starboard  = xt::view(_directions, xt::all(), 1);

        const auto azimuth_full = xt::atan2(forward, -starboard);
        // fold to (-90, 90] by removing +-180 deg on the starboard side
        return xt::eval(xt::where(starboard > 0.f,
                                  azimuth_full - xt::sign(azimuth_full) * pi,
                                  azimuth_full) *
                        rad_to_deg);
    }

    /**
     * @brief Unsigned beam take-off angle (deg) from nadir (straight down), always >= 0.
     *
     * 0 deg is nadir, 90 deg is horizontal. Equals |beam pointing angle|.
     */
    xt::xtensor<float, 1> get_beam_takeoff_angles_in_degrees() const
    {
        constexpr float rad_to_deg = float(180.0 / M_PI);
        const auto      forward    = xt::view(_directions, xt::all(), 0);
        const auto      starboard  = xt::view(_directions, xt::all(), 1);
        const auto      down       = xt::view(_directions, xt::all(), 2);
        const auto      horizontal = xt::sqrt(forward * forward + starboard * starboard);
        return xt::eval(xt::atan2(horizontal, down) * rad_to_deg);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamDirections", float_precision, superscript_exponents);

        printer.register_value("number_of_beams", get_number_of_beams());
        printer.register_section("derived");
        printer.register_container(
            "beam_pointing_angle", get_beam_pointing_angles_in_degrees(), "deg");
        printer.register_container("beam_azimuth", get_beam_azimuth_angles_in_degrees(), "deg");

        return printer;
    }

  public:
    static BeamDirections from_stream(std::istream& is)
    {
        size_t n = 0;
        is.read(reinterpret_cast<char*>(&n), sizeof(size_t));

        BeamDirections obj;
        obj._directions = xt::xtensor<float, 2>::from_shape({ n, size_t(3) });
        is.read(reinterpret_cast<char*>(obj._directions.data()), sizeof(float) * n * 3);

        return obj;
    }

    void to_stream(std::ostream& os) const
    {
        size_t n = _directions.shape(0);
        os.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(_directions.data()), sizeof(float) * n * 3);
    }

  public:
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamDirections)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

/**
 * @brief Decompose a ship-frame beam direction into a signed pointing angle and azimuth.
 *
 * Scalar counterpart of BeamDirections::get_beam_pointing_angles_in_degrees /
 * get_beam_azimuth_angles_in_degrees, using the identical convention: the pointing
 * angle is 0 deg at nadir, +90 deg horizontal to port, -90 deg to starboard; the
 * azimuth is the fore-aft rotation about the vertical (down) axis, principal value
 * in (-90, 90]. Together they reconstruct the direction:
 *   d = R_down(azimuth) * (0, -sin(pointing), cos(pointing)).
 *
 * @param forward   forward (x) component of the unit direction.
 * @param starboard starboard (y) component.
 * @param down      down (z) component.
 * @return {pointing_angle_in_degrees, azimuth_in_degrees}.
 */
inline std::array<float, 2> beam_direction_to_pointing_and_azimuth_in_degrees(float forward,
                                                                             float starboard,
                                                                             float down)
{
    constexpr float rad_to_deg = float(180.0 / M_PI);
    constexpr float pi         = float(M_PI);

    const float horizontal   = std::sqrt(forward * forward + starboard * starboard);
    const float take_off     = std::atan2(horizontal, down);
    const float pointing      = (starboard > 0.f) ? -take_off : take_off;
    const float azimuth_full = std::atan2(forward, -starboard);
    const float azimuth      = (starboard > 0.f) ? (azimuth_full - std::copysign(pi, azimuth_full))
                                                 : azimuth_full;
    return { pointing * rad_to_deg, azimuth * rad_to_deg };
}

/**
 * @brief Snell correction of a beam steering angle for a changed surface sound speed.
 *
 * Multibeam beamforming forms every steering angle in the near field using the
 * surface sound speed measured at the transducer. If that surface sound speed is
 * wrong, the true beam angle in the water refracts across the transducer face
 * following Snell's law:
 *   sin(corrected) = (c_corrected / c_used) * sin(steering).
 * The correction must be applied to the transmit and receive steering angles
 * *before* compute_beam_directions, because the steering angles are defined at the
 * array face, which is where the refraction happens. Angles that would exceed the
 * horizon are clamped to +-90 deg.
 *
 * @param steering_angles_in_degrees                          nominal steering angles (deg).
 * @param surface_sound_speed_used_in_meters_per_second       surface sound speed used when the beams were formed.
 * @param surface_sound_speed_corrected_in_meters_per_second  true / corrected surface sound speed.
 * @return corrected steering angles (deg).
 */
inline xt::xtensor<float, 1> correct_steering_angles_for_surface_sound_speed(
    const xt::xtensor<float, 1>& steering_angles_in_degrees,
    float                        surface_sound_speed_used_in_meters_per_second,
    float                        surface_sound_speed_corrected_in_meters_per_second)
{
    constexpr float deg_to_rad = float(M_PI / 180.0);
    constexpr float rad_to_deg = float(180.0 / M_PI);

    const float sound_speed_ratio = surface_sound_speed_corrected_in_meters_per_second /
                                    surface_sound_speed_used_in_meters_per_second;

    const auto sin_corrected =
        xt::clip(sound_speed_ratio * xt::sin(deg_to_rad * steering_angles_in_degrees), -1.f, 1.f);
    return xt::eval(xt::asin(sin_corrected) * rad_to_deg);
}

/**
 * @brief Scalar overload of correct_steering_angles_for_surface_sound_speed.
 * @see correct_steering_angles_for_surface_sound_speed
 */
inline float correct_steering_angle_for_surface_sound_speed(
    float steering_angle_in_degrees,
    float surface_sound_speed_used_in_meters_per_second,
    float surface_sound_speed_corrected_in_meters_per_second)
{
    constexpr float deg_to_rad = float(M_PI / 180.0);
    constexpr float rad_to_deg = float(180.0 / M_PI);

    const float sound_speed_ratio = surface_sound_speed_corrected_in_meters_per_second /
                                    surface_sound_speed_used_in_meters_per_second;

    const float sin_corrected =
        std::clamp(sound_speed_ratio * std::sin(deg_to_rad * steering_angle_in_degrees), -1.f, 1.f);
    return std::asin(sin_corrected) * rad_to_deg;
}

/**
 * @brief Compute the ship-referenced pointing direction of every beam.
 *
 * Each beam is the downward intersection of the transmit and receive array "fans"
 * (Mills cross). The transmit array long axis (forward) and the receive array long
 * axis (starboard) are placed in the world frame from their installation
 * orientation and the vessel attitude at transmit / receive; the transmit and
 * receive steering angles then fix the beam's projection onto each axis. The
 * intersection is solved directly (no orthogonality assumption), so array
 * non-orthogonality is exact and reverse mounts are handled by the installation
 * quaternion alone (no manual sign flips).
 *
 * @param transmit_installation_ypr_in_degrees (yaw, pitch, roll) mounting orientation of the transmit array.
 * @param receive_installation_ypr_in_degrees  (yaw, pitch, roll) mounting orientation of the receive array.
 * @param transmit_attitude_ypr_in_degrees     [n_beams, 3] vessel (yaw, pitch, roll) at transmit time.
 * @param receive_attitude_ypr_in_degrees      [n_beams, 3] vessel (yaw, pitch, roll) at receive time.
 * @param transmit_steering_angles_in_degrees  [n_beams] fore-aft transmit tilt (positive forward).
 * @param receive_steering_angles_in_degrees   [n_beams] across-track receive angle (positive to PORT).
 * @param reference_heading_in_degrees         heading the output is expressed relative to.
 * @param mp_cores                             number of OpenMP cores for the per-beam solve (default 1).
 * @return BeamDirections with one ship-referenced unit pointing vector per beam.
 */
inline BeamDirections compute_beam_directions(
    const std::array<double, 3>& transmit_installation_ypr_in_degrees,
    const std::array<double, 3>& receive_installation_ypr_in_degrees,
    const xt::xtensor<float, 2>& transmit_attitude_ypr_in_degrees,
    const xt::xtensor<float, 2>& receive_attitude_ypr_in_degrees,
    const xt::xtensor<float, 1>& transmit_steering_angles_in_degrees,
    const xt::xtensor<float, 1>& receive_steering_angles_in_degrees,
    double                       reference_heading_in_degrees,
    int                          mp_cores = 1)
{
    using tools::rotationfunctions::quaternion_from_ypr;

    const size_t number_of_beams = transmit_steering_angles_in_degrees.size();

    if (transmit_attitude_ypr_in_degrees.shape(0) != number_of_beams ||
        transmit_attitude_ypr_in_degrees.shape(1) != 3 ||
        receive_attitude_ypr_in_degrees.shape(0) != number_of_beams ||
        receive_attitude_ypr_in_degrees.shape(1) != 3 ||
        receive_steering_angles_in_degrees.size() != number_of_beams)
        throw std::invalid_argument("compute_beam_directions: inconsistent input shapes "
                                    "(need attitudes [n_beams, 3] and steering [n_beams]).");

    if (mp_cores < 1)
        mp_cores = 1;

    constexpr double degrees_to_radians = M_PI / 180.0;

    // Installation quaternions (shared by all beams) and the reference-heading
    // rotation Rz(-reference_heading) that expresses the result ship-referenced.
    const Eigen::Quaterniond transmit_installation_quaternion =
        quaternion_from_ypr<double>(transmit_installation_ypr_in_degrees, true);
    const Eigen::Quaterniond receive_installation_quaternion =
        quaternion_from_ypr<double>(receive_installation_ypr_in_degrees, true);
    const Eigen::Quaterniond reference_heading_quaternion =
        quaternion_from_ypr<double>(-reference_heading_in_degrees, 0.0, 0.0, true);

    // Each array's long axis in its own frame: transmit = forward, receive = starboard.
    const Eigen::Vector3d transmit_axis_in_array_frame(1.0, 0.0, 0.0);
    const Eigen::Vector3d receive_axis_in_array_frame(0.0, 1.0, 0.0);

    xt::xtensor<float, 2> directions =
        xt::xtensor<float, 2>::from_shape({ number_of_beams, size_t(3) });

#pragma omp parallel for num_threads(mp_cores)
    for (int64_t beam_index = 0; beam_index < int64_t(number_of_beams); ++beam_index)
    {
        // World orientation of each array:  world = attitude * installation.
        const Eigen::Quaterniond transmit_quaternion =
            quaternion_from_ypr<double>(double(transmit_attitude_ypr_in_degrees(beam_index, 0)),
                                        double(transmit_attitude_ypr_in_degrees(beam_index, 1)),
                                        double(transmit_attitude_ypr_in_degrees(beam_index, 2)),
                                        true) *
            transmit_installation_quaternion;
        const Eigen::Quaterniond receive_quaternion =
            quaternion_from_ypr<double>(double(receive_attitude_ypr_in_degrees(beam_index, 0)),
                                        double(receive_attitude_ypr_in_degrees(beam_index, 1)),
                                        double(receive_attitude_ypr_in_degrees(beam_index, 2)),
                                        true) *
            receive_installation_quaternion;

        // World-frame array long axes.
        const Eigen::Vector3d transmit_array_axis = transmit_quaternion * transmit_axis_in_array_frame;
        const Eigen::Vector3d receive_array_axis  = receive_quaternion * receive_axis_in_array_frame;

        // Steering constraints: projection of the beam onto each array axis. Receive
        // steering is positive to port, hence the minus sign on the starboard axis.
        const double projection_on_transmit_axis =
            std::sin(degrees_to_radians * double(transmit_steering_angles_in_degrees(beam_index)));
        const double projection_on_receive_axis =
            -std::sin(degrees_to_radians * double(receive_steering_angles_in_degrees(beam_index)));

        // Solve  d . transmit_axis = projection_transmit,  d . receive_axis = projection_receive
        // for the unit beam direction, then pick the downward (max +z) root. Exact for
        // any non-degenerate angle between the two axes (array non-orthogonality).
        const double axis_dot        = transmit_array_axis.dot(receive_array_axis);
        const double inverse_gramian = 1.0 / std::max(1.0 - axis_dot * axis_dot, 1e-12);

        const double transmit_coefficient =
            (projection_on_transmit_axis - axis_dot * projection_on_receive_axis) * inverse_gramian;
        const double receive_coefficient =
            (projection_on_receive_axis - axis_dot * projection_on_transmit_axis) * inverse_gramian;

        const Eigen::Vector3d in_plane_component =
            transmit_coefficient * transmit_array_axis + receive_coefficient * receive_array_axis;
        // plane_normal = transmit_axis x receive_axis, with |plane_normal|^2 = 1 - axis_dot^2.
        const Eigen::Vector3d plane_normal = transmit_array_axis.cross(receive_array_axis);

        // squared coefficient on the (unnormalized) plane normal that makes |d| = 1.
        const double normal_coefficient_squared =
            (1.0 - in_plane_component.squaredNorm()) * inverse_gramian;

        Eigen::Vector3d beam_direction;
        if (normal_coefficient_squared > 0.0)
        {
            const double          normal_coefficient = std::sqrt(normal_coefficient_squared);
            const Eigen::Vector3d beam_down = in_plane_component + normal_coefficient * plane_normal;
            const Eigen::Vector3d beam_up   = in_plane_component - normal_coefficient * plane_normal;
            beam_direction = (beam_down.z() >= beam_up.z()) ? beam_down : beam_up;
        }
        else
        {
            // Requested steering lies beyond the horizon: clamp to horizontal.
            beam_direction     = in_plane_component;
            beam_direction.z() = 0.0;
        }

        // Express relative to the reference heading and normalize.
        beam_direction = reference_heading_quaternion * beam_direction;
        beam_direction.normalize();

        directions.unchecked(beam_index, 0) = float(beam_direction.x());
        directions.unchecked(beam_index, 1) = float(beam_direction.y());
        directions.unchecked(beam_index, 2) = float(beam_direction.z());
    }

    return BeamDirections(std::move(directions));
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
