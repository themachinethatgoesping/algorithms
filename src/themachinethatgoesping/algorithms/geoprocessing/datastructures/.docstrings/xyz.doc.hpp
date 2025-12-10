//sourcehash: 68f1d1fb341efefecff68292add326c2929288dada3e0ae19c64355112665948

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ =
R"doc(A structure to store a georeferenced sample location. It is used as
output for the raytracers functions. This object stores local x
(forward coordinate), y (starboard coordinate) and z (depth). These
coordinates can be converted to UTM or Lat/Lon if a reference position
(for coordinate 0) is known.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_XYZ = R"doc(Construct a new sample location object (all values set to 0))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_XYZ_2 =
R"doc(Construct a new sample location object (initialize all tensors using
the specified shape (empty))

Args:
    shape: shape of the internal tensors)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_XYZ_3 =
R"doc(Construct a new XYZ object

Args:
    x: in m, positive forward
    y: in m, positive starboard
    z: in m, positive downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_check_shape = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_concat =
R"doc(Concatenate multiple XYZ objects
Note: the dimensionality of the XYZ objects will be lost (transformed
      to XYZ<1>)

Args:
    vector: of XYZ objects

Returns:
    XYZ<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_get_minmax_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_get_minmax_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_get_minmax_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_rotate =
R"doc(Rotate the XYZ object using a quaternion

Args:
    q: quaternion)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_rotate_2 =
R"doc(Rotate the XYZ object using yaw, pitch, roll in °

Args:
    yaw: in °
    pitch: in °
    roll: in °)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_shape = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_translate = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_x = R"doc(x coordinate in m, positive forward)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_y = R"doc(y coordinate in m, positive starboard)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_XYZ_z = R"doc(z coordinate in m, positive downwards)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


