//sourcehash: 1cca7db4115f6fed760d9abe7f0a9b53e2a78d78ffcba1125450272d2a964a23

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D =
R"doc(A 1D affine transform per beam: value = offset + slope * sample_nr

Stores one offset and one slope per beam in contiguous arrays (SoA
layout), enabling SIMD vectorization across beams.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_BeamAffine1D = R"doc(Construct an empty BeamAffine1D)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_BeamAffine1D_2 =
R"doc(Construct a BeamAffine1D for a given number of beams (uninitialized
values)

Args:
    n_beams: number of beams)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_BeamAffine1D_3 =
R"doc(Construct a BeamAffine1D from existing offsets and slopes

Args:
    offsets_: base value per beam [n_beams]
    slopes_: slope per beam [n_beams])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_forward =
R"doc(Compute value = offset + slope * sample_nr for a single beam

Args:
    beam_index: beam index
    sample_nr: sample number (float for sub-sample interpolation)

Returns:
    float)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_from_base_and_endpoints =
R"doc(Create a BeamAffine1D from a base value and per-beam end values at
known sample numbers.

slope = (end_values - base_value) / end_sample_numbers offset =
base_value (for all beams)

Args:
    base_value: scalar base value (at sample 0)
    end_values: per-beam values at the end sample [n_beams]
    end_sample_numbers: per-beam sample number of the end values
                        [n_beams]

Returns:
    BeamAffine1D)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_inverse =
R"doc(Compute sample_nr = (value - offset) / slope for a single beam

Args:
    beam_index: beam index
    value: the value to invert

Returns:
    float sample number)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_offsets = R"doc([n_beams] base value per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_slopes = R"doc([n_beams] slope (change per sample) per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamAffine1D_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


