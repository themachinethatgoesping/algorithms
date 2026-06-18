// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "bottom_detection.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>
#include <vector>

#include <fmt/format.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/generators/xbuilder.hpp>
#include <xtensor/views/xview.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {
namespace functions {

namespace {

constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
constexpr double NEG_INF = -std::numeric_limits<double>::infinity();

/// Theoretical length (m) of the bottom echo along range (ESP3 echo_length.m).
double echo_length(double r_p, double theta_b, double beta, double r)
{
    using std::numbers::pi;
    beta              = std::abs(beta);
    const double half = theta_b * 0.5;
    const double d2r  = pi / 180.0;

    if (beta < half)
        return r_p + r * (1.0 / std::cos((beta + half) * d2r) - 1.0);
    if (beta < (90.0 - half))
        return r_p + r * (std::cos((beta - half) * d2r) / std::cos((beta + half) * d2r) - 1.0);
    return std::numeric_limits<double>::infinity();
}

/// 1D majority filter: a sample is set when >= @p fraction of a centered window
/// of width @p window is set. Implemented with a prefix sum (O(n)).
xt::xtensor<uint8_t, 1> majority_filter(const xt::xtensor<uint8_t, 1>& mask,
                                        int                            window,
                                        double                         fraction)
{
    const int               n   = static_cast<int>(mask.size());
    xt::xtensor<uint8_t, 1> out = xt::zeros<uint8_t>({ std::max(n, 0) });
    if (n == 0)
        return out;

    window         = std::max(window, 1);
    const int half = window / 2;

    std::vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i)
        prefix[i + 1] = prefix[i] + mask(i);

    for (int i = 0; i < n; ++i)
    {
        const int lo    = std::max(i - half, 0);
        const int hi    = std::min(i + half, n - 1);
        const int sum   = prefix[hi + 1] - prefix[lo];
        const int count = hi - lo + 1;
        if (count > 0 && static_cast<double>(sum) / static_cast<double>(count) >= fraction)
            out(i) = 1;
    }
    return out;
}

} // namespace

// =============================================================================
// Per-ping context
// =============================================================================

BottomDetector::PingContext BottomDetector::compute_ping_context(
    const xt::xtensor<double, 1>& sv_db,
    double                        range_offset,
    double                        range_resolution,
    double                        pulse_nsamples) const
{
    PingContext ctx;
    const int   n = static_cast<int>(sv_db.size());
    if (n == 0)
        return ctx;

    // range and surface backscatter, fully vectorized
    ctx.range            = range_offset + range_resolution * xt::arange<double>(n);
    auto safe_range      = xt::where(ctx.range > 0.0, ctx.range, 1.0);
    ctx.bs               = sv_db + 10.0 * xt::log10(safe_range);
    ctx.bs               = xt::where(xt::isfinite(sv_db), ctx.bs, NaN);
    ctx.bs               = xt::where(ctx.range > 0.0, ctx.bs, NaN);

    // usable range window: near-field guard (2 pulse lengths) + r_min/r_max
    int s_min = static_cast<int>(std::lround(2.0 * std::max(pulse_nsamples, 0.0)));
    if (range_resolution > 0.0 && std::isfinite(r_min))
        s_min = std::max(
            s_min, static_cast<int>(std::ceil((r_min - range_offset) / range_resolution)));
    s_min = std::clamp(s_min, 0, n - 1);

    int s_max = n - 1;
    if (range_resolution > 0.0 && std::isfinite(r_max))
        s_max = std::min(
            s_max, static_cast<int>(std::floor((r_max - range_offset) / range_resolution)));
    s_max = std::clamp(s_max, 0, n - 1);

    ctx.s_min = s_min;
    ctx.s_max = s_max;
    if (s_max <= s_min)
        return ctx;

    double max_bs = NEG_INF;
    for (int i = s_min; i <= s_max; ++i)
        if (std::isfinite(ctx.bs(i)))
            max_bs = std::max(max_bs, ctx.bs(i));

    if (max_bs == NEG_INF)
        return ctx;

    ctx.max_bs = max_bs;
    ctx.usable = true;
    return ctx;
}

// =============================================================================
// Candidate mask
// =============================================================================

