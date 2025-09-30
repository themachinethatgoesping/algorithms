// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/i_raytracer.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_I_Raytracer(ARG)                                                                       \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, I_Raytracer, ARG)

void init_c_i_raytracer(nb::module_& m)
{
    nb::class_<I_Raytracer>(
        m,
        "I_Raytracer",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, I_Raytracer))
        .def(nb::init<navigation::datastructures::Geolocation, std::string>(),
             DOC_I_Raytracer(I_Raytracer),
             nb::arg("sensor_location"),
             nb::arg("raytracer_name"))
        .def("__eq__", &I_Raytracer::operator==, DOC_I_Raytracer(operator_eq), nb::arg("other"))
        .def("trace_point",
             nb::overload_cast<float, float, float>(&I_Raytracer::trace_point, nb::const_),
             DOC_I_Raytracer(trace_point),
             nb::arg("two_way_travel_time"),
             nb::arg("alongtrack_angle"),
             nb::arg("crosstrack_angle"))
        .def("trace_points",
             nb::overload_cast<const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_points, nb::const_),
             DOC_I_Raytracer(trace_points),
             nb::arg("two_way_travel_times"),
             nb::arg("alongtrack_angles"),
             nb::arg("crosstrack_angles"),
             nb::arg("mp_cores") = 1)
        .def("trace_points",
             nb::overload_cast<const SampleDirectionsTime<1>&, unsigned int>(
                 &I_Raytracer::trace_points, nb::const_),
             DOC_I_Raytracer(trace_points_2),
             nb::arg("sample_directions"),
             nb::arg("mp_cores") = 1)
        .def("trace_beam",
             nb::overload_cast<const xt::xtensor<unsigned int, 1>&, float, float, float, float>(
                 &I_Raytracer::trace_beam, nb::const_),
             DOC_I_Raytracer(trace_beam),
             nb::arg("sample_numbers"),
             nb::arg("sampling_time"),
             nb::arg("sampling_time_offset"),
             nb::arg("alongtrack_angle"),
             nb::arg("crosstrack_angle"))
        .def(
            "trace_beam",
            nb::overload_cast<unsigned int, unsigned int, unsigned int, float, float, float, float>(
                &I_Raytracer::trace_beam, nb::const_),
            DOC_I_Raytracer(trace_beam_2),
            nb::arg("first_sample_number"),
            nb::arg("number_of_samples"),
            nb::arg("sample_step"),
            nb::arg("sampling_time"),
            nb::arg("sampling_time_offset"),
            nb::arg("alongtrack_angle"),
            nb::arg("crosstrack_angle"))
        .def("trace_swath",
             nb::overload_cast<const xt::xtensor<unsigned int, 2>&,
                               float,
                               float,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_swath, nb::const_),
             DOC_I_Raytracer(trace_swath),
             nb::arg("sample_numbers"),
             nb::arg("sampling_time"),
             nb::arg("sampling_time_offset"),
             nb::arg("alongtrack_angle"),
             nb::arg("crosstrack_angles"),
             nb::arg("mp_cores") = 1)
        .def("trace_swath",
             nb::overload_cast<unsigned int,
                               unsigned int,
                               unsigned int,
                               float,
                               float,
                               const xt::xtensor<float, 1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&I_Raytracer::trace_swath, nb::const_),
             DOC_I_Raytracer(trace_swath_2),
             nb::arg("first_sample_number"),
             nb::arg("number_of_samples"),
             nb::arg("sample_step"),
             nb::arg("sampling_time"),
             nb::arg("sampling_time_offset"),
             nb::arg("alongtrack_angles"),
             nb::arg("crosstrack_angles"),
             nb::arg("mp_cores") = 1)

        .def("set_sensor_location",
             &I_Raytracer::set_sensor_location,
             DOC_I_Raytracer(set_sensor_location),
             nb::arg("sensor_location"))
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