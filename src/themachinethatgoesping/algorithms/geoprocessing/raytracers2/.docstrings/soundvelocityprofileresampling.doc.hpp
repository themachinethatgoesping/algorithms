//sourcehash: 900c24a452295a9031625b49e1ee04e9ce35349e967cef7ae8f9ab074e66f34d

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_resample_soundvelocityprofile =
R"doc(Return a copy of ``svp`` with fewer depth/sound-speed knots, chosen so
that raytracing
       through the simplified profile stays close to the original
       while traversing fewer layers.

Exactly one of the three targets must be set:
  - ``max_sound_speed_error_in_meters_per_second`` : Douglas-Peucker
    on c(z); keeps the interpolated sound speed within this tolerance
    (self-contained, fastest).
  - ``max_raytrace_error_in_meters`` : ray-error greedy; guarantees
    the seabed position error (horizontal and depth) stays within this
    budget for launch angles up to ``max_launch_angle_in_degrees,``
    for beams launched at ``launch_depth_in_meters`` down to
    ``max_depth_in_meters`` (fewest layers for a given accuracy).
  - ``number_of_entries`` : simplify until about this many knots
    remain (ray-error ranking).

The sound speed at ``launch_depth_in_meters`` (the transducer depth)
is always preserved exactly so a beam launched there keeps the
original ray parameter; set this to the transducer depth used by the
raytracer.

Args:
    svp: source profile.
    max_sound_speed_error_in_meters_per_second: sound-speed tolerance
                                                (m/s), or
                                                std::nullopt.
    max_raytrace_error_in_meters: seabed-position error budget (m), or
                                  std::nullopt.
    number_of_entries: target knot count (>= 2), or std::nullopt.
    launch_depth_in_meters: transducer/launch depth whose sound speed
                            is preserved (default 0).
    max_depth_in_meters: only bound the error down to this depth; <= 0
                         uses the profile bottom.
    max_launch_angle_in_degrees: widest launch angle considered for
                                 the error (default 75).
    number_of_test_angles: number of launch angles sampled for the
                           error (default 8).

Returns:
    simplified SoundVelocityProfile (metadata preserved).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_svpresampling_detail_douglas_peucker_c =
R"doc(Douglas-Peucker on c(z): keep[i]=true for knots to retain (endpoints
always kept).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_svpresampling_detail_greedy_raytrace =
R"doc(Bottom-up greedy knot removal ranked by raytracing error. The knots at
index 0,
       ``protected_index`` (the launch-depth knot) and n-1 are never
       removed. Stops when removing the next knot would push the
       summed error past ``max_error,`` or when ``target_entries``
       knots remain. ``max_error`` <= 0 disables the error stop;
       ``target_entries`` == 0 disables the count stop.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_svpresampling_detail_segment_error =
R"doc(Raytracing error (m) introduced by approximating knots [L..R] with the
straight segment
       (L,R). Traces from z[L] over a spread of ray parameters through
       the exact profile vs the two-knot linear segment down to
       min(z[R], max_depth) and returns the worst of |d horizontal
       offset| and |d one-way-time| * (sound speed at the target).
       Segments that a beam launched at ``launch_depth`` never
       traverses (entirely above it) contribute nothing.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_svpresampling_detail_subset =
R"doc(Build a SoundVelocityProfile from the kept knots of (z, c), copying
metadata from ``meta.``)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_svpresampling_detail_to_vector = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


