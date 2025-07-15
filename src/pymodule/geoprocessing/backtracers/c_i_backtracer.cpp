// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/i_backtracer.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_I_Backtracer(ARG)                                                                      \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, I_Backtracer, ARG)

void init_i_backtracer(py::module& m)
{
    py::class_<I_Backtracer>(
        m,
        "I_Backtracer",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, I_Backtracer))
        .def(py::init<navigation::datastructures::Geolocation, float, float, std::string>(),
             DOC_I_Backtracer(I_Backtracer),
             py::arg("sensor_location"),
             py::arg("sensor_x"),
             py::arg("sensor_y"),
             py::arg("backtracer_name"))
        .def("__eq__", &I_Backtracer::operator==, DOC_I_Backtracer(operator_eq), py::arg("other"))
        .def("backtrace_points",
             py::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Backtracer::backtrace_points, py::const_),
             DOC_I_Backtracer(backtrace_points),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("mp_cores") = 1)
        .def("backtrace_points",
             py::overload_cast<const XYZ<1>&, unsigned int>(&I_Backtracer::backtrace_points,
                                                            py::const_),
             DOC_I_Backtracer(backtrace_points),
             py::arg("xyz"),
             py::arg("mp_cores") = 1)
        .def("backtrace_image",
             py::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Backtracer::backtrace_image, py::const_),
             DOC_I_Backtracer(backtrace_image),
             py::arg("y_coordinates"),
             py::arg("z_coordinates"),
             py::arg("mp_cores") = 1)

        .def("lookup",
             &I_Backtracer::lookup,
             DOC_I_Backtracer(lookup),
             py::arg("wci"),
             py::arg("beam_reference_directions"),
             py::arg("beam_reference_sample_numbers"),
             py::arg("target_directions"),
             py::arg("wci_first_sample_number"),
             py::arg("wci_sample_number_step") = 1,
             py::arg("mp_cores")               = 1)

        .def("set_sensor_location",
             &I_Backtracer::set_sensor_location,
             DOC_I_Backtracer(set_sensor_location),
             py::arg("sensor_location"),
             py::arg("sensor_x"),
             py::arg("sensor_y"))
        .def("get_sensor_x", &I_Backtracer::get_sensor_x, DOC_I_Backtracer(get_sensor_x))
        .def("get_sensor_y", &I_Backtracer::get_sensor_y, DOC_I_Backtracer(get_sensor_y))
        .def("get_sensor_location",
             &I_Backtracer::get_sensor_location,
             DOC_I_Backtracer(get_sensor_location))
        .def("get_sensor_orientation_quat_ypr",
             &I_Backtracer::get_sensor_orientation_quat_ypr,
             DOC_I_Backtracer(get_sensor_orientation_quat_ypr))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(I_Backtracer)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(I_Backtracer)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(I_Backtracer)
        // end I_Backtracer
        ;
}

} // namespace py_backtracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping