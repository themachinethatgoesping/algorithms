// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
#pragma once

/* generated doc strings */
#include ".docstrings/weighted_median.doc.hpp"

#include <stdexcept>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <xtensor/xindex_view.hpp>
#include <xtensor/xmanipulation.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace imageprocessing {
namespace functions {

/**
 * @brief Computes the weighted median of a 1D xtensor without using Boost.
 *
 * The weighted median is defined as the smallest value for which the
 * cumulative weight is >= 50% of the total weight.
 * Note: Edge case with inbalanced partial weights is handled by returning the weighted mean of the
 * two closest values.
 *
 * @tparam t_xtensor_val The type of the 1D xtensor for values.
 * @tparam t_xtensor_weight The type of the 1D xtensor for weights.
 * @param values  1D xtensor of numerical values.
 * @param weights 1D xtensor of corresponding weights (same shape as values). Weights must be
 * positive!
 * @return The weighted median as a single scalar value.
 */
template<tools::helper::c_xtensor_1d t_xtensor_val, tools::helper::c_xtensor_1d t_xtensor_weight>
typename t_xtensor_val::value_type weighted_median(const t_xtensor_val&    values,
                                                   const t_xtensor_weight& weights)
{
    using value_type = typename t_xtensor_val::value_type;

    static_assert(tools::helper::c_xtensor_1d<t_xtensor_val>,
                  "Template parameter 1 must be a 1D xtensor-like type");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_weight>,
                  "Template parameter 2 must be a 1D xtensor-like type");

    // Basic checks
    if (values.shape()[0] != weights.shape()[0])
        throw std::invalid_argument("Size mismatch between values and weights.");
    if (values.size() == 0)
        throw std::invalid_argument("Input tensors cannot be empty.");

    // 1) Sort by values using argsort
    auto idx         = xt::argsort(values);
    auto sorted_vals = xt::index_view(values, idx);
    auto sorted_wts  = xt::index_view(weights, idx);

    // 2) Cumulative sum of weights
    auto cumsum_wts = xt::nancumsum(sorted_wts);

    // 3) Total weight
    value_type total_weight = cumsum_wts(cumsum_wts.size() - 1);
    if (total_weight <= 0)
    {
        throw std::invalid_argument("Total weight must be positive.");
    }

    // 4) Find the point where cumulative weight >= 50% of total
    value_type half = total_weight / value_type(2);
    auto       it   = std::lower_bound(cumsum_wts.begin(), cumsum_wts.end(), half);
    if (it != cumsum_wts.end())
    {
        std::size_t idx = std::distance(cumsum_wts.begin(), it);

        // If the cumulative weight is exactly 50% of total, return the weighted mean of the two
        // closest values
        if (cumsum_wts.unchecked(idx) == half)
        {
            value_type val1 = sorted_vals.unchecked(idx);
            value_type val2 = sorted_vals.unchecked(idx + 1);
            value_type wt1  = sorted_wts.unchecked(idx);
            value_type wt2  = sorted_wts.unchecked(idx + 1);
            return (val1 * wt1 + val2 * wt2) / (wt1 + wt2);
        }

        return sorted_vals(idx);
    }

    // Fallback: if we never returned, pick the final value
    return sorted_vals(sorted_vals.shape()[0] - 1);
}

template<tools::helper::c_xtensor_1d t_xtensor_val, tools::helper::c_xtensor_1d t_xtensor_weight>
std::pair<typename t_xtensor_val::value_type, typename t_xtensor_val::value_type> weighted_median(
    const t_xtensor_val&    values_x,
    const t_xtensor_val&    values_y,
    const t_xtensor_weight& weights)
{
    return { weighted_median(values_x, weights), weighted_median(values_y, weights) };
}

template<tools::helper::c_xtensor_1d t_xtensor_val, tools::helper::c_xtensor_1d t_xtensor_weight>
std::tuple<typename t_xtensor_val::value_type,
           typename t_xtensor_val::value_type,
           typename t_xtensor_val::value_type>
weighted_median(const t_xtensor_val&    values_x,
                const t_xtensor_val&    values_y,
                const t_xtensor_val&    values_z,
                const t_xtensor_weight& weights)
{
    return { weighted_median(values_x, weights),
             weighted_median(values_y, weights),
             weighted_median(values_z, weights) };
}

} // namespace functions
} // namespace imageprocessing
} // namespace algorithms
} // namespace themachinethatgoesping