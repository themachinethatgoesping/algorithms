// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
#pragma once

/* generated doc strings */
#include ".docstrings/segment_in_weighted_quantiles.doc.hpp"

#include <stdexcept>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/vectorinterpolators.hpp>

#include <xtensor/views/xindex_view.hpp>
#include <xtensor/misc/xsort.hpp>
#include <xtensor/containers/xtensor.hpp>
#include <xtensor/views/xview.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pointprocessing {
namespace functions {

template<tools::helper::c_xtensor_1d t_xtensor_val, tools::helper::c_xtensor_1d t_xtensor_weight>
/**
 * @brief Segments input values into weighted quantiles.
 *
 * This function segments the input values into a specified number of quantiles,
 * taking into account the provided weights. The segments are created based on
 * the cumulative sum of the weights, ensuring that each segment has approximately
 * equal total weight.
 *
 * @tparam t_xtensor_val Type of the input values tensor (must be a 1D xtensor-like type).
 * @tparam t_xtensor_weight Type of the input weights tensor (must be a 1D xtensor-like type).
 *
 * @param values Input tensor containing the values to be segmented.
 * @param weights Input tensor containing the weights corresponding to the values.
 * @param n_quantiles Number of quantiles to segment the values into.
 * @param return_empty_segments If true, empty segments will be included in the result. (e.g. if
 * n_quantiles is > len(values))
 *
 * @return A vector of segments, where each segment is a tensor containing the indices
 *         of the original values that belong to that segment.
 *
 * @throws std::invalid_argument if the number of quantiles is less than 1.
 * @throws std::invalid_argument if the size of values and weights do not match.
 * @throws std::invalid_argument if the input tensors are empty.
 * @throws std::invalid_argument if the total weight is not positive.
 *
 * @note The function does not handle the case where the weights are unbalanced with care.
 */
auto segment_in_weighted_quantiles(const t_xtensor_val&    values,
                                   const t_xtensor_weight& weights,
                                   const int64_t           n_quantiles,
                                   const bool              return_empty_segments = false)
{
    using value_type = typename t_xtensor_val::value_type;

    static_assert(tools::helper::c_xtensor_1d<t_xtensor_val>,
                  "Template parameter 1 must be a 1D xtensor-like type");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_weight>,
                  "Template parameter 2 must be a 1D xtensor-like type");

    if (n_quantiles < 1)
        throw std::invalid_argument("Number of quantiles must be positive.");

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

    // 4) compute sorted positions for weighted quantiles
    std::vector<int64_t> quantiles(n_quantiles);
    value_type           target_weight = total_weight / value_type(n_quantiles);

    for (int64_t current_segment = 1; current_segment <= n_quantiles; ++current_segment)
    {
        auto it =
            std::lower_bound(cumsum_wts.begin(), cumsum_wts.end(), current_segment * target_weight);
        if (it != cumsum_wts.end())
        {
            quantiles[current_segment - 1] = std::distance(cumsum_wts.begin(), it);
        }
        else
        {
            quantiles[current_segment - 1] = cumsum_wts.size() - 1;
        }
    }

    // 5) Create segments based on quantiles, segments contain unsorted (original) indices
    using idx_type = std::decay_t<decltype(idx)>;
    std::vector<idx_type> segments;
    segments.reserve(n_quantiles);

    segments.push_back(xt::view(idx, xt::range(0, quantiles[0] + 1)));

    for (int64_t i = 1; i < n_quantiles; ++i)
    {
        if (quantiles[i] <= quantiles[i - 1])
        {
            if (return_empty_segments)
                segments.push_back(idx_type({ 0 }));
        }
        else
        {
            segments.push_back(xt::view(idx, xt::range(quantiles[i - 1] + 1, quantiles[i] + 1)));
        }
    }

    if (quantiles.back() + 1 >= int64_t(idx.size()))
    {
        if (return_empty_segments)
            segments.push_back(idx_type({ 0 }));
    }
    else
        segments.push_back(xt::view(idx, xt::range(quantiles.back() + 1, idx.size())));

    return segments;
}

} // namespace functions
} // namespace pointprocessing
} // namespace algorithms
} // namespace themachinethatgoesping