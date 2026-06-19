// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <limits>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>

#include <xtensor-python/nanobind/pytensor.hpp>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <themachinethatgoesping/algorithms/echogramprocessing/bottomdetector.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_echogramprocessing {

namespace nb = nanobind;
using algorithms::echogramprocessing::BottomDetector;

// Register add_ping for one pytensor float type. Instantiated for float32 and
// float64 so either numpy dtype is accepted without an implicit copy; the
// templated BottomDetector::add_ping materializes the internal float buffer once.
template<typename t_float>
void add_detection_methods(nb::class_<BottomDetector>& cls)
{
    cls.def(
        "add_ping",
        [](BottomDetector&                           self,
           const xt::nanobind::pytensor<t_float, 1>& sv_db,
           float                                     range_offset,
           float                                     range_resolution,
           float                                     pulse_nsamples,
           float                                     beamwidth_deg) {
            self.add_ping(sv_db, range_offset, range_resolution, pulse_nsamples, beamwidth_deg);
        },
        "Add one ping of Sv data (dB) to the detector.\n\n"
        "Range geometry is the affine range = range_offset + range_resolution * sample. "
        "Call get_bottom() / get_bottom_backscatter() afterwards to run the cross-ping "
        "detection and read the detected track.",
        nb::arg("sv_db").noconvert(),
        nb::arg("range_offset"),
        nb::arg("range_resolution"),
        nb::arg("pulse_nsamples"),
        nb::arg("beamwidth_deg"));
}

void init_c_bottomdetector(nb::module_& m)
{
    auto cls = nb::class_<BottomDetector>(
        m,
        "BottomDetector",
        "Robust echogram bottom detector (ESP3 detec_bottom_algo_v4 port). "
        "Set the threshold/option fields (or pass them to the constructor as keyword "
        "arguments), add pings with add_ping, then read the detected track with "
        "get_bottom / get_bottom_backscatter.");

    cls.def(nb::init<float,
                     float,
                     float,
                     float,
                     float,
                     float,
                     float,
                     bool,
                     float,
                     int,
                     float,
                     bool,
                     int,
                     float,
                     bool,
                     int>(),
            nb::arg("thr_bottom")            = -35.0,
            nb::arg("thr_echo")              = -35.0,
            nb::arg("thr_backstep")          = -1.0,
            nb::arg("thr_cum_percent")       = 1.0,
            nb::arg("r_min")                 = 0.0,
            nb::arg("r_max")                 = std::numeric_limits<float>::infinity(),
            nb::arg("shift_bot_m")           = 0.0,
            nb::arg("denoised")              = true,
            nb::arg("incidence_angle_deg")   = 10.0,
            nb::arg("n_ping_smoothing")      = 5,
            nb::arg("mask_fill_fraction")    = 0.7,
            nb::arg("remove_outliers")       = false,
            nb::arg("outlier_window")        = 7,
            nb::arg("outlier_threshold")     = 3.0,
            nb::arg("interpolate_gaps")      = false,
            nb::arg("max_interpolation_gap") = 10,
            "Construct a BottomDetector. All parameters are keyword arguments with ESP3 "
            "defaults.")
        .def_rw("thr_bottom",
                &BottomDetector::thr_bottom,
                "Absolute floor (dB) on the strongest surface backscatter per ping.")
        .def_rw("thr_echo",
                &BottomDetector::thr_echo,
                "Candidate offset (dB) below the per-ping maximum backscatter.")
        .def_rw("thr_backstep",
                &BottomDetector::thr_backstep,
                "Back-step threshold (dB) for walking up the leading edge.")
        .def_rw("thr_cum_percent",
                &BottomDetector::thr_cum_percent,
                "Cumulative-energy threshold (percent, 0..100).")
        .def_rw("r_min", &BottomDetector::r_min, "Minimum range (m).")
        .def_rw("r_max", &BottomDetector::r_max, "Maximum range (m).")
        .def_rw("shift_bot_m",
                &BottomDetector::shift_bot_m,
                "Vertical shift of the detected bottom (m, positive = up).")
        .def_rw("denoised", &BottomDetector::denoised, "Whether the input Sv is denoised.")
        .def_rw("incidence_angle_deg",
                &BottomDetector::incidence_angle_deg,
                "Incidence angle (deg) for the echo-length filter.")
        .def_rw("n_ping_smoothing",
                &BottomDetector::n_ping_smoothing,
                "Neighbouring pings used for cross-ping mask smoothing (1 = off).")
        .def_rw("mask_fill_fraction",
                &BottomDetector::mask_fill_fraction,
                "Majority-filter fill fraction (0..1).")
        .def_rw("remove_outliers",
                &BottomDetector::remove_outliers,
                "Reject cross-ping outliers (robust MAD).")
        .def_rw("outlier_window",
                &BottomDetector::outlier_window,
                "Half-width (pings) of the outlier median window.")
        .def_rw("outlier_threshold",
                &BottomDetector::outlier_threshold,
                "Outlier threshold in multiples of the MAD.")
        .def_rw("interpolate_gaps",
                &BottomDetector::interpolate_gaps,
                "Interpolate the bottom across short invalid gaps.")
        .def_rw("max_interpolation_gap",
                &BottomDetector::max_interpolation_gap,
                "Maximum interpolated gap length (pings).")
        .def("__eq__", &BottomDetector::operator==, nb::arg("other"))
        // streaming result collectors (always float64 arrays)
        .def(
            "get_bottom",
            [](BottomDetector& self) {
                return xt::nanobind::pytensor<float, 1>(self.get_bottom());
            },
            "Detected bottom sample index per added ping (NaN where no bottom).")
        .def(
            "get_bottom_backscatter",
            [](BottomDetector& self) {
                return xt::nanobind::pytensor<float, 1>(self.get_bottom_backscatter());
            },
            "Surface backscatter (dB) at the detected bottom per added ping (NaN where none).")
        .def("reset",
             &BottomDetector::reset,
             "Clear all accumulated state so a new ping sequence can be started.")
        .def("__len__", [](const BottomDetector& self) { return self.size(); })
            __PYCLASS_DEFAULT_COPY__(BottomDetector) __PYCLASS_DEFAULT_PRINTING__(BottomDetector);

    add_detection_methods<float>(cls);
    add_detection_methods<double>(cls);
}

} // namespace py_echogramprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
