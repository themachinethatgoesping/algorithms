//sourcehash: 5b99008011edcf71003336b86f1de018e1b81601bd2e9513a1ec5d816a241073

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth =
R"doc(Endpoint of one ray leg traced down to a target depth (fast, no
polyline).

Produced by trace_beam_to_depth. The horizontal offset and path length
are magnitudes for the single leg between the launch depth and the
target depth; the caller carries the horizontal azimuth of the leg
separately.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth_cos_angle_at_target =
R"doc(Cosine of the ray angle from straight down at the target depth (after
refraction).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth_horizontal_offset_in_meters =
R"doc(Horizontal distance (m, >= 0) from the launch point to the target
depth.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth_one_way_travel_time_in_seconds = R"doc(One-way travel time (s) from the launch point to the target depth.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth_path_length_in_meters = R"doc(Along-ray path length (m) from the launch point to the target depth.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_RayToDepth_reached_target =
R"doc(True if the ray reached the target depth (false if it turned or left
the profile first).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_trace_beam =
R"doc(Trace a single beam through a layered sound velocity profile.

Emits one point at launch, one at each layer crossing and turning
point, and a final point at the requested travel time (or when the ray
exits the profile).

The Snell ray parameter (the invariant that governs refraction) is
defined by the launch angle and the sound speed at which the beam was
formed. For a multibeam that is the measured surface/transducer sound
speed (SSV). Pass it as ``surface_sound_speed_in_meters_per_second``
whenever it differs from the profile value at the launch depth (e.g.
the real-time SSV differs from the archived cast); otherwise the
profile value at the launch depth is used, and both agree exactly when
the two speeds are equal. Using the wrong launch sound speed
introduces an angle-dependent (outer-beam) depth bias.

Args:
    launch_depth_in_meters: launch depth (m, positive down); must be
                            inside the profile range.
    launch_angle_in_degrees: angle from straight down (deg); 0 = down,
                             positive = port.
    sound_velocity_profile: profile to trace through.
    two_way_travel_time_in_seconds: two-way travel time budget (s).
    surface_sound_speed_in_meters_per_second: sound speed (m/s) at
                                              which the beam was
                                              formed; the ray
                                              parameter is
                                              sin(angle)/this.
                                              std::nullopt (default,
                                              i.e. not provided) falls
                                              back to the profile
                                              value at the launch
                                              depth.

Returns:
    BeamTrace with the launch point, layer crossings, turning points
    and the final point.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_trace_beam_to_depth =
R"doc(Trace one ray leg from a launch depth/angle down to a target depth.

Uses the identical layered-Snell principle as trace_beam (the same
tracebeam_detail closed-form iso/gradient segment kernels), but
integrates to a target depth instead of a travel-time budget and only
accumulates the endpoint (no polyline). This is the fast inner step of
the bistatic solver, which calls it many times per beam while
searching for the seabed point; once converged, the full per-layer
polyline of each leg is produced with trace_beam.

The launch is downward (0 = nadir); if the ray turns (becomes
horizontal) or leaves the profile before the target depth,
reached_target is false.

Args:
    sound_velocity_profile: layered profile to trace through.
    launch_depth_in_meters: depth (m, positive down) of the leg
                            origin; must be within the profile.
    launch_zenith_angle_in_radians: ray angle from straight down at
                                    the launch point (0 = nadir).
    target_depth_in_meters: depth (m, positive down) to trace to; must
                            be > launch depth and within the profile.
    surface_sound_speed_in_meters_per_second: sound speed (m/s) at
                                              which the beam was
                                              formed; the ray
                                              parameter is
                                              sin(zenith)/this.
                                              std::nullopt (default,
                                              i.e. not provided) falls
                                              back to the profile
                                              value at the launch
                                              depth. Must match
                                              trace_beam so
                                              mono/bistatic agree.

Returns:
    RayToDepth endpoint of the leg.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_tracebeam_detail_layer_segment_gradient =
R"doc(Closed-form geometry of one circular-arc ray segment in a constant-
gradient layer.

For a ray with Snell parameter p in a layer of gradient ``gradient``
(1/s, != 0), going from sound speed ``sound_speed_1`` (cosine
``cosine_1)`` to ``sound_speed_2`` (cosine ``cosine_2),`` returns the
horizontal distance (>= 0), the signed one-way travel time (positive
along increasing depth) and the along-ray path length. This is the
shared kernel used by both trace_beam and trace_beam_to_depth.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_tracebeam_detail_layer_segment_iso =
R"doc(Closed-form geometry of one straight ray segment in an iso-velocity
layer.

For a ray with Snell parameter p = sin(theta) / c in a layer of
constant sound speed ``sound_speed,`` spanning a vertical extent
``vertical_extent`` (m, >= 0), returns the horizontal distance, one-
way travel time and along-ray path length of that segment. The ray
angle theta is constant across an iso-velocity layer. This is the
shared kernel used by both trace_beam and trace_beam_to_depth.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


