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

void init_c_soundvelocityprofile(nb::module_& m)
{
    nb::class_<SoundVelocityProfile>(
        m,
        "SoundVelocityProfile",
        "1-D depth-dependent sound velocity profile with precomputed layer constants.")

        .def(nb::init<>(), "Construct an empty SoundVelocityProfile")
        .def(nb::init<xt::xtensor<float, 1>, xt::xtensor<float, 1>>(),
             "Construct from depth (m) and sound speed (m/s) arrays.",
             nb::arg("depths_in_meters"),
             nb::arg("sound_speeds_in_meters_per_second"))
        .def("__eq__", &SoundVelocityProfile::operator==, nb::arg("other"))

        .def_static("uniform",
                    &SoundVelocityProfile::uniform,
                    "Convenience: constant SVP from surface to z_max.",
                    nb::arg("c"),
                    nb::arg("z_max") = 12000.f)

        .def("set",
             &SoundVelocityProfile::set,
             "Set depth/sound-speed tables and recompute layer constants.",
             nb::arg("depths_in_meters"),
             nb::arg("sound_speeds_in_meters_per_second"))

        // primary table accessors
        .def("get_depths_in_meters",
             &SoundVelocityProfile::get_depths_in_meters,
             nb::rv_policy::reference_internal,
             "All depth knots (m), absolute coordinates.")
        .def("get_sound_speeds_in_meters_per_second",
             &SoundVelocityProfile::get_sound_speeds_in_meters_per_second,
             nb::rv_policy::reference_internal,
             "All sound speeds (m/s), one per depth knot.")
        .def("get_sound_speed_gradients_in_per_second",
             &SoundVelocityProfile::get_sound_speed_gradients_in_per_second,
             nb::rv_policy::reference_internal,
             "Sound-speed gradient dc/dz (s⁻¹) per layer (size = number_of_layers).")
        .def("get_inverse_sound_speed_gradients_in_seconds",
             &SoundVelocityProfile::get_inverse_sound_speed_gradients_in_seconds,
             nb::rv_policy::reference_internal,
             "1/gradient (s) per layer; 0 for iso-velocity layers.")
        .def("get_isovelocity_flags",
             &SoundVelocityProfile::get_isovelocity_flags,
             nb::rv_policy::reference_internal,
             "Per-layer iso-velocity flag (true when |gradient| < ISO_EPS).")
        .def("get_number_of_layers", &SoundVelocityProfile::get_number_of_layers,
             "Number of layers (= number of knots - 1).")
        .def("get_sound_speed",
             &SoundVelocityProfile::get_sound_speed,
             "Sound speed at depth z (linear interp; clamped at endpoints).",
             nb::arg("depth_in_meters"))

        // per-knot accessors
        .def("get_depth_in_meters",
             &SoundVelocityProfile::get_depth_in_meters,
             "Depth (m) at the given knot index.",
             nb::arg("index"))
        .def("get_sound_speed_in_meters_per_second",
             &SoundVelocityProfile::get_sound_speed_in_meters_per_second,
             "Sound speed (m/s) at the given knot index.",
             nb::arg("index"))
        .def("get_number_of_entries", &SoundVelocityProfile::get_number_of_entries,
             "Number of (depth, sound speed) entries (= number_of_layers + 1).")

        // optional metadata
        .def("get_timestamp",
             &SoundVelocityProfile::get_timestamp,
             "Unix timestamp (seconds since epoch, UTC), or None if not set.")
        .def("set_timestamp",
             &SoundVelocityProfile::set_timestamp,
             nb::arg("timestamp"),
             "Set the unix timestamp (or None to clear).")
        .def("has_timestamp", &SoundVelocityProfile::has_timestamp)
        .def("get_latitude",
             &SoundVelocityProfile::get_latitude,
             "Latitude (decimal degrees, +N), or None if not set.")
        .def("set_latitude",
             &SoundVelocityProfile::set_latitude,
             nb::arg("latitude"),
             "Set latitude (or None to clear).")
        .def("get_longitude",
             &SoundVelocityProfile::get_longitude,
             "Longitude (decimal degrees, +E), or None if not set.")
        .def("set_longitude",
             &SoundVelocityProfile::set_longitude,
             nb::arg("longitude"),
             "Set longitude (or None to clear).")
        .def("set_location",
             &SoundVelocityProfile::set_location,
             nb::arg("latitude"),
             nb::arg("longitude"),
             "Set both latitude and longitude at once.")
        .def("has_location", &SoundVelocityProfile::has_location)
        .def("get_date_string",
             &SoundVelocityProfile::get_date_string,
             nb::arg("fractionalSecondsDigits") = 2,
             nb::arg("format")                  = "%z__%d-%m-%Y__%H:%M:%S",
             "Format the timestamp as a date string.")

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