xt::xtensor<uint8_t, 1> BottomDetector::candidate_raw(const PingContext& ctx) const
{
    const int               n   = static_cast<int>(ctx.bs.size());
    xt::xtensor<uint8_t, 1> raw = xt::zeros<uint8_t>({ std::max(n, 0) });
    if (!ctx.usable || ctx.max_bs < thr_bottom)
        return raw;

    const double level = ctx.max_bs + thr_echo;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (ctx.bs(i) > level) // NaN compares false -> excluded
            raw(i) = 1;
    return raw;
}

// =============================================================================
// Detection given a (smoothed) candidate mask
// =============================================================================

BottomDetectionResult BottomDetector::detect_from_mask(const PingContext&             ctx,
                                                       const xt::xtensor<uint8_t, 1>& mask,
                                                       double pulse_nsamples,
                                                       double range_resolution,
                                                       double beamwidth_deg) const
{
    BottomDetectionResult result;
    const int             n = static_cast<int>(ctx.bs.size());
    if (!ctx.usable || ctx.max_bs < thr_bottom || n == 0)
        return result;

    // topmost candidate
    int bot_top = -1;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
        if (mask(i))
        {
            bot_top = i;
            break;
        }
    if (bot_top < 0)
        return result;

    // cumulative-energy pick: energy = (masked linear BS)^2
    std::vector<double> energy(n, 0.0);
    double              total = 0.0;
    for (int i = 0; i < n; ++i)
        if (mask(i) && std::isfinite(ctx.bs(i)))
        {
            const double lin = std::pow(10.0, ctx.bs(i) * 0.1);
            energy[i]        = lin * lin;
            total += energy[i];
        }

    int          bot     = bot_top;
    const double thr_cum = std::clamp(thr_cum_percent / 100.0, 0.0, 1.0);
    if (total > 0.0)
    {
        double cum = 0.0;
        for (int i = 0; i < n; ++i)
        {
            cum += energy[i];
            if (cum / total >= thr_cum)
            {
                bot = std::max(bot_top, i);
                break;
            }
        }
    }

    // back-stepping up the leading edge by one pulse length
    const int backstep = std::max(4, static_cast<int>(std::lround(pulse_nsamples)));
    if (bot > 2 * backstep)
    {
        const double accept_floor = thr_bottom + thr_echo + thr_backstep;
        while (bot > backstep)
        {
            int    idx_max = -1;
            double bs_val  = NEG_INF;
            for (int j = bot - backstep; j <= bot - 1; ++j)
                if (std::isfinite(ctx.bs(j)) && ctx.bs(j) > bs_val)
                {
                    bs_val  = ctx.bs(j);
                    idx_max = j;
                }
            if (idx_max < 0)
                break;

            const double bs_bot = std::isfinite(ctx.bs(bot)) ? ctx.bs(bot) : NEG_INF;
            if (!(bs_val >= bs_bot + thr_backstep && bs_val > accept_floor) || idx_max >= bot)
                break;
            bot = idx_max;
        }
        bot = std::max(bot - backstep, 0);
    }
    bot = std::max(bot, ctx.s_min);

    // echo-length validation filter (moving-average amplitude in dB)
    const double bot_range = ctx.range(std::clamp(bot, 0, n - 1));
    const double r_p       = std::max(pulse_nsamples * range_resolution, 0.0);
    const double el        = echo_length(r_p, beamwidth_deg, incidence_angle_deg, bot_range);

    int win = 5;
    if (std::isfinite(el) && range_resolution > 0.0)
        win = std::max(5, static_cast<int>(std::lround(el / range_resolution)));
    const int half = win / 2;

    std::vector<double> amp_prefix(n + 1, 0.0);
    std::vector<double> cnt_prefix(n + 1, 0.0);
    for (int i = 0; i < n; ++i)
    {
        const bool fin   = std::isfinite(ctx.bs(i));
        amp_prefix[i + 1] = amp_prefix[i] + (fin ? std::pow(10.0, ctx.bs(i) * 0.05) : 0.0);
        cnt_prefix[i + 1] = cnt_prefix[i] + (fin ? 1.0 : 0.0);
    }

    double bs_bottom = NEG_INF;
    for (int i = ctx.s_min; i <= ctx.s_max; ++i)
    {
        if (!mask(i))
            continue;
        const int    lo  = std::max(i - half, 0);
        const int    hi  = std::min(i + half, n - 1);
        const double amp = amp_prefix[hi + 1] - amp_prefix[lo];
        const double cnt = cnt_prefix[hi + 1] - cnt_prefix[lo];
        if (cnt > 0.0)
            bs_bottom = std::max(bs_bottom, 20.0 * std::log10(amp / cnt));
    }
    if (bs_bottom < thr_bottom)
        return result; // echo too weak after filtering

    // shifts: range shift, denoise offset, pulse half-length
    if (range_resolution > 0.0 && shift_bot_m != 0.0)
        bot -= static_cast<int>(std::ceil(shift_bot_m / range_resolution));
    if (denoised)
        bot -= 1;
    bot = std::max(bot, 0);

    const double bottom_sample = std::floor(static_cast<double>(bot) + pulse_nsamples * 0.5);

    result.bottom_sample         = std::clamp(bottom_sample, 0.0, static_cast<double>(n - 1));
    result.bottom_backscatter_db = bs_bottom;
    result.valid                 = true;
    return result;
}

