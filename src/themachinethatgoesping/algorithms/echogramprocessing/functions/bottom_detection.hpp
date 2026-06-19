// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

/**
 * @file bottom_detection.hpp
 * @brief Stateless helper functions for echogram bottom detection.
 *
 * These helpers are used by
 * @ref themachinethatgoesping::algorithms::echogramprocessing::BottomDetector
 * but are kept free (non-member) because they are generic and reusable. The
 * ``BottomDetector`` class (with its tunable parameters and the detection
 * routines) lives in ``echogramprocessing/bottomdetector.hpp``.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/bottom_detection.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <vector>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/generators/xbuilder.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {
namespace functions {

/**
 * @brief Theoretical length (m) of the bottom echo along range.
 *
 * Port of ESP3's ``echo_length.m``.
 *
 * @param r_p pulse range ``c*tau/2`` in m.
 * @param theta_b beam aperture in degrees.
 * @param beta incidence angle at the bottom in degrees (seafloor slope plus
 *        transducer tilt).
 * @param r range of the start of the echo (bottom range) in m.
 * @return echo length in m (``inf`` for grazing incidence).
 */
inline float echo_length(float r_p, float theta_b, float beta, float r)
{
    constexpr float pi  = std::numbers::pi_v<float>;
    constexpr float d2r = pi / 180.0f;

    beta             = std::abs(beta);
    const float half = theta_b * 0.5f;

    if (beta < half)
        return r_p + r * (1.0f / std::cos((beta + half) * d2r) - 1.0f);
    if (beta < (90.0f - half))
        return r_p + r * (std::cos((beta - half) * d2r) / std::cos((beta + half) * d2r) - 1.0f);
    return std::numeric_limits<float>::infinity();
}

/**
 * @brief 1D majority filter on a binary mask (range or ping direction).
 *
 * A sample becomes set when at least @p fraction of a centered window of width
 * @p window is set. This removes isolated detections and fills small holes (the
 * separable equivalent of ESP3's ``filter2(ones(win, k), ...)``). Implemented
 * with a prefix sum, O(n).
 *
 * @tparam t_xtensor_1d_in 1D integer tensor type of the input mask.
 * @param mask Input binary mask (non-zero = set).
 * @param window Window width (clamped to >= 1).
 * @param fraction Required fraction of set samples in the window (0..1).
 * @return ``xt::xtensor<uint8_t, 1>`` of the same length (1 = set).
 */
template<tools::helper::c_xtensor_1d t_xtensor_1d_in>
inline xt::xtensor<uint8_t, 1> majority_filter(const t_xtensor_1d_in& mask,
                                               int                    window,
                                               float                 fraction)
{
    const int               n   = static_cast<int>(mask.size());
    xt::xtensor<uint8_t, 1> out = xt::zeros<uint8_t>({ std::max(n, 0) });
    if (n == 0)
        return out;

    window         = std::max(window, 1);
    const int half = window / 2;

    std::vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i)
        prefix[i + 1] = prefix[i] + (mask.unchecked(i) != 0 ? 1 : 0);

    for (int i = 0; i < n; ++i)
    {
        const int lo    = std::max(i - half, 0);
        const int hi    = std::min(i + half, n - 1);
        const int sum   = prefix[hi + 1] - prefix[lo];
        const int count = hi - lo + 1;
        if (count > 0 && static_cast<float>(sum) / static_cast<float>(count) >= fraction)
            out.unchecked(i) = 1;
    }
    return out;
}

} // namespace functions
} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
