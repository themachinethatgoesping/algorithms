//sourcehash: 1712e4895f824d9520d80d2e7cb049b27abf7e34b1ec53207828f1bac0168e56

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


static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_RTConstantSVP =
R"doc(Construct a new RTConstantSVP object

Parameter ``sensor_location``:
    Orientation and depth of the echo sounder

Parameter ``sound_velocity``:
    Sound velocity in m/s)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_RTConstantSVP_2 =
R"doc(Construct a new RTConstantSVP object from a base raytracer object
leaving sound velocity uninitialized (for reading from stream)

Parameter ``raytracer``:
    base raytracer object)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_get_sound_velocity = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_beam =
R"doc(Compute the sample locations of a single beam by scaling between the
transducer location and a known target location

Parameter ``sample_numbers``:
    Sample numbers to trace (starting from 0)

Parameter ``sampling_time``:
    Time betweens samples in s

Parameter ``sampling_time_offset``:
    Time offset for sample number 0 in s

Parameter ``scale_x``:
    known target x position at scale_time

Parameter ``scale_y``:
    known target y position at scale_time

Parameter ``scale_z``:
    known target z position at scale_time

Parameter ``scale_true_range``:
    known target range at scale_time

Parameter ``scale_time``:
    known target two way travel time

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_beam_2 =
R"doc(Compute the sample locations of a single beam by scaling between the
transducer location and a known target location

Parameter ``sample_numbers``:
    Sample numbers to trace (starting from 0)

Parameter ``sampling_time``:
    Time betweens samples in s

Parameter ``sampling_time_offset``:
    Time offset for sample number 0 in s

Parameter ``scale_target``:
    known target location at scale_time

Parameter ``scale_time``:
    known target two way travel time

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_swath =
R"doc(Compute the sample locations of a swath by scaling between the
transducer location and known target locations

Parameter ``sample_numbers``:
    Sample numbers to trace (starting from 0)

Parameter ``sampling_time``:
    Time betweens samples in s

Parameter ``sampling_time_offset``:
    Time offset for sample number 0 in s

Parameter ``scale_target``:
    known target location at scale_time

Parameter ``scale_time``:
    two way travel time for the known target location

Parameter ``mp_cores``:
    number of cores to use for parallelization

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_set_sound_velocity = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_sound_velocity = R"doc(< sound velocity in m/s)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_sound_velocity_2 = R"doc(< sound velocity in m/s divided by 2 (for 2 way travel time))doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_to_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_beam = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_point = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_points = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_points_2 = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_swath = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