// =============================================================================
// Public: single ping
// =============================================================================

BottomDetectionResult BottomDetector::find_bottom_in_ping(const xt::xtensor<double, 1>& sv_db,
                                                          double range_offset,
                                                          double range_resolution,
                                                          double pulse_nsamples,
                                                          double beamwidth_deg) const
{
    const auto ctx = compute_ping_context(sv_db, range_offset, range_resolution, pulse_nsamples);
    if (!ctx.usable || ctx.max_bs < thr_bottom)
        return {};

    const auto raw       = candidate_raw(ctx);
    const int  win_range = std::max(5, static_cast<int>(std::lround(2.0 * pulse_nsamples + 1.0)));
    const auto mask      = majority_filter(raw, win_range, mask_fill_fraction);
    return detect_from_mask(ctx, mask, pulse_nsamples, range_resolution, beamwidth_deg);
}

// =============================================================================
// Public: batch of pings
// =============================================================================

std::pair<xt::xtensor<double, 1>, xt::xtensor<double, 1>> BottomDetector::find_bottom(
    const xt::xtensor<double, 2>& sv_db,
    const xt::xtensor<double, 1>& range_offsets,
    const xt::xtensor<double, 1>& range_resolutions,
    const xt::xtensor<double, 1>& pulse_nsamples,
    const xt::xtensor<double, 1>& beamwidths_deg,
    int                           mp_cores) const
{
    const int n_pings   = static_cast<int>(sv_db.shape()[0]);
    const int n_samples = static_cast<int>(sv_db.shape()[1]);

    auto check = [&](const xt::xtensor<double, 1>& a, const char* name) {
        if (static_cast<int>(a.size()) != n_pings)
            throw std::runtime_error(fmt::format(
                "find_bottom: {} length ({}) must match n_pings ({})", name, a.size(), n_pings));
    };
    check(range_offsets, "range_offsets");
    check(range_resolutions, "range_resolutions");
    check(pulse_nsamples, "pulse_nsamples");
    check(beamwidths_deg, "beamwidths_deg");

    xt::xtensor<double, 1> bottom_sample = xt::zeros<double>({ std::max(n_pings, 0) });
    xt::xtensor<double, 1> bottom_bs     = xt::zeros<double>({ std::max(n_pings, 0) });
    std::fill(bottom_sample.begin(), bottom_sample.end(), NaN);
    std::fill(bottom_bs.begin(), bottom_bs.end(), NaN);

    if (n_pings == 0 || n_samples == 0)
        return { bottom_sample, bottom_bs };

    // --- 1. per-ping context + raw candidate masks ---
    std::vector<PingContext>        contexts(n_pings);
    xt::xtensor<uint8_t, 2>         raw = xt::zeros<uint8_t>({ n_pings, n_samples });
    double                          max_pulse = 0.0;
    for (int p = 0; p < n_pings; ++p)
        if (std::isfinite(pulse_nsamples(p)))
            max_pulse = std::max(max_pulse, pulse_nsamples(p));

#pragma omp parallel for num_threads(mp_cores) schedule(guided)
    for (int p = 0; p < n_pings; ++p)
    {
        xt::xtensor<double, 1> ping = xt::view(sv_db, p, xt::all());
        contexts[p]                 = compute_ping_context(
            ping, range_offsets(p), range_resolutions(p), pulse_nsamples(p));
        xt::view(raw, p, xt::all()) = candidate_raw(contexts[p]);
    }

    // --- 2. smooth the mask: range direction, then ping direction ---
    // (ESP3 uses a single block pulse Np = max(pulse) and a box filter)
    const int win_range = std::max(5, static_cast<int>(std::lround(2.0 * max_pulse + 1.0)));
    const int win_ping  = std::max(n_ping_smoothing, 1);

    xt::xtensor<uint8_t, 2> smoothed = xt::zeros<uint8_t>({ n_pings, n_samples });

#pragma omp parallel for num_threads(mp_cores) schedule(guided)
    for (int p = 0; p < n_pings; ++p)
    {
        xt::xtensor<uint8_t, 1> row = xt::view(raw, p, xt::all());
        xt::view(smoothed, p, xt::all()) = majority_filter(row, win_range, mask_fill_fraction);
    }

    if (win_ping > 1)
    {
        xt::xtensor<uint8_t, 2> tmp = smoothed;
#pragma omp parallel for num_threads(mp_cores) schedule(guided)
        for (int s = 0; s < n_samples; ++s)
        {
            xt::xtensor<uint8_t, 1> col = xt::view(tmp, xt::all(), s);
            const auto sm = majority_filter(col, win_ping, mask_fill_fraction);
            for (int p = 0; p < n_pings; ++p)
                smoothed(p, s) = sm(p);
        }
    }

    // --- 3. per-ping detection with the smoothed masks ---
#pragma omp parallel for num_threads(mp_cores) schedule(guided)
    for (int p = 0; p < n_pings; ++p)
    {
        xt::xtensor<uint8_t, 1> mask = xt::view(smoothed, p, xt::all());
        const auto res = detect_from_mask(
            contexts[p], mask, pulse_nsamples(p), range_resolutions(p), beamwidths_deg(p));
        if (res.valid)
        {
            bottom_sample(p) = res.bottom_sample;
            bottom_bs(p)     = res.bottom_backscatter_db;
        }
    }

    // --- 4. optional cross-ping outlier rejection (robust MAD) ---
    if (remove_outliers)
    {
        const int              half = std::max(1, outlier_window);
        xt::xtensor<double, 1> cleaned = bottom_sample;
        for (int i = 0; i < n_pings; ++i)
        {
            if (!std::isfinite(bottom_sample(i)))
                continue;
            std::vector<double> window;
            for (int j = std::max(i - half, 0); j <= std::min(i + half, n_pings - 1); ++j)
                if (std::isfinite(bottom_sample(j)))
                    window.push_back(bottom_sample(j));
            if (window.size() < 3)
                continue;

            std::nth_element(window.begin(), window.begin() + window.size() / 2, window.end());
            const double median = window[window.size() / 2];

            std::vector<double> dev;
            dev.reserve(window.size());
            for (double v : window)
                dev.push_back(std::abs(v - median));
            std::nth_element(dev.begin(), dev.begin() + dev.size() / 2, dev.end());
            const double scaled_mad = 1.4826 * dev[dev.size() / 2];

            if (scaled_mad > 0.0 &&
                std::abs(bottom_sample(i) - median) > outlier_threshold * scaled_mad)
                cleaned(i) = NaN;
        }
        for (int i = 0; i < n_pings; ++i)
            if (!std::isfinite(cleaned(i)))
            {
                bottom_sample(i) = NaN;
                bottom_bs(i)     = NaN;
            }
    }

    // --- 5. optional gap interpolation ---
    if (interpolate_gaps)
    {
        int i = 0;
        while (i < n_pings)
        {
            if (std::isfinite(bottom_sample(i)))
            {
                ++i;
                continue;
            }
            const int start = i;
            while (i < n_pings && !std::isfinite(bottom_sample(i)))
                ++i;
            const int end  = i;       // first finite (or n_pings)
            const int prev = start - 1;
            if (prev < 0 || end >= n_pings)
                continue; // gap touches an edge
            if (end - start > max_interpolation_gap)
                continue;

            const double v0 = bottom_sample(prev);
            const double v1 = bottom_sample(end);
            for (int k = start; k < end; ++k)
            {
                const double t   = static_cast<double>(k - prev) / static_cast<double>(end - prev);
                bottom_sample(k) = v0 + (v1 - v0) * t;
            }
        }
    }

    return { bottom_sample, bottom_bs };
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

} // namespace functions
} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
