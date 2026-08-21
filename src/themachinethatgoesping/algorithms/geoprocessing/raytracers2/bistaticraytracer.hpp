// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// BistaticBeamTrace / trace_bistatic_beam
// -----------------------------------------------------------------------------
// True bistatic (non-concentric) seabed solution for a SINGLE multibeam beam.
//
// A Mills-cross multibeam transmits from one linear array and receives on a second,
// physically separated array. Because the vessel also moves during the two-way
// travel time, the transmit and receive phase centres do not coincide. The usual
// "concentric" model (BeamDirections / compute_beam_directions) collapses both
// arrays onto one point and traces a single ray; that leaves a first-order footprint
// error of order half the transmit/receive baseline (mostly along-track for a fast
// vessel, and an outer-beam depth bias for a vertical array offset).
//
// This routine solves the full two-leg geometry for one beam: the transmit ray
// leaves the transmit array (at its position and orientation at transmit time), the
// receive ray leaves the receive array (at its position and orientation at receive
// time), both refract through the layered sound-velocity profile, and they must meet
// at the same seabed point with a combined one-way travel time equal to the measured
// two-way travel time. The seabed point is found with a small Newton iteration that
// is *seeded with the concentric beam direction* (see the concentric_beam_direction
// argument, e.g. BeamDirections::get_beam_direction). When the two arrays coincide
// the solution degenerates exactly to the monostatic trace_beam.
//
// Consistency with the single-beam 2-D model (raytracers2::trace_beam):
//   * the fast inner step of the solver is trace_beam_to_depth (tracebeam.hpp), which
//     shares the closed-form per-layer iso/gradient kernels used by trace_beam;
//   * after convergence each leg's per-layer polyline is produced with trace_beam and
//     stored as a BeamTrace, so a monostatic trace_beam and a bistatic trace with
//     identical transmit/receive poses yield identical legs.
//
// Frame and sign conventions (ping standard, identical to beamdirections.hpp):
//   * Common frame: x = forward, y = starboard, z = down (right-handed). The transmit
//     and receive positions and the returned seabed position are all in this single
//     metric frame (e.g. the ship frame at ping time); the result is NOT geo-referenced.
//     The concentric guess must be expressed in this same frame (i.e. produced with
//     compute_beam_directions using reference_heading = 0).
//   * Orientation (yaw, pitch, roll): body->common rotation R = Rz(yaw) Ry(pitch)
//     Rx(roll); each array orientation is attitude * installation.
//   * transmit array long axis = forward (1,0,0); transmit steering positive forward.
//   * receive  array long axis = starboard (0,1,0); receive steering positive to PORT.
//
// Method basis (published, no GPL sources used): the concentric Mills-cross geometry
// used only for the initial guess is the construction of Beaudoin, Hughes Clarke &
// Bartlett (2004), "Application of Surface Sound Speed Measurements in Post-Processing
// for Multi-Sector Multibeam Echosounders", Int. Hydrographic Review 5(3):26-31 (as
// reimplemented in beamdirections.hpp). The per-leg layered-Snell ray tracing follows
// Mohammadloo, Snellen, Renoud, Beaudoin & Simons (2019), "Correcting Multibeam
// Echosounder Bathymetric Measurements for Errors Induced by Inaccurate Water Column
// Sound Speeds", IEEE Access 7 (CC BY 4.0), and the two-point / eigenray formulation
// in M. B. Porter, "The BELLHOP Manual and User's Guide" (HLS Research). The two-leg,
// travel-time-constrained intersection is solved from first principles here.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/bistaticraytracer.doc.hpp"

#include "beamdirections.hpp"
#include "beamtrace.hpp"
#include "soundvelocityprofile.hpp"
#include "tracebeam.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/rotation.hpp>

