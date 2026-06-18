// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

/**
 * @file bottom_detection.hpp
 * @brief Robust echogram bottom detection ported from ESP3's detec_bottom_algo_v4.
 *
 * The detector operates on water-column Sv data (in dB) and finds, for each
 * ping, the sample index of the seafloor echo. It reproduces ESP3's robustness
 * against premature detection (ringdown, biology, near-field noise) by
 *  1. range-normalizing Sv to a "surface backscatter" ``BS = Sv + 10*log10(r)``
 *     so fixed dB thresholds work at every range,
 *  2. masking samples close (in dB) to the strongest echo,
 *  3. (batch) smoothing that mask along range and across neighbouring pings,
 *  4. picking the bottom with a normalized cumulative-energy threshold,
 *  5. back-stepping up the leading edge by one pulse length, and
 *  6. validating with a beam-width derived echo-length filter.
 *
 * All tunables and the detection routines live on the @ref BottomDetector
 * class: configure the public member fields, then call
 * @ref BottomDetector::find_bottom_in_ping (single ping, easy to debug) or
 * @ref BottomDetector::find_bottom (a block of pings, adds the cross-ping
 * smoothing and optional outlier rejection / gap interpolation).
 *
 * Per-ping range geometry is a linear affine
 * ``range(sample) = range_offset + range_resolution * sample`` matching the
 * per-ping range affine used by the echogram builder.
 *
 * The heavy implementation lives in ``bottom_detection.cpp`` (concrete
 * ``xt::xtensor<double, N>`` overloads) to keep compile times low; the Python
 * bindings simply cast their input to ``double`` and call these.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/bottom_detection.doc.hpp"

#include <cstdint>
#include <limits>
#include <utility>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace echogramprocessing {
namespace functions {

/**
 * @brief Result of a single-ping bottom detection.
 */
struct BottomDetectionResult
{
    /// Detected bottom sample index (fractional). NaN if no bottom was found.
    double bottom_sample = std::numeric_limits<double>::quiet_NaN();

    /// Surface backscatter (dB) at the detected bottom. NaN if no bottom.
    double bottom_backscatter_db = std::numeric_limits<double>::quiet_NaN();

    /// Whether a valid bottom was detected.
    bool valid = false;

    bool operator==(const BottomDetectionResult& rhs) const = default;

    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BottomDetectionResult", float_precision, superscript_exponents);
        printer.register_value("bottom_sample", bottom_sample);
        printer.register_value("bottom_backscatter_db", bottom_backscatter_db, "dB");
        printer.register_value("valid", valid);
        return printer;
    }
};

/**
 * @brief Robust echogram bottom detector (ESP3 detec_bottom_algo_v4 port).
 *
 * Configure the public threshold/option fields (their defaults reproduce ESP3's
 * defaults), then call @ref find_bottom_in_ping or @ref find_bottom.
 */
class BottomDetector
{
  public:
    // ---- detection thresholds (dB) ----

    /// Absolute floor on the strongest surface-backscatter (dB) for a ping to
    /// contain a bottom. Pings whose maximum BS is below this yield no bottom.
    double thr_bottom = -35.0;

    /// Samples whose BS is within this (negative) dB offset of the per-ping
    /// maximum are kept as bottom candidates.
    double thr_echo = -35.0;

    /// Back-step threshold (dB). While walking up the leading edge a sample
    /// counts as "still bottom" if its BS >= ``BS(bottom) + thr_backstep``.
    double thr_backstep = -1.0;

    /// Cumulative-energy threshold in percent (0..100): the bottom is placed at
    /// the first sample where the normalized cumulative masked energy reaches
    /// this fraction.
    double thr_cum_percent = 1.0;

    // ---- range gating (m) ----

    /// Minimum range (m). Samples below this range are ignored.
    double r_min = 0.0;

    /// Maximum range (m). Samples above this range are ignored.
    double r_max = std::numeric_limits<double>::infinity();

    /// Vertical shift applied to the detected bottom (m, positive = upward).
    double shift_bot_m = 0.0;

    /// Whether the input Sv is denoised (moves the bottom up by one sample,
    /// mirroring ESP3's denoised handling).
    bool denoised = true;

    // ---- geometry / morphology ----

    /// Assumed incidence angle (deg) for the theoretical echo-length filter
    /// (combines seafloor slope and transducer tilt). ESP3 uses 10 deg.
    double incidence_angle_deg = 10.0;

    /// Number of neighbouring pings used by the cross-ping mask smoothing in
    /// batch mode. Use 1 to disable cross-ping smoothing.
    int n_ping_smoothing = 5;

