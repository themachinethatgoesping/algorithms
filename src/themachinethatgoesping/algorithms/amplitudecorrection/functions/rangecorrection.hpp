// SPDX-FileCopyrightText: 2024 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/rangecorrection.doc.hpp"

/* generated doc strings */
#include <Eigen/Core>
#include <Eigen/Dense>
#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>

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

template<typename t_float>
inline t_float approximate_range_factor(t_float sample_interval_s, t_float sound_velocity_m_s)
{
    // calculate the ranges
    return sample_interval_s * sound_velocity_m_s * t_float(0.5);
}

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

template<tools::helper::c_xtensor t_xtensor_1d,
         tools::helper::c_xtensor t_xtensor_1d_int>
inline t_xtensor_1d approximate_ranges(
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sample_interval_s,
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type sound_velocity_m_s,
    const t_xtensor_1d_int&                                          sample_numbers)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d_int>,
                  "Template parameter must be a 1D tensor");
    using t_float = tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    return (sample_numbers + t_float(0.5)) *
           approximate_range_factor(sample_interval_s, sound_velocity_m_s);
}

template<tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_1d compute_cw_range_correction(
    const t_xtensor_1d&                                          ranges_m,
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type absorption_db_m,
    typename tools::helper::xtensor_datatype<t_xtensor_1d>::type tvg_factor)
{
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    using t_float = typename tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    // range correction = absorption*R + tvg_factor*log10(R)
    if (tools::helper::float_is_finite_and_not_zero(absorption_db_m))
        if (tools::helper::float_is_finite_and_not_zero(tvg_factor))
            return (2*absorption_db_m) * ranges_m + tvg_factor * xt::log10(ranges_m);

    if (tools::helper::float_is_finite_and_not_zero(tvg_factor))
        return tvg_factor * xt::log10(ranges_m);

    if (tools::helper::float_is_finite_and_not_zero(absorption_db_m))
        return (2*absorption_db_m) * ranges_m;

    return xt::zeros_like(ranges_m);
    // range correction = absorption*R + tvg_factor*log10(R)
}

}
}
}
}
