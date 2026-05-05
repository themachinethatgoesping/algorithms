// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/soundvelocityprofile.hpp"

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

void init_c_soundvelocityprofile(nb::module_& m)
{
    nb::class_<SoundVelocityProfile>(
        m,
        "SoundVelocityProfile",
        "1-D depth-dependent sound velocity profile with precomputed layer constants.")

        .def(nb::init<>(), "Construct an empty SoundVelocityProfile")
        .def(nb::init<xt::xtensor<float, 1>, xt::xtensor<float, 1>>(),
             "Construct from depth (m) and sound speed (m/s) arrays.",
             nb::arg("z"),
             nb::arg("c"))
        .def("__eq__", &SoundVelocityProfile::operator==, nb::arg("other"))

        .def_static("uniform",
                    &SoundVelocityProfile::uniform,
                    "Convenience: constant SVP from surface to z_max.",
                    nb::arg("c"),
                    nb::arg("z_max") = 12000.f)

        .def("set",
             &SoundVelocityProfile::set,
             "Set depth/sound-speed tables and recompute layer constants.",
             nb::arg("z"),
             nb::arg("c"))

        .def("get_z", &SoundVelocityProfile::get_z, nb::rv_policy::reference_internal)
        .def("get_c", &SoundVelocityProfile::get_c, nb::rv_policy::reference_internal)
        .def("get_g", &SoundVelocityProfile::get_g, nb::rv_policy::reference_internal)
        .def("get_n_layers", &SoundVelocityProfile::get_n_layers)
        .def("get_sound_speed",
             &SoundVelocityProfile::get_sound_speed,
             "Sound speed at depth z (linear interp; clamped at endpoints).",
             nb::arg("z"))

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
