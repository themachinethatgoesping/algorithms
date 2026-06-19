// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "bottomdetector.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include <xtensor/core/xmath.hpp>
#include <xtensor/generators/xbuilder.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {

namespace {
constexpr float NaN     = std::numeric_limits<float>::quiet_NaN();
constexpr float NEG_INF = -std::numeric_limits<float>::infinity();
} // namespace

// =============================================================================
// Constructor
// =============================================================================

BottomDetector::BottomDetector(float thr_bottom,
                               float thr_echo,
                               float thr_backstep,
                               float thr_cum_percent,
                               float r_min,
                               float r_max,
                               float shift_bot_m,
                               bool  denoised,
                               float incidence_angle_deg,
                               int   n_ping_smoothing,
                               float mask_fill_fraction,
                               bool  remove_outliers,
                               int   outlier_window,
                               float outlier_threshold,
                               bool  interpolate_gaps,
                               int   max_interpolation_gap)
    : thr_bottom(thr_bottom)
    , thr_echo(thr_echo)
    , thr_backstep(thr_backstep)
    , thr_cum_percent(thr_cum_percent)
    , r_min(r_min)
    , r_max(r_max)
    , shift_bot_m(shift_bot_m)
    , denoised(denoised)
    , incidence_angle_deg(incidence_angle_deg)
    , n_ping_smoothing(n_ping_smoothing)
    , mask_fill_fraction(mask_fill_fraction)
    , remove_outliers(remove_outliers)
    , outlier_window(outlier_window)
    , outlier_threshold(outlier_threshold)
    , interpolate_gaps(interpolate_gaps)
    , max_interpolation_gap(max_interpolation_gap)
{
}

// =============================================================================
// Per-ping gating (range window + maximum BS)
// =============================================================================

BottomDetector::Context BottomDetector::analyze_ping(const Ping& ping) const
{
    Context   ctx;
    const int n = static_cast<int>(ping.bs.size());
    if (n == 0)
        return ctx;

    // usable range window: near-field guard (2 pulse lengths) + r_min / r_max
    int s_min = static_cast<int>(std::lround(2.0f * std::max(ping.pulse_nsamples, 0.0f)));
    if (ping.range_resolution > 0.0f && std::isfinite(r_min))
        s_min = std::max(
            s_min,
            static_cast<int>(std::ceil((r_min - ping.range_offset) / ping.range_resolution)));
    s_min = std::clamp(s_min, 0, n - 1);

    int s_max = n - 1;
    if (ping.range_resolution > 0.0f && std::isfinite(r_max))
        s_max = std::min(
            s_max,
            static_cast<int>(std::floor((r_max - ping.range_offset) / ping.range_resolution)));
    s_max = std::clamp(s_max, 0, n - 1);

    ctx.s_min = s_min;
    ctx.s_max = s_max;
    if (s_max <= s_min)
        return ctx;

    float max_bs = NEG_INF;
    for (int i = s_min; i <= s_max; ++i)
        if (std::isfinite(ping.bs.unchecked(i)))
            max_bs = std::max(max_bs, ping.bs.unchecked(i));
    if (max_bs == NEG_INF)
        return ctx;

    ctx.max_bs = max_bs;
    ctx.usable = true;
    return ctx;
}

// =============================================================================
// Range-direction candidate mask (threshold + majority filter)
// =============================================================================

xt::xtensor<uint8_t, 1> BottomDetector::range_mask(const Ping& ping, const Context& ctx) const
{
    const int               n   = static_cast<int>(ping.bs.size());
    xt::xtensor<uint8_t, 1> raw = xt::zeros<uint8_t>({ std::max(n, 0) });
    if (!ctx.usable || ctx.max_bs < thr_bottom)
        return raw;

    const float level = ctx.max_bs + thr_echo;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (ping.bs.unchecked(i) > level) // NaN compares false -> excluded
            raw.unchecked(i) = 1;

    const int win = std::max(5, static_cast<int>(std::lround(2.0f * ping.pulse_nsamples + 1.0f)));
    return functions::majority_filter(raw, win, mask_fill_fraction);
}

// =============================================================================
// Cross-ping (ping-direction) majority smoothing of the candidate masks
// =============================================================================

xt::xtensor<uint8_t, 1> BottomDetector::cross_ping_mask(
    const std::vector<xt::xtensor<uint8_t, 1>>& masks,
    int                                         center,
    int                                         half) const
{
    if (std::max(n_ping_smoothing, 1) <= 1)
        return masks[center]; // no cross-ping smoothing

    const int               n   = static_cast<int>(masks[center].size());
    xt::xtensor<uint8_t, 1> out = xt::zeros<uint8_t>({ std::max(n, 0) });

    const int lo  = std::max(center - half, 0);
    const int hi  = std::min(center + half, static_cast<int>(masks.size()) - 1);
    const int len = hi - lo + 1;
    for (int s = 0; s < n; ++s)
    {
        int sum = 0;
        for (int q = lo; q <= hi; ++q)
        {
            const auto& mq = masks[q];
            if (s < static_cast<int>(mq.size()))
                sum += mq.unchecked(s);
        }
        if (len > 0 && static_cast<float>(sum) / static_cast<float>(len) >= mask_fill_fraction)
            out.unchecked(s) = 1;
    }
    return out;
}

