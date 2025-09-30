// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/i_backtracer.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_I_Backtracer(ARG)                                                                      \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, I_Backtracer, ARG)

void init_i_backtracer(nb::module_& m)
{
    nb::class_<I_Backtracer>(
        m,
        "I_Backtracer",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, I_Backtracer))
        .def(nb::init<navigation::datastructures::Geolocation, float, float, std::string>(),
             DOC_I_Backtracer(I_Backtracer),
             nb::arg("sensor_location"),
             nb::arg("sensor_x"),
             nb::arg("sensor_y"),
             nb::arg("backtracer_name"))
        .def("__eq__", &I_Backtracer::operator==, DOC_I_Backtracer(operator_eq), nb::arg("other"))
        .def("backtrace_points",
             nb::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Backtracer::backtrace_points, nb::const_),
             DOC_I_Backtracer(backtrace_points),
             nb::arg("x"),
             nb::arg("y"),
             nb::arg("z"),
             nb::arg("mp_cores") = 1)
        .def("backtrace_points",
             nb::overload_cast<const XYZ<1>&, unsigned int>(&I_Backtracer::backtrace_points,
                                                            nb::const_),
             DOC_I_Backtracer(backtrace_points),
             nb::arg("xyz"),
             nb::arg("mp_cores") = 1)
        .def("backtrace_image",
             nb::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Backtracer::backtrace_image, nb::const_),
             DOC_I_Backtracer(backtrace_image),
             nb::arg("y_coordinates"),
             nb::arg("z_coordinates"),
             nb::arg("mp_cores") = 1)

        .def("lookup",
             &I_Backtracer::lookup,
             DOC_I_Backtracer(lookup),
             nb::arg("wci"),
             nb::arg("beam_reference_directions"),
             nb::arg("beam_reference_sample_numbers"),
             nb::arg("target_directions"),
             nb::arg("wci_first_sample_number"),
             nb::arg("wci_sample_number_step") = 1,
             nb::arg("mp_cores")               = 1)

        .def("set_sensor_location",
             &I_Backtracer::set_sensor_location,
             DOC_I_Backtracer(set_sensor_location),
             nb::arg("sensor_location"),
             nb::arg("sensor_x"),
             nb::arg("sensor_y"))
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