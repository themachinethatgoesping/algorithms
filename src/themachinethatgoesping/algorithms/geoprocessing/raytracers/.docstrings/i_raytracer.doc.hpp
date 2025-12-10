//sourcehash: 0e2ee4107bdf5b6a37cd60e2a7561d3db3160cca25b4e112cf6dd8edd4812da6

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_I_Raytracer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_class_name = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_get_sensor_location = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_get_sensor_orientation_quat = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_get_sensor_orientation_quat_ypr = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_name = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_not_implemented = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_not_implemented_not_implemented = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_sensor_location = R"doc(Location/Orientation of the senor)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_sensor_orientation_quat = R"doc(Quaternion describing the orientation of the sensor)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_set_sensor_location = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_beam =
R"doc(Trace the sample locations of a single beam.

Args:
    sample_numbers: Sample numbers to trace (starting from 0)
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    alongtrack_angle: Along track angle in °
    crosstrack_angle: Across track angle in °

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_beam_2 =
R"doc(Trace the sample locations of a single beam.

Args:
    first_sample_number: First sample number to trace
    number_of_samples: Number of samples to trace
    sample_step: Step between samples
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    alongtrack_angle: Along track angle in °
    crosstrack_angle: Across track angle in °

Returns:
    datastructures::RaytraceResults<1>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_point =
R"doc(Trace the location of a single point.

Args:
    two_way_travel_time: Two way travel time in s
    alongtrack_angle: Along track angle in °
    crosstrack_angle: Across track angle in °

Returns:
    datastructures::RaytraceResult)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_points =
R"doc(Trace the location of a set of points.

Args:
    two_way_travel_time: Two way travel time in s
    alongtrack_angle: Along track angle in °
    crosstrack_angles: Across track angle in °
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::RaytraceResult)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_points_2 =
R"doc(Trace the location of a set of points.

Args:
    sampledirections: One dimensional sample directions array
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::RaytraceResult)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_swath =
R"doc(Trace the sample locations of a multiple beams in a swath.
Note: a 2d Array for sample numbers is expected where the first
      dimension is the beam and the
second dimension is the sample number. The beam dimension must be the
same as for "crosstrack_angles"

Args:
    sample_numbers: Sample numbers to trace (starting from 0)
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    alongtrack_angle: Along track angle of the swath in °
    crosstrack_angles: Across track angle of each beam in °
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::RaytraceResults<2>)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers_I_Raytracer_trace_swath_2 =
R"doc(Trace the sample locations of a multiple beams in a swath.
Note: The number of beams is controlled by the dimension of
      crosstrack_angles

Args:
    first_sample_number: First sample number to trace
    number_of_samples: Number of samples to trace
    sample_step: Step between samples
    sampling_time: Time betweens samples in s
    sampling_time_offset: Time offset for sample number 0 in s
    alongtrack_angle: Along track angle of the swath in °
    crosstrack_angles: Across track angle of each beam in °
    mp_cores: Number of cores to use for parallelization

Returns:
    datastructures::RaytraceResults<2>)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