// =============================================================================
// Per-ping detection from a (smoothed) candidate mask
// =============================================================================

std::pair<float, float> BottomDetector::detect(const Ping&                    ping,
                                               const Context&                 ctx,
                                               const xt::xtensor<uint8_t, 1>& mask) const
{
    const int n = static_cast<int>(ping.bs.size());
    if (!ctx.usable || ctx.max_bs < thr_bottom || n == 0)
        return { NaN, NaN };

    const auto& bs = ping.bs;

    // topmost candidate sample
    int bot_top = -1;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (mask.unchecked(i))
        {
            bot_top = i;
            break;
        }
    if (bot_top < 0)
        return { NaN, NaN };

    // cumulative-energy pick: energy = (masked linear BS)^2 = 10^(BS/5).
    // The pow is vectorized; non-masked samples are then zeroed.
    xt::xtensor<float, 1> energy = xt::where(xt::isfinite(bs), xt::pow(10.0f, bs * 0.2f), 0.0f);
    float                 total  = 0.0f;
    for (int i = 0; i < n; ++i)
    {
        if (!mask.unchecked(i))
            energy.unchecked(i) = 0.0f;
        total += energy.unchecked(i);
    }

    int         bot     = bot_top;
    const float thr_cum = std::clamp(thr_cum_percent / 100.0f, 0.0f, 1.0f);
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

    // back-stepping up the leading edge by one pulse length
    const int backstep = std::max(4, static_cast<int>(std::lround(ping.pulse_nsamples)));
    if (bot > 2 * backstep)
    {
        const float accept_floor = thr_bottom + thr_echo + thr_backstep;
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
            if (!(bs_val >= bs_bot + thr_backstep && bs_val > accept_floor) || idx_max >= bot)
                break;
            bot = idx_max;
        }
        bot = std::max(bot - backstep, 0);
    }
    bot = std::max(bot, ctx.s_min);

    // echo-length validation filter (moving-average amplitude in dB)
    const float bot_range = ping.range_offset + ping.range_resolution * static_cast<float>(bot);
    const float r_p       = std::max(ping.pulse_nsamples * ping.range_resolution, 0.0f);
    const float el =
        functions::echo_length(r_p, ping.beamwidth_deg, incidence_angle_deg, bot_range);

    int win = 5;
    if (std::isfinite(el) && ping.range_resolution > 0.0f)
        win = std::max(5, static_cast<int>(std::lround(el / ping.range_resolution)));
    const int half = win / 2;

    // amplitude = 10^(BS/20), counted only where BS is finite (vectorized pow)
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
    if (bs_bottom < thr_bottom)
        return { NaN, NaN }; // echo too weak after filtering

    // shifts: range shift, denoise offset, pulse half-length
    if (ping.range_resolution > 0.0f && shift_bot_m != 0.0f)
        bot -= static_cast<int>(std::ceil(shift_bot_m / ping.range_resolution));
    if (denoised)
        bot -= 1;
    bot = std::max(bot, 0);

    const float bottom_sample = std::floor(static_cast<float>(bot) + ping.pulse_nsamples * 0.5f);
    return { std::clamp(bottom_sample, 0.0f, static_cast<float>(n - 1)), bs_bottom };
}

// =============================================================================
// Step 2: compile the whole sequence (cross-ping smoothing + detection)
// =============================================================================

void BottomDetector::compile()
{
    if (!_dirty)
        return;

    const int         n_pings = static_cast<int>(_pings.size());
    const std::size_t shape   = static_cast<std::size_t>(std::max(n_pings, 0));
    _bottom_sample            = xt::xtensor<float, 1>::from_shape({ shape });
    _bottom_bs                = xt::xtensor<float, 1>::from_shape({ shape });
    if (n_pings == 0)
    {
        _dirty = false;
        return;
    }

    // first pass: per-ping gating + range-direction candidate masks
    std::vector<Context>                 ctxs(n_pings);
    std::vector<xt::xtensor<uint8_t, 1>> masks(n_pings);
    for (int p = 0; p < n_pings; ++p)
    {
        ctxs[p]  = analyze_ping(_pings[p]);
        masks[p] = range_mask(_pings[p], ctxs[p]);
    }

    // second pass: cross-ping smoothing + per-ping detection
    const int half = std::max(n_ping_smoothing, 1) / 2;
    for (int p = 0; p < n_pings; ++p)
    {
        const auto mask         = cross_ping_mask(masks, p, half);
        const auto [sample, bs] = detect(_pings[p], ctxs[p], mask);
        _bottom_sample.unchecked(p) = sample;
        _bottom_bs.unchecked(p)     = bs;
    }

    if (remove_outliers)
        reject_outliers(_bottom_sample, _bottom_bs);
    if (interpolate_gaps)
        interpolate_bottom_gaps(_bottom_sample);

    _dirty = false;
}

