// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/btconstantsvp.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures.hpp"
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
namespace py_backtracers {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_BTConstantSVP(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BTConstantSVP, ARG)

void init_c_btconstantsvp(py::module& m)
{
    py::class_<BTConstantSVP, I_Backtracer>(
        m,
        "BTConstantSVP",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BTConstantSVP))
        .def(py::init<navigation::datastructures::GeoLocation, float, float>(),
             DOC_BTConstantSVP(BTConstantSVP),
             py::arg("sensor_location"),
             py::arg("sensor_x"),
             py::arg("sensor_y"))
        .def("__eq__", &BTConstantSVP::operator==, DOC_BTConstantSVP(operator_eq), py::arg("other"))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BTConstantSVP)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BTConstantSVP)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BTConstantSVP)
        // end BTConstantSVP
        ;
}

} // namespace py_backtracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping