// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/backwardmapping.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <xtensor/containers/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace imageprocessing {
namespace functions {
namespace detail {

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline size_t lower_bound_index(const t_xtensor_1d& coords, const T value)
{
    size_t left  = 0;
    size_t right = coords.size();

    while (left < right)
    {
        const size_t mid = left + (right - left) / 2;
        if (coords(mid) < value)
            left = mid + 1;
        else
            right = mid;
    }

    return left;
}

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline size_t nearest_index(const t_xtensor_1d& coords, const T value)
{
    if (coords.size() == 0)
        throw std::invalid_argument("Coordinate array must not be empty");

    if (coords.size() == 1)
        return 0;

    const size_t upper = lower_bound_index(coords, value);

    if (upper == 0)
        return 0;

    if (upper >= coords.size())
        return coords.size() - 1;

    const size_t lower = upper - 1;

    const auto lower_diff = std::abs(static_cast<double>(value - coords(lower)));
    const auto upper_diff = std::abs(static_cast<double>(coords(upper) - value));

    return (upper_diff < lower_diff) ? upper : lower;
}

struct Bracket
{
    size_t lower;
    size_t upper;
    double weight;
};

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline Bracket bracket_indices(const t_xtensor_1d& coords, const T value)
{
    if (coords.size() == 0)
        throw std::invalid_argument("Coordinate array must not be empty");

    if (coords.size() == 1)
        return Bracket{ 0, 0, 0.0 };

    const size_t upper = lower_bound_index(coords, value);

    if (upper == 0)
        return Bracket{ 0, 0, 0.0 };

    if (upper >= coords.size())
        return Bracket{ coords.size() - 1, coords.size() - 1, 0.0 };

    const size_t lower = upper - 1;
    const auto  x0     = static_cast<double>(coords(lower));
    const auto  x1     = static_cast<double>(coords(upper));

    const double denom = x1 - x0;
    const double w     = (denom == 0.0)
                             ? 0.0
                             : std::clamp((static_cast<double>(value) - x0) / denom, 0.0, 1.0);

    return Bracket{ lower, upper, w };
}

} // namespace detail

template<tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_1d t_xtensor_new_x,
         tools::helper::c_xtensor_1d t_xtensor_new_y>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_nearest(
    const t_xtensor_2d&    reference,
    const t_xtensor_ref_x& reference_x,
    const t_xtensor_ref_y& reference_y,
    const t_xtensor_new_x& new_x,
    const t_xtensor_new_y& new_y,
    const int              mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size(), new_y.size() });

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < new_y.size(); ++iy)
        {
            const auto ref_ix = detail::nearest_index(reference_x, new_x(ix));
            const auto ref_iy = detail::nearest_index(reference_y, new_y(iy));
            output(ix, iy)    = reference(ref_ix, ref_iy);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_1d t_xtensor_new_x,
         tools::helper::c_xtensor_1d t_xtensor_new_y>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_bilinear(
    const t_xtensor_2d&    reference,
    const t_xtensor_ref_x& reference_x,
    const t_xtensor_ref_y& reference_y,
    const t_xtensor_new_x& new_x,
    const t_xtensor_new_y& new_y,
    const int              mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size(), new_y.size() });

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < new_y.size(); ++iy)
        {
            const auto bx = detail::bracket_indices(reference_x, new_x(ix));
            const auto by = detail::bracket_indices(reference_y, new_y(iy));

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) +
                                               wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) +
                                               wy1 * static_cast<double>(v11));

            output(ix, iy) = static_cast<value_type>(interpolated);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_reference,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_2d t_xtensor_target,
         tools::helper::c_xtensor_1d t_xtensor_target_x,
         tools::helper::c_xtensor_1d t_xtensor_target_y>
void backward_map_nearest_add(
    const t_xtensor_reference&    reference,
    const t_xtensor_ref_x&        reference_x,
    const t_xtensor_ref_y&        reference_y,
    t_xtensor_target&             target,
    const t_xtensor_target_x&     target_x,
    const t_xtensor_target_y&     target_y,
    const int                     mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    if (target.shape()[0] != target_x.size() || target.shape()[1] != target_y.size())
        throw std::invalid_argument("Target coordinate arrays must match target image shape");

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < target_y.size(); ++iy)
        {
            const auto ref_ix = detail::nearest_index(reference_x, target_x(ix));
            const auto ref_iy = detail::nearest_index(reference_y, target_y(iy));
            target(ix, iy) += static_cast<target_value_type>(reference(ref_ix, ref_iy));
        }
    }
}

template<tools::helper::c_xtensor_2d t_xtensor_reference,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_2d t_xtensor_target,
         tools::helper::c_xtensor_1d t_xtensor_target_x,
         tools::helper::c_xtensor_1d t_xtensor_target_y>
void backward_map_bilinear_add(
    const t_xtensor_reference&    reference,
    const t_xtensor_ref_x&        reference_x,
    const t_xtensor_ref_y&        reference_y,
    t_xtensor_target&             target,
    const t_xtensor_target_x&     target_x,
    const t_xtensor_target_y&     target_y,
    const int                     mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    if (target.shape()[0] != target_x.size() || target.shape()[1] != target_y.size())
        throw std::invalid_argument("Target coordinate arrays must match target image shape");

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < target_y.size(); ++iy)
        {
            const auto bx = detail::bracket_indices(reference_x, target_x(ix));
            const auto by = detail::bracket_indices(reference_y, target_y(iy));

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) +
                                               wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) +
                                               wy1 * static_cast<double>(v11));

            target(ix, iy) += static_cast<target_value_type>(interpolated);
        }
    }
}

} // namespace functions
} // namespace imageprocessing
} // namespace algorithms
} // namespace themachinethatgoesping