// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0


// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>



// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/rtconstantsvp.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <xtensor/containers/xadapt.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::algorithms::geoprocessing;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_RTConstantSVP(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP, ARG)

void init_c_rtconstantsvp(nb::module_& m)
{
    nb::class_<RTConstantSVP, I_Raytracer>(
        m,
        "RTConstantSVP",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP))
        .def(nb::init<navigation::datastructures::Geolocation, float>(),
             DOC_RTConstantSVP(RTConstantSVP),
             nb::arg("sensor_location"),
             nb::arg("sound_velocity"))
        .def("__eq__", &RTConstantSVP::operator==, DOC_RTConstantSVP(operator_eq), nb::arg("other"))

        .def("scale_beam",
             nb::overload_cast<const xt::xtensor<unsigned int, 1>&,
                               float,
                               float,
                               float,
                               float,
                               float,
                               float,
                               float>(&RTConstantSVP::scale_beam, nb::const_),
             DOC_RTConstantSVP(scale_beam),
             nb::arg("sample_numbers"),
             nb::arg("sampling_time"),
             nb::arg("sampling_time_offset"),
             nb::arg("scale_x"),
             nb::arg("scale_y"),
             nb::arg("scale_z"),
             nb::arg("scale_true_range"),
             nb::arg("scale_time"))
        .def("scale_swath",
             nb::overload_cast<const xt::xtensor<unsigned int, 2>&,
                               float,
                               float,
                               const datastructures::RaytraceResults<1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&RTConstantSVP::scale_swath, nb::const_),
             DOC_RTConstantSVP(scale_swath),
             nb::arg("sample_numbers"),
             nb::arg("sampling_time"),
             nb::arg("sampling_time_offset"),
             nb::arg("scale_targets"),
             nb::arg("scale_times"),
             nb::arg("mp_cores") = 1)

     // default copy functions
     __PYCLASS_DEFAULT_COPY__(RTConstantSVP)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(RTConstantSVP)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(RTConstantSVP)
        // end RTConstantSVP
        ;
}

} // namespace py_raytracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping