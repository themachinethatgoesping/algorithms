// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/layerraytracer.hpp"

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;
using themachinethatgoesping::navigation::datastructures::Geolocation;

void init_c_layerraytracer(nb::module_& m)
{
    nb::class_<LayerRaytracer>(
        m,
        "LayerRaytracer",
        "Closed-form Snell raytracer through a 1-D layered SVP. "
        "Returns world-frame [K+1, n_beams, 3] xyz at user-supplied one-way "
        "travel-time knots, anchored exactly to each knot.")

        .def(nb::init<>(), "Construct an empty LayerRaytracer (SVP must be set before tracing)")
        .def(nb::init<SoundVelocityProfile>(),
             "Construct a LayerRaytracer with the given SoundVelocityProfile.",
             nb::arg("svp"))
        .def("__eq__", &LayerRaytracer::operator==, nb::arg("other"))

        .def("get_svp", &LayerRaytracer::get_svp, nb::rv_policy::reference_internal)
        .def("set_svp", &LayerRaytracer::set_svp, nb::arg("svp"))

        .def("trace_at_times",
             [](const LayerRaytracer&                       self,
                const xt::nanobind::pytensor<float, 2>&     launch_dirs,
                const xt::nanobind::pytensor<float, 1>&     knot_times,
                const std::vector<Geolocation>&             tx_poses,
                const std::vector<Geolocation>&             rx_poses,
                int                                         mp_cores) {
                 return self.trace_at_times(launch_dirs, knot_times, tx_poses, rx_poses, mp_cores);
             },
             "Trace beams to one-way travel-time knots using separate TX/RX poses.\n"
             "launch_dirs: [n_beams, 3] vehicle-frame unit vectors (forward, starboard, down).\n"
             "knot_times:  [K+1] one-way travel times (s), monotone, [0] >= 0.\n"
             "tx_poses, rx_poses: length K+1 lists of Geolocation (one pose per knot).\n"
             "Returns [K+1, n_beams, 3] world-frame xyz (NaN where ray turned/exited).",
             nb::arg("launch_dirs"),
             nb::arg("knot_times"),
             nb::arg("tx_poses"),
             nb::arg("rx_poses"),
             nb::arg("mp_cores") = 1)

        .def("trace_at_times",
             [](const LayerRaytracer&                       self,
                const xt::nanobind::pytensor<float, 2>&     launch_dirs,
                const xt::nanobind::pytensor<float, 1>&     knot_times,
                const std::vector<Geolocation>&             poses,
                int                                         mp_cores) {
                 return self.trace_at_times(launch_dirs, knot_times, poses, mp_cores);
             },
             "Trace beams to one-way travel-time knots using a single per-knot pose "
             "(equivalent to passing tx_poses == rx_poses).",
             nb::arg("launch_dirs"),
             nb::arg("knot_times"),
             nb::arg("poses"),
             nb::arg("mp_cores") = 1)

        .def_static("launch_dirs_from_angles",
             [](const xt::nanobind::pytensor<float, 1>& tilt_deg,
                const xt::nanobind::pytensor<float, 1>& crosstrack_deg) {
                 return LayerRaytracer::launch_dirs_from_angles(tilt_deg, crosstrack_deg);
             },
             "Convert per-beam (tilt_deg, crosstrack_deg) into [n_beams, 3] vehicle-frame "
             "unit launch directions (forward, starboard, down). "
             "tilt: positive forward; crosstrack: positive starboard.",
             nb::arg("tilt_deg"),
             nb::arg("crosstrack_deg"))

        .def("trace_at_angles",
             [](const LayerRaytracer&                       self,
                const xt::nanobind::pytensor<float, 1>&     tilt_deg,
                const xt::nanobind::pytensor<float, 1>&     crosstrack_deg,
                const xt::nanobind::pytensor<float, 1>&     knot_times,
                const std::vector<Geolocation>&             tx_poses,
                const std::vector<Geolocation>&             rx_poses,
                int                                         mp_cores) {
                 return self.trace_at_angles(
                     tilt_deg, crosstrack_deg, knot_times, tx_poses, rx_poses, mp_cores);
             },
             "Trace beams given per-beam (tilt_deg, crosstrack_deg) and TX/RX poses.\n"
             "tilt_deg, crosstrack_deg: [n_beams] floats. tilt positive forward, "
             "crosstrack positive starboard.",
             nb::arg("tilt_deg"),
             nb::arg("crosstrack_deg"),
             nb::arg("knot_times"),
             nb::arg("tx_poses"),
             nb::arg("rx_poses"),
             nb::arg("mp_cores") = 1)

        .def("trace_at_angles",
             [](const LayerRaytracer&                       self,
                const xt::nanobind::pytensor<float, 1>&     tilt_deg,
                const xt::nanobind::pytensor<float, 1>&     crosstrack_deg,
                const xt::nanobind::pytensor<float, 1>&     knot_times,
                const std::vector<Geolocation>&             poses,
                int                                         mp_cores) {
                 return self.trace_at_angles(
                     tilt_deg, crosstrack_deg, knot_times, poses, mp_cores);
             },
             "Same as trace_at_angles(tx_poses, rx_poses) but with a single per-knot pose.",
             nb::arg("tilt_deg"),
             nb::arg("crosstrack_deg"),
             nb::arg("knot_times"),
             nb::arg("poses"),
             nb::arg("mp_cores") = 1)

        // default copy/binary/printing
        __PYCLASS_DEFAULT_COPY__(LayerRaytracer)
        __PYCLASS_DEFAULT_BINARY__(LayerRaytracer)
        __PYCLASS_DEFAULT_PRINTING__(LayerRaytracer)
        ;
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
