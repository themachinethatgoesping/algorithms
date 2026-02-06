//sourcehash: 0f69347ba06e1b4e3479310d9532b1518728cb6aba6c00b1167e1e0f19debf85

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


static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_apply_wci_sidelobe_correction =
R"doc(Apply SRSN sidelobe correction to a WCI tensor.

Applies the Slant Range Signal Normalization by subtracting the per-
sample average across beams and adding a reference level:

  result[beam, sample] = wci[beam, sample] -
  per_sample_average[sample] + reference_level

The per_sample_average should be precomputed using
compute_nanmean_across_beams or compute_nanmedian_across_beams. The
reference_level is a scalar that re-anchors the corrected data to a
meaningful dB range (e.g., computed via compute_reference_nanmean or
compute_reference_nanpercentile).

Reference: Schimel, A.C.G. et al. (2020). "Multibeam Sonar Backscatter
           Data
Processing." Remote Sensing, 12(9), 1371.

Args:
    wci: The input 2D WCI tensor (beams x samples).
    per_sample_average: A 1D tensor (n_samples) with the average
                        across beams.
    reference_level: A scalar reference level (dB) to re-anchor the
                     data.
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor (beams x samples).
    t_xtensor_1d: Type of the 1D tensor (samples).

Returns:
    A 2D tensor with the sidelobe correction applied.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_nanmean_across_beams =
R"doc(Compute the mean value across all beams for each sample position,
ignoring NaN.

For a WCI tensor of shape (n_beams x n_samples), this computes the
NaN-aware mean along axis 0 (beams), producing a 1D tensor of shape
(n_samples).

This is the first step of the SRSN sidelobe correction: computing the
average signal level across all beams at each range/sample position.

NaN values are excluded from the mean (equivalent to MATLAB
mean(...,'omitnan')). If all beams at a sample position are NaN, the
result for that sample is NaN.

Args:
    wci: The input 2D WCI tensor (beams x samples).
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D input tensor (beams x samples).
    t_xtensor_1d: Type of the 1D output tensor (samples).

Returns:
    A 1D tensor of shape (n_samples) with the NaN-aware mean across
    beams.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_nanmedian_across_beams =
R"doc(Compute the median value across all beams for each sample position,
ignoring NaN.

For a WCI tensor of shape (n_beams x n_samples), this computes the
NaN-aware median along axis 0 (beams), producing a 1D tensor of shape
(n_samples).

The median is more robust to outliers than the mean, which can be
useful when strong targets (e.g., fish schools) are present in a few
beams.

NaN values are excluded before computing the median (equivalent to
MATLAB median(...,'omitnan')). If all beams at a sample position are
NaN, the result for that sample is NaN.

Uses std::nth_element for O(n) average complexity per sample. The
computation across samples is parallelized with OpenMP.

Args:
    wci: The input 2D WCI tensor (beams x samples).
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D input tensor (beams x samples).
    t_xtensor_1d: Type of the 1D output tensor (samples).

Returns:
    A 1D tensor of shape (n_samples) with the NaN-aware median across
    beams.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_reference_nanmean =
R"doc(Compute a reference level as the NaN-aware mean of all elements in a
2D WCI region.

This is used in the SRSN algorithm to compute a reference level from
the "clean" water column (e.g., data above the minimum slant range).
The caller should extract the appropriate sub-region before calling
this function.

NaN values are excluded from the computation.

Args:
    wci_region: A 2D tensor representing the reference region (e.g.,
                clean WC above bottom).

Template Args:
    t_xtensor_2d: Type of the 2D tensor.

Returns:
    The NaN-aware mean of all elements as a scalar.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_reference_nanpercentile =
R"doc(Compute a reference level as a NaN-aware percentile of all elements in
a 2D WCI region.

This is used in the SRSN algorithm to compute a reference level from
the "clean" water column. Common choices are the 25th percentile
(default in Schimel et al., 2020), 5th, 10th percentile, or 50th
percentile (median).

NaN values are excluded before computing the percentile.

Args:
    wci_region: A 2D tensor representing the reference region (e.g.,
                clean WC above bottom).
    percentile: The percentile to compute, in range [0, 100]. E.g., 25
                for 25th percentile.

Template Args:
    t_xtensor_2d: Type of the 2D tensor.

Returns:
    The NaN-aware percentile of all elements as a scalar.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_inplace_wci_sidelobe_correction =
R"doc(Inplace apply SRSN sidelobe correction to a WCI tensor.

Applies the Slant Range Signal Normalization in-place:

  wci[beam, sample] += reference_level - per_sample_average[sample]

Optionally limits correction to a beam index range [min_beam_index,
max_beam_index].

Args:
    wci: The 2D WCI tensor to correct in-place (beams x samples).
    per_sample_average: A 1D tensor (n_samples) with the average
                        across beams.
    reference_level: A scalar reference level (dB) to re-anchor the
                     data.
    min_beam_index: Optional minimum beam index to start correction
                    from.
    max_beam_index: Optional maximum beam index to end correction at.
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: Type of the 2D tensor (beams x samples).
    t_xtensor_1d: Type of the 1D tensor (samples).)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


