//sourcehash: e809ff27066e3040dfc8e53ad5f30f28314d5e5d25805c09650cc1e56ca75572

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


static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults =
R"doc(A structure to store a georeferenced sample location. It is used as
output for the raytracers functions. This object stores local x
(forward coordinate), y (starboard coordinate) depth and true range.
These coordinates can be converted to UTM or Lat/Lon if a reference
position (for coordinate 0) is known.)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_RaytraceResults = R"doc(Construct a new sample location object (all values set to 0))doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_RaytraceResults_2 =
R"doc(Construct a new sample location object (initialize all tensors using
the specified shape (empty))

Parameter ``shape``:
    shape of the internal tensors)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_RaytraceResults_3 =
R"doc(Construct a new RaytraceResults object

Parameter ``x``:
    in m, positive forward

Parameter ``y``:
    in m, positive starboard

Parameter ``z``:
    in m, positive downwards

Parameter ``true_range``:
    in m, accumulated ray path length)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_RaytraceResults_4 =
R"doc(Construct a new RaytraceResults object

Parameter ``xyz``:
    XYZ object

Parameter ``true_range``:
    in m, accumulated ray path length)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_RaytraceResults_5 =
R"doc(Construct a new Raytrace Results object with an uninitialized
true_range tensor This is private and only used for the from_stream
function

Parameter ``xyz``:)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_check_shape = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_concat =
R"doc(Concatenate multiple RaytraceResults objects Note: the dimensionality
of the RaytraceResults objects will be lost (transformed
RaytraceResults XYZ<1>)

Parameter ``vector``:
    of RaytraceResults objects

Returns:
    RaytraceResults<1>)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_to_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_RaytraceResults_true_range = R"doc(< in m, accumulated ray path length)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


