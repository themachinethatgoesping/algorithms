// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/i_raytracer.hpp"
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

#define DOC_I_Raytracer(ARG)                                                                       \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, I_Raytracer, ARG)

void init_i_raytracer(py::module& m)
{
    py::class_<I_Raytracer>(
        m,
        "I_Raytracer",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, I_Raytracer))
        .def(py::init<navigation::datastructures::GeoLocation, std::string>(),
             DOC_I_Raytracer(I_Raytracer),
             py::arg("sensor_location"),
             py::arg("raytracer_name"))
        .def("__eq__", &I_Raytracer::operator==, DOC_I_Raytracer(operator_eq), py::arg("other"))

        .def("set_sensor_location",
             &I_Raytracer::set_sensor_location,
             DOC_I_Raytracer(set_sensor_location),
             py::arg("sensor_location"))
        .def("get_sensor_location",
             &I_Raytracer::get_sensor_location,
             DOC_I_Raytracer(get_sensor_location))
        .def("get_sensor_orientation_local_ypr",
             &I_Raytracer::get_sensor_orientation_local_ypr,
             DOC_I_Raytracer(get_sensor_orientation_local_ypr))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(I_Raytracer)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(I_Raytracer)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(I_Raytracer)
        // end I_Raytracer
        ;
}

} // namespace py_raytracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping