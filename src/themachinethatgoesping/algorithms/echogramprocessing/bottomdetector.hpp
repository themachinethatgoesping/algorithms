// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University, Urcoustics
//
// SPDX-License-Identifier: MPL-2.0

/**
 * @file bottomdetector.hpp
 * @brief Echogram bottom detector, a simplified port of ESP3's
 * detec_bottom_algo_v4 (Yoann Ladroit, NIWA).
 *
 * The detector works in two steps:
 *   1. add_ping():   store a ping's surface backscatter and range geometry.
 *   2. get_bottom() / get_bottom_backscatter(): lazily "compile" the whole
 *      sequence once, applying the cross-ping mask smoothing and the per-ping
 *      detection, plus optional outlier rejection / gap interpolation.
 *
 * All maths is done in single precision (float) for speed.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/bottomdetector.doc.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/generators/xbuilder.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "functions/bottom_detection.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {

/**
 * @brief Two-step echogram bottom detector (ESP3 detec_bottom_algo_v4 port).
 *
 * Feed pings one by one with add_ping(); read the detected track back with
 * get_bottom() / get_bottom_backscatter(). The cross-ping processing is run
 * once, on demand, the first time a result is requested after the last change.
 */
class BottomDetector
{
  public:
    // ---- detection thresholds (dB) ----
    float thr_bottom      = -35.0f; ///< absolute floor on the per-ping maximum surface BS
    float thr_echo        = -35.0f; ///< candidate offset below the per-ping maximum BS
    float thr_backstep    = -1.0f;  ///< back-step threshold while walking up the leading edge
    float thr_cum_percent = 1.0f;   ///< cumulative-energy threshold (percent, 0..100)

    // ---- range gating (m) ----
    float r_min       = 0.0f;                                   ///< minimum range (m)
    float r_max       = std::numeric_limits<float>::infinity(); ///< maximum range (m)
    float shift_bot_m = 0.0f; ///< vertical shift of the detected bottom (m, positive = up)
    bool  denoised    = true; ///< whether the input Sv is denoised (moves the bottom up 1 sample)

    // ---- geometry / morphology ----
    float incidence_angle_deg = 10.0f; ///< incidence angle (deg) for the echo-length filter
    int   n_ping_smoothing    = 5;     ///< neighbouring pings for cross-ping smoothing (1 = off)
    float mask_fill_fraction  = 0.7f;  ///< majority-filter fill fraction (0..1)

    // ---- optional cross-ping post-processing ----
    bool  remove_outliers       = false; ///< robust (MAD) cross-ping outlier rejection
    int   outlier_window        = 7;     ///< half-width (pings) of the outlier median window
    float outlier_threshold     = 3.0f;  ///< outlier threshold in multiples of the MAD
    bool  interpolate_gaps      = false; ///< interpolate the bottom across short invalid gaps
    int   max_interpolation_gap = 10;    ///< maximum interpolated gap length (pings)

    /// Default constructor (all parameters keep their ESP3 defaults).
    BottomDetector() = default;

    /// Construct a detector with explicit parameters (convenient as a Python
    /// named-parameter constructor since every argument has a default).
    BottomDetector(float thr_bottom,
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
                   int   max_interpolation_gap);

    /// Equality compares the configuration parameters only (not the buffered pings).
    bool operator==(const BottomDetector& rhs) const
    {
        return thr_bottom == rhs.thr_bottom && thr_echo == rhs.thr_echo &&
               thr_backstep == rhs.thr_backstep && thr_cum_percent == rhs.thr_cum_percent &&
               r_min == rhs.r_min && r_max == rhs.r_max && shift_bot_m == rhs.shift_bot_m &&
               denoised == rhs.denoised && incidence_angle_deg == rhs.incidence_angle_deg &&
               n_ping_smoothing == rhs.n_ping_smoothing &&
               mask_fill_fraction == rhs.mask_fill_fraction &&
               remove_outliers == rhs.remove_outliers && outlier_window == rhs.outlier_window &&
               outlier_threshold == rhs.outlier_threshold &&
               interpolate_gaps == rhs.interpolate_gaps &&
               max_interpolation_gap == rhs.max_interpolation_gap;
    }

    // =========================================================================
    // Step 1: add pings
    // =========================================================================

