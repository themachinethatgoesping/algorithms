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
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numbers>
#include <utility>
#include <vector>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/generators/xbuilder.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {
namespace functions {

struct PingRecord
{
    xt::xtensor<float, 1> bs;
    float                 range_offset     = 0.0f;
    float                 range_resolution = 0.0f;
    float                 pulse_nsamples   = 0.0f;
    float                 beamwidth_deg    = 0.0f;
};

struct BottomDetectorConfig
{
    float thr_bottom           = -35.0f;
    float thr_echo             = -35.0f;
    float thr_backstep         = -1.0f;
    float thr_cum_percent      = 1.0f;
    float r_min                = 0.0f;
    float r_max                = std::numeric_limits<float>::infinity();
    float shift_bot_m          = 0.0f;
    bool  denoised             = true;
    float incidence_angle_deg  = 10.0f;
    int   n_ping_smoothing     = 5;
    float mask_fill_fraction   = 0.7f;
    bool  remove_outliers      = false;
    int   outlier_window       = 7;
    float outlier_threshold    = 3.0f;
    bool  interpolate_gaps     = false;
    int   max_interpolation_gap = 10;
    int   mp_cores             = 1;
};

struct PingContext
{
    int   s_min  = 0;
    int   s_max  = -1;
    float max_bs = std::numeric_limits<float>::quiet_NaN();
    bool  usable = false;
};

template<tools::helper::c_xtensor_1d t_xtensor_1d_in>
inline xt::xtensor<uint8_t, 1> majority_filter(const t_xtensor_1d_in& mask,
                                               int                    window,
                                               float                  fraction);

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

template<typename t_buffer>
inline PingContext analyze_ping(const t_buffer&             pings,
                                std::size_t                ping_index,
                                const BottomDetectorConfig& config)
{
    const auto& ping = pings[ping_index];
    PingContext  ctx;
    const int    n = static_cast<int>(ping.bs.size());
    if (n == 0)
        return ctx;

    int s_min = static_cast<int>(std::lround(2.0f * std::max(ping.pulse_nsamples, 0.0f)));
    if (ping.range_resolution > 0.0f && std::isfinite(config.r_min))
        s_min = std::max(s_min,
                         static_cast<int>(std::ceil((config.r_min - ping.range_offset) /
                                                    ping.range_resolution)));
    s_min = std::clamp(s_min, 0, n - 1);

    int s_max = n - 1;
    if (ping.range_resolution > 0.0f && std::isfinite(config.r_max))
        s_max = std::min(s_max,
                         static_cast<int>(std::floor((config.r_max - ping.range_offset) /
                                                     ping.range_resolution)));
    s_max = std::clamp(s_max, 0, n - 1);

    ctx.s_min = s_min;
    ctx.s_max = s_max;
    if (s_max <= s_min)
        return ctx;

    float max_bs = -std::numeric_limits<float>::infinity();
    for (int i = s_min; i <= s_max; ++i)
        if (std::isfinite(ping.bs.unchecked(i)))
            max_bs = std::max(max_bs, ping.bs.unchecked(i));
    if (!std::isfinite(max_bs))
        return ctx;

    ctx.max_bs = max_bs;
    ctx.usable = true;
    return ctx;
}

template<typename t_buffer>
inline xt::xtensor<uint8_t, 1> range_mask(const t_buffer&              pings,
                                          std::size_t                 ping_index,
                                          const PingContext&          ctx,
                                          const BottomDetectorConfig& config)
{
    const auto& ping = pings[ping_index];
    const int   n    = static_cast<int>(ping.bs.size());
    xt::xtensor<uint8_t, 1> raw = xt::zeros<uint8_t>({ std::max(n, 0) });
    if (!ctx.usable || ctx.max_bs < config.thr_bottom)
        return raw;

    const float level = ctx.max_bs + config.thr_echo;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (ping.bs.unchecked(i) > level)
            raw.unchecked(i) = 1;

    const int win = std::max(5, static_cast<int>(std::lround(2.0f * ping.pulse_nsamples + 1.0f)));
    return majority_filter(raw, win, config.mask_fill_fraction);
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

template<typename t_mask_buffer>
inline xt::xtensor<uint8_t, 1> cross_ping_mask(const t_mask_buffer& masks,
                                               std::size_t          center,
                                               int                  half,
                                               float                fill_fraction)
{
    if (masks.empty())
        return {};

    const int n = static_cast<int>(masks[center].size());
    xt::xtensor<uint8_t, 1> out = xt::zeros<uint8_t>({ std::max(n, 0) });

    const int lo  = std::max(static_cast<int>(center) - half, 0);
    const int hi  = std::min(static_cast<int>(center) + half, static_cast<int>(masks.size()) - 1);
    const int len = hi - lo + 1;
    for (int s = 0; s < n; ++s)
    {
        int sum = 0;
        for (int q = lo; q <= hi; ++q)
        {
            const auto& mq = masks[static_cast<std::size_t>(q)];
            if (s < static_cast<int>(mq.size()))
                sum += mq.unchecked(s);
        }
        if (len > 0 && static_cast<float>(sum) / static_cast<float>(len) >= fill_fraction)
            out.unchecked(s) = 1;
    }
    return out;
}

template<typename t_buffer>
inline std::pair<float, float> detect(const t_buffer&                 pings,
                                      std::size_t                    ping_index,
                                      const PingContext&             ctx,
                                      const xt::xtensor<uint8_t, 1>& mask,
                                      const BottomDetectorConfig&    config)
{
    constexpr float NaN     = std::numeric_limits<float>::quiet_NaN();
    constexpr float NEG_INF = -std::numeric_limits<float>::infinity();

    const auto& ping = pings[ping_index];
    const int   n    = static_cast<int>(ping.bs.size());
    if (!ctx.usable || ctx.max_bs < config.thr_bottom || n == 0)
        return { NaN, NaN };

    const auto& bs = ping.bs;

    int bot_top = -1;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (mask.unchecked(i))
        {
            bot_top = i;
            break;
        }
    if (bot_top < 0)
        return { NaN, NaN };

    xt::xtensor<float, 1> energy = xt::where(xt::isfinite(bs), xt::pow(10.0f, bs * 0.2f), 0.0f);
    float                 total  = 0.0f;
    for (int i = 0; i < n; ++i)
    {
        if (!mask.unchecked(i))
            energy.unchecked(i) = 0.0f;
        total += energy.unchecked(i);
    }

    int         bot     = bot_top;
    const float thr_cum = std::clamp(config.thr_cum_percent / 100.0f, 0.0f, 1.0f);
    if (total > 0.0f)
    {
        float cum = 0.0f;
        for (int i = 0; i < n; ++i)
        {
            cum += energy.unchecked(i);
            if (cum / total >= thr_cum)
            {
                bot = std::max(bot_top, i);
                break;
            }
        }
    }

    const int backstep = std::max(4, static_cast<int>(std::lround(ping.pulse_nsamples)));
    if (bot > 2 * backstep)
    {
        const float accept_floor = config.thr_bottom + config.thr_echo + config.thr_backstep;
        while (bot > backstep)
        {
            int   idx_max = -1;
            float bs_val  = NEG_INF;
            for (int j = bot - backstep; j <= bot - 1; ++j)
                if (std::isfinite(bs.unchecked(j)) && bs.unchecked(j) > bs_val)
                {
                    bs_val  = bs.unchecked(j);
                    idx_max = j;
                }
            if (idx_max < 0)
                break;

            const float bs_bot = std::isfinite(bs.unchecked(bot)) ? bs.unchecked(bot) : NEG_INF;
            if (!(bs_val >= bs_bot + config.thr_backstep && bs_val > accept_floor) || idx_max >= bot)
                break;
            bot = idx_max;
        }
        bot = std::max(bot - backstep, 0);
    }
    bot = std::max(bot, ctx.s_min);

    const float bot_range = ping.range_offset + ping.range_resolution * static_cast<float>(bot);
    const float r_p = std::max(ping.pulse_nsamples * ping.range_resolution, 0.0f);
    const float el = echo_length(r_p, ping.beamwidth_deg, config.incidence_angle_deg, bot_range);

    int win = 5;
    if (std::isfinite(el) && ping.range_resolution > 0.0f)
        win = std::max(5, static_cast<int>(std::lround(el / ping.range_resolution)));
    const int half = win / 2;

    xt::xtensor<float, 1> amp = xt::where(xt::isfinite(bs), xt::pow(10.0f, bs * 0.05f), 0.0f);
    std::vector<float>    amp_prefix(n + 1, 0.0f);
    std::vector<float>    cnt_prefix(n + 1, 0.0f);
    for (int i = 0; i < n; ++i)
    {
        amp_prefix[i + 1] = amp_prefix[i] + amp.unchecked(i);
        cnt_prefix[i + 1] = cnt_prefix[i] + (std::isfinite(bs.unchecked(i)) ? 1.0f : 0.0f);
    }

    float bs_bottom = NEG_INF;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
    {
        if (!mask.unchecked(i))
            continue;
        const int   lo      = std::max(i - half, 0);
        const int   hi      = std::min(i + half, n - 1);
        const float amp_sum = amp_prefix[hi + 1] - amp_prefix[lo];
        const float cnt     = cnt_prefix[hi + 1] - cnt_prefix[lo];
        if (cnt > 0.0f)
            bs_bottom = std::max(bs_bottom, 20.0f * std::log10(amp_sum / cnt));
    }
    if (bs_bottom < config.thr_bottom)
        return { NaN, NaN };

    if (ping.range_resolution > 0.0f && config.shift_bot_m != 0.0f)
        bot -= static_cast<int>(std::ceil(config.shift_bot_m / ping.range_resolution));
    if (config.denoised)
        bot -= 1;
    bot = std::max(bot, 0);

    const float bottom_sample = std::floor(static_cast<float>(bot) + ping.pulse_nsamples * 0.5f);
    return { std::clamp(bottom_sample, 0.0f, static_cast<float>(n - 1)), bs_bottom };
}

} // namespace functions
} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
