// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/soundvelocityprofile.hpp"

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>

#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define DOC_SoundVelocityProfile(ARG) \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, SoundVelocityProfile, ARG)

void init_c_soundvelocityprofile(nb::module_& m)
{
    nb::class_<SoundVelocityProfile>(
        m,
        "SoundVelocityProfile",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, SoundVelocityProfile))

        .def(nb::init<>(),
             DOC_SoundVelocityProfile(SoundVelocityProfile))
        .def(nb::init<xt::xtensor<float, 1>, xt::xtensor<float, 1>>(),
             DOC_SoundVelocityProfile(SoundVelocityProfile_2),
             nb::arg("depths_in_meters"),
             nb::arg("sound_speeds_in_meters_per_second"))
        .def("__eq__", &SoundVelocityProfile::operator==,
             DOC_SoundVelocityProfile(operator_eq), nb::arg("other"))

        .def_static("uniform",
                    &SoundVelocityProfile::uniform,
                    DOC_SoundVelocityProfile(uniform),
                    nb::arg("c"),
                    nb::arg("z_max") = 12000.f)

        .def("set",
             &SoundVelocityProfile::set,
             DOC_SoundVelocityProfile(set),
             nb::arg("depths_in_meters"),
             nb::arg("sound_speeds_in_meters_per_second"))

        // primary table accessors
        .def("get_depths_in_meters",
             &SoundVelocityProfile::get_depths_in_meters,
             nb::rv_policy::reference_internal,
             DOC_SoundVelocityProfile(get_depths_in_meters))
        .def("get_sound_speeds_in_meters_per_second",
             &SoundVelocityProfile::get_sound_speeds_in_meters_per_second,
             nb::rv_policy::reference_internal,
             DOC_SoundVelocityProfile(get_sound_speeds_in_meters_per_second))
        .def("get_sound_speed_gradients_in_per_second",
             &SoundVelocityProfile::get_sound_speed_gradients_in_per_second,
             nb::rv_policy::reference_internal,
             DOC_SoundVelocityProfile(get_sound_speed_gradients_in_per_second))
        .def("get_inverse_sound_speed_gradients_in_seconds",
             &SoundVelocityProfile::get_inverse_sound_speed_gradients_in_seconds,
             nb::rv_policy::reference_internal,
             DOC_SoundVelocityProfile(get_inverse_sound_speed_gradients_in_seconds))
        .def("get_isovelocity_flags",
             &SoundVelocityProfile::get_isovelocity_flags,
             nb::rv_policy::reference_internal,
             DOC_SoundVelocityProfile(get_isovelocity_flags))
        .def("get_number_of_layers",
             &SoundVelocityProfile::get_number_of_layers,
             DOC_SoundVelocityProfile(get_number_of_layers))
        .def("get_sound_speed",
             &SoundVelocityProfile::get_sound_speed,
             DOC_SoundVelocityProfile(get_sound_speed),
             nb::arg("depth_in_meters"))

        // per-knot accessors
        .def("get_depth_in_meters",
             &SoundVelocityProfile::get_depth_in_meters,
             DOC_SoundVelocityProfile(get_depth_in_meters),
             nb::arg("index"))
        .def("get_sound_speed_in_meters_per_second",
             &SoundVelocityProfile::get_sound_speed_in_meters_per_second,
             DOC_SoundVelocityProfile(get_sound_speed_in_meters_per_second),
             nb::arg("index"))
        .def("get_number_of_entries",
             &SoundVelocityProfile::get_number_of_entries,
             DOC_SoundVelocityProfile(get_number_of_entries))

        // optional metadata
        .def("get_timestamp",
             &SoundVelocityProfile::get_timestamp,
             DOC_SoundVelocityProfile(get_timestamp))
        .def("set_timestamp",
             &SoundVelocityProfile::set_timestamp,
             nb::arg("timestamp"),
             DOC_SoundVelocityProfile(set_timestamp))
        .def("has_timestamp",
             &SoundVelocityProfile::has_timestamp,
             DOC_SoundVelocityProfile(has_timestamp))
        .def("get_latitude",
             &SoundVelocityProfile::get_latitude,
             DOC_SoundVelocityProfile(get_latitude))
        .def("set_latitude",
             &SoundVelocityProfile::set_latitude,
             nb::arg("latitude"),
             DOC_SoundVelocityProfile(set_latitude))
        .def("get_longitude",
             &SoundVelocityProfile::get_longitude,
             DOC_SoundVelocityProfile(get_longitude))
        .def("set_longitude",
             &SoundVelocityProfile::set_longitude,
             nb::arg("longitude"),
             DOC_SoundVelocityProfile(set_longitude))
        .def("set_location",
             &SoundVelocityProfile::set_location,
             nb::arg("latitude"),
             nb::arg("longitude"),
             DOC_SoundVelocityProfile(set_location))
        .def("has_location",
             &SoundVelocityProfile::has_location,
             DOC_SoundVelocityProfile(has_location))
        .def("get_date_string",
             &SoundVelocityProfile::get_date_string,
             nb::arg("fractionalSecondsDigits") = 2,
             nb::arg("format")                  = "%z__%d-%m-%Y__%H:%M:%S",
             DOC_SoundVelocityProfile(get_date_string))

        // default copy/binary/printing
        __PYCLASS_DEFAULT_COPY__(SoundVelocityProfile)
        __PYCLASS_DEFAULT_BINARY__(SoundVelocityProfile)
        __PYCLASS_DEFAULT_PRINTING__(SoundVelocityProfile)
        ;
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
