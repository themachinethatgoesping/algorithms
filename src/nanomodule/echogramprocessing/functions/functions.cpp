// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>

#include <xtensor/core/xmath.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <themachinethatgoesping/algorithms/echogramprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_echogramprocessing {
namespace py_functions {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::echogramprocessing::functions;

// Add the detection methods to the BottomDetector class for one Sv float type.
// The Sv input is cast to double and dispatched to the concrete (compiled)
// implementation, so the algorithm itself is compiled only once.
template<typename t_float>
void add_detection_methods(nb::class_<BottomDetector>& cls)
{
    cls.def(
        "find_bottom_in_ping",
        [](const BottomDetector&                     self,
           const xt::nanobind::pytensor<t_float, 1>& sv_db,
           double                                    range_offset,
           double                                    range_resolution,
           double                                    pulse_nsamples,
           double                                    beamwidth_deg) {
            const xt::xtensor<double, 1> sv = xt::cast<double>(sv_db);
            return self.find_bottom_in_ping(
                sv, range_offset, range_resolution, pulse_nsamples, beamwidth_deg);
        },
        "Detect the seafloor sample in a single ping of Sv data (dB).\n\n"
        "Range geometry is the affine range = range_offset + range_resolution * sample.\n"
        "Returns a BottomDetectionResult (bottom_sample, bottom_backscatter_db, valid).",
        nb::arg("sv_db").noconvert(),
        nb::arg("range_offset"),
        nb::arg("range_resolution"),
        nb::arg("pulse_nsamples"),
        nb::arg("beamwidth_deg"));

    cls.def(
        "find_bottom",
        [](const BottomDetector&                     self,
           const xt::nanobind::pytensor<t_float, 2>& sv_db,
           const xt::nanobind::pytensor<t_float, 1>& range_offsets,
           const xt::nanobind::pytensor<t_float, 1>& range_resolutions,
           const xt::nanobind::pytensor<t_float, 1>& pulse_nsamples,
           const xt::nanobind::pytensor<t_float, 1>& beamwidths_deg,
           int                                       mp_cores) {
            const xt::xtensor<double, 2> sv  = xt::cast<double>(sv_db);
            const xt::xtensor<double, 1> ro  = xt::cast<double>(range_offsets);
            const xt::xtensor<double, 1> rr  = xt::cast<double>(range_resolutions);
            const xt::xtensor<double, 1> pn  = xt::cast<double>(pulse_nsamples);
            const xt::xtensor<double, 1> bw  = xt::cast<double>(beamwidths_deg);
            auto res = self.find_bottom(sv, ro, rr, pn, bw, mp_cores);
            return std::make_pair(xt::nanobind::pytensor<double, 1>(std::move(res.first)),
                                  xt::nanobind::pytensor<double, 1>(std::move(res.second)));
        },
        "Detect the seafloor for a block of pings.\n\n"
        "sv_db is ping-major (sv_db[ping, sample], dB). Per-ping range geometry is the "
        "affine range = range_offset + range_resolution * sample (matching the echogram "
        "range affine). Builds per-ping candidate masks, smooths them along range and "
        "across neighbouring pings, runs the per-ping detector and applies the optional "
        "cross-ping outlier rejection / gap interpolation.\n\n"
        "Returns a tuple (bottom_sample, bottom_backscatter_db), each of length n_pings; "
        "invalid pings are NaN.",
        nb::arg("sv_db").noconvert(),
        nb::arg("range_offsets").noconvert(),
        nb::arg("range_resolutions").noconvert(),
        nb::arg("pulse_nsamples").noconvert(),
        nb::arg("beamwidths_deg").noconvert(),
        nb::arg("mp_cores") = 1);
}

void init_m_functions(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("functions");
    submodule.doc()       = "Echogram processing functions (ESP3-style bottom detection)";

    // result struct
    nb::class_<BottomDetectionResult>(
        submodule, "BottomDetectionResult", "Result of a single-ping bottom detection.")
        .def(nb::init<>())
        .def_rw("bottom_sample", &BottomDetectionResult::bottom_sample,
                "Detected bottom sample index (fractional); NaN if no bottom.")
        .def_rw("bottom_backscatter_db", &BottomDetectionResult::bottom_backscatter_db,
                "Surface backscatter (dB) at the detected bottom; NaN if no bottom.")
        .def_rw("valid", &BottomDetectionResult::valid, "Whether a valid bottom was detected.")
        .def("__eq__", &BottomDetectionResult::operator==, nb::arg("other"))
        __PYCLASS_DEFAULT_COPY__(BottomDetectionResult)
        __PYCLASS_DEFAULT_PRINTING__(BottomDetectionResult);

    // detector class (parameters as read/write fields + detection methods)
    auto cls =
        nb::class_<BottomDetector>(
            submodule,
            "BottomDetector",
            "Robust echogram bottom detector (ESP3 detec_bottom_algo_v4 port). "
            "Set the threshold/option fields, then call find_bottom_in_ping or find_bottom.")
            .def(nb::init<>())
            .def_rw("thr_bottom", &BottomDetector::thr_bottom,
                    "Absolute floor (dB) on the strongest surface backscatter per ping.")
            .def_rw("thr_echo", &BottomDetector::thr_echo,
                    "Candidate offset (dB) below the per-ping maximum backscatter.")
            .def_rw("thr_backstep", &BottomDetector::thr_backstep,
                    "Back-step threshold (dB) for walking up the leading edge.")
            .def_rw("thr_cum_percent", &BottomDetector::thr_cum_percent,
                    "Cumulative-energy threshold (percent, 0..100).")
            .def_rw("r_min", &BottomDetector::r_min, "Minimum range (m).")
            .def_rw("r_max", &BottomDetector::r_max, "Maximum range (m).")
            .def_rw("shift_bot_m", &BottomDetector::shift_bot_m,
                    "Vertical shift of the detected bottom (m, positive = up).")
            .def_rw("denoised", &BottomDetector::denoised, "Whether the input Sv is denoised.")
            .def_rw("incidence_angle_deg", &BottomDetector::incidence_angle_deg,
                    "Incidence angle (deg) for the echo-length filter.")
            .def_rw("n_ping_smoothing", &BottomDetector::n_ping_smoothing,
                    "Neighbouring pings used for cross-ping mask smoothing (1 = off).")
            .def_rw("mask_fill_fraction", &BottomDetector::mask_fill_fraction,
                    "Majority-filter fill fraction (0..1).")
            .def_rw("remove_outliers", &BottomDetector::remove_outliers,
                    "Reject cross-ping outliers (robust MAD).")
            .def_rw("outlier_window", &BottomDetector::outlier_window,
                    "Half-width (pings) of the outlier median window.")
            .def_rw("outlier_threshold", &BottomDetector::outlier_threshold,
                    "Outlier threshold in multiples of the MAD.")
            .def_rw("interpolate_gaps", &BottomDetector::interpolate_gaps,
                    "Interpolate the bottom across short invalid gaps.")
            .def_rw("max_interpolation_gap", &BottomDetector::max_interpolation_gap,
                    "Maximum interpolated gap length (pings).")
            .def("__eq__", &BottomDetector::operator==, nb::arg("other"))
            __PYCLASS_DEFAULT_COPY__(BottomDetector)
            __PYCLASS_DEFAULT_PRINTING__(BottomDetector);

    add_detection_methods<float>(cls);
    add_detection_methods<double>(cls);
}

} // namespace py_functions
} // namespace py_echogramprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
