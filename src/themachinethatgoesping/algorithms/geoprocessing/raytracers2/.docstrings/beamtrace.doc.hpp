//sourcehash: e844c023297c223242e6f4767bfc0a5321eb222594fe9637969cd6f1fb146a83

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace =
R"doc(Path of a single traced beam through a layered sound velocity profile.

The stored points describe the ray polyline in the athwartships y-z
plane (forward coordinate x is zero). Point 0 is the launch point.
Depth, horizontal offset, two-way travel time and the cosine of the
ray angle are stored; incident angle (deg) and cumulative range (m)
are derived on access.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_BeamTrace = R"doc(Construct an empty BeamTrace (no points stored).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_BeamTrace_2 =
R"doc(Construct from the four stored per-point tables (all must have the
same length).
Args:
    depths: depth z (m, positive down); point 0 is the launch point.
    horizontal_offsets: signed athwartships offset y (m, positive
                        starboard).
    two_way_travel_times: two-way travel time (s).
    cos_incident_angles: cosine of the ray angle from +z (1=down,
                         0=turning, −1=up).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_cos_incident_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_depths = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_cos_incident_angles =
R"doc(Cosine of the ray angle from +z (1 down, 0 turning, -1 up) at each
point.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_depths_in_meters = R"doc(Depth z (m, positive down) at each point.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_horizontal_offsets_in_meters = R"doc(Signed athwartships offset y (m, positive starboard) at each point.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_incident_angles_in_degrees =
R"doc(Incident angle (deg) from straight down at each point.

0 deg points down, +-90 deg is horizontal, +-180 deg points up.
Positive angles correspond to the port side (negative y), consistent
with a right-handed rotation about +x.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_number_of_points =
R"doc(Number of stored points (>= 1 for a valid trace; point 0 is the launch
point).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_ranges_in_meters =
R"doc(Cumulative along-ray range (m) from the launch point.
Returns:
    Range array; element 0 is always 0 (launch point).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_get_two_way_travel_times_in_seconds = R"doc(Two-way travel time (s) at each point.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_horizontal_offsets = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_horizontal_side = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_operator_eq = R"doc(Equality comparison.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_set =
R"doc(Set all stored tables at once (all must have the same length).
Raises:
    std::runtime_error: if table sizes differ.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_BeamTrace_two_way_travel_times = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


