//sourcehash: 3ee57b2a685cdd2e179ceb7e19e9aa5a52c64c157e450e4a3e5374281f0b32b8

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


static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_I_Backtracer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_backtrace_points =
R"doc(Backtrace the location of a set of points.

Parameter ``two_way_travel_time``:
    Two way travel time in s

Parameter ``alongtrack_angle``:
    Along track angle in °

Parameter ``crosstrack_angles``:
    Across track angle in °

Parameter ``mp_cores``:
    Number of cores to use for parallelization

Returns:
    datastructures::SampleLocationLocal)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_name = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_location = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_orientation_quat = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_get_sensor_orientation_quat_ypr = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_name = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_not_implemented = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_not_implemented_not_implemented = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_location = R"doc(< Location/Orientation of the senor)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_sensor_orientation_quat = R"doc(< Quaternion describing the orientation < of the sensor)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_set_sensor_location = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_backtracers_I_Backtracer_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

