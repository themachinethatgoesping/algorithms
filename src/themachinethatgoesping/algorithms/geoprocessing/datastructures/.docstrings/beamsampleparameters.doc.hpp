//sourcehash: 41751d6bb8c70caec244be35fff4b29cfd8e0ae2480793fd92efde5da08a9c97

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


static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters =
R"doc(A structure to store directional parameters of multibeam system.
Usefull as input for raytracing water column images.)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_BeamSampleParameters =
R"doc(Construct a new BeamSampleParameters object (all values uninitialized)

Parameter ``number_of_beams``:
    number of beams)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_BeamSampleParameters_2 =
R"doc(Construct a new Sample Indices object from existing beam parameters

Parameter ``alongtrack_angles``:
    alongtrack angles of the MBES in °

Parameter ``crosstrack_angles``:
    crosstrack angles of the MBES in °

Parameter ``first_sample_offset``:
    time offset of the first sample (sample number 0) in s

Parameter ``sample_interval``:
    time interval between samples in s

Parameter ``number_of_samples``:
    number of samples per beam

@note all parameters must have the same size)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_alongtrack_angles = R"doc(< in °, positive bow up, 0 == downwards)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_check_size = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_crosstrack_angles = R"doc(< in °, positive portside up, 0 == downwards)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_first_sample_offset = R"doc(< in s, time offset of the first sample (sample number 0))doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_alongtrack_angles = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_crosstrack_angles = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_first_sample_offset = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_number_of_samples = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_sample_interval = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_number_of_samples = R"doc(< number of samples per beam)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_sample_interval = R"doc(< in s, time interval between samples)doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_alongtrack_angles = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_crosstrack_angles = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_first_sample_offset = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_number_of_samples = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_sample_interval = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


