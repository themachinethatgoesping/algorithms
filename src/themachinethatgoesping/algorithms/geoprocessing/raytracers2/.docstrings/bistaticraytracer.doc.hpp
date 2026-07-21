//sourcehash: dfdd702b0efd331e89af0cedc67973530f2d0c751d54360ad002706cd73826c2

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__
#define __DOCSTRINGS_HPP__

#define MKD_EXPAND(x)                                      x
#define MKD_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...)  COUNT
#define MKD_VA_SIZE(...)                                   MKD_EXPAND(MKD_COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define MKD_CAT1(a, b)                                     a ## b
#define MKD_CAT2(a, b)                                     MKD_CAT1(a, b)
#define MKD_DOC1(n1)                                       mkd_doc_##n1
#define MKD_DOC2(n1, n2)                                   mkd_doc_##n1##_##n2
#define MKD_DOC3(n1, n2, n3)                               mkd_doc_##n1##_##n2##_##n3
#define MKD_DOC4(n1, n2, n3, n4)                           mkd_doc_##n1##_##n2##_##n3##_##n4
#define MKD_DOC5(n1, n2, n3, n4, n5)                       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5
#define MKD_DOC6(n1, n2, n3, n4, n5, n6)                   mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define MKD_DOC7(n1, n2, n3, n4, n5, n6, n7)               mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define MKD_DOC8(n1, n2, n3, n4, n5, n6, n7, n8)           mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define MKD_DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define MKD_DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...)                                           MKD_EXPAND(MKD_EXPAND(MKD_CAT2(MKD_DOC, MKD_VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#endif // __DOCSTRINGS_HPP__
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace =
R"doc(True-bistatic trace of a single multibeam beam: two refracted legs
meeting at the seabed.

Stores the transmit leg and the receive leg each as a BeamTrace (the
per-layer polyline in that leg's own vertical plane, exactly as
trace_beam produces them), together with the horizontal azimuth of
each leg (used to lift its 2-D polyline into the common ship frame),
the solved seabed point (forward, starboard, down) and the final
solver residual. Launch angles, the seabed incidence and the modelled
two-way travel time are derived from the two legs on access rather
than stored.

The per-layer TRANSMIT ray direction used for backscatter is the
transmit leg's incident-angle series
(BeamTrace::get_incident_angles_in_degrees / get_cos_incident_angles);
its value at the last point is the seabed incidence.

A 2-D leg point (horizontal_offset, depth) is lifted into the ship
frame by the leg azimuth psi and that leg's array position P:
  x_forward   = P_forward   - horizontal_offset * sin(psi) y_starboard
  = P_starboard + horizontal_offset * cos(psi) z_down      = depth)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_BistaticBeamTrace = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_BistaticBeamTrace_2 =
R"doc(Construct from the two converged legs and the solved seabed point.

Args:
    transmit_leg: per-layer transmit polyline (from trace_beam).
    receive_leg: per-layer receive polyline (from trace_beam).
    transmit_azimuth_in_degrees: azimuth (deg, forward->starboard) of
                                 the transmit plane.
    receive_azimuth_in_degrees: azimuth (deg, forward->starboard) of
                                the receive plane.
    bottom_position: solved seabed point (forward, starboard, down) in
                     m.
    solver_residual_in_meters: final solver residual in m.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_bottom_position = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_bottom_incidence_angle_in_degrees =
R"doc(Seabed incidence angle (deg from nadir, signed) of the TRANSMIT ray,
for backscatter.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_bottom_position =
R"doc(Solved seabed point (forward, starboard, down) in the common input
frame [m].)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_receive_azimuth_in_degrees =
R"doc(Receive leg azimuth (deg): rotation about the down axis,
BeamDirections convention.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_receive_launch_angle_in_degrees =
R"doc(Receive-leg launch angle (deg from nadir, port +) at the receive
array.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_receive_leg = R"doc(Receive leg polyline (per-layer points in the receive vertical plane).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_solver_residual_in_meters =
R"doc(Final solver residual [m]; small values indicate a converged bistatic
solve.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_transmit_azimuth_in_degrees =
R"doc(Transmit leg azimuth (deg): rotation about the down axis,
BeamDirections convention.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_transmit_launch_angle_in_degrees =
R"doc(Transmit-leg launch angle (deg from nadir, port +) at the transmit
array.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_transmit_leg =
R"doc(Transmit leg polyline (per-layer points in the transmit vertical
plane).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_get_two_way_travel_time_in_seconds = R"doc(Modelled two-way travel time [s] = transmit one-way + receive one-way.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_last_two_way = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_receive_azimuth_in_degrees = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_receive_leg = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_solver_residual_in_meters = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_transmit_azimuth_in_degrees = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BistaticBeamTrace_transmit_leg = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone =
R"doc(One-parameter family of unit rays with a fixed projection onto an
array axis.

