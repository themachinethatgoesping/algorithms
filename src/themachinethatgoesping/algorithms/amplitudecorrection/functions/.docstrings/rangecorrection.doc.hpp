//sourcehash: 0f3cbeeb8f90f8c2c4f87232b93d46025ba6d72c21311c41471cb0aa1a59e9c0

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


static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_range_factor =
R"doc(Approximates the range factor based on the sample interval and a
single sound velocity.

This function calculates the range factor using the given sample
interval and sound velocity. The formula used is: range_factor =
sample_interval_s * sound_velocity_m_s * 0.5

Args:
    sample_interval_s: The sample interval in seconds.
    sound_velocity_m_s: The sound velocity in meters per second.

Template Args:
    t_float: The floating-point type used for the calculations.

Returns:
    The approximated range factor.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_ranges =
R"doc(Approximates the ranges based on the provided sample interval and a
single sound velocity.

This function calculates the approximate ranges for a given set of
sample numbers by multiplying the sample numbers (plus half) with the
approximate range factor.

Args:
    sample_interval_s: The sample interval in seconds.
    sound_velocity_m_s: The sound velocity in meters per second.
    first_sample_nr: The first sample number.
    last_sample_nr: The last sample number.
    step: The step size between sample numbers (default is 1).

Template Args:
    t_xtensor_1d: A 1D tensor type that satisfies the c_xtensor_1d
                  concept.
    t_int: An integer type for sample numbers.

Returns:
    A 1D tensor containing the approximated ranges.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_ranges_2 =
R"doc(Approximates the ranges based on sample interval, a single sound
velocity, and sample numbers.

This function calculates the approximate ranges by using the provided
sample interval, sound velocity, and sample numbers. The calculation
is performed by adding 0.5 to each sample number and then multiplying
by the approximate range factor.

Args:
    sample_interval_s: The interval between samples in seconds.
    sound_velocity_m_s: The velocity of sound in meters per second.
    sample_numbers: A 1D tensor containing the sample numbers.

Template Args:
    t_xtensor_1d: A 1D tensor type that satisfies the
                  tools::helper::c_xtensor concept.
    t_xtensor_1d_int: A 1D tensor type for integers that satisfies the
                      tools::helper::c_xtensor concept.

Returns:
    A 1D tensor containing the approximated ranges.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_cw_range_correction =
R"doc(Computes the continuous wave (CW) range correction.

This function calculates the range correction based on the provided
ranges, absorption coefficient, and time-varying gain (TVG) factor.
The range correction is computed using the formula:

\f[ \text{range correction} = 2 \cdot \text{absorption\_db\_m} \cdot
\text{ranges\_m} + \text{tvg\_factor} \cdot
\log_{10}(\text{ranges\_m}) \f]

If the absorption coefficient is finite and non-zero, it is used in
the calculation. If the TVG factor is finite and non-zero, it is also
included in the calculation. If neither the absorption coefficient nor
the TVG factor are finite and non-zero, the function returns a tensor
of zeros with the same shape as the input ranges.

Args:
    ranges_m: A 1D tensor representing the ranges in meters.
    absorption_db_m: The absorption coefficient in decibels per meter.
    tvg_factor: The time-varying gain factor.

Template Args:
    t_xtensor_1d: A 1D tensor type that satisfies the
                  tools::helper::c_xtensor_1d concept.

Returns:
    A 1D tensor representing the computed range correction.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_cw_range_correction_per_beam =
R"doc(Computes the per-beam continuous wave (CW) range correction for 2D
water column data.

This function calculates the range correction for each beam
individually, allowing for per-beam absorption coefficients. This is
useful for multi-sector sonars where each transmit sector may have a
different absorption coefficient.

The range correction for each beam is computed using the formula: \f[
\text{range correction}[beam, sample] = 2 \cdot
\text{absorption\_db\_m}[beam] \cdot \text{ranges\_m}[sample] +
\text{tvg\_factor} \cdot \log_{10}(\text{ranges\_m}[sample]) \f]

Args:
    ranges_m: A 1D tensor representing the ranges in meters (size =
              number of samples).
    absorption_db_m_per_beam: A 1D tensor of absorption coefficients
                              in dB/m per beam (size = number of
                              beams).
    tvg_factor: Optional time-varying gain factor (applied uniformly
                to all beams).
    mp_cores: The number of cores to use for parallel processing
              (default is 1).

Template Args:
    t_xtensor_2d: A 2D tensor type that satisfies the
                  tools::helper::c_xtensor_2d concept.
    t_xtensor_1d: A 1D tensor type that satisfies the
                  tools::helper::c_xtensor_1d concept.

Returns:
    A 2D tensor (beams x samples) representing the computed range
    correction per beam.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_get_sample_numbers_plus_half =
R"doc(Generates a 1D tensor of sample numbers incremented by half. (used for
range compuation)

This function calculates a range of sample numbers starting from
`first_sample_nr + 0.5` to `last_sample_nr + 1.5` with a specified
step.

Args:
    first_sample_nr: The starting sample number.
    last_sample_nr: The ending sample number.
    step: The step size for the range (default is 1).

Template Args:
    t_xtensor_1d: A 1D tensor type that satisfies the
                  `tools::helper::c_xtensor` concept.
    t_int: An integer type for the sample numbers.

Returns:
    A 1D tensor of sample numbers incremented by half.

@note The template parameter must be a 1D tensor.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


