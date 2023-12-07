//sourcehash: eadffc59f2b84922204021d6f5bef71316a067e42e884367467852ada75bb0ac

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


static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters =
R"doc(@struct FMSignalParameters Struct representing information about a
frequency modulated wave signal (chirp).)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_FMSignalParameters = R"doc(Default constructor.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_FMSignalParameters_2 =
R"doc(Constructor.

Parameter ``center_frequency``:
    The center frequency of the signal in Hz.

Parameter ``bandwidth``:
    The bandwidth of the signal in Hz.

Parameter ``effective_pulse_duration``:
    The effective pulse duration of the signal in seconds.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_FMSignalParameters_3 =
R"doc(Constructor.

Parameter ``center_frequency``:
    The center frequency of the signal in Hz.

Parameter ``bandwidth``:
    The bandwidth of the signal in Hz.

Parameter ``effective_pulse_duration``:
    The effective pulse duration of the signal in seconds.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_bandwidth = R"doc(< Bandwidth of the signal in Hz.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_center_frequency = R"doc(< Center frequency of the signal in Hz.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_effective_pulse_duration = R"doc(< Effective pulse duration of the signal in seconds.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_from_stream =
R"doc(Read FMSignalParameters from a stream.

Parameter ``is``:
    The input stream.

Returns:
    The read FMSignalParameters object.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_get_tx_signal_type = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_operator_eq =
R"doc(Equality operator.

Parameter ``rhs``:
    The right-hand side of the operator.

Returns:
    True if the objects are equal, false otherwise.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_printer =
R"doc(Get the ObjectPrinter for FMSignalParameters.

Parameter ``float_precision``:
    The precision for floating-point values.

Returns:
    The ObjectPrinter.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_to_stream =
R"doc(Write FMSignalParameters to a stream.

Parameter ``os``:
    The output stream.)doc";

static const char *__doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_FMSignalParameters_up_sweep = R"doc(< True if the signal is an up sweep, false otherwise.)doc";


#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


