// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamtrace.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/tracebeam.hpp"

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define DOC_BeamTrace(ARG) \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BeamTrace, ARG)

void init_c_beamtrace(nb::module_& m)
{
    nb::class_<BeamTrace>(
        m,
        "BeamTrace",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BeamTrace))

        .def(nb::init<>(),
             DOC_BeamTrace(BeamTrace))
        .def(nb::init<xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>>(),
             DOC_BeamTrace(BeamTrace_2),
             nb::arg("depths_in_meters"),
             nb::arg("horizontal_offsets_in_meters"),
             nb::arg("two_way_travel_times_in_seconds"),
             nb::arg("cos_incident_angles"))
        .def("__eq__", &BeamTrace::operator==, DOC_BeamTrace(operator_eq), nb::arg("other"))

        .def("set",
             &BeamTrace::set,
             DOC_BeamTrace(set),
             nb::arg("depths_in_meters"),
             nb::arg("horizontal_offsets_in_meters"),
             nb::arg("two_way_travel_times_in_seconds"),
             nb::arg("cos_incident_angles"))

        .def("get_number_of_points",
             &BeamTrace::get_number_of_points,
             DOC_BeamTrace(get_number_of_points))

        // stored quantities
        .def("get_depths_in_meters",
             &BeamTrace::get_depths_in_meters,
             nb::rv_policy::reference_internal,
             DOC_BeamTrace(get_depths_in_meters))
        .def("get_horizontal_offsets_in_meters",
             &BeamTrace::get_horizontal_offsets_in_meters,
             nb::rv_policy::reference_internal,
             DOC_BeamTrace(get_horizontal_offsets_in_meters))
        .def("get_two_way_travel_times_in_seconds",
             &BeamTrace::get_two_way_travel_times_in_seconds,
             nb::rv_policy::reference_internal,
             DOC_BeamTrace(get_two_way_travel_times_in_seconds))
        .def("get_cos_incident_angles",
             &BeamTrace::get_cos_incident_angles,
             nb::rv_policy::reference_internal,
             DOC_BeamTrace(get_cos_incident_angles))

        // derived quantities
        .def("get_incident_angles_in_degrees",
             &BeamTrace::get_incident_angles_in_degrees,
             DOC_BeamTrace(get_incident_angles_in_degrees))
        .def("get_ranges_in_meters",
             &BeamTrace::get_ranges_in_meters,
             DOC_BeamTrace(get_ranges_in_meters))

        // default copy/binary/printing
        __PYCLASS_DEFAULT_COPY__(BeamTrace)
        __PYCLASS_DEFAULT_BINARY__(BeamTrace)
        __PYCLASS_DEFAULT_PRINTING__(BeamTrace)
        ;

    m.def("trace_beam",
          &trace_beam,
          DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, trace_beam),
          nb::arg("launch_depth_in_meters"),
          nb::arg("launch_angle_in_degrees"),
          nb::arg("sound_velocity_profile"),
          nb::arg("two_way_travel_time_in_seconds"));
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
