// SPDX-FileCopyrightText: 2024 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/rangecorrection.doc.hpp"

#include <optional>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <xtensor/containers/xtensor.hpp>


#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>

/**
 * @brief Generates a 1D tensor of sample numbers incremented by half. (used for range compuation)
 *
 * This function calculates a range of sample numbers starting from
 * `first_sample_nr + 0.5` to `last_sample_nr + 1.5` with a specified step.
 *
 * @tparam t_xtensor_1d A 1D tensor type that satisfies the `tools::helper::c_xtensor` concept.
 * @tparam t_int An integer type for the sample numbers.
 * @param first_sample_nr The starting sample number.
 * @param last_sample_nr The ending sample number.
 * @param step The step size for the range (default is 1).
 * @return A 1D tensor of sample numbers incremented by half.
 * @note The template parameter must be a 1D tensor.
 */
template<tools::helper::c_xtensor t_xtensor_1d, typename t_int>
inline t_xtensor_1d get_sample_numbers_plus_half(t_int first_sample_nr,
                                                 t_int last_sample_nr,
                                                 t_int step = 1)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    using t_float = tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    // calculate the sample numbers
    return xt::arange<t_float>(first_sample_nr + t_float(0.5), last_sample_nr + t_float(1.5), step);
}

/**
 * @brief Approximates the range factor based on the sample interval and a single sound velocity.
 *
 * This function calculates the range factor using the given sample interval and sound velocity.
 * The formula used is: range_factor = sample_interval_s * sound_velocity_m_s * 0.5
 *
 * @tparam t_float The floating-point type used for the calculations.
 * @param sample_interval_s The sample interval in seconds.
 * @param sound_velocity_m_s The sound velocity in meters per second.
 * @return The approximated range factor.
 */
template<typename t_float>
inline t_float approximate_range_factor(t_float sample_interval_s, t_float sound_velocity_m_s)
{
    // calculate the ranges
    return sample_interval_s * sound_velocity_m_s * t_float(0.5);
}

/**
 * @brief Approximates the ranges based on the provided sample interval and a single sound velocity.
 *
 * This function calculates the approximate ranges for a given set of sample numbers
 * by multiplying the sample numbers (plus half) with the approximate range factor.
 *
 * @tparam t_xtensor_1d A 1D tensor type that satisfies the c_xtensor_1d concept.
 * @tparam t_int An integer type for sample numbers.
 * @param sample_interval_s The sample interval in seconds.
 * @param sound_velocity_m_s The sound velocity in meters per second.
 * @param first_sample_nr The first sample number.
 * @param last_sample_nr The last sample number.
 * @param step The step size between sample numbers (default is 1).
 * @return A 1D tensor containing the approximated ranges.
 */
template<tools::helper::c_xtensor t_xtensor_1d, typename t_int>
inline t_xtensor_1d approximate_ranges(
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sample_interval_s,
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sound_velocity_m_s,
    t_int                                                        first_sample_nr,
    t_int                                                        last_sample_nr,
    t_int                                                        step = 1)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    return get_sample_numbers_plus_half<t_xtensor_1d, t_int>(
               first_sample_nr, last_sample_nr, step) *
           approximate_range_factor(sample_interval_s, sound_velocity_m_s);
}

/**
 * @brief Approximates the ranges based on sample interval, a single sound velocity, and sample
 * numbers.
 *
 * This function calculates the approximate ranges by using the provided sample interval,
 * sound velocity, and sample numbers. The calculation is performed by adding 0.5 to each
 * sample number and then multiplying by the approximate range factor.
 *
 * @tparam t_xtensor_1d A 1D tensor type that satisfies the tools::helper::c_xtensor concept.
 * @tparam t_xtensor_1d_int A 1D tensor type for integers that satisfies the
 * tools::helper::c_xtensor concept.
 * @param sample_interval_s The interval between samples in seconds.
 * @param sound_velocity_m_s The velocity of sound in meters per second.
 * @param sample_numbers A 1D tensor containing the sample numbers.
 * @return A 1D tensor containing the approximated ranges.
 */
template<tools::helper::c_xtensor t_xtensor_1d, tools::helper::c_xtensor t_xtensor_1d_int>
inline t_xtensor_1d approximate_ranges(
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sample_interval_s,
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sound_velocity_m_s,
    const t_xtensor_1d_int&                                      sample_numbers)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d_int>,
                  "Template parameter must be a 1D tensor");
    using t_float = tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    return (sample_numbers + t_float(0.5)) *
           approximate_range_factor(sample_interval_s, sound_velocity_m_s);
}

/**
 * @brief Computes the continuous wave (CW) range correction.
 *
 * This function calculates the range correction based on the provided ranges,
 * absorption coefficient, and time-varying gain (TVG) factor. The range correction
 * is computed using the formula:
 *
 * \f[
 * \text{range correction} = 2 \cdot \text{absorption\_db\_m} \cdot \text{ranges\_m} +
 * \text{tvg\_factor} \cdot \log_{10}(\text{ranges\_m}) \f]
 *
 * If the absorption coefficient is finite and non-zero, it is used in the calculation.
 * If the TVG factor is finite and non-zero, it is also included in the calculation.
 * If neither the absorption coefficient nor the TVG factor are finite and non-zero,
 * the function returns a tensor of zeros with the same shape as the input ranges.
 *
 * @tparam t_xtensor_1d A 1D tensor type that satisfies the tools::helper::c_xtensor_1d concept.
 * @param ranges_m A 1D tensor representing the ranges in meters.
 * @param absorption_db_m The absorption coefficient in decibels per meter.
 * @param tvg_factor The time-varying gain factor.
 * @return A 1D tensor representing the computed range correction.
 */
template<tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_1d compute_cw_range_correction(
    const t_xtensor_1d&                                                         ranges_m,
    std::optional<typename tools::helper::xtensor_datatype<t_xtensor_1d>::type> absorption_db_m,
    std::optional<typename tools::helper::xtensor_datatype<t_xtensor_1d>::type> tvg_factor)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    // using t_float = typename tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    // range correction = absorption*R + tvg_factor*log10(R)
    if (absorption_db_m.has_value())
    {
        if (tvg_factor.has_value())
            return (2 * absorption_db_m.value()) * ranges_m + tvg_factor.value() * xt::log10(ranges_m);

        return (2 * absorption_db_m.value()) * ranges_m;
    }

    if (tvg_factor.has_value())
        return tvg_factor.value() * xt::log10(ranges_m);

    return xt::zeros_like(ranges_m);
    // range correction = absorption*R + tvg_factor*log10(R)
}

}
}
}
}
