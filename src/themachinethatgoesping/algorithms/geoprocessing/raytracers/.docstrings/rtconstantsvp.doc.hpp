//sourcehash: e8d93684872492703d4e9be15dbfb5b97441250e83a0dd355d5c83990226c4e0

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_RTConstantSVP =
R"doc(Construct a new RTConstantSVP object

Args:
    sensor_location: Orientation and depth of the echo sounder
    sound_velocity: Sound velocity in m/s)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_RTConstantSVP_2 =
R"doc(Construct a new RTConstantSVP object from a base raytracer object
leaving sound velocity uninitialized (for reading from stream)

Args:
    raytracer: base raytracer object)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_get_sound_velocity = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_beam =
R"doc(Compute the sample locations of a single beam by scaling between the
transducer location and a known target location

Args:
    sample_numbers: Sample numbers to trace (starting from 0)
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    scale_x: known target x position at scale_time
    scale_y: known target y position at scale_time
    scale_z: known target z position at scale_time
    scale_true_range: known target range at scale_time
    scale_time: known target two way travel time

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_beam_2 =
R"doc(Compute the sample locations of a single beam by scaling between the
transducer location and a known target location

Args:
    sample_numbers: Sample numbers to trace (starting from 0)
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    scale_target: known target location at scale_time
    scale_time: known target two way travel time

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_scale_swath =
R"doc(Compute the sample locations of a swath by scaling between the
transducer location and known target locations

Args:
    sample_numbers: Sample numbers to trace (starting from 0)
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    scale_target: known target location at scale_time
    scale_time: two way travel time for the known target location
    mp_cores: number of cores to use for parallelization

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_set_sound_velocity = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_sound_velocity = R"doc(sound velocity in m/s)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_sound_velocity_2 = R"doc(sound velocity in m/s divided by 2 (for 2 way travel time))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_beam = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_point = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_points = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_RTConstantSVP_trace_swath = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