#include <themachinethatgoesping/navigation/datastructures/positionaloffsets.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief True-bistatic trace of a single multibeam beam: two refracted legs meeting at the seabed.
 *
 * Stores the transmit leg and the receive leg each as a BeamTrace (the per-layer
 * polyline in that leg's own vertical plane, exactly as trace_beam produces them),
 * together with the horizontal azimuth of each leg (used to lift its 2-D polyline into
 * the common ship frame), the solved seabed point (forward, starboard, down) and the
 * final solver residual. Launch angles, the seabed incidence and the modelled two-way
 * travel time are derived from the two legs on access rather than stored.
 *
 * The per-layer TRANSMIT ray direction used for backscatter is the transmit leg's
 * incident-angle series (BeamTrace::get_incident_angles_in_degrees /
 * get_cos_incident_angles); its value at the last point is the seabed incidence.
 *
 * A 2-D leg point (horizontal_offset, depth) is lifted into the ship frame by the leg
 * azimuth psi and that leg's array position P:
 *   x_forward   = P_forward   - horizontal_offset * sin(psi)
 *   y_starboard = P_starboard + horizontal_offset * cos(psi)
 *   z_down      = depth
 */
class BistaticBeamTrace
{
    BeamTrace            _transmit_leg;
    BeamTrace            _receive_leg;
    float                _transmit_azimuth_in_degrees = 0.f;
    float                _receive_azimuth_in_degrees  = 0.f;
    std::array<float, 3> _bottom_position           = { 0.f, 0.f, 0.f }; // forward, starboard, down
    float                _solver_residual_in_meters = 0.f;

  public:
    BistaticBeamTrace() = default;

    /**
     * @brief Construct from the two converged legs and the solved seabed point.
     *
     * @param transmit_leg                per-layer transmit polyline (from trace_beam).
     * @param receive_leg                 per-layer receive polyline (from trace_beam).
     * @param transmit_azimuth_in_degrees azimuth (deg, forward->starboard) of the transmit plane.
     * @param receive_azimuth_in_degrees  azimuth (deg, forward->starboard) of the receive plane.
     * @param bottom_position             solved seabed point (forward, starboard, down) in m.
     * @param solver_residual_in_meters   final solver residual in m.
     */
    BistaticBeamTrace(BeamTrace            transmit_leg,
                      BeamTrace            receive_leg,
                      float                transmit_azimuth_in_degrees,
                      float                receive_azimuth_in_degrees,
                      std::array<float, 3> bottom_position,
                      float                solver_residual_in_meters)
        : _transmit_leg(std::move(transmit_leg))
        , _receive_leg(std::move(receive_leg))
        , _transmit_azimuth_in_degrees(transmit_azimuth_in_degrees)
        , _receive_azimuth_in_degrees(receive_azimuth_in_degrees)
        , _bottom_position(bottom_position)
        , _solver_residual_in_meters(solver_residual_in_meters)
    {
    }

    bool operator==(const BistaticBeamTrace& other) const
    {
        return _transmit_leg == other._transmit_leg && _receive_leg == other._receive_leg &&
               _transmit_azimuth_in_degrees == other._transmit_azimuth_in_degrees &&
               _receive_azimuth_in_degrees == other._receive_azimuth_in_degrees &&
               _bottom_position == other._bottom_position &&
               _solver_residual_in_meters == other._solver_residual_in_meters;
    }

    // ----------------------------------------------------------------- stored quantities

    /// @brief Transmit leg polyline (per-layer points in the transmit vertical plane).
    const BeamTrace& get_transmit_leg() const { return _transmit_leg; }
    /// @brief Receive leg polyline (per-layer points in the receive vertical plane).
    const BeamTrace& get_receive_leg() const { return _receive_leg; }

    /// @brief Transmit leg azimuth (deg): rotation about the down axis, BeamDirections convention.
    float get_transmit_azimuth_in_degrees() const { return _transmit_azimuth_in_degrees; }
    /// @brief Receive leg azimuth (deg): rotation about the down axis, BeamDirections convention.
    float get_receive_azimuth_in_degrees() const { return _receive_azimuth_in_degrees; }

    /// @brief Solved seabed point (forward, starboard, down) in the common input frame [m].
    const std::array<float, 3>& get_bottom_position() const { return _bottom_position; }

    /// @brief Final solver residual [m]; small values indicate a converged bistatic solve.
    float get_solver_residual_in_meters() const { return _solver_residual_in_meters; }

    // ---------------------------------------------------------------- derived quantities

    /// @brief Transmit-leg launch angle (deg from nadir, port +) at the transmit array.
    float get_transmit_launch_angle_in_degrees() const
    {
        return _transmit_leg.get_incident_angles_in_degrees().unchecked(0);
    }
    /// @brief Receive-leg launch angle (deg from nadir, port +) at the receive array.
    float get_receive_launch_angle_in_degrees() const
    {
        return _receive_leg.get_incident_angles_in_degrees().unchecked(0);
    }

    /// @brief Seabed incidence angle (deg from nadir, signed) of the TRANSMIT ray, for backscatter.
    float get_bottom_incidence_angle_in_degrees() const
    {
        const auto angles = _transmit_leg.get_incident_angles_in_degrees();
        return angles.size() ? angles.unchecked(angles.size() - 1) : 0.f;
    }

    /// @brief Modelled two-way travel time [s] = transmit one-way + receive one-way.
    float get_two_way_travel_time_in_seconds() const
    {
        return last_two_way_(_transmit_leg) + last_two_way_(_receive_leg);
    }

  private:
    // Each stored leg is a trace_beam result whose two-way-time series ends at 2x the
    // one-way time to the seabed; half of the last sample is therefore this leg's
    // one-way travel time.
    static float last_two_way_(const BeamTrace& leg)
    {
        const auto& times = leg.get_two_way_travel_times_in_seconds();
        return times.size() ? 0.5f * times.unchecked(times.size() - 1) : 0.f;
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BistaticBeamTrace", float_precision, superscript_exponents);

        printer.register_container("bottom_position", _bottom_position, "m (fwd, stbd, down)");
        printer.register_value("transmit_azimuth", _transmit_azimuth_in_degrees, "deg");
        printer.register_value("receive_azimuth", _receive_azimuth_in_degrees, "deg");
        printer.register_value("solver_residual", _solver_residual_in_meters, "m");

        printer.register_section("derived");
        printer.register_value(
            "transmit_launch_angle", get_transmit_launch_angle_in_degrees(), "deg");
        printer.register_value(
            "receive_launch_angle", get_receive_launch_angle_in_degrees(), "deg");
        printer.register_value(
            "bottom_incidence_angle", get_bottom_incidence_angle_in_degrees(), "deg");
        printer.register_value("two_way_travel_time", get_two_way_travel_time_in_seconds(), "s");

        return printer;
    }

    static BistaticBeamTrace from_stream(std::istream& is)
    {
        BistaticBeamTrace object;
        object._transmit_leg = BeamTrace::from_stream(is);
        object._receive_leg  = BeamTrace::from_stream(is);
        is.read(reinterpret_cast<char*>(&object._transmit_azimuth_in_degrees), sizeof(float));
        is.read(reinterpret_cast<char*>(&object._receive_azimuth_in_degrees), sizeof(float));
        is.read(reinterpret_cast<char*>(object._bottom_position.data()), sizeof(float) * 3);
        is.read(reinterpret_cast<char*>(&object._solver_residual_in_meters), sizeof(float));
        return object;
    }

    void to_stream(std::ostream& os) const
    {
        _transmit_leg.to_stream(os);
        _receive_leg.to_stream(os);
        os.write(reinterpret_cast<const char*>(&_transmit_azimuth_in_degrees), sizeof(float));
        os.write(reinterpret_cast<const char*>(&_receive_azimuth_in_degrees), sizeof(float));
        os.write(reinterpret_cast<const char*>(_bottom_position.data()), sizeof(float) * 3);
        os.write(reinterpret_cast<const char*>(&_solver_residual_in_meters), sizeof(float));
    }

  public:
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BistaticBeamTrace)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

namespace bistatic_detail {

/**
 * @brief One-parameter family of unit rays with a fixed projection onto an array axis.
 *
 * A linear array only constrains the beam's angle to the array axis: every unit ray
 * whose dot product with the array axis equals the steering projection lies on a cone
 * around that axis. Parametrising a leg by the rotation angle around this cone (rather
 * than by an azimuth/zenith pair) never degenerates - in particular for a flat array
 * with zero steering, where the "cone" opens into a plane and an azimuth parametrisation
 * would be singular. This is what lets the bistatic solver iterate robustly on the two
 * legs' cone angles plus the seabed depth.
 */
struct SteeringCone
{
    Eigen::Vector3f axis;       ///< unit array long axis (world frame)
    float           projection; ///< fixed dot(ray, axis) = sin(steering angle)
    Eigen::Vector3f basis_u;    ///< first unit vector spanning the plane orthogonal to axis
    Eigen::Vector3f basis_v;    ///< second orthonormal vector (axis, basis_u, basis_v right-handed)
    float           sine_half_angle; ///< radius of the cone circle = sqrt(1 - projection^2)

    /**
     * @brief Build the cone for a given array axis and steering projection.
     * @param array_axis      unit array long axis in the world frame.
     * @param axis_projection required dot(ray, axis) = sin(steering angle).
     */
    SteeringCone(const Eigen::Vector3f& array_axis, float axis_projection)
        : axis(array_axis)
        , projection(axis_projection)
    {
        // any vector not (near) parallel to the axis gives a stable orthogonal basis
        const Eigen::Vector3f helper = (std::abs(axis.z()) < 0.9f)
                                           ? Eigen::Vector3f(0.0f, 0.0f, 1.0f)
                                           : Eigen::Vector3f(1.0f, 0.0f, 0.0f);
        basis_u                      = axis.cross(helper).normalized();
        basis_v                      = axis.cross(basis_u);
        sine_half_angle              = std::sqrt(std::max(0.0f, 1.0f - projection * projection));
    }

    /// @brief Unit ray at rotation angle @p around_axis (rad) around the cone.
    Eigen::Vector3f ray(float around_axis) const
    {
        return projection * axis + sine_half_angle * (std::cos(around_axis) * basis_u +
                                                      std::sin(around_axis) * basis_v);
    }

    /// @brief Cone rotation angle (rad) whose ray best matches @p direction.
    float angle_of(const Eigen::Vector3f& direction) const
    {
        return std::atan2(direction.dot(basis_v), direction.dot(basis_u));
    }
};

} // namespace bistatic_detail

namespace bistatic_detail {

/**
 * @brief Core bistatic seabed solve, shared by the single-beam and batched entry points.
 *
 * Given each array's world-frame long axis, position and steering projection (already
 * heading-removed via the poses), traces both legs through the layered profile and finds the
 * seabed point where they meet with a combined one-way time equal to the measured two-way time,
 * via a damped Newton iteration seeded by the concentric beam direction. The seabed solve stays in
 * double because its finite-difference Jacobian (steps ~5e-5) would lose all significance in float.
 * Positions and axes are in the common x=forward, y=starboard, z=down ship frame.
 */
inline BistaticBeamTrace solve_bistatic_beam(
    const Eigen::Vector3f&      transmit_position,
    const Eigen::Vector3f&      transmit_axis,
    float                       transmit_projection,
    const Eigen::Vector3f&      receive_position,
    const Eigen::Vector3f&      receive_axis,
    float                       receive_projection,
    float                       two_way_travel_time_in_seconds,
    const SoundVelocityProfile& sound_velocity_profile,
    const std::array<float, 3>& concentric_beam_direction,
    int                         max_iterations,
    float                       tolerance_in_percent,
    std::optional<float>        surface_sound_speed_in_meters_per_second)
{
    constexpr float degrees_to_radians = 1.0f / 180.0f * 3.1415926535f;

    const SteeringCone transmit_cone(transmit_axis, transmit_projection);
    const SteeringCone receive_cone(receive_axis, receive_projection);

    // seed the two cone angles from the concentric beam direction
    const Eigen::Vector3f guess_direction(
        concentric_beam_direction[0], concentric_beam_direction[1], concentric_beam_direction[2]);
    const float initial_transmit_angle = transmit_cone.angle_of(guess_direction);
    const float initial_receive_angle  = receive_cone.angle_of(guess_direction);
    const float guess_takeoff_angle =
        std::atan2(std::hypot(guess_direction.x(), guess_direction.y()), guess_direction.z());
    const float guess_takeoff_sin_zenith = std::sin(guess_takeoff_angle);

    const size_t number_of_layers = sound_velocity_profile.get_number_of_layers();
    const float  profile_bottom_depth =
        sound_velocity_profile.get_depths_in_meters().unchecked(number_of_layers);
    const float reference_sound_speed =
        sound_velocity_profile.get_sound_speed(profile_bottom_depth);

    const float deepest_array_depth = std::max(transmit_position.z(), receive_position.z());
    const float midpoint_depth      = 0.5f * (transmit_position.z() + receive_position.z());

    // initial seabed depth by concentric bisection so the one-way time is about half the TWTT
    float initial_depth = 0.5f * (deepest_array_depth + profile_bottom_depth);
    {
        float depth_low  = deepest_array_depth + 1e-3f;
        float depth_high = profile_bottom_depth;
        for (int iteration = 0; iteration < 60 && depth_high - depth_low > 1e-4f; ++iteration)
        {
            initial_depth    = 0.5f * (depth_low + depth_high);
            const auto probe = tracebeam_detail::trace_to_depth_impl<false>(
                sound_velocity_profile,
                midpoint_depth,
                guess_takeoff_sin_zenith,
                initial_depth,
                surface_sound_speed_in_meters_per_second);
            if (!probe.reached_target ||
                probe.one_way_travel_time_in_seconds > 0.5f * two_way_travel_time_in_seconds)
                depth_high = initial_depth;
            else
                depth_low = initial_depth;
        }
    }

    const float relative_tolerance = std::max(tolerance_in_percent * 0.01f, 1e-5f);
    const float nominal_slant_range =
        std::max(0.5f * reference_sound_speed * two_way_travel_time_in_seconds, 1.0f);
    const float absolute_tolerance = relative_tolerance * nominal_slant_range;

    // solver state = (seabed depth, transmit cone angle, receive cone angle)
    Eigen::Vector3f state(initial_depth, initial_transmit_angle, initial_receive_angle);

    // residual(state) = ( transmit_x - receive_x,
    //                     transmit_y - receive_y,
    //                     ref_c * (transmit_one_way + receive_one_way - TWTT) )
    auto evaluate = [&](const Eigen::Vector3f& current,
                        Eigen::Vector3f&       residual,
                        float&                 transmit_sin_zenith,
                        float&                 receive_sin_zenith) -> bool {
        const float depth = current[0];
        if (!(depth > deepest_array_depth) || depth > profile_bottom_depth + 1e-3f)
            return false;

        const Eigen::Vector3f transmit_ray = transmit_cone.ray(current[1]);
        const Eigen::Vector3f receive_ray  = receive_cone.ray(current[2]);
        if (transmit_ray.z() <= 1e-6f || receive_ray.z() <= 1e-6f)
            return false; // ray points up or horizontal - cannot reach the seabed

        // sin(zenith) = sqrt(1 - cos^2) straight from the ray's down component (ray.z > 0 here),
        // avoiding an acos here and a sin inside the depth trace.
        transmit_sin_zenith = std::sqrt(std::max(0.0f, 1.0f - transmit_ray.z() * transmit_ray.z()));
        receive_sin_zenith  = std::sqrt(std::max(0.0f, 1.0f - receive_ray.z() * receive_ray.z()));

        const auto transmit_leg = tracebeam_detail::trace_to_depth_impl<false>(
            sound_velocity_profile,
            transmit_position.z(),
            transmit_sin_zenith,
            depth,
            surface_sound_speed_in_meters_per_second);
        const auto receive_leg = tracebeam_detail::trace_to_depth_impl<false>(
            sound_velocity_profile,
            receive_position.z(),
            receive_sin_zenith,
            depth,
            surface_sound_speed_in_meters_per_second);
        if (!transmit_leg.reached_target || !receive_leg.reached_target)
            return false;

        const float transmit_azimuth = std::atan2(transmit_ray.y(), transmit_ray.x());
        const float receive_azimuth  = std::atan2(receive_ray.y(), receive_ray.x());

        const float transmit_x = transmit_position.x() + transmit_leg.horizontal_offset_in_meters *
                                                             std::cos(transmit_azimuth);
        const float transmit_y = transmit_position.y() + transmit_leg.horizontal_offset_in_meters *
                                                             std::sin(transmit_azimuth);
        const float receive_x = receive_position.x() +
                                receive_leg.horizontal_offset_in_meters * std::cos(receive_azimuth);
        const float receive_y = receive_position.y() +
                                receive_leg.horizontal_offset_in_meters * std::sin(receive_azimuth);

        residual[0] = transmit_x - receive_x;
        residual[1] = transmit_y - receive_y;
        residual[2] = reference_sound_speed *
                      (transmit_leg.one_way_travel_time_in_seconds +
                       receive_leg.one_way_travel_time_in_seconds - two_way_travel_time_in_seconds);
        return true;
    };

    Eigen::Vector3f residual;
    float           transmit_sin_zenith = guess_takeoff_sin_zenith;
    float           receive_sin_zenith  = guess_takeoff_sin_zenith;
    bool            ok = evaluate(state, residual, transmit_sin_zenith, receive_sin_zenith);

    Eigen::Vector3f best_state               = state;
    float           best_residual_norm       = ok ? residual.norm() : std::numeric_limits<float>::max();
    float           best_transmit_sin_zenith = transmit_sin_zenith;
    float           best_receive_sin_zenith  = receive_sin_zenith;

    const std::array<float, 3> finite_difference_steps = { 5e-3f, 5e-5f, 5e-5f };

    for (int iteration = 0; ok && iteration < max_iterations; ++iteration)
    {
        if (residual.norm() < absolute_tolerance)
            break;

        Eigen::Matrix3f jacobian;
        bool            jacobian_ok = true;
        for (int column = 0; column < 3; ++column)
        {
            Eigen::Vector3f perturbed_state = state;
            perturbed_state[column] += finite_difference_steps[column];
            Eigen::Vector3f perturbed_residual;
            float           dummy_transmit_sin_zenith, dummy_receive_sin_zenith;
            if (!evaluate(perturbed_state,
                          perturbed_residual,
                          dummy_transmit_sin_zenith,
                          dummy_receive_sin_zenith))
            {
                jacobian_ok = false;
                break;
            }
            jacobian.col(column) =
                (perturbed_residual - residual) / finite_difference_steps[column];
        }
        if (!jacobian_ok)
            break;

        const Eigen::Vector3f step = jacobian.colPivHouseholderQr().solve((-residual).eval());
        if (!step.allFinite())
            break;

        // damp the step: bounded depth move and bounded cone-angle move keep the solve stable
        Eigen::Vector3f damped_step    = step;
        const float     max_depth_step = std::max(1.0f, 0.5f * (state[0] - deepest_array_depth));
        damped_step[0] = std::clamp(damped_step[0], -max_depth_step, max_depth_step);
        damped_step[1] = std::clamp(damped_step[1], -0.3f, 0.3f);
        damped_step[2] = std::clamp(damped_step[2], -0.3f, 0.3f);
        state += damped_step;

        ok = evaluate(state, residual, transmit_sin_zenith, receive_sin_zenith);
        if (!ok)
            break;

        if (residual.norm() < best_residual_norm)
        {
            best_residual_norm       = residual.norm();
            best_state               = state;
            best_transmit_sin_zenith = transmit_sin_zenith;
            best_receive_sin_zenith  = receive_sin_zenith;
        }
    }

    // ---- build the output polylines with trace_beam (shared with the monostatic model) ---
    const Eigen::Vector3f transmit_ray = transmit_cone.ray(best_state[1]);
    const Eigen::Vector3f receive_ray  = receive_cone.ray(best_state[2]);

    const std::array<float, 2> transmit_pointing_azimuth =
        beam_direction_to_pointing_and_azimuth_in_degrees(
            transmit_ray.x(), transmit_ray.y(), transmit_ray.z());
    const std::array<float, 2> receive_pointing_azimuth =
        beam_direction_to_pointing_and_azimuth_in_degrees(
            receive_ray.x(), receive_ray.y(), receive_ray.z());

    const auto transmit_endpoint = tracebeam_detail::trace_to_depth_impl<false>(
        sound_velocity_profile,
        transmit_position.z(),
        best_transmit_sin_zenith,
        best_state[0],
        surface_sound_speed_in_meters_per_second);
    const auto receive_endpoint = tracebeam_detail::trace_to_depth_impl<false>(
        sound_velocity_profile,
        receive_position.z(),
        best_receive_sin_zenith,
        best_state[0],
        surface_sound_speed_in_meters_per_second);

    BeamTrace transmit_leg = trace_beam(float(transmit_position.z()),
                                        transmit_pointing_azimuth[0],
                                        sound_velocity_profile,
                                        2.f * transmit_endpoint.one_way_travel_time_in_seconds,
                                        surface_sound_speed_in_meters_per_second);
    BeamTrace receive_leg  = trace_beam(float(receive_position.z()),
                                       receive_pointing_azimuth[0],
                                       sound_velocity_profile,
                                       2.f * receive_endpoint.one_way_travel_time_in_seconds,
                                       surface_sound_speed_in_meters_per_second);

    // seabed point from the transmit leg's last point, lifted by the transmit azimuth. This is
    // exactly the monostatic reconstruction, so with identical transmit/receive poses the
    // bistatic seabed matches the concentric one.
    const auto& transmit_depths     = transmit_leg.get_depths_in_meters();
    const auto& transmit_horizontal = transmit_leg.get_horizontal_offsets_in_meters();
    const float last_horizontal_offset =
        transmit_horizontal.size() ? transmit_horizontal.unchecked(transmit_horizontal.size() - 1)
                                   : 0.f;
    const float last_depth               = transmit_depths.size()
                                               ? transmit_depths.unchecked(transmit_depths.size() - 1)
                                               : best_state[0];
    const float transmit_azimuth_radians = transmit_pointing_azimuth[1] * degrees_to_radians;

    const std::array<float, 3> bottom_position = {
        transmit_position.x() - last_horizontal_offset * std::sin(transmit_azimuth_radians),
        transmit_position.y() + last_horizontal_offset * std::cos(transmit_azimuth_radians),
        last_depth
    };

    return BistaticBeamTrace(std::move(transmit_leg),
                             std::move(receive_leg),
                             transmit_pointing_azimuth[1],
                             receive_pointing_azimuth[1],
                             bottom_position,
                             best_residual_norm);
}

} // namespace bistatic_detail

/**
 * @brief Solve the true-bistatic seabed trace of a single multibeam beam from ready-made poses.
 *
 * The transmit and receive poses already carry the array installation, the vessel attitude and
 * the removal of a common reference heading (e.g. from SensorConfiguration::compute_target_pose),
 * so this only places each array's long axis (transmit = forward, receive = starboard), applies
 * the electronic steering and runs the shared bistatic solve. All quantities are in the common
 * x=forward, y=starboard, z=down ship frame; the concentric guess must be in that same frame.
 *
 * @param transmit_pose transmit array pose (position + ship-frame orientation).
 * @param transmit_steering_angle_in_degrees electronic transmit steering (positive forward).
 * @param receive_pose receive array pose (position + ship-frame orientation).
 * @param receive_steering_angle_in_degrees electronic receive steering (positive to port).
 * @param two_way_travel_time_in_seconds measured two-way travel time [s].
 * @param sound_velocity_profile layered profile to trace through.
 * @param concentric_beam_direction ship-frame unit guess (fwd, stbd, down), e.g.
 *        BeamDirections::get_beam_direction(beam).
 * @param max_iterations maximum Newton iterations (default 30).
 * @param tolerance_in_percent convergence tolerance as a percentage of the nominal slant range.
 * @param surface_sound_speed_in_meters_per_second sound speed (m/s) the beams were formed at;
 *        std::nullopt (default) uses the profile value at each array depth.
 * @return BistaticBeamTrace with both legs, azimuths, seabed point and residual.
 */
inline BistaticBeamTrace trace_bistatic_beam(
    const navigation::datastructures::SensorPose& transmit_pose,
    float                                                transmit_steering_angle_in_degrees,
    const navigation::datastructures::SensorPose& receive_pose,
    float                                                receive_steering_angle_in_degrees,
    float                                                two_way_travel_time_in_seconds,
    const SoundVelocityProfile&                          sound_velocity_profile,
    const std::array<float, 3>&                          concentric_beam_direction,
    int                                                  max_iterations       = 30,
    float                                                tolerance_in_percent = 0.001f,
    std::optional<float> surface_sound_speed_in_meters_per_second             = std::nullopt)
{
    if (sound_velocity_profile.get_number_of_layers() == 0)
        throw std::runtime_error("trace_bistatic_beam: sound velocity profile is not initialized");

    constexpr float degrees_to_radians = 1.0f / 180.0f * 3.1415926535f;

    const Eigen::Vector3f transmit_position(transmit_pose.x, transmit_pose.y, transmit_pose.z);
    const Eigen::Vector3f receive_position(receive_pose.x, receive_pose.y, receive_pose.z);

    // The poses already carry installation + attitude + heading removal, so each world axis is just
    // pose.rotation applied to the array long axis (transmit = forward, receive = starboard).
    const Eigen::Vector3f transmit_axis = (transmit_pose.rotation * Eigen::Vector3f(1.f, 0.f, 0.f));
    const Eigen::Vector3f receive_axis  = (receive_pose.rotation * Eigen::Vector3f(0.f, 1.f, 0.f));

    const float transmit_projection =
        std::sin(degrees_to_radians * transmit_steering_angle_in_degrees);
    const float receive_projection =
        -std::sin(degrees_to_radians * receive_steering_angle_in_degrees);

    return bistatic_detail::solve_bistatic_beam(transmit_position,
                                                transmit_axis,
                                                transmit_projection,
                                                receive_position,
                                                receive_axis,
                                                receive_projection,
                                                two_way_travel_time_in_seconds,
                                                sound_velocity_profile,
                                                concentric_beam_direction,
                                                max_iterations,
                                                tolerance_in_percent,
                                                surface_sound_speed_in_meters_per_second);
}

/**
 * @brief Batched true-bistatic trace of a sector: one shared transmit pose, N receive poses.
 *
 * Equivalent to calling trace_bistatic_beam once per beam, but the transmit side is placed once and
 * the per-beam solves run in a single C++ loop (parallelisable via @p mp_cores), avoiding the
 * per-beam Python round-trips. The concentric guesses come from @p concentric_beam_directions
 * (e.g. the output of compute_beam_directions).
 *
 * @param transmit_pose shared transmit array pose (position + ship-frame orientation).
 * @param transmit_steering_angle_in_degrees shared electronic transmit steering (positive forward).
 * @param receive_poses per-beam receive array poses (size n_beams).
 * @param receive_steering_angles_in_degrees [n_beams] electronic receive steering (positive to
 * port).
 * @param two_way_travel_times_in_seconds [n_beams] measured two-way travel times [s].
 * @param sound_velocity_profile layered profile to trace through.
 * @param concentric_beam_directions [n_beams] ship-frame concentric guesses.
 * @param max_iterations maximum Newton iterations (default 30).
 * @param tolerance_in_percent convergence tolerance (% of nominal slant range).
 * @param surface_sound_speed_in_meters_per_second sound speed (m/s) the beams were formed at.
 * @param mp_cores number of OpenMP cores for the per-beam solve (default 1).
 * @return vector of BistaticBeamTrace, one per beam.
 */
inline std::vector<BistaticBeamTrace> trace_bistatic_beams(
    const navigation::datastructures::SensorPose& transmit_pose,
    float                                                transmit_steering_angle_in_degrees,
    const std::vector<navigation::datastructures::SensorPose>& receive_poses,
    const xt::xtensor<float, 1>& receive_steering_angles_in_degrees,
    const xt::xtensor<float, 1>& two_way_travel_times_in_seconds,
    const SoundVelocityProfile&  sound_velocity_profile,
    const BeamDirections&        concentric_beam_directions,
    int                          max_iterations                           = 30,
    float                        tolerance_in_percent                     = 0.001f,
    std::optional<float>         surface_sound_speed_in_meters_per_second = std::nullopt,
    int                          mp_cores                                 = 1)
{
    const size_t number_of_beams = receive_poses.size();

    if (receive_steering_angles_in_degrees.size() != number_of_beams ||
        two_way_travel_times_in_seconds.size() != number_of_beams ||
        concentric_beam_directions.get_number_of_beams() != number_of_beams)
        throw std::invalid_argument(
            "trace_bistatic_beams: inconsistent input shapes (receive_poses, receive steering, "
            "two-way travel times and concentric_beam_directions must all have size n_beams).");

    if (sound_velocity_profile.get_number_of_layers() == 0)
        throw std::runtime_error("trace_bistatic_beams: sound velocity profile is not initialized");

    if (mp_cores < 1)
        mp_cores = 1;

    constexpr float degrees_to_radians = 1.0f / 180.0f * 3.1415926535f;

    // Transmit side is shared by every beam; compute it once.
    const Eigen::Vector3f transmit_position(transmit_pose.x, transmit_pose.y, transmit_pose.z);
    const Eigen::Vector3f transmit_axis = (transmit_pose.rotation * Eigen::Vector3f(1.f, 0.f, 0.f));
    const float           transmit_projection =
        std::sin(degrees_to_radians * transmit_steering_angle_in_degrees);

    std::vector<BistaticBeamTrace> results(number_of_beams);

#pragma omp parallel for num_threads(mp_cores)
    for (int64_t beam_index = 0; beam_index < int64_t(number_of_beams); ++beam_index)
    {
        const auto&           receive_pose = receive_poses[beam_index];
        const Eigen::Vector3f receive_position(receive_pose.x, receive_pose.y, receive_pose.z);
        const Eigen::Vector3f receive_axis =
            (receive_pose.rotation * Eigen::Vector3f(0.f, 1.f, 0.f));
        const float receive_projection =
            -std::sin(degrees_to_radians * receive_steering_angles_in_degrees(beam_index));

        results[beam_index] = bistatic_detail::solve_bistatic_beam(
            transmit_position,
            transmit_axis,
            transmit_projection,
            receive_position,
            receive_axis,
            receive_projection,
            two_way_travel_times_in_seconds(beam_index),
            sound_velocity_profile,
            concentric_beam_directions.get_beam_direction(beam_index),
            max_iterations,
            tolerance_in_percent,
            surface_sound_speed_in_meters_per_second);
    }

    return results;
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping