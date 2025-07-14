//sourcehash: ed1ecf05ac6149ab4c9b09fc1707a979f563c4312c9baf09c6cb13b03a668c9d

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


static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_range_factor =
R"doc(Approximates the range factor based on the sample interval and a
single sound velocity.

This function calculates the range factor using the given sample
interval and sound velocity. The formula used is: range_factor =
sample_interval_s * sound_velocity_m_s * 0.5

Template parameter ``t_float``:
    The floating-point type used for the calculations.

Parameter ``sample_interval_s``:
    The sample interval in seconds.

Parameter ``sound_velocity_m_s``:
    The sound velocity in meters per second.

Returns:
    The approximated range factor.)doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_ranges =
R"doc(Approximates the ranges based on the provided sample interval and a
single sound velocity.

This function calculates the approximate ranges for a given set of
sample numbers by multiplying the sample numbers (plus half) with the
approximate range factor.

Template parameter ``t_xtensor_1d``:
    A 1D tensor type that satisfies the c_xtensor_1d concept.

Template parameter ``t_int``:
    An integer type for sample numbers.

Parameter ``sample_interval_s``:
    The sample interval in seconds.

Parameter ``sound_velocity_m_s``:
    The sound velocity in meters per second.

Parameter ``first_sample_nr``:
    The first sample number.

Parameter ``last_sample_nr``:
    The last sample number.

Parameter ``step``:
    The step size between sample numbers (default is 1).

Returns:
    A 1D tensor containing the approximated ranges.)doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_approximate_ranges_2 =
R"doc(Approximates the ranges based on sample interval, a single sound
velocity, and sample numbers.

This function calculates the approximate ranges by using the provided
sample interval, sound velocity, and sample numbers. The calculation
is performed by adding 0.5 to each sample number and then multiplying
by the approximate range factor.

Template parameter ``t_xtensor_1d``:
    A 1D tensor type that satisfies the tools::helper::c_xtensor
    concept.

Template parameter ``t_xtensor_1d_int``:
    A 1D tensor type for integers that satisfies the
    tools::helper::c_xtensor concept.

Parameter ``sample_interval_s``:
    The interval between samples in seconds.

Parameter ``sound_velocity_m_s``:
    The velocity of sound in meters per second.

Parameter ``sample_numbers``:
    A 1D tensor containing the sample numbers.

Returns:
    A 1D tensor containing the approximated ranges.)doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_compute_cw_range_correction =
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

Template parameter ``t_xtensor_1d``:
    A 1D tensor type that satisfies the tools::helper::c_xtensor_1d
    concept.

Parameter ``ranges_m``:
    A 1D tensor representing the ranges in meters.

Parameter ``absorption_db_m``:
    The absorption coefficient in decibels per meter.

Parameter ``tvg_factor``:
    The time-varying gain factor.

Returns:
    A 1D tensor representing the computed range correction.)doc";

static const char *__doc_themachinethatgoesping_algorithms_amplitudecorrection_functions_get_sample_numbers_plus_half =
R"doc(Generates a 1D tensor of sample numbers incremented by half. (used for
range compuation)

This function calculates a range of sample numbers starting from
`first_sample_nr + 0.5` to `last_sample_nr + 1.5` with a specified
step.

Template parameter ``t_xtensor_1d``:
    A 1D tensor type that satisfies the `tools::helper::c_xtensor`
    concept.

Template parameter ``t_int``:
    An integer type for the sample numbers.

Parameter ``first_sample_nr``:
    The starting sample number.

Parameter ``last_sample_nr``:
    The ending sample number.

Parameter ``step``:
    The step size for the range (default is 1).

Returns:
    A 1D tensor of sample numbers incremented by half. @note The
    template parameter must be a 1D tensor.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