    /**
     * @brief Add one ping of Sv data (dB).
     *
     * Templated on the input tensor type so numpy / xtensor arrays of any float
     * type are accepted directly. The surface backscatter
     * ``BS = Sv + 10*log10(range)`` is materialized exactly once into the stored
     * float buffer, so there is no redundant pytensor -> xtensor copy.
     *
     * @tparam t_xtensor_1d 1D tensor type holding the Sv samples (dB).
     * @param sv_db            per-sample Sv (dB).
     * @param range_offset     range (m) of sample 0 (affine offset).
     * @param range_resolution range increment per sample (m, the affine slope).
     * @param pulse_nsamples   effective pulse length in samples.
     * @param beamwidth_deg    beam aperture (deg) for the echo-length filter.
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d>
    void add_ping(const t_xtensor_1d& sv_db,
                  float               range_offset,
                  float               range_resolution,
                  float               pulse_nsamples,
                  float               beamwidth_deg)
    {
        Ping ping;
        ping.range_offset     = range_offset;
        ping.range_resolution = range_resolution;
        ping.pulse_nsamples   = pulse_nsamples;
        ping.beamwidth_deg    = beamwidth_deg;

        const std::size_t n = sv_db.size();
        if (n > 0)
        {
            // range / safe_range stay lazy: only the BS buffer is allocated.
            auto range = range_offset + range_resolution * xt::arange<float>(static_cast<int>(n));
            auto safe  = xt::where(range > 0.0f, range, 1.0f);
            ping.bs    = xt::where(xt::isfinite(sv_db) && (range > 0.0f),
                                xt::cast<float>(sv_db) + 10.0f * xt::log10(safe),
                                std::numeric_limits<float>::quiet_NaN());
        }

        _pings.push_back(std::move(ping));
        _dirty = true;
    }

    // =========================================================================
    // Step 2: results (cross-ping processing happens lazily on first access)
    // =========================================================================

    /// Detected bottom sample index per added ping (NaN where no bottom).
    const xt::xtensor<float, 1>& get_bottom()
    {
        compile();
        return _bottom_sample;
    }

    /// Surface backscatter (dB) at the detected bottom per added ping (NaN where none).
    const xt::xtensor<float, 1>& get_bottom_backscatter()
    {
        compile();
        return _bottom_bs;
    }

    /// Clear all accumulated pings and cached results.
    void reset();

    /// Number of pings added so far.
    std::size_t size() const { return _pings.size(); }

    /// Get the ObjectPrinter for BottomDetector.
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

  private:
    /// One buffered ping: the surface backscatter plus its affine range geometry.
    struct Ping
    {
        xt::xtensor<float, 1> bs;                   ///< surface BS per sample (dB), NaN if invalid
        float                 range_offset     = 0.0f;
        float                 range_resolution = 0.0f;
        float                 pulse_nsamples   = 0.0f;
        float                 beamwidth_deg    = 0.0f;
    };

    /// Per-ping derived gating, computed transiently during compile().
    struct Context
    {
        int   s_min  = 0;                                    ///< first usable sample (inclusive)
        int   s_max  = -1;                                   ///< last usable sample (inclusive)
        float max_bs = std::numeric_limits<float>::quiet_NaN();
        bool  usable = false;
    };

    // ---- detection pipeline (non-templated, defined in bottomdetector.cpp) ----
    Context                 analyze_ping(const Ping& ping) const;
    xt::xtensor<uint8_t, 1> range_mask(const Ping& ping, const Context& ctx) const;
    xt::xtensor<uint8_t, 1> cross_ping_mask(const std::vector<xt::xtensor<uint8_t, 1>>& masks,
                                            int                                          center,
                                            int                                          half) const;
    std::pair<float, float> detect(const Ping&                    ping,
                                   const Context&                 ctx,
                                   const xt::xtensor<uint8_t, 1>& mask) const;

    void compile();
    void reject_outliers(xt::xtensor<float, 1>& bottom_sample,
                         xt::xtensor<float, 1>& bottom_bs) const;
    void interpolate_bottom_gaps(xt::xtensor<float, 1>& bottom_sample) const;

    // ---- state ----
    std::vector<Ping>     _pings;         ///< buffered input pings (step 1)
    xt::xtensor<float, 1> _bottom_sample; ///< cached detected bottom (step 2)
    xt::xtensor<float, 1> _bottom_bs;     ///< cached bottom backscatter (step 2)
    bool                  _dirty = true;  ///< whether the cached result needs rebuilding
};

} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
