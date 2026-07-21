// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamdirections.hpp"

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

#define DOC_BeamDirections(ARG)                                                                    \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BeamDirections, ARG)

void init_c_beamdirections(nb::module_& m)
{
    nb::class_<BeamDirections>(
        m,
        "BeamDirections",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, BeamDirections))

        .def(nb::init<>(), DOC_BeamDirections(BeamDirections))
        .def(nb::init<xt::xtensor<float, 2>>(),
             DOC_BeamDirections(BeamDirections_2),
             nb::arg("directions"))
        .def("__eq__", &BeamDirections::operator==, DOC_BeamDirections(operator_eq), nb::arg("other"))

        .def("set", &BeamDirections::set, DOC_BeamDirections(set), nb::arg("directions"))

        .def("get_number_of_beams",
             &BeamDirections::get_number_of_beams,
             DOC_BeamDirections(get_number_of_beams))
        .def("get_directions",
             &BeamDirections::get_directions,
             nb::rv_policy::reference_internal,
             DOC_BeamDirections(get_directions))
        .def("get_beam_direction",
             &BeamDirections::get_beam_direction,
             DOC_BeamDirections(get_beam_direction),
             nb::arg("beam_index"))

        .def("get_beam_pointing_angles_in_degrees",
             &BeamDirections::get_beam_pointing_angles_in_degrees,
             DOC_BeamDirections(get_beam_pointing_angles_in_degrees))
        .def("get_beam_azimuth_angles_in_degrees",
             &BeamDirections::get_beam_azimuth_angles_in_degrees,
             DOC_BeamDirections(get_beam_azimuth_angles_in_degrees))
        .def("get_beam_takeoff_angles_in_degrees",
             &BeamDirections::get_beam_takeoff_angles_in_degrees,
             DOC_BeamDirections(get_beam_takeoff_angles_in_degrees))

        // default copy/binary/printing
        __PYCLASS_DEFAULT_COPY__(BeamDirections)
        __PYCLASS_DEFAULT_BINARY__(BeamDirections)
        __PYCLASS_DEFAULT_PRINTING__(BeamDirections)
        ;

    m.def("compute_beam_directions",
          &compute_beam_directions,
          DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, compute_beam_directions),
          nb::arg("transmit_installation_ypr_in_degrees"),
          nb::arg("receive_installation_ypr_in_degrees"),
          nb::arg("transmit_attitude_ypr_in_degrees"),
          nb::arg("receive_attitude_ypr_in_degrees"),
          nb::arg("transmit_steering_angles_in_degrees"),
          nb::arg("receive_steering_angles_in_degrees"),
          nb::arg("reference_heading_in_degrees"),
          nb::arg("mp_cores") = 1);

    m.def("beam_direction_to_pointing_and_azimuth_in_degrees",
          &beam_direction_to_pointing_and_azimuth_in_degrees,
          DOC(themachinethatgoesping,
              algorithms,
              geoprocessing,
              raytracers2,
              beam_direction_to_pointing_and_azimuth_in_degrees),
          nb::arg("forward"),
          nb::arg("starboard"),
          nb::arg("down"));

    m.def("correct_steering_angles_for_surface_sound_speed",
          &correct_steering_angles_for_surface_sound_speed,
          DOC(themachinethatgoesping,
              algorithms,
              geoprocessing,
              raytracers2,
              correct_steering_angles_for_surface_sound_speed),
          nb::arg("steering_angles_in_degrees"),
          nb::arg("surface_sound_speed_used_in_meters_per_second"),
          nb::arg("surface_sound_speed_corrected_in_meters_per_second"));

    m.def("correct_steering_angle_for_surface_sound_speed",
          &correct_steering_angle_for_surface_sound_speed,
          DOC(themachinethatgoesping,
              algorithms,
              geoprocessing,
              raytracers2,
              correct_steering_angle_for_surface_sound_speed),
          nb::arg("steering_angle_in_degrees"),
          nb::arg("surface_sound_speed_used_in_meters_per_second"),
          nb::arg("surface_sound_speed_corrected_in_meters_per_second"));
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
