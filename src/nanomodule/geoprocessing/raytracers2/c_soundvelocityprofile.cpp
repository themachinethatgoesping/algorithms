// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/soundvelocityprofile.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/soundvelocityprofileresampling.hpp"

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
        .def("get_surface_sound_speed",
             &SoundVelocityProfile::get_surface_sound_speed,
             DOC_SoundVelocityProfile(get_surface_sound_speed))
        .def("set_surface_sound_speed",
             &SoundVelocityProfile::set_surface_sound_speed,
             nb::arg("surface_sound_speed"),
             DOC_SoundVelocityProfile(set_surface_sound_speed))
        .def("has_surface_sound_speed",
             &SoundVelocityProfile::has_surface_sound_speed,
             DOC_SoundVelocityProfile(has_surface_sound_speed))
        .def("get_profile_with_surface_sound_speed",
             &SoundVelocityProfile::get_profile_with_surface_sound_speed,
             nb::arg("surface_sound_speed_in_meters_per_second"),
             nb::arg("transducer_depth_in_meters"),
             DOC_SoundVelocityProfile(get_profile_with_surface_sound_speed))
        .def("get_resampled",
             &resample_soundvelocityprofile,
             nb::arg("max_sound_speed_error_in_meters_per_second") = std::nullopt,
             nb::arg("max_raytrace_error_in_meters")               = std::nullopt,
             nb::arg("number_of_entries")                          = std::nullopt,
             nb::arg("launch_depth_in_meters")                     = 0.f,
             nb::arg("max_depth_in_meters")                        = -1.f,
             nb::arg("max_launch_angle_in_degrees")                = 75.f,
             nb::arg("number_of_test_angles")                      = 8,
             "Return a copy of the profile with fewer depth/sound-speed knots so the raytracer "
             "traverses fewer layers (a direct speed-up) while keeping the raytracing error bounded.\n\n"
             "Set exactly ONE target:\n"
             "- max_sound_speed_error_in_meters_per_second: Douglas-Peucker on c(z); keeps the "
             "interpolated sound speed within this tolerance (self-contained, fastest).\n"
             "- max_raytrace_error_in_meters: ray-error greedy; guarantees the seabed position error "
             "(horizontal and depth) stays within this budget for launch angles up to "
             "max_launch_angle_in_degrees down to max_depth_in_meters (fewest layers for a given accuracy).\n"
             "- number_of_entries: simplify until about this many knots remain (ray-error ranking).\n\n"
             "max_depth_in_meters <= 0 uses the profile bottom. Metadata is preserved.")
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
