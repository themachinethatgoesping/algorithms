//sourcehash: d44f34fc89e70cd9ec5cd7fd6c1f0c5705ea5ffc1580d988286671f65dc57d95

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


static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector =
R"doc(Two-step echogram bottom detector (ESP3 detec_bottom_algo_v4 port).

Feed pings one by one with add_ping(); read the detected track back
with get_bottom() / get_bottom_backscatter(). The cross-ping
processing is run once, on demand, the first time a result is
requested after the last change.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_BottomDetector = R"doc(Default constructor (all parameters keep their ESP3 defaults).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_BottomDetector_2 =
R"doc(Construct a detector with explicit parameters (convenient as a Python
named-parameter constructor since every argument has a default).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Context = R"doc(Per-ping derived gating, computed transiently during compile().)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Context_max_bs = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Context_s_max = R"doc(last usable sample (inclusive))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Context_s_min = R"doc(first usable sample (inclusive))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Context_usable = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping =
R"doc(One buffered ping: the surface backscatter plus its affine range
geometry.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping_beamwidth_deg = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping_bs = R"doc(surface BS per sample (dB), NaN if invalid)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping_pulse_nsamples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping_range_offset = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_Ping_range_resolution = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_add_ping =
R"doc(Add one ping of Sv data (dB).

Templated on the input tensor type so numpy / xtensor arrays of any
float type are accepted directly. The surface backscatter ``BS = Sv +
10*log10(range)`` is materialized exactly once into the stored float
buffer, so there is no redundant pytensor -> xtensor copy.

Args:
    sv_db: per-sample Sv (dB).
    range_offset: range (m) of sample 0 (affine offset).
    range_resolution: range increment per sample (m, the affine
                      slope).
    pulse_nsamples: effective pulse length in samples.
    beamwidth_deg: beam aperture (deg) for the echo-length filter.

Template Args:
    t_xtensor_1d: 1D tensor type holding the Sv samples (dB).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_analyze_ping = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_bottom_bs = R"doc(cached bottom backscatter (step 2))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_bottom_sample = R"doc(cached detected bottom (step 2))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_compile = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_cross_ping_mask = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_denoised = R"doc(whether the input Sv is denoised (moves the bottom up 1 sample))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_detect = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_dirty = R"doc(whether the cached result needs rebuilding)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_get_bottom = R"doc(Detected bottom sample index per added ping (NaN where no bottom).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_get_bottom_backscatter =
R"doc(Surface backscatter (dB) at the detected bottom per added ping (NaN
where none).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_incidence_angle_deg = R"doc(incidence angle (deg) for the echo-length filter)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_interpolate_bottom_gaps = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_interpolate_gaps = R"doc(interpolate the bottom across short invalid gaps)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_mask_fill_fraction = R"doc(majority-filter fill fraction (0..1))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_max_interpolation_gap = R"doc(maximum interpolated gap length (pings))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_n_ping_smoothing = R"doc(neighbouring pings for cross-ping smoothing (1 = off))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_operator_eq =
R"doc(Equality compares the configuration parameters only (not the buffered
pings).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_outlier_threshold = R"doc(outlier threshold in multiples of the MAD)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_outlier_window = R"doc(half-width (pings) of the outlier median window)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_pings = R"doc(buffered input pings (step 1))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_printer = R"doc(Get the ObjectPrinter for BottomDetector.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_r_max = R"doc(maximum range (m))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_r_min = R"doc(minimum range (m))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_range_mask = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_reject_outliers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_remove_outliers = R"doc(robust (MAD) cross-ping outlier rejection)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_reset = R"doc(Clear all accumulated pings and cached results.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_shift_bot_m = R"doc(vertical shift of the detected bottom (m, positive = up))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_size = R"doc(Number of pings added so far.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_thr_backstep = R"doc(back-step threshold while walking up the leading edge)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_thr_bottom = R"doc(absolute floor on the per-ping maximum surface BS)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_thr_cum_percent = R"doc(cumulative-energy threshold (percent, 0..100))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_echogramprocessing_BottomDetector_thr_echo = R"doc(candidate offset below the per-ping maximum BS)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


