//sourcehash: 54bf0668598cbd418b0b6ab4ab8932dc0359c8ffad2744f7d4c9db5ec3c64f26

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


static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_loop = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xsimd = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor2 = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor3 = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_assert_wci_axis_shape = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_assert_wci_beam_sample_shape = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_sample_correction =
R"doc(Template parameter ``t_xtensor_2d``:
    $Template parameter ``t_xtensor_1d``:

Parameter ``wci``:
    $Parameter ``per_sample_offset``:

Parameter ``mp_cores``:)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


