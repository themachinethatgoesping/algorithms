//sourcehash: b4172810b4f9dac48e4c29ba9ddd0da5f49c9100730115c947ead86819b044a1

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__

#define __EXPAND(x)                                                  x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...) COUNT
#define __VA_SIZE(...)                                               __EXPAND(__COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                                 a##b
#define __CAT2(a, b)                                                 __CAT1(a, b)
#define __DOC1(n1)                                                   __doc_##n1
#define __DOC2(n1, n2)                                               __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                                           __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                                       __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                                   __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)                           __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define __DOC8(n1, n2, n3, n4, n5, n6, n7, n8)                                                     \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define __DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)                                                 \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define __DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)                                           \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...) __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#endif // NEW_DOC_HEADER_HPP
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult =
R"doc(A structure to store a georeferenced sample location. It is used as
output for the raytracers functions. This object stores local x
(forward coordinate), y (starboard coordinate) depth and true range.
These coordinates can be converted to UTM or Lat/Lon if a reference
position (for coordinate 0) is known.)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_RaytraceResult = R"doc(Construct a new sample location object (all values set to 0))doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_RaytraceResult_2 =
R"doc(Construct a new RaytraceResult object

Parameter ``x``:
    in m, positive forward

Parameter ``y``:
    in m, positive starboard

Parameter ``z``:
    in m, positive downwards

Parameter ``true_range``:
    in m, accumulated ray path length)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_operator_ne = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_to_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_true_range = R"doc(< in m, accumulated ray path length)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_x = R"doc(< in m, positive forward)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_y = R"doc(< in m, positive starboard)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResult_z = R"doc(< in m, positive downwards)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


