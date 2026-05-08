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
using themachinethatgoesping::navigation::datastructures::PositionalOffsets;
using themachinethatgoesping::navigation::NavigationInterpolatorLatLon;

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

        .def("trace_to_xyz",
             [](const LayerRaytracer&                       self,
                const xt::nanobind::pytensor<float, 1>&     tilt_deg,
                const xt::nanobind::pytensor<float, 1>&     crosstrack_deg,
                const xt::nanobind::pytensor<float, 1>&     two_way_travel_times,
                const xt::nanobind::pytensor<float, 1>&     tx_delays,
                const PositionalOffsets&                    tx_mount,
                const PositionalOffsets&                    rx_mount,
                float                                       tx_face_depth_m,
                size_t                                      n_knots,
                const NavigationInterpolatorLatLon*         nav,
                double                                      t_tx_ping,
                int                                         mp_cores) {
                 return self.trace_to_xyz(tilt_deg,
                                          crosstrack_deg,
                                          two_way_travel_times,
                                          tx_delays,
                                          tx_mount,
                                          rx_mount,
                                          tx_face_depth_m,
                                          n_knots,
                                          nav,
                                          t_tx_ping,
                                          mp_cores);
             },
             "Trace beams using Kongsberg-native dual-array inputs.\n"
             "Output frame: TX-body axes (forward, starboard, down) at\n"
             "t_tx_ping, origin = TX transducer face at t_tx_ping. Apply\n"
             "BeamSampleGeometry::with_rigid_transform with the world TX-face\n"
             "pose at t_tx_ping to obtain world coordinates.\n"
             "\n"
             "tilt_deg:        [N] tilt re TX array, +forward (deg)\n"
             "crosstrack_deg:  [N] beam pointing re RX array, +starboard (deg)\n"
             "two_way_travel_times: [N] (s)\n"
             "tx_delays:       [N] per-beam sector TX delay re t_tx_ping (s)\n"
             "tx_mount, rx_mount: PositionalOffsets of the TX and RX arrays\n"
             "tx_face_depth_m: absolute world depth of TX face at t_tx_ping (m)\n"
             "n_knots:         number of trace knots (>=2). Knot k is at\n"
             "                 one-way time twtt[i]*k/(2*(n_knots-1));\n"
             "                 k = n_knots-1 is the bottom return.\n"
             "nav:             optional NavigationInterpolatorLatLon for\n"
             "                 motion compensation (sampled at t_tx_eff,\n"
             "                 t_rx_eff and t_tx_ping). Pass None to skip.\n"
             "t_tx_ping:       wall-clock time of the ping (s).\n"
             "Returns [n_knots, N, 3] xyz in TX-body-at-t_tx_ping; NaN where\n"
             "the ray turned or input was non-finite.",
             nb::arg("tilt_deg"),
             nb::arg("crosstrack_deg"),
             nb::arg("two_way_travel_times"),
             nb::arg("tx_delays"),
             nb::arg("tx_mount"),
             nb::arg("rx_mount"),
             nb::arg("tx_face_depth_m"),
             nb::arg("n_knots") = size_t(2),
             nb::arg("nav").none() = nb::none(),
             nb::arg("t_tx_ping") = 0.0,
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
