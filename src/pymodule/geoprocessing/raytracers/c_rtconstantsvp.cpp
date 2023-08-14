// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/rtconstantsvp.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_RTConstantSVP(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP, ARG)

void init_c_rtconstantsvp(py::module& m)
{
    py::class_<RTConstantSVP, I_Raytracer>(
        m,
        "RTConstantSVP",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP))
        .def(py::init<navigation::datastructures::GeoLocation, float>(),
             DOC_RTConstantSVP(RTConstantSVP),
             py::arg("sensor_location"),
             py::arg("sound_velocity"))
        .def("__eq__", &RTConstantSVP::operator==, DOC_RTConstantSVP(operator_eq), py::arg("other"))

        .def("trace",
             py::overload_cast<float, float, float>(&RTConstantSVP::trace, py::const_),
             DOC_RTConstantSVP(trace),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"),
             py::arg("two_way_travel_time"))

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