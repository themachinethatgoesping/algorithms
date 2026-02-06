// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

/**
 * @brief Sidelobe artifact correction for water column data.
 *
 * Implementation of the Slant Range Signal Normalization (SRSN) algorithm
 * (Schimel et al., 2020. DOI: 10.3390/rs12091371) for filtering sidelobe
 * artifacts in water-column imagery (WCI).
 *
 * The core principle is normalization across beams at each sample position:
 *   corrected[beam, sample] = wci[beam, sample] - avg_across_beams[sample] + reference_level
 *
 * This removes the range-dependent sidelobe pattern that bleeds equally into
 * all beams from the bottom echo, then re-anchors the data to a meaningful
 * reference level.
 *
 * Reference: Schimel, A.C.G. et al. (2020). "Multibeam Sonar Backscatter Data
 * Processing." Remote Sensing, 12(9), 1371.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/wcisidelobecorrection.doc.hpp"

#include <algorithm>
#include <cmath>
#include <fmt/core.h>
#include <optional>
#include <vector>

#include <xtensor/containers/xadapt.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "_shape_assertations.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// --- compute per-sample statistics across beams ---

/**
 * @brief Compute the mean value across all beams for each sample position, ignoring NaN.
 *
 * For a WCI tensor of shape (n_beams x n_samples), this computes the NaN-aware
 * mean along axis 0 (beams), producing a 1D tensor of shape (n_samples).
 *
 * This is the first step of the SRSN sidelobe correction: computing the
 * average signal level across all beams at each range/sample position.
 *
 * NaN values are excluded from the mean (equivalent to MATLAB mean(...,'omitnan')).
 * If all beams at a sample position are NaN, the result for that sample is NaN.
 *
 * @tparam t_xtensor_2d Type of the 2D input tensor (beams x samples).
 * @tparam t_xtensor_1d Type of the 1D output tensor (samples).
 * @param wci The input 2D WCI tensor (beams x samples).
 * @param mp_cores The number of cores to use for parallel processing (default is 1).
 * @return A 1D tensor of shape (n_samples) with the NaN-aware mean across beams.
 */
template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_1d compute_nanmean_across_beams(const t_xtensor_2d& wci, int mp_cores = 1)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter t_xtensor_2d must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter t_xtensor_1d must be a 1D tensor");

    using t_float = typename tools::helper::xtensor_datatype<t_xtensor_2d>::type;

    size_t n_beams   = wci.shape(0);
    size_t n_samples = wci.shape(1);

    t_xtensor_1d result = xt::empty<t_float>({ n_samples });

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int si = 0; si < n_samples; ++si)
    {
        t_float sum   = 0;
        size_t  count = 0;
        for (size_t bi = 0; bi < n_beams; ++bi)
        {
            t_float val = wci.unchecked(bi, si);
            if (!std::isnan(val))
            {
                sum += val;
                ++count;
            }
        }
        result.unchecked(si) = (count > 0) ? (sum / static_cast<t_float>(count))
                                           : std::numeric_limits<t_float>::quiet_NaN();
    }

    return result;
}

/**
 * @brief Compute the median value across all beams for each sample position, ignoring NaN.
 *
 * For a WCI tensor of shape (n_beams x n_samples), this computes the NaN-aware
 * median along axis 0 (beams), producing a 1D tensor of shape (n_samples).
 *
 * The median is more robust to outliers than the mean, which can be useful
 * when strong targets (e.g., fish schools) are present in a few beams.
 *
 * NaN values are excluded before computing the median (equivalent to MATLAB
 * median(...,'omitnan')). If all beams at a sample position are NaN, the result
 * for that sample is NaN.
 *
 * Uses std::nth_element for O(n) average complexity per sample. The
 * computation across samples is parallelized with OpenMP.
 *
 * @tparam t_xtensor_2d Type of the 2D input tensor (beams x samples).
 * @tparam t_xtensor_1d Type of the 1D output tensor (samples).
 * @param wci The input 2D WCI tensor (beams x samples).
 * @param mp_cores The number of cores to use for parallel processing (default is 1).
 * @return A 1D tensor of shape (n_samples) with the NaN-aware median across beams.
 */
template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_1d compute_nanmedian_across_beams(const t_xtensor_2d& wci, int mp_cores = 1)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter t_xtensor_2d must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter t_xtensor_1d must be a 1D tensor");

    using t_float = typename tools::helper::xtensor_datatype<t_xtensor_2d>::type;

    size_t n_beams   = wci.shape(0);
    size_t n_samples = wci.shape(1);

    t_xtensor_1d result = xt::empty<t_float>({ n_samples });

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int si = 0; si < n_samples; ++si)
    {
        // Extract column (all beams for this sample), skipping NaN
        std::vector<t_float> col;
        col.reserve(n_beams);
        for (size_t bi = 0; bi < n_beams; ++bi)
        {
            t_float val = wci.unchecked(bi, si);
            if (!std::isnan(val))
                col.push_back(val);
        }

        if (col.empty())
        {
            result.unchecked(si) = std::numeric_limits<t_float>::quiet_NaN();
            continue;
        }

        // Use nth_element for O(n) median finding
        size_t n   = col.size();
        size_t mid = n / 2;
        std::nth_element(col.begin(), col.begin() + mid, col.end());

        if (n % 2 == 0)
        {
            // For even count, median = average of the two middle elements
            auto max_lower       = *std::max_element(col.begin(), col.begin() + mid);
            result.unchecked(si) = (max_lower + col[mid]) / t_float(2);
        }
        else
        {
            result.unchecked(si) = col[mid];
        }
    }

    return result;
}

// --- compute reference level from data ---

/**
 * @brief Compute a reference level as the NaN-aware mean of all elements in a 2D WCI region.
 *
 * This is used in the SRSN algorithm to compute a reference level from the
 * "clean" water column (e.g., data above the minimum slant range). The caller
 * should extract the appropriate sub-region before calling this function.
 *
 * NaN values are excluded from the computation.
 *
 * @tparam t_xtensor_2d Type of the 2D tensor.
 * @param wci_region A 2D tensor representing the reference region (e.g., clean WC above bottom).
 * @return The NaN-aware mean of all elements as a scalar.
 */
template<tools::helper::c_xtensor t_xtensor_2d>
inline auto compute_reference_nanmean(const t_xtensor_2d& wci_region)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");

    using t_float = typename tools::helper::xtensor_datatype<t_xtensor_2d>::type;

    t_float sum   = 0;
    size_t  count = 0;
    for (size_t bi = 0; bi < wci_region.shape(0); ++bi)
    {
        for (size_t si = 0; si < wci_region.shape(1); ++si)
        {
            t_float val = wci_region.unchecked(bi, si);
            if (!std::isnan(val))
            {
                sum += val;
                ++count;
            }
        }
    }

    if (count == 0)
        return std::numeric_limits<t_float>::quiet_NaN();

    return sum / static_cast<t_float>(count);
}

/**
 * @brief Compute a reference level as a NaN-aware percentile of all elements in a 2D WCI region.
 *
 * This is used in the SRSN algorithm to compute a reference level from the
 * "clean" water column. Common choices are the 25th percentile (default in
 * Schimel et al., 2020), 5th, 10th percentile, or 50th percentile (median).
 *
 * NaN values are excluded before computing the percentile.
 *
 * @tparam t_xtensor_2d Type of the 2D tensor.
 * @param wci_region A 2D tensor representing the reference region (e.g., clean WC above bottom).
 * @param percentile The percentile to compute, in range [0, 100]. E.g., 25 for 25th percentile.
 * @return The NaN-aware percentile of all elements as a scalar.
 */
template<tools::helper::c_xtensor t_xtensor_2d>
inline auto compute_reference_nanpercentile(
    const t_xtensor_2d& wci_region,
    double              percentile)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");

    using t_float = typename tools::helper::xtensor_datatype<t_xtensor_2d>::type;

    if (percentile < 0.0 || percentile > 100.0)
        throw std::invalid_argument(
            fmt::format("ERROR[{}]: percentile must be in [0, 100], got {}", __func__, percentile));

    // Collect all non-NaN values
    std::vector<t_float> values;
    values.reserve(wci_region.shape(0) * wci_region.shape(1));
    for (size_t bi = 0; bi < wci_region.shape(0); ++bi)
    {
        for (size_t si = 0; si < wci_region.shape(1); ++si)
        {
            t_float val = wci_region.unchecked(bi, si);
            if (!std::isnan(val))
                values.push_back(val);
        }
    }

    if (values.empty())
        return std::numeric_limits<t_float>::quiet_NaN();

    size_t n = values.size();

    // Edge cases
    if (percentile == 0.0)
        return *std::min_element(values.begin(), values.end());
    if (percentile == 100.0)
        return *std::max_element(values.begin(), values.end());

    // Compute the fractional index (linear interpolation, matching numpy/MATLAB default)
    double frac_index = (percentile / 100.0) * static_cast<double>(n - 1);
    size_t lower_idx  = static_cast<size_t>(std::floor(frac_index));
    size_t upper_idx  = static_cast<size_t>(std::ceil(frac_index));
    double frac       = frac_index - static_cast<double>(lower_idx);

    // Partial sort to find the two elements we need
    if (lower_idx == upper_idx)
    {
        std::nth_element(values.begin(), values.begin() + lower_idx, values.end());
        return values[lower_idx];
    }

    std::nth_element(values.begin(), values.begin() + upper_idx, values.end());
    t_float upper_val = values[upper_idx];
    // After nth_element for upper_idx, all elements before upper_idx are <= upper_val
    // The lower_idx element is the max of [begin, begin+upper_idx)
    t_float lower_val = *std::max_element(values.begin(), values.begin() + upper_idx);

    return static_cast<t_float>(lower_val + frac * (upper_val - lower_val));
}

