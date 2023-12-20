// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0


// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>



// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/rtconstantsvp.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pyarray.hpp>  // Numpy bindings
#include <xtensor-python/pytensor.hpp> // Numpy bindings

#include <xtensor/xadapt.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::algorithms::geoprocessing;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_RTConstantSVP(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP, ARG)

void init_c_rtconstantsvp(py::module& m)
{
    py::class_<RTConstantSVP, I_Raytracer>(
        m,
        "RTConstantSVP",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers, RTConstantSVP))
        .def(py::init<navigation::datastructures::Geolocation, float>(),
             DOC_RTConstantSVP(RTConstantSVP),
             py::arg("sensor_location"),
             py::arg("sound_velocity"))
        .def("__eq__", &RTConstantSVP::operator==, DOC_RTConstantSVP(operator_eq), py::arg("other"))

        .def("scale_beam",
             py::overload_cast<const xt::xtensor<unsigned int, 1>&,
                               float,
                               float,
                               float,
                               float,
                               float,
                               float,
                               float>(&RTConstantSVP::scale_beam, py::const_),
             DOC_RTConstantSVP(scale_beam),
             py::arg("sample_numbers"),
             py::arg("sampling_time"),
             py::arg("sampling_time_offset"),
             py::arg("scale_x"),
             py::arg("scale_y"),
             py::arg("scale_z"),
             py::arg("scale_true_range"),
             py::arg("scale_time"))
        .def("scale_swath",
             py::overload_cast<const xt::xtensor<unsigned int, 2>&,
                               float,
                               float,
                               const datastructures::RaytraceResults<1>&,
                               const xt::xtensor<float, 1>&,
                               unsigned int>(&RTConstantSVP::scale_swath, py::const_),
             DOC_RTConstantSVP(scale_swath),
             py::arg("sample_numbers"),
             py::arg("sampling_time"),
             py::arg("sampling_time_offset"),
             py::arg("scale_targets"),
             py::arg("scale_times"),
             py::arg("mp_cores") = 1)

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