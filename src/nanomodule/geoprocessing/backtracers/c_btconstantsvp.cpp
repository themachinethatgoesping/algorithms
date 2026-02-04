// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/btconstantsvp.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing;
using namespace themachinethatgoesping::navigation::datastructures;

#define DOC_BTConstantSVP(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BTConstantSVP, ARG)

void init_c_btconstantsvp(nb::module_& m)
{
    nb::class_<BTConstantSVP, I_Backtracer>(
        m,
        "BTConstantSVP",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BTConstantSVP))
        .def(nb::init<navigation::datastructures::Geolocation, float, float>(),
             DOC_BTConstantSVP(BTConstantSVP),
             nb::arg("sensor_location"),
             nb::arg("sensor_x"),
             nb::arg("sensor_y"))
        .def("__eq__", &BTConstantSVP::operator==, DOC_BTConstantSVP(operator_eq), nb::arg("other"))

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