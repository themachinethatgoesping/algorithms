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

functions::BottomDetectorConfig BottomDetector::make_config() const
{
    functions::BottomDetectorConfig config;
    config.thr_bottom            = thr_bottom;
    config.thr_echo              = thr_echo;
    config.thr_backstep          = thr_backstep;
    config.thr_cum_percent       = thr_cum_percent;
    config.r_min                 = r_min;
    config.r_max                 = r_max;
    config.shift_bot_m           = shift_bot_m;
    config.denoised              = denoised;
    config.incidence_angle_deg   = incidence_angle_deg;
    config.n_ping_smoothing      = n_ping_smoothing;
    config.mask_fill_fraction    = mask_fill_fraction;
    config.remove_outliers       = remove_outliers;
    config.outlier_window        = outlier_window;
    config.outlier_threshold     = outlier_threshold;
    config.interpolate_gaps      = interpolate_gaps;
    config.max_interpolation_gap = max_interpolation_gap;
    config.mp_cores              = mp_cores;
    return config;
}

int BottomDetector::smoothing_half() const
{
    return std::max(n_ping_smoothing, 1) / 2;
}

std::size_t BottomDetector::batch_size() const
{
    // Size the parallel ingest/finalize batch so each worker gets enough
    // independent pings to amortize OpenMP overhead. With mp_cores == 1 this is
    // still a small batch that keeps add_ping cheap and memory bounded.
    const std::size_t cores = static_cast<std::size_t>(std::max(1, mp_cores));
    return std::max<std::size_t>(64, cores * 16);
}

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
                               int   max_interpolation_gap,
                               int   mp_cores)
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
    , mp_cores(std::max(1, mp_cores))
{
}

// =============================================================================
// Streaming pipeline: parallel per-ping precompute + cross-ping detection over
// a bounded sliding window. add_ping() only appends; the heavy work happens
// here, batched so it parallelizes across mp_cores and each ping is touched
// exactly once.
// =============================================================================

void BottomDetector::advance()
{
    const auto        config = make_config();
    const int         half   = smoothing_half();
    const std::size_t size   = _pings.size();

    // 1) Per-ping precompute (range gating + candidate mask) for the pings that
    //    were appended since the last advance(). Each ping is independent, so
    //    this is embarrassingly parallel and every ping is processed only once.
    if (_n_precomputed < size)
    {
        const int lo = static_cast<int>(_n_precomputed);
        const int hi = static_cast<int>(size);
#pragma omp parallel for schedule(static) num_threads(mp_cores)
        for (int p = lo; p < hi; ++p)
        {
            const auto idx = static_cast<std::size_t>(p);
            _ctx[idx]      = functions::analyze_ping(_pings, idx, config);
            _mask[idx]     = functions::range_mask(_pings, idx, _ctx[idx], config);
        }
        _n_precomputed = size;
    }

    // 2) Finalize every ping that already has its full right-hand smoothing
    //    support (half neighbours to the right). Detections are independent and
    //    run in parallel; results are appended to the finalized history in order.
    const std::size_t q0    = _next_finalize - _window_front;
    std::size_t       q_end = q0;
    while (q_end < size && q_end + static_cast<std::size_t>(half) < size)
        ++q_end;

    if (q_end > q0)
    {
        const std::size_t  count = q_end - q0;
        std::vector<float> out_sample(count);
        std::vector<float> out_bs(count);
#pragma omp parallel for schedule(static) num_threads(mp_cores)
        for (int t = 0; t < static_cast<int>(count); ++t)
        {
            const std::size_t q      = q0 + static_cast<std::size_t>(t);
            const auto        mask_s = functions::cross_ping_mask(_mask, q, half, mask_fill_fraction);
            const auto [sample, bs]  = functions::detect(_pings, q, _ctx[q], mask_s, config);
            out_sample[static_cast<std::size_t>(t)] = sample;
            out_bs[static_cast<std::size_t>(t)]     = bs;
        }
        _finalized_bottom_sample.insert(
            _finalized_bottom_sample.end(), out_sample.begin(), out_sample.end());
        _finalized_bottom_bs.insert(_finalized_bottom_bs.end(), out_bs.begin(), out_bs.end());
        _next_finalize += count;
    }

    // 3) Drop front pings that no future detection can reference any more,
    //    retaining `half` pings behind the finalize frontier for left support.
    const std::size_t keep_from = (_next_finalize > static_cast<std::size_t>(half))
                                      ? _next_finalize - static_cast<std::size_t>(half)
                                      : 0;
    while (_window_front < keep_from)
    {
        _pings.pop_front();
        _ctx.pop_front();
        _mask.pop_front();
        ++_window_front;
        --_n_precomputed;
    }
}

void BottomDetector::compile()
{
    if (!_dirty)
        return;

    // Finalize everything that can be safely finalized (parallel + dropped),
    // then assemble the returned snapshot from the finalized history plus an
    // on-the-fly pass over the unfinalized tail (at most `half` pings). The tail
    // is recomputed each call but NOT persisted, so it is re-evaluated with full
    // neighbour context once later pings arrive.
    advance();

    const auto        config         = make_config();
    const int         half           = smoothing_half();
    const std::size_t finalized_size = _finalized_bottom_sample.size();
    const std::size_t total_size     = _n_added;

    _bottom_sample = xt::xtensor<float, 1>::from_shape({ total_size });
    _bottom_bs     = xt::xtensor<float, 1>::from_shape({ total_size });

    for (std::size_t i = 0; i < finalized_size; ++i)
    {
        _bottom_sample.unchecked(i) = _finalized_bottom_sample[i];
        _bottom_bs.unchecked(i)     = _finalized_bottom_bs[i];
    }

    const std::size_t tail = total_size - finalized_size; // unfinalized window tail
    if (tail > 0)
    {
        const std::size_t q0 = _next_finalize - _window_front; // deque pos of first tail ping
#pragma omp parallel for schedule(static) num_threads(mp_cores)
        for (int t = 0; t < static_cast<int>(tail); ++t)
        {
            const std::size_t q      = q0 + static_cast<std::size_t>(t);
            const auto        mask_s = functions::cross_ping_mask(_mask, q, half, mask_fill_fraction);
            const auto [sample, bs]  = functions::detect(_pings, q, _ctx[q], mask_s, config);
            _bottom_sample.unchecked(finalized_size + static_cast<std::size_t>(t)) = sample;
            _bottom_bs.unchecked(finalized_size + static_cast<std::size_t>(t))     = bs;
        }
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
    _ctx.clear();
    _mask.clear();
    _finalized_bottom_sample.clear();
    _finalized_bottom_bs.clear();
    _bottom_sample = xt::xtensor<float, 1>();
    _bottom_bs     = xt::xtensor<float, 1>();
    _window_front  = 0;
    _n_precomputed = 0;
    _next_finalize = 0;
    _n_added       = 0;
    _dirty         = false;
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
        const int end  = i; // first finite (or n)
        const int prev = start - 1;
        if (prev < 0 || end >= n)
            continue; // gap touches an edge
        if (end - start > max_interpolation_gap)
            continue;

        const float v0 = bottom_sample.unchecked(prev);
        const float v1 = bottom_sample.unchecked(end);
        for (int k = start; k < end; ++k)
        {
            const float t = static_cast<float>(k - prev) / static_cast<float>(end - prev);
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
    printer.register_value("window_pings", static_cast<int>(_pings.size()));
    printer.register_value("finalized_pings", static_cast<int>(_next_finalize));
    printer.register_value("total_pings", static_cast<int>(_n_added));

    return printer;
}

} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