A linear array only constrains the beam's angle to the array axis:
every unit ray whose dot product with the array axis equals the
steering projection lies on a cone around that axis. Parametrising a
leg by the rotation angle around this cone (rather than by an
azimuth/zenith pair) never degenerates - in particular for a flat
array with zero steering, where the "cone" opens into a plane and an
azimuth parametrisation would be singular. This is what lets the
bistatic solver iterate robustly on the two legs' cone angles plus the
seabed depth.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_SteeringCone =
R"doc(Build the cone for a given array axis and steering projection.
Args:
    array_axis: unit array long axis in the world frame.
    axis_projection: required dot(ray, axis) = sin(steering angle).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_angle_of = R"doc(Cone rotation angle (rad) whose ray best matches ``direction.``)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_axis = R"doc(unit array long axis (world frame))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_basis_u = R"doc(first unit vector spanning the plane orthogonal to axis)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_basis_v = R"doc(second orthonormal vector (axis, basis_u, basis_v right-handed))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_projection = R"doc(fixed dot(ray, axis) = sin(steering angle))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_ray = R"doc(Unit ray at rotation angle ``around_axis`` (rad) around the cone.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_bistatic_detail_SteeringCone_sine_half_angle = R"doc(radius of the cone circle = sqrt(1 - projection^2))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_trace_bistatic_beam =
R"doc(Solve the true-bistatic seabed trace of a single multibeam beam.

Traces the transmit ray from the transmit array and the receive ray
from the receive array through the layered sound-velocity profile and
finds the seabed point where the two legs meet with a combined one-way
travel time equal to the measured two-way travel time. The seabed
depth and each leg's cone rotation angle (see bistatic_detail::
SteeringCone) are found with a damped Newton iteration seeded by the
concentric beam direction. Each converged leg is then re-traced with
trace_beam so the returned legs are identical to the monostatic model
when the transmit and receive poses coincide.

All poses are in the common x=forward, y=starboard, z=down frame; the
concentric guess must be in the same frame (compute_beam_directions
with reference_heading = 0).

Args:
    transmit_installation_ypr_in_degrees: (yaw, pitch, roll) mounting
                                          of the transmit array.
    transmit_attitude_ypr_in_degrees: (yaw, pitch, roll) vessel
                                      attitude at transmit time.
    transmit_steering_angle_in_degrees: electronic transmit steering
                                        (positive forward).
    transmit_position_xyz: transmit array position (forward,
                           starboard, down) [m].
    receive_installation_ypr_in_degrees: (yaw, pitch, roll) mounting
                                         of the receive array.
    receive_attitude_ypr_in_degrees: (yaw, pitch, roll) vessel
                                     attitude at receive time.
    receive_steering_angle_in_degrees: electronic receive steering
                                       (positive to port).
    receive_position_xyz: receive array position (forward, starboard,
                          down) [m].
    two_way_travel_time_in_seconds: measured two-way travel time [s].
    sound_velocity_profile: layered profile to trace through.
    concentric_beam_direction: ship-frame unit guess (fwd, stbd,
                               down), e.g. BeamDirections::get_beam_di
                               rection(beam).
    max_iterations: maximum Newton iterations (default 30).
    tolerance_in_percent: convergence tolerance as a percentage of the
                          nominal slant range (default 0.001).

Returns:
    BistaticBeamTrace with both legs, azimuths, seabed point and
    residual.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


