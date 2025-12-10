//sourcehash: 03228fe5c8dfeac0b7883e7eef90fca3c86786db1466195dbc4f08a6784ba5dc

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


static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters = R"doc(Class representing information about a continuous wave signal.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_CWSignalParameters = R"doc(Default constructor.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_CWSignalParameters_2 =
R"doc(Constructor.
Args:
    center_frequency: The center frequency of the signal in Hz.
    bandwidth: The bandwidth of the signal in Hz.
    effective_pulse_duration: The effective pulse duration of the
                              signal in seconds.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_bandwidth = R"doc(Bandwidth of the signal in Hz.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_binary_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_center_frequency = R"doc(Center frequency of the signal in Hz.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_effective_pulse_duration = R"doc(Effective pulse duration of the signal in seconds.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_from_stream =
R"doc(Read CWSignalParameters from a stream.
Args:
    is: The input stream.

Returns:
    The read CWSignalParameters object.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_get_bandwidth = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_get_center_frequency = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_get_effective_pulse_duration = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_get_tx_signal_type = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_operator_eq =
R"doc(Equality operator.
Args:
    rhs: The right-hand side of the operator.

Returns:
    True if the objects are equal, false otherwise.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_printer =
R"doc(Get the ObjectPrinter for CWSignalParameters.
Args:
    float_precision: The precision for floating-point values.

Returns:
    The ObjectPrinter.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_set_bandwidth = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_set_center_frequency = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_set_effective_pulse_duration = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_CWSignalParameters_to_stream =
R"doc(Write CWSignalParameters to a stream.
Args:
    os: The output stream.)doc";


#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


