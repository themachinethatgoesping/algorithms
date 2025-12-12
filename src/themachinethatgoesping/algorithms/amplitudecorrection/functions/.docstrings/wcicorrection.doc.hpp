//sourcehash: 91c06489d6352edd3327957425b1cdf5f6f9b8e3d7d25bad042b36015e673d96

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


static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_correction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction =
R"doc(Applies beam and sample corrections to the given 2D tensor.

Apply beam and sample corrections to the input 2D tensor. per_beam
correction is applied to each sample in a beam, per_sample correction
to each sample nr for each beam

Args:
    wci: The input 2D tensor to which corrections will be applied.
    per_beam_offset: A 1D tensor containing the per-beam offsets.
    per_sample_offset: A 1D tensor containing the per-sample offsets.
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor.
    t_xtensor_1d: Type of the 1D tensor.

Returns:
    A 2D tensor with the applied beam and sample corrections.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_loop = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_with_absorption =
R"doc(Applies beam, sample, and per-beam absorption corrections to the given
2D tensor.

This function applies per-beam offset, per-sample offset, and per-beam
absorption correction to the input 2D tensor. The absorption
correction is computed as 2 * absorption_db_m * ranges_m for each beam
individually, allowing for multi-sector sonar data with different
absorption coefficients per sector/beam.

Args:
    wci: The input 2D tensor to which corrections will be applied.
    per_beam_offset: A 1D tensor containing the per-beam offsets (size
                     = number of beams).
    per_sample_offset: A 1D tensor containing the per-sample offsets
                       (size = number of samples).
    ranges_m: A 1D tensor containing the ranges in meters (size =
              number of samples).
    absorption_db_m_per_beam: A 1D tensor of absorption coefficients
                              in dB/m per beam (size = number of
                              beams).
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor.
    t_xtensor_1d: Type of the 1D tensor.

Returns:
    A 2D tensor with the applied corrections.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xsimd = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor2 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_beam_sample_correction_xtensor3 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_sample_correction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_system_offset = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_correction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_sample_correction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_beam_sample_correction_with_absorption =
R"doc(Inplace applies beam, sample, and per-beam absorption corrections to
the given 2D tensor.

This function applies per-beam offset, per-sample offset, and per-beam
absorption correction in-place to the input 2D tensor. The absorption
correction is computed as 2 * absorption_db_m * ranges_m for each beam
individually.

Args:
    wci: The input 2D tensor to which corrections will be applied
         (modified in-place).
    per_beam_offset: A 1D tensor containing the per-beam offsets (size
                     = number of beams).
    per_sample_offset: A 1D tensor containing the per-sample offsets
                       (size = number of samples).
    ranges_m: A 1D tensor containing the ranges in meters (size =
              number of samples).
    absorption_db_m_per_beam: A 1D tensor of absorption coefficients
                              in dB/m per beam (size = number of
                              beams).
    min_beam_index: Optional minimum beam index to start correction
                    from.
    max_beam_index: Optional maximum beam index to end correction at.
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor.
    t_xtensor_1d: Type of the 1D tensor.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_sample_correction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_sample_correction_with_absorption =
R"doc(Inplace applies sample and per-beam absorption corrections to the
given 2D tensor.

This function applies per-sample offset and per-beam absorption
correction in-place to the input 2D tensor (without per-beam offset).
The absorption correction is computed as 2 * absorption_db_m
* ranges_m for each beam individually.

Args:
    wci: The input 2D tensor to which corrections will be applied
         (modified in-place).
    per_sample_offset: A 1D tensor containing the per-sample offsets
                       (size = number of samples).
    ranges_m: A 1D tensor containing the ranges in meters (size =
              number of samples).
    absorption_db_m_per_beam: A 1D tensor of absorption coefficients
                              in dB/m per beam (size = number of
                              beams).
    min_beam_index: Optional minimum beam index to start correction
                    from.
    max_beam_index: Optional maximum beam index to end correction at.
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor.
    t_xtensor_1d: Type of the 1D tensor.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_system_offset = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


