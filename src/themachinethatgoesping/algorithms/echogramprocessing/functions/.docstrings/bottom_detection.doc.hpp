//sourcehash: 0c765391c8a77fe1e122e970d76739d58c4561c028ecfc22802e1af56b97947a

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__
#define __DOCSTRINGS_HPP__

#define MKD_EXPAND(x)                                      x
#define MKD_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...)  COUNT
#define MKD_VA_SIZE(...)                                   MKD_EXPAND(MKD_COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define MKD_CAT1(a, b)                                     a ## b
#define MKD_CAT2(a, b)                                     MKD_CAT1(a, b)
#define MKD_DOC1(n1)                                       mkd_doc_##n1
#define MKD_DOC2(n1, n2)                                   mkd_doc_##n1##_##n2
#define MKD_DOC3(n1, n2, n3)                               mkd_doc_##n1##_##n2##_##n3
#define MKD_DOC4(n1, n2, n3, n4)                           mkd_doc_##n1##_##n2##_##n3##_##n4
#define MKD_DOC5(n1, n2, n3, n4, n5)                       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5
#define MKD_DOC6(n1, n2, n3, n4, n5, n6)                   mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define MKD_DOC7(n1, n2, n3, n4, n5, n6, n7)               mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define MKD_DOC8(n1, n2, n3, n4, n5, n6, n7, n8)           mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define MKD_DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define MKD_DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...)                                           MKD_EXPAND(MKD_EXPAND(MKD_CAT2(MKD_DOC, MKD_VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#endif // __DOCSTRINGS_HPP__
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult = R"doc(Result of a single-ping bottom detection.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult_bottom_backscatter_db = R"doc(Surface backscatter (dB) at the detected bottom. NaN if no bottom.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult_bottom_sample = R"doc(Detected bottom sample index (fractional). NaN if no bottom was found.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetectionResult_valid = R"doc(Whether a valid bottom was detected.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector =
R"doc(Robust echogram bottom detector (ESP3 detec_bottom_algo_v4 port).

Configure the public threshold/option fields (their defaults reproduce
ESP3's defaults), then call find_bottom_in_ping or find_bottom.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_BottomDetector = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext = R"doc(Per-ping derived quantities shared by mask building and detection.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_bs = R"doc(surface backscatter per sample (dB), NaN if invalid)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_max_bs = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_range = R"doc(range per sample (m))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_s_max = R"doc(last usable sample (inclusive))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_s_min = R"doc(first usable sample (inclusive))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_PingContext_usable = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_candidate_raw =
R"doc(Raw (unsmoothed) candidate mask: BS above ``max_bs + thr_echo`` within
the usable range window.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_compute_ping_context = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_denoised =
R"doc(Whether the input Sv is denoised (moves the bottom up by one sample,
mirroring ESP3's denoised handling).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_detect_from_mask = R"doc(Core detection given a (smoothed) candidate mask.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_find_bottom_in_ping =
R"doc(Detect the seafloor sample in a single ping of Sv data (dB).

Args:
    sv_db: Per-sample Sv values of the ping in dB.
    range_offset: Range (m) of sample 0 (affine offset).
    range_resolution: Range increment per sample (m, the affine
                      slope).
    pulse_nsamples: Effective pulse length expressed in samples.
    beamwidth_deg: Average beam aperture (deg) for the echo-length
                   validation filter.

Returns:
    BottomDetectionResult (sample, backscatter, validity).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_incidence_angle_deg =
R"doc(Assumed incidence angle (deg) for the theoretical echo-length filter
(combines seafloor slope and transducer tilt). ESP3 uses 10 deg.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_interpolate_gaps =
R"doc(Linearly interpolate the detected bottom across short gaps of invalid
pings.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_mask_fill_fraction =
R"doc(Fraction (0..1) of a smoothing window that must be set for a sample to
remain a candidate (majority filter). ESP3 default: 0.7.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_max_interpolation_gap = R"doc(Maximum gap length (in pings) that will be interpolated.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_n_ping_smoothing =
R"doc(Number of neighbouring pings used by the cross-ping mask smoothing in
batch mode. Use 1 to disable cross-ping smoothing.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_outlier_threshold = R"doc(Outlier threshold in multiples of the median absolute deviation (MAD).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_outlier_window = R"doc(Half-width (in pings) of the median window used for outlier rejection.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_r_max = R"doc(Maximum range (m). Samples above this range are ignored.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_r_min = R"doc(Minimum range (m). Samples below this range are ignored.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_remove_outliers =
R"doc(Reject detections that deviate strongly from a local median (robust
MAD-based outlier rejection across pings).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_shift_bot_m = R"doc(Vertical shift applied to the detected bottom (m, positive = upward).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_thr_backstep =
R"doc(Back-step threshold (dB). While walking up the leading edge a sample
counts as "still bottom" if its BS >= ``BS(bottom) + thr_backstep``.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_thr_bottom =
R"doc(Absolute floor on the strongest surface-backscatter (dB) for a ping to
contain a bottom. Pings whose maximum BS is below this yield no
bottom.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_thr_cum_percent =
R"doc(Cumulative-energy threshold in percent (0..100): the bottom is placed
at the first sample where the normalized cumulative masked energy
reaches this fraction.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_functions_BottomDetector_thr_echo =
R"doc(Samples whose BS is within this (negative) dB offset of the per-ping
maximum are kept as bottom candidates.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


