//sourcehash: 81e1e8e4e9b9d772f63ac32e01699d67eac8a38e98d9ab18c1aa9e9a798d9dc1

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_I_Backtracer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_backtrace_image =
R"doc(Backtrace the location of an image specified by two coordinate vectors
x is assumed to be 0

Args:
    y_coordinates: in m, positive starboard
    z_coordinates: in m, positive downwards
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::SampleDirectionsRange<2>, shape is
                   (y_coordinates.size(), z_coordinates.size()))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_backtrace_points =
R"doc(Backtrace the location of a set of points.

Args:
    x: in m, positive forward
    y: in m, positive starboard
    z: in m, positive downwards
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::SampleDirectionsRange)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_backtrace_points_2 =
R"doc(Backtrace the location of a set of points.

Args:
    xyz: structure with x/y/z coordinates
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::SampleDirectionsRange)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_class_name = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_location = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_orientation_quat = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_orientation_quat_ypr = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_lookup = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_name = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_not_implemented = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_not_implemented_not_implemented = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_location = R"doc(Location/Orientation of the senor)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_orientation_quat = R"doc(Quaternion describing the orientation of the sensor)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_set_sensor_location = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


