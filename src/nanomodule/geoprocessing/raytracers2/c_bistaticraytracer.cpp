// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/bistaticraytracer.hpp"

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>

#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define DOC_BistaticBeamTrace(ARG)                                                                 \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BistaticBeamTrace, ARG)

void init_c_bistaticraytracer(nb::module_& m)
{
    // ----- BistaticBeamTrace -----
    nb::class_<BistaticBeamTrace>(
        m,
        "BistaticBeamTrace",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BistaticBeamTrace))

        .def(nb::init<>(), DOC_BistaticBeamTrace(BistaticBeamTrace))
        .def(nb::init<BeamTrace,
                      BeamTrace,
                      float,
                      float,
                      std::array<float, 3>,
                      float>(),
             DOC_BistaticBeamTrace(BistaticBeamTrace_2),
             nb::arg("transmit_leg"),
             nb::arg("receive_leg"),
             nb::arg("transmit_azimuth_in_degrees"),
             nb::arg("receive_azimuth_in_degrees"),
             nb::arg("bottom_position"),
             nb::arg("solver_residual_in_meters"))
        .def("__eq__",
             &BistaticBeamTrace::operator==,
             DOC_BistaticBeamTrace(operator_eq),
             nb::arg("other"))

        // stored quantities
        .def("get_transmit_leg",
             &BistaticBeamTrace::get_transmit_leg,
             nb::rv_policy::reference_internal,
             DOC_BistaticBeamTrace(get_transmit_leg))
        .def("get_receive_leg",
             &BistaticBeamTrace::get_receive_leg,
             nb::rv_policy::reference_internal,
             DOC_BistaticBeamTrace(get_receive_leg))
        .def("get_transmit_azimuth_in_degrees",
             &BistaticBeamTrace::get_transmit_azimuth_in_degrees,
             DOC_BistaticBeamTrace(get_transmit_azimuth_in_degrees))
        .def("get_receive_azimuth_in_degrees",
             &BistaticBeamTrace::get_receive_azimuth_in_degrees,
             DOC_BistaticBeamTrace(get_receive_azimuth_in_degrees))
        .def("get_bottom_position",
             &BistaticBeamTrace::get_bottom_position,
             DOC_BistaticBeamTrace(get_bottom_position))
        .def("get_solver_residual_in_meters",
             &BistaticBeamTrace::get_solver_residual_in_meters,
             DOC_BistaticBeamTrace(get_solver_residual_in_meters))

        // derived quantities
        .def("get_transmit_launch_angle_in_degrees",
             &BistaticBeamTrace::get_transmit_launch_angle_in_degrees,
             DOC_BistaticBeamTrace(get_transmit_launch_angle_in_degrees))
        .def("get_receive_launch_angle_in_degrees",
             &BistaticBeamTrace::get_receive_launch_angle_in_degrees,
             DOC_BistaticBeamTrace(get_receive_launch_angle_in_degrees))
        .def("get_bottom_incidence_angle_in_degrees",
             &BistaticBeamTrace::get_bottom_incidence_angle_in_degrees,
             DOC_BistaticBeamTrace(get_bottom_incidence_angle_in_degrees))
        .def("get_two_way_travel_time_in_seconds",
             &BistaticBeamTrace::get_two_way_travel_time_in_seconds,
             DOC_BistaticBeamTrace(get_two_way_travel_time_in_seconds))

        // default copy/binary/printing
        __PYCLASS_DEFAULT_COPY__(BistaticBeamTrace)
        __PYCLASS_DEFAULT_BINARY__(BistaticBeamTrace)
        __PYCLASS_DEFAULT_PRINTING__(BistaticBeamTrace)
        ;

    m.def("trace_bistatic_beam",
          &trace_bistatic_beam,
          DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, trace_bistatic_beam),
          nb::arg("transmit_installation_ypr_in_degrees"),
          nb::arg("transmit_attitude_ypr_in_degrees"),
          nb::arg("transmit_steering_angle_in_degrees"),
          nb::arg("transmit_position_xyz"),
          nb::arg("receive_installation_ypr_in_degrees"),
          nb::arg("receive_attitude_ypr_in_degrees"),
          nb::arg("receive_steering_angle_in_degrees"),
          nb::arg("receive_position_xyz"),
          nb::arg("two_way_travel_time_in_seconds"),
          nb::arg("sound_velocity_profile"),
          nb::arg("concentric_beam_direction"),
          nb::arg("max_iterations")       = 30,
          nb::arg("tolerance_in_percent") = 0.001f,
          nb::arg("surface_sound_speed_in_meters_per_second") = -1.0,
          nb::arg("reference_heading_in_degrees")             = 0.0);
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
