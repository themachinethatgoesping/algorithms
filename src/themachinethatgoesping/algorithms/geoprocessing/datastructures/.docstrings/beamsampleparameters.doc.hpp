//sourcehash: 0035dc107f5a7fd2170f855433dbe616120267562a0ac61ade5e423cc08cd910

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters =
R"doc(A structure to store directional parameters of multibeam system.
Usefull as input for raytracing water column images.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_BeamSampleParameters =
R"doc(Construct a new BeamSampleParameters object (all values uninitialized)

Args:
    number_of_beams: number of beams)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_BeamSampleParameters_2 =
R"doc(Construct a new Sample Indices object from existing beam parameters

Args:
    alongtrack_angles: alongtrack angles of the MBES in °
    crosstrack_angles: crosstrack angles of the MBES in °
    first_sample_offset: time offset of the first sample (sample
                         number 0) in s
    sample_interval: time interval between samples in s
    number_of_samples: number of samples per beam

@note all parameters must have the same size)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_alongtrack_angles = R"doc(in °, positive bow up, 0 == downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_check_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_crosstrack_angles = R"doc(in °, positive portside up, 0 == downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_first_sample_offset = R"doc(in s, time offset of the first sample (sample number 0))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_alongtrack_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_crosstrack_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_first_sample_offset = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_number_of_samples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_get_sample_interval = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_number_of_samples = R"doc(number of samples per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_sample_interval = R"doc(in s, time interval between samples)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_alongtrack_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_crosstrack_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_first_sample_offset = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_number_of_samples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_set_sample_interval = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleParameters_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