    /// Fraction (0..1) of a smoothing window that must be set for a sample to
    /// remain a candidate (majority filter). ESP3 default: 0.7.
    double mask_fill_fraction = 0.7;

    // ---- optional cross-ping post-processing (batch only) ----

    /// Reject detections that deviate strongly from a local median (robust
    /// MAD-based outlier rejection across pings).
    bool remove_outliers = false;

    /// Half-width (in pings) of the median window used for outlier rejection.
    int outlier_window = 7;

    /// Outlier threshold in multiples of the median absolute deviation (MAD).
    double outlier_threshold = 3.0;

    /// Linearly interpolate the detected bottom across short gaps of invalid
    /// pings.
    bool interpolate_gaps = false;

    /// Maximum gap length (in pings) that will be interpolated.
    int max_interpolation_gap = 10;

    BottomDetector() = default;

    bool operator==(const BottomDetector& rhs) const = default;

    // =========================================================================
    // Detection (implemented in bottom_detection.cpp)
    // =========================================================================

    /**
     * @brief Detect the seafloor sample in a single ping of Sv data (dB).
     *
     * @param sv_db Per-sample Sv values of the ping in dB.
     * @param range_offset Range (m) of sample 0 (affine offset).
     * @param range_resolution Range increment per sample (m, the affine slope).
     * @param pulse_nsamples Effective pulse length expressed in samples.
     * @param beamwidth_deg Average beam aperture (deg) for the echo-length
     *        validation filter.
     * @return BottomDetectionResult (sample, backscatter, validity).
     */
    BottomDetectionResult find_bottom_in_ping(const xt::xtensor<double, 1>& sv_db,
                                              double                        range_offset,
                                              double                        range_resolution,
                                              double                        pulse_nsamples,
                                              double                        beamwidth_deg) const;

    /**
     * @brief Detect the seafloor for a block of pings.
     *
     * The Sv array is ping-major ``sv_db(ping, sample)``. The driver builds the
     * per-ping candidate masks, smooths them along range and across
     * @ref n_ping_smoothing pings, runs the per-ping detector, and applies the
     * optional @ref remove_outliers / @ref interpolate_gaps post-processing.
     *
     * For millions of pings, call this per block (the Sv image is the heavy
     * data; the returned tracks are tiny). The loops are OpenMP-parallel.
     *
     * @param sv_db Sv data in dB, shape ``[n_pings, n_samples]``.
     * @param range_offsets Range (m) of sample 0 per ping ``[n_pings]``.
     * @param range_resolutions Range increment per sample (m) per ping.
     * @param pulse_nsamples Effective pulse length in samples per ping.
     * @param beamwidths_deg Average beam aperture (deg) per ping.
     * @param mp_cores Number of OpenMP threads (default 1).
     * @return Pair of 1D tensors ``(bottom_sample, bottom_backscatter_db)``;
     *         invalid pings are NaN.
     */
    std::pair<xt::xtensor<double, 1>, xt::xtensor<double, 1>> find_bottom(
        const xt::xtensor<double, 2>& sv_db,
        const xt::xtensor<double, 1>& range_offsets,
        const xt::xtensor<double, 1>& range_resolutions,
        const xt::xtensor<double, 1>& pulse_nsamples,
        const xt::xtensor<double, 1>& beamwidths_deg,
        int                           mp_cores = 1) const;

    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

  private:
    /// Per-ping derived quantities shared by mask building and detection.
    struct PingContext
    {
        xt::xtensor<double, 1> bs;    ///< surface backscatter per sample (dB), NaN if invalid
        xt::xtensor<double, 1> range; ///< range per sample (m)
        double                 max_bs = std::numeric_limits<double>::quiet_NaN();
        int                    s_min  = 0;  ///< first usable sample (inclusive)
        int                    s_max  = -1; ///< last usable sample (inclusive)
        bool                   usable = false;
    };

    PingContext compute_ping_context(const xt::xtensor<double, 1>& sv_db,
                                     double                        range_offset,
                                     double                        range_resolution,
                                     double                        pulse_nsamples) const;

    /// Raw (unsmoothed) candidate mask: BS above ``max_bs + thr_echo`` within
    /// the usable range window.
    xt::xtensor<uint8_t, 1> candidate_raw(const PingContext& ctx) const;

    /// Core detection given a (smoothed) candidate mask.
    BottomDetectionResult detect_from_mask(const PingContext&             ctx,
                                           const xt::xtensor<uint8_t, 1>& mask,
                                           double                         pulse_nsamples,
                                           double                         range_resolution,
                                           double                         beamwidth_deg) const;
};

} // namespace functions
} // namespace echogramprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