void BottomDetector::reset()
{
    _pings.clear();
    _bottom_sample = xt::xtensor<float, 1>();
    _bottom_bs     = xt::xtensor<float, 1>();
    _dirty         = true;
}

// =============================================================================
// Cross-ping post-processing
// =============================================================================

void BottomDetector::reject_outliers(xt::xtensor<float, 1>& bottom_sample,
                                     xt::xtensor<float, 1>& bottom_bs) const
{
    const int n    = static_cast<int>(bottom_sample.size());
    const int half = std::max(1, outlier_window);

    xt::xtensor<uint8_t, 1> drop = xt::zeros<uint8_t>({ std::max(n, 0) });
    for (int i = 0; i < n; ++i)
    {
        if (!std::isfinite(bottom_sample.unchecked(i)))
            continue;
        std::vector<float> window;
        for (int j = std::max(i - half, 0); j <= std::min(i + half, n - 1); ++j)
            if (std::isfinite(bottom_sample.unchecked(j)))
                window.push_back(bottom_sample.unchecked(j));
        if (window.size() < 3)
            continue;

        std::nth_element(window.begin(), window.begin() + window.size() / 2, window.end());
        const float median = window[window.size() / 2];

        std::vector<float> dev;
        dev.reserve(window.size());
        for (float v : window)
            dev.push_back(std::abs(v - median));
        std::nth_element(dev.begin(), dev.begin() + dev.size() / 2, dev.end());
        const float scaled_mad = 1.4826f * dev[dev.size() / 2];

        if (scaled_mad > 0.0f &&
            std::abs(bottom_sample.unchecked(i) - median) > outlier_threshold * scaled_mad)
            drop.unchecked(i) = 1;
    }
    for (int i = 0; i < n; ++i)
        if (drop.unchecked(i))
        {
            bottom_sample.unchecked(i) = NaN;
            bottom_bs.unchecked(i)     = NaN;
        }
}

void BottomDetector::interpolate_bottom_gaps(xt::xtensor<float, 1>& bottom_sample) const
{
    const int n = static_cast<int>(bottom_sample.size());
    int       i = 0;
    while (i < n)
    {
        if (std::isfinite(bottom_sample.unchecked(i)))
        {
            ++i;
            continue;
        }
        const int start = i;
        while (i < n && !std::isfinite(bottom_sample.unchecked(i)))
            ++i;
        const int end  = i;       // first finite (or n)
        const int prev = start - 1;
        if (prev < 0 || end >= n)
            continue; // gap touches an edge
        if (end - start > max_interpolation_gap)
            continue;

        const float v0 = bottom_sample.unchecked(prev);
        const float v1 = bottom_sample.unchecked(end);
        for (int k = start; k < end; ++k)
        {
            const float t              = static_cast<float>(k - prev) / static_cast<float>(end - prev);
            bottom_sample.unchecked(k) = v0 + (v1 - v0) * t;
        }
    }
}

// =============================================================================
// Printer
// =============================================================================

tools::classhelper::ObjectPrinter BottomDetector::__printer__(unsigned int float_precision,
                                                              bool superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer(
        "BottomDetector", float_precision, superscript_exponents);

    printer.register_section("thresholds");
    printer.register_value("thr_bottom", thr_bottom, "dB");
    printer.register_value("thr_echo", thr_echo, "dB");
    printer.register_value("thr_backstep", thr_backstep, "dB");
    printer.register_value("thr_cum_percent", thr_cum_percent, "%");

    printer.register_section("range gating");
    printer.register_value("r_min", r_min, "m");
    printer.register_value("r_max", r_max, "m");
    printer.register_value("shift_bot_m", shift_bot_m, "m");
    printer.register_value("denoised", denoised);

    printer.register_section("geometry / morphology");
    printer.register_value("incidence_angle_deg", incidence_angle_deg, "deg");
    printer.register_value("n_ping_smoothing", n_ping_smoothing);
    printer.register_value("mask_fill_fraction", mask_fill_fraction);

    printer.register_section("post-processing");
    printer.register_value("remove_outliers", remove_outliers);
    printer.register_value("outlier_window", outlier_window);
    printer.register_value("outlier_threshold", outlier_threshold);
    printer.register_value("interpolate_gaps", interpolate_gaps);
    printer.register_value("max_interpolation_gap", max_interpolation_gap);

    return printer;
}

} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
