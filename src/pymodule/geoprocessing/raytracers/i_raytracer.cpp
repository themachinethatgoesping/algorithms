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
#include <xtensor-python/pyarray.hpp> // Numpy bindings

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
        .def("trace_point",
             py::overload_cast<float, float, float>(&I_Raytracer::trace_point, py::const_),
             DOC_I_Raytracer(trace_point),
             py::arg("two_way_travel_time"),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"))
        .def("trace_points",
             py::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_points, py::const_),
             DOC_I_Raytracer(trace_points),
             py::arg("two_way_travel_times"),
             py::arg("alongtrack_angles"),
             py::arg("crosstrack_angles"),
             py::arg("mp_cores") = 1)
        .def("trace_points",
             py::overload_cast<const xt::xtensor<float, 1>&,
                               float,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_points, py::const_),
             DOC_I_Raytracer(trace_points_2),
             py::arg("two_way_travel_times"),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angles"),
             py::arg("mp_cores") = 1)
        .def("trace_beam",
             py::overload_cast<const xt::xtensor<unsigned int, 1>&, float, float, float, float>(
                 &I_Raytracer::trace_beam, py::const_),
             DOC_I_Raytracer(trace_beam),
             py::arg("sample_numbers"),
             py::arg("sampling_time"),
             py::arg("sampling_time_offset"),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"))
        .def(
            "trace_beam",
            py::overload_cast<unsigned int, unsigned int, unsigned int, float, float, float, float>(
                &I_Raytracer::trace_beam, py::const_),
            DOC_I_Raytracer(trace_beam_2),
            py::arg("first_sample_number"),
            py::arg("number_of_samples"),
            py::arg("sample_step"),
            py::arg("sampling_time"),
            py::arg("sampling_time_offset"),
            py::arg("alongtrack_angle"),
            py::arg("crosstrack_angle"))
        .def("trace_swath",
             py::overload_cast<const xt::xtensor<unsigned int, 2>&,
                               float,
                               float,
                               float,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_swath, py::const_),
             DOC_I_Raytracer(trace_swath),
             py::arg("sample_numbers"),
             py::arg("sampling_time"),
             py::arg("sampling_time_offset"),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angles"),
             py::arg("mp_cores") = 1)
        .def("trace_swath",
             py::overload_cast<unsigned int,
                               unsigned int,
                               unsigned int,
                               float,
                               float,
                               float,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_swath, py::const_),
             DOC_I_Raytracer(trace_swath_2),
             py::arg("first_sample_number"),
             py::arg("number_of_samples"),
             py::arg("sample_step"),
             py::arg("sampling_time"),
             py::arg("sampling_time_offset"),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angles"),
             py::arg("mp_cores") = 1)

        .def("set_sensor_location",
             &I_Raytracer::set_sensor_location,
             DOC_I_Raytracer(set_sensor_location),
             py::arg("sensor_location"))
        .def("get_sensor_location",
             &I_Raytracer::get_sensor_location,
             DOC_I_Raytracer(get_sensor_location))
        .def("get_sensor_orientation_quat_ypr",
             &I_Raytracer::get_sensor_orientation_quat_ypr,
             DOC_I_Raytracer(get_sensor_orientation_quat_ypr))

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