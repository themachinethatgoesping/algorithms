//sourcehash: 18347264b42f2564edeeef86aeaafdc91e01023b0baa3dc4e60cd04afad461e0

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections =
R"doc(Per-beam pointing directions of a multibeam swath.

Stores one ship-referenced unit pointing vector per beam (x = forward,
y = starboard, z = down, reference heading removed). From each
direction the class derives, on access:
  * the signed beam pointing angle (the athwartships launch angle
    handed to
    trace_beam: 0 deg = nadir/down, +90 deg = horizontal to port, -90
                deg =
    horizontal to starboard),
  * the beam azimuth (the fore-aft rotation about the vertical/down
    axis that lifts a 2-D trace (horizontal_offset, depth) back into
    ship-frame xyz),
  * the unsigned beam take-off angle from nadir.

A traced beam point is lifted back into the ship frame by rotating it
about the vertical (down) axis by the beam azimuth:
  x_forward   = -horizontal_offset * sin(beam_azimuth) y_starboard =
  horizontal_offset * cos(beam_azimuth) z_down      =  depth
i.e. xyz = R_down(beam_azimuth) * (0, horizontal_offset, depth).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_BeamDirections = R"doc(Construct an empty BeamDirections (no beams stored).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_BeamDirections_2 =
R"doc(Construct from per-beam unit pointing vectors.
Args:
    directions: [n_beams, 3] tensor of (forward, starboard, down)
                components.

Raises:
    std::runtime_error: if the second dimension is not 3.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_directions = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_beam_azimuth_angles_in_degrees =
R"doc(Beam azimuth (deg): the fore-aft rotation about the vertical (down)
axis.

Principal value in (-90, 90]; 0 deg means the beam lies in the
athwartships plane (no fore-aft component). Used to lift a 2-D trace
back into 3-D:
  xyz = R_down(beam_azimuth) * (0, horizontal_offset, depth).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_beam_direction =
R"doc(Ship-referenced unit pointing vector (forward, starboard, down) of a
single beam.

Convenience accessor for feeding one beam's direction as the
concentric initial guess into trace_bistatic_beam.
Args:
    beam_index: index of the beam.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_beam_pointing_angles_in_degrees =
R"doc(Signed beam pointing angle (deg): the athwartships launch angle for
trace_beam.

0 deg is nadir (straight down), +90 deg is horizontal to port, -90 deg
is horizontal to starboard. Its magnitude equals the take-off angle
from nadir; its sign follows the port (+) / starboard (-) side of the
beam. Combined with get_beam_azimuth_angles_in_degrees() it
reconstructs the full 3-D direction:
  d = R_down(beam_azimuth) * (0, -sin(beam_pointing),
  cos(beam_pointing)).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_beam_takeoff_angles_in_degrees =
R"doc(Unsigned beam take-off angle (deg) from nadir (straight down), always
>= 0.

0 deg is nadir, 90 deg is horizontal. Equals |beam pointing angle|.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_directions =
R"doc(Per-beam ship-referenced unit pointing vectors [n_beams, 3] =
(forward, starboard, down).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_get_number_of_beams = R"doc(Number of beams stored.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_operator_eq = R"doc(Equality comparison.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_set =
R"doc(Set the per-beam unit pointing vectors.
Args:
    directions: [n_beams, 3] tensor of (forward, starboard, down)
                components.

Raises:
    std::runtime_error: if the second dimension is not 3.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamDirections_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_beam_direction_to_pointing_and_azimuth_in_degrees =
R"doc(Decompose a ship-frame beam direction into a signed pointing angle and
azimuth.

Scalar counterpart of
BeamDirections::get_beam_pointing_angles_in_degrees /
get_beam_azimuth_angles_in_degrees, using the identical convention:
the pointing angle is 0 deg at nadir, +90 deg horizontal to port, -90
deg to starboard; the azimuth is the fore-aft rotation about the
vertical (down) axis, principal value in (-90, 90]. Together they
reconstruct the direction:
  d = R_down(azimuth) * (0, -sin(pointing), cos(pointing)).

Args:
    forward: forward (x) component of the unit direction.
    starboard: starboard (y) component.
    down: down (z) component.

Returns:
    {pointing_angle_in_degrees, azimuth_in_degrees}.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_compute_beam_directions =
R"doc(Compute the ship-referenced pointing direction of every beam.

Each beam is the downward intersection of the transmit and receive
array "fans" (Mills cross). The transmit array long axis (forward) and
the receive array long axis (starboard) are placed in the world frame
from their installation orientation and the vessel attitude at
transmit / receive; the transmit and receive steering angles then fix
the beam's projection onto each axis. The intersection is solved
directly (no orthogonality assumption), so array non-orthogonality is
exact and reverse mounts are handled by the installation quaternion
alone (no manual sign flips).

Args:
    transmit_installation_ypr_in_degrees: (yaw, pitch, roll) mounting
                                          orientation of the transmit
                                          array.
    receive_installation_ypr_in_degrees: (yaw, pitch, roll) mounting
                                         orientation of the receive
                                         array.
    transmit_attitude_ypr_in_degrees: [n_beams, 3] vessel (yaw, pitch,
                                      roll) at transmit time.
    receive_attitude_ypr_in_degrees: [n_beams, 3] vessel (yaw, pitch,
                                     roll) at receive time.
    transmit_steering_angles_in_degrees: [n_beams] fore-aft transmit
                                         tilt (positive forward).
    receive_steering_angles_in_degrees: [n_beams] across-track receive
                                        angle (positive to PORT).
    reference_heading_in_degrees: heading the output is expressed
                                  relative to.
    mp_cores: number of OpenMP cores for the per-beam solve (default
              1).

Returns:
    BeamDirections with one ship-referenced unit pointing vector per
    beam.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_correct_steering_angle_for_surface_sound_speed =
R"doc(Scalar overload of correct_steering_angles_for_surface_sound_speed.


$See also:

correct_steering_angles_for_surface_sound_speed)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_correct_steering_angles_for_surface_sound_speed =
R"doc(Snell correction of a beam steering angle for a changed surface sound
speed.

Multibeam beamforming forms every steering angle in the near field
using the surface sound speed measured at the transducer. If that
surface sound speed is wrong, the true beam angle in the water
refracts across the transducer face following Snell's law:
  sin(corrected) = (c_corrected / c_used) * sin(steering).
The correction must be applied to the transmit and receive steering
angles *before* compute_beam_directions, because the steering angles
are defined at the array face, which is where the refraction happens.
Angles that would exceed the horizon are clamped to +-90 deg.

Args:
    steering_angles_in_degrees: nominal steering angles (deg).
    surface_sound_speed_used_in_meters_per_second: surface sound speed
                                                   used when the beams
                                                   were formed.
    surface_sound_speed_corrected_in_meters_per_second: true /
                                                        corrected
                                                        surface sound
                                                        speed.

Returns:
    corrected steering angles (deg).)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


