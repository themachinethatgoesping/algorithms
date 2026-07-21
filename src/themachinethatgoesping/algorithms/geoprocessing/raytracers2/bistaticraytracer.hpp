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
#include <stdexcept>
#include <utility>

#include <fmt/format.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

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
    std::array<float, 3> _bottom_position             = { 0.f, 0.f, 0.f }; // forward, starboard, down
    float                _solver_residual_in_meters   = 0.f;

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
        printer.register_value("receive_launch_angle", get_receive_launch_angle_in_degrees(), "deg");
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
    Eigen::Vector3d axis;            ///< unit array long axis (world frame)
    double          projection;      ///< fixed dot(ray, axis) = sin(steering angle)
    Eigen::Vector3d basis_u;         ///< first unit vector spanning the plane orthogonal to axis
    Eigen::Vector3d basis_v;         ///< second orthonormal vector (axis, basis_u, basis_v right-handed)
    double          sine_half_angle; ///< radius of the cone circle = sqrt(1 - projection^2)

    /**
     * @brief Build the cone for a given array axis and steering projection.
     * @param array_axis      unit array long axis in the world frame.
     * @param axis_projection required dot(ray, axis) = sin(steering angle).
     */
    SteeringCone(const Eigen::Vector3d& array_axis, double axis_projection)
        : axis(array_axis)
        , projection(axis_projection)
    {
        // any vector not (near) parallel to the axis gives a stable orthogonal basis
        const Eigen::Vector3d helper =
            (std::abs(axis.z()) < 0.9) ? Eigen::Vector3d(0.0, 0.0, 1.0) : Eigen::Vector3d(1.0, 0.0, 0.0);
        basis_u         = axis.cross(helper).normalized();
        basis_v         = axis.cross(basis_u);
        sine_half_angle = std::sqrt(std::max(0.0, 1.0 - projection * projection));
    }

    /// @brief Unit ray at rotation angle @p around_axis (rad) around the cone.
    Eigen::Vector3d ray(double around_axis) const
    {
        return projection * axis +
               sine_half_angle * (std::cos(around_axis) * basis_u + std::sin(around_axis) * basis_v);
    }

    /// @brief Cone rotation angle (rad) whose ray best matches @p direction.
    double angle_of(const Eigen::Vector3d& direction) const
    {
        return std::atan2(direction.dot(basis_v), direction.dot(basis_u));
    }
};

} // namespace bistatic_detail

/**
 * @brief Solve the true-bistatic seabed trace of a single multibeam beam.
 *
 * Traces the transmit ray from the transmit array and the receive ray from the receive
 * array through the layered sound-velocity profile and finds the seabed point where the
 * two legs meet with a combined one-way travel time equal to the measured two-way travel
 * time. The seabed depth and each leg's cone rotation angle (see bistatic_detail::
 * SteeringCone) are found with a damped Newton iteration seeded by the concentric beam
 * direction. Each converged leg is then re-traced with trace_beam so the returned legs
 * are identical to the monostatic model when the transmit and receive poses coincide.
 *
 * All poses are in the common x=forward, y=starboard, z=down frame; the concentric guess
 * must be in the same frame (compute_beam_directions with reference_heading = 0).
 *
 * @param transmit_installation_ypr_in_degrees (yaw, pitch, roll) mounting of the transmit array.
 * @param transmit_attitude_ypr_in_degrees     (yaw, pitch, roll) vessel attitude at transmit time.
 * @param transmit_steering_angle_in_degrees   electronic transmit steering (positive forward).
 * @param transmit_position_xyz                transmit array position (forward, starboard, down) [m].
 * @param receive_installation_ypr_in_degrees  (yaw, pitch, roll) mounting of the receive array.
 * @param receive_attitude_ypr_in_degrees      (yaw, pitch, roll) vessel attitude at receive time.
 * @param receive_steering_angle_in_degrees    electronic receive steering (positive to port).
 * @param receive_position_xyz                 receive array position (forward, starboard, down) [m].
 * @param two_way_travel_time_in_seconds       measured two-way travel time [s].
 * @param sound_velocity_profile               layered profile to trace through.
 * @param concentric_beam_direction            ship-frame unit guess (fwd, stbd, down),
 *                                             e.g. BeamDirections::get_beam_direction(beam).
 * @param max_iterations                       maximum Newton iterations (default 30).
 * @param tolerance_in_percent                 convergence tolerance as a percentage of the
 *                                             nominal slant range (default 0.001).
 * @return BistaticBeamTrace with both legs, azimuths, seabed point and residual.
 */
