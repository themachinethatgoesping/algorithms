//sourcehash: 92e9c9c569118ac302d157e562ab4cf702c772bbd52e1a1ff465c03dd258ddc7

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


static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection =
R"doc(A class to hold the selected beams/sample range for a single
transducer)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_BeamSampleSelection = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_BeamSampleSelection_2 =
R"doc(Construct a new Beam Sample Selection object

Args:
    beam_selection:

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_BeamSampleSelection_3 =
R"doc(Initialize a beam sample selection from a whole swath

Args:
    first_sample_number_per_beam: first sample number per beam
    last_sample_number_per_beam: last sample number per beam
    sample_step_ensemble: sample step size (same for the entire
                          ensemble)

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_add_beam =
R"doc(Add a beam to the selection

Args:
    beam_nr: beam number
    first_sample_number: first sample number to select
    last_sample_number: last sample number to select

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_binary_hash =
R"doc(compute a 64 bit hash of the object using xxhash and the       \
to_binary function. This  function is called binary because the
\ to_binary  function of the object is used

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_empty =
R"doc(return if the selection contains beams and samples

Returns:
    true false

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_first_sample_number_ensemble = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_first_sample_number_per_beam = R"doc(first sample number per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_from_binary =
R"doc(convert object to vector of bytes
\ \

Args:
    check_buffer_is_read_completely: variable for interface
                                     compatibility, does not do    \
                                     anything
                                     \ \

Returns:
    vector of bytes
    \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_from_stream =
R"doc(Return a BeamSampleSelection read from a binary stream

Args:
    is: input stream

Returns:
    BeamSampleSelection

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_first_sample_number_ensemble =
R"doc(Return the first sample number of the ensemble

Returns:
    uint32_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_first_sample_number_per_beam =
R"doc(Return the first sample number per beam

Returns:
    std::vector<uint32_t>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_last_sample_number_ensemble =
R"doc(Return the last sample number of the ensemble

Returns:
    uint32_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_last_sample_number_per_beam =
R"doc(Return the max number of samples per beam

Returns:
    std::vector<uint32_t>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_number_of_samples_ensemble =
R"doc(return the number of samples within the ensemble

Returns:
    uint32_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_read_sample_range =
R"doc(Return the read sample range for a given beam

Args:
    beam_index: index of the beam within the beam sample selection
    first_sample_offset: offset to the first sample (often 0)
    number_of_samples: number of samples in the real beam structure

Returns:
    ReadSampleRange read sample range

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_sample_numbers_ensemble_1d = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_sample_numbers_ensemble_2d = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_get_sample_step_ensemble =
R"doc(Return the sample step size

Returns:
    uint32_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_info_string =
R"doc(                                                                                           \
return an info string using the class __printer__ object
\

Args:
    float_precision: number of digits for floating point values
                     \
    superscript_exponents: print exponents in superscript
                           \

Returns:
    std::string
    \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_last_sample_number_ensemble = R"doc(maximum sample number (max(last_sample_number_per_beam)))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_last_sample_number_per_beam = R"doc(last sample number per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_print =
R"doc(                                                                                           \
print the object information to the given outpustream
\ \

Args:
    os: output stream, e.g. file stream or std::out or std::cerr
        \
    float_precision: number of digits for floating point values
                     \
    superscript_exponents: print exponents in superscript
                           \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_printer =
R"doc(Print function, needs __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro
See also: classhelper/objectprinter.hpp

Args:
    float_precision: Precision of floating point numbers

Returns:
    classhelper::ObjectPrinter

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_sample_step_ensemble = R"doc(sample step size (same for the entire ensemble))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_set_first_sample_number_ensemble = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_set_last_sample_number_ensemble = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_set_sample_step_ensemble = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_to_binary =
R"doc(convert object to vector of bytes
\ \

Args:
    resize_buffer: variable for interface compatibility, does not do
                   anything             \ \

Returns:
    vector of bytes
    \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_pingtools_BeamSampleSelection_to_stream =
R"doc(Write a BeamSampleSelection to a binary stream

Args:
    os: output stream

)doc";


#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