// --- apply sidelobe correction ---

/**
 * @brief Apply SRSN sidelobe correction to a WCI tensor.
 *
 * Applies the Slant Range Signal Normalization by subtracting the per-sample
 * average across beams and adding a reference level:
 *
 *   result[beam, sample] = wci[beam, sample] - per_sample_average[sample] + reference_level
 *
 * The per_sample_average should be precomputed using compute_nanmean_across_beams
 * or compute_nanmedian_across_beams. The reference_level is a scalar that
 * re-anchors the corrected data to a meaningful dB range (e.g., computed via
 * compute_reference_nanmean or compute_reference_nanpercentile).
 *
 * Reference: Schimel, A.C.G. et al. (2020). "Multibeam Sonar Backscatter Data
 * Processing." Remote Sensing, 12(9), 1371.
 *
 * @tparam t_xtensor_2d Type of the 2D tensor (beams x samples).
 * @tparam t_xtensor_1d Type of the 1D tensor (samples).
 * @param wci The input 2D WCI tensor (beams x samples).
 * @param per_sample_average A 1D tensor (n_samples) with the average across beams.
 * @param reference_level A scalar reference level (dB) to re-anchor the data.
 * @param mp_cores The number of cores to use for parallel processing (default is 1).
 * @return A 2D tensor with the sidelobe correction applied.
 */
template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_wci_sidelobe_correction(
    const t_xtensor_2d&                                          wci,
    const t_xtensor_1d&                                          per_sample_average,
    typename tools::helper::xtensor_datatype<t_xtensor_2d>::type reference_level,
    int                                                          mp_cores = 1)
{
    assert_wci_axis_shape<1>(wci, per_sample_average, "per_sample_average");

    // Precompute the per-sample correction vector: reference_level - average
    t_xtensor_1d correction = reference_level - per_sample_average;

    t_xtensor_2d result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < wci.shape(0); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + correction;

    return result;
}

/**
 * @brief Inplace apply SRSN sidelobe correction to a WCI tensor.
 *
 * Applies the Slant Range Signal Normalization in-place:
 *
 *   wci[beam, sample] += reference_level - per_sample_average[sample]
 *
 * Optionally limits correction to a beam index range [min_beam_index, max_beam_index].
 *
 * @tparam t_xtensor_2d Type of the 2D tensor (beams x samples).
 * @tparam t_xtensor_1d Type of the 1D tensor (samples).
 * @param wci The 2D WCI tensor to correct in-place (beams x samples).
 * @param per_sample_average A 1D tensor (n_samples) with the average across beams.
 * @param reference_level A scalar reference level (dB) to re-anchor the data.
 * @param min_beam_index Optional minimum beam index to start correction from.
 * @param max_beam_index Optional maximum beam index to end correction at.
 * @param mp_cores The number of cores to use for parallel processing (default is 1).
 */
template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void inplace_wci_sidelobe_correction(
    [[maybe_unused]] t_xtensor_2d&                               wci,
    const t_xtensor_1d&                                          per_sample_average,
    typename tools::helper::xtensor_datatype<t_xtensor_2d>::type reference_level,
    std::optional<size_t>                                        min_beam_index = std::nullopt,
    std::optional<size_t>                                        max_beam_index = std::nullopt,
    int                                                          mp_cores       = 1)
{
    assert_wci_axis_shape<1>(wci, per_sample_average, "per_sample_average");

    // Precompute the per-sample correction vector: reference_level - average
    t_xtensor_1d correction = reference_level - per_sample_average;

    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = min_beam_index.value_or(0); bi <= max_beam_nr; ++bi)
        xt::row(wci, bi) += correction;
}

}
}
}
}