inline BistaticBeamTrace trace_bistatic_beam(
    const std::array<double, 3>& transmit_installation_ypr_in_degrees,
    const std::array<double, 3>& transmit_attitude_ypr_in_degrees,
    double                       transmit_steering_angle_in_degrees,
    const std::array<double, 3>& transmit_position_xyz,
    const std::array<double, 3>& receive_installation_ypr_in_degrees,
    const std::array<double, 3>& receive_attitude_ypr_in_degrees,
    double                       receive_steering_angle_in_degrees,
    const std::array<double, 3>& receive_position_xyz,
    double                       two_way_travel_time_in_seconds,
    const SoundVelocityProfile&  sound_velocity_profile,
    const std::array<float, 3>&  concentric_beam_direction,
    int                          max_iterations       = 30,
    float                        tolerance_in_percent = 0.001f)
{
    using tools::rotationfunctions::quaternion_from_ypr;

    if (sound_velocity_profile.get_number_of_layers() == 0)
        throw std::runtime_error("trace_bistatic_beam: sound velocity profile is not initialized");

    constexpr double degrees_to_radians = M_PI / 180.0;

    const Eigen::Vector3d transmit_position(
        transmit_position_xyz[0], transmit_position_xyz[1], transmit_position_xyz[2]);
    const Eigen::Vector3d receive_position(
        receive_position_xyz[0], receive_position_xyz[1], receive_position_xyz[2]);

    // World orientation of each array: world = attitude * installation (as in
    // compute_beam_directions). Transmit long axis = forward, receive long axis = starboard.
    const Eigen::Quaterniond transmit_quaternion =
        quaternion_from_ypr<double>(transmit_attitude_ypr_in_degrees[0],
                                    transmit_attitude_ypr_in_degrees[1],
                                    transmit_attitude_ypr_in_degrees[2],
                                    true) *
        quaternion_from_ypr<double>(transmit_installation_ypr_in_degrees, true);
    const Eigen::Quaterniond receive_quaternion =
        quaternion_from_ypr<double>(receive_attitude_ypr_in_degrees[0],
                                    receive_attitude_ypr_in_degrees[1],
                                    receive_attitude_ypr_in_degrees[2],
                                    true) *
        quaternion_from_ypr<double>(receive_installation_ypr_in_degrees, true);

    const Eigen::Vector3d transmit_axis = transmit_quaternion * Eigen::Vector3d(1.0, 0.0, 0.0);
    const Eigen::Vector3d receive_axis  = receive_quaternion * Eigen::Vector3d(0.0, 1.0, 0.0);

    // steering fixes the projection of the beam onto each array axis (receive positive to port)
    const double transmit_projection =
        std::sin(degrees_to_radians * transmit_steering_angle_in_degrees);
    const double receive_projection =
        -std::sin(degrees_to_radians * receive_steering_angle_in_degrees);

    const bistatic_detail::SteeringCone transmit_cone(transmit_axis, transmit_projection);
    const bistatic_detail::SteeringCone receive_cone(receive_axis, receive_projection);

    // seed the two cone angles from the concentric beam direction
    const Eigen::Vector3d guess_direction(concentric_beam_direction[0],
                                          concentric_beam_direction[1],
                                          concentric_beam_direction[2]);
    const double initial_transmit_angle = transmit_cone.angle_of(guess_direction);
    const double initial_receive_angle  = receive_cone.angle_of(guess_direction);
    const double guess_takeoff_angle =
        std::atan2(std::hypot(guess_direction.x(), guess_direction.y()), guess_direction.z());

    const size_t number_of_layers = sound_velocity_profile.get_number_of_layers();
    const double profile_bottom_depth =
        sound_velocity_profile.get_depths_in_meters().unchecked(number_of_layers);
    const double reference_sound_speed =
        sound_velocity_profile.get_sound_speed(float(profile_bottom_depth));

    const double deepest_array_depth = std::max(transmit_position.z(), receive_position.z());
    const double midpoint_depth      = 0.5 * (transmit_position.z() + receive_position.z());

    // initial seabed depth by concentric bisection so the one-way time is about half the TWTT
    double initial_depth = 0.5 * (deepest_array_depth + profile_bottom_depth);
    {
        double depth_low  = deepest_array_depth + 1e-3;
        double depth_high = profile_bottom_depth;
        for (int iteration = 0; iteration < 60 && depth_high - depth_low > 1e-4; ++iteration)
        {
            initial_depth   = 0.5 * (depth_low + depth_high);
            const auto probe = trace_beam_to_depth(
                sound_velocity_profile, midpoint_depth, guess_takeoff_angle, initial_depth);
            if (!probe.reached_target ||
                probe.one_way_travel_time_in_seconds > 0.5 * two_way_travel_time_in_seconds)
                depth_high = initial_depth;
            else
                depth_low = initial_depth;
        }
    }

    const double relative_tolerance = std::max(double(tolerance_in_percent) * 0.01, 1e-12);
    const double nominal_slant_range =
        std::max(0.5 * reference_sound_speed * two_way_travel_time_in_seconds, 1.0);
    const double absolute_tolerance = relative_tolerance * nominal_slant_range;

    // solver state = (seabed depth, transmit cone angle, receive cone angle)
    Eigen::Vector3d state(initial_depth, initial_transmit_angle, initial_receive_angle);

    // residual(state) = ( transmit_x - receive_x,
    //                     transmit_y - receive_y,
    //                     ref_c * (transmit_one_way + receive_one_way - TWTT) )
    auto evaluate = [&](const Eigen::Vector3d& current,
                        Eigen::Vector3d&       residual,
                        double&                transmit_zenith,
                        double&                receive_zenith) -> bool {
        const double depth = current[0];
        if (!(depth > deepest_array_depth) || depth > profile_bottom_depth + 1e-3)
            return false;

        const Eigen::Vector3d transmit_ray = transmit_cone.ray(current[1]);
        const Eigen::Vector3d receive_ray  = receive_cone.ray(current[2]);
        if (transmit_ray.z() <= 1e-6 || receive_ray.z() <= 1e-6)
            return false; // ray points up or horizontal - cannot reach the seabed

        transmit_zenith = std::acos(std::clamp(transmit_ray.z(), -1.0, 1.0));
        receive_zenith  = std::acos(std::clamp(receive_ray.z(), -1.0, 1.0));

        const auto transmit_leg = trace_beam_to_depth(
            sound_velocity_profile, transmit_position.z(), transmit_zenith, depth);
        const auto receive_leg = trace_beam_to_depth(
            sound_velocity_profile, receive_position.z(), receive_zenith, depth);
        if (!transmit_leg.reached_target || !receive_leg.reached_target)
            return false;

        const double transmit_azimuth = std::atan2(transmit_ray.y(), transmit_ray.x());
        const double receive_azimuth  = std::atan2(receive_ray.y(), receive_ray.x());

        const double transmit_x =
            transmit_position.x() + transmit_leg.horizontal_offset_in_meters * std::cos(transmit_azimuth);
        const double transmit_y =
            transmit_position.y() + transmit_leg.horizontal_offset_in_meters * std::sin(transmit_azimuth);
        const double receive_x =
            receive_position.x() + receive_leg.horizontal_offset_in_meters * std::cos(receive_azimuth);
        const double receive_y =
            receive_position.y() + receive_leg.horizontal_offset_in_meters * std::sin(receive_azimuth);

        residual[0] = transmit_x - receive_x;
        residual[1] = transmit_y - receive_y;
        residual[2] = reference_sound_speed *
                      (double(transmit_leg.one_way_travel_time_in_seconds) +
                       double(receive_leg.one_way_travel_time_in_seconds) -
                       two_way_travel_time_in_seconds);
        return true;
    };

    Eigen::Vector3d residual;
    double          transmit_zenith = guess_takeoff_angle;
    double          receive_zenith  = guess_takeoff_angle;
    bool            ok = evaluate(state, residual, transmit_zenith, receive_zenith);

    Eigen::Vector3d best_state           = state;
    double          best_residual_norm   = ok ? residual.norm() : std::numeric_limits<double>::max();
    double          best_transmit_zenith = transmit_zenith;
    double          best_receive_zenith  = receive_zenith;

    const std::array<double, 3> finite_difference_steps = { 5e-3, 5e-5, 5e-5 };

    for (int iteration = 0; ok && iteration < max_iterations; ++iteration)
    {
        if (residual.norm() < absolute_tolerance)
            break;

        Eigen::Matrix3d jacobian;
        bool            jacobian_ok = true;
        for (int column = 0; column < 3; ++column)
        {
            Eigen::Vector3d perturbed_state = state;
            perturbed_state[column] += finite_difference_steps[column];
            Eigen::Vector3d perturbed_residual;
            double          dummy_transmit_zenith, dummy_receive_zenith;
            if (!evaluate(perturbed_state, perturbed_residual, dummy_transmit_zenith, dummy_receive_zenith))
            {
                jacobian_ok = false;
                break;
            }
            jacobian.col(column) = (perturbed_residual - residual) / finite_difference_steps[column];
        }
        if (!jacobian_ok)
            break;

        const Eigen::Vector3d step = jacobian.colPivHouseholderQr().solve((-residual).eval());
        if (!step.allFinite())
            break;

        // damp the step: bounded depth move and bounded cone-angle move keep the solve stable
        Eigen::Vector3d damped_step  = step;
        const double    max_depth_step = std::max(1.0, 0.5 * (state[0] - deepest_array_depth));
        damped_step[0]                = std::clamp(damped_step[0], -max_depth_step, max_depth_step);
        damped_step[1]                = std::clamp(damped_step[1], -0.3, 0.3);
        damped_step[2]                = std::clamp(damped_step[2], -0.3, 0.3);
        state += damped_step;

        ok = evaluate(state, residual, transmit_zenith, receive_zenith);
        if (!ok)
            break;

        if (residual.norm() < best_residual_norm)
        {
            best_residual_norm   = residual.norm();
            best_state           = state;
            best_transmit_zenith = transmit_zenith;
            best_receive_zenith  = receive_zenith;
        }
    }

    // ---- build the output polylines with trace_beam (shared with the monostatic model) ----
    const Eigen::Vector3d transmit_ray = transmit_cone.ray(best_state[1]);
    const Eigen::Vector3d receive_ray  = receive_cone.ray(best_state[2]);

    const std::array<float, 2> transmit_pointing_azimuth =
        beam_direction_to_pointing_and_azimuth_in_degrees(
            float(transmit_ray.x()), float(transmit_ray.y()), float(transmit_ray.z()));
    const std::array<float, 2> receive_pointing_azimuth =
        beam_direction_to_pointing_and_azimuth_in_degrees(
            float(receive_ray.x()), float(receive_ray.y()), float(receive_ray.z()));

    const auto transmit_endpoint = trace_beam_to_depth(
        sound_velocity_profile, transmit_position.z(), best_transmit_zenith, best_state[0]);
    const auto receive_endpoint = trace_beam_to_depth(
        sound_velocity_profile, receive_position.z(), best_receive_zenith, best_state[0]);

    BeamTrace transmit_leg = trace_beam(float(transmit_position.z()),
                                        transmit_pointing_azimuth[0],
                                        sound_velocity_profile,
                                        2.f * transmit_endpoint.one_way_travel_time_in_seconds);
    BeamTrace receive_leg  = trace_beam(float(receive_position.z()),
                                       receive_pointing_azimuth[0],
                                       sound_velocity_profile,
                                       2.f * receive_endpoint.one_way_travel_time_in_seconds);

    // seabed point from the transmit leg's last point, lifted by the transmit azimuth. This is
    // exactly the monostatic reconstruction, so with identical transmit/receive poses the
    // bistatic seabed matches the concentric one.
    const auto&  transmit_depths           = transmit_leg.get_depths_in_meters();
    const auto&  transmit_horizontal       = transmit_leg.get_horizontal_offsets_in_meters();
    const float  last_horizontal_offset    = transmit_horizontal.size()
                                                 ? transmit_horizontal.unchecked(transmit_horizontal.size() - 1)
                                                 : 0.f;
    const float  last_depth                = transmit_depths.size()
                                                 ? transmit_depths.unchecked(transmit_depths.size() - 1)
                                                 : float(best_state[0]);
    const float  transmit_azimuth_radians  = transmit_pointing_azimuth[1] * float(degrees_to_radians);

    const std::array<float, 3> bottom_position = {
        float(transmit_position.x()) - last_horizontal_offset * std::sin(transmit_azimuth_radians),
        float(transmit_position.y()) + last_horizontal_offset * std::cos(transmit_azimuth_radians),
        last_depth
    };

    return BistaticBeamTrace(std::move(transmit_leg),
                             std::move(receive_leg),
                             transmit_pointing_azimuth[1],
                             receive_pointing_azimuth[1],
                             bottom_position,
                             float(best_residual_norm));
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
