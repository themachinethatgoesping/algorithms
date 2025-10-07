//sourcehash: 4541bc0b319cee3880f339f21f019ef6cf99ee996a20718ed87ebab0d97fb888

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

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction =
R"doc(Applies beam and sample corrections to the given 2D tensor.

Apply beam and sample corrections to the input 2D tensor. per_beam
correction is applied to each sample in a beam, per_sample correction
to each sample nr for each beam

Template parameter ``t_xtensor_2d``:
    Type of the 2D tensor.

Template parameter ``t_xtensor_1d``:
    Type of the 1D tensor.

Parameter ``wci``:
    The input 2D tensor to which corrections will be applied.

Parameter ``per_beam_offset``:
    A 1D tensor containing the per-beam offsets.

Parameter ``per_sample_offset``:
    A 1D tensor containing the per-sample offsets.

Parameter ``mp_cores``:
    The number of cores to use for parallel processing (default is 1).

Returns:
    A 2D tensor with the applied beam and sample corrections.)doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_loop = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xsimd = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor2 = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor3 = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_system_offset = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_sample_correction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_system_offset = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


