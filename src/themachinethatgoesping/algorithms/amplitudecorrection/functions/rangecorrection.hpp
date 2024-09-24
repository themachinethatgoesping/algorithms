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

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

template<template<typename, size_t> typename t_xtensor, typename t_float, typename t_int>
inline t_xtensor<t_float, 1> get_sample_numbers_plus_half(t_int first_sample_nr,
                                                          t_int last_sample_nr,
                                                          t_int step = 1)
{
    // calculate the sample numbers
    return xt::arange<t_float>(first_sample_nr + 0.5, last_sample_nr + 0.5 + 1, step);
}

template<typename t_float>
inline t_float approximate_range_factor(t_float sample_interval_s, t_float sound_velocity_m_s)
{
    // calculate the ranges
    return sample_interval_s * sound_velocity_m_s * 0.5;
}

template<template<typename, size_t> typename t_xtensor, typename t_float, typename t_int>
inline t_xtensor<t_float, 1> approximate_ranges(t_float sample_interval_s,
                                                t_float sound_velocity_m_s,
                                                t_int   first_sample_nr,
                                                t_int   last_sample_nr,
                                                t_int   step = 1)
{
    return get_sample_numbers_plus_half<t_xtensor, t_float, t_int>(
               first_sample_nr, last_sample_nr, step) *
           approximate_range_factor(sample_interval_s, sound_velocity_m_s);
}

template<template<typename, size_t> typename t_xtensor, typename t_float>
inline t_xtensor<t_float, 1> compute_cw_range_correction(const t_xtensor<t_float, 1>& ranges_m,
                                                         t_float absorption_db_m,
                                                         t_float tvg_factor)
{
    // range correction = absorption*R + tvg_factor*log10(R)
    return absorption_db_m * ranges_m + tvg_factor * xt::log10(ranges_m);
}

}
}
}
}
