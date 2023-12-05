// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/signalprocessing/datastructures.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <xtensor-python/pyarray.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_signalprocessing {
namespace py_datastructures {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::signalprocessing::datastructures;

#define DOC_XYZ(ARG)                                                                               \
    DOC(themachinethatgoesping, algorithms, signalprocessing, datastructures, CWSignalInfos, ARG)

void init_c_cwsignalinfos(py::module& m)
{
    py::class_<CWSignalInfos, std::shared_ptr<CWSignalInfos>>(
        m,
        "CWSignalInfos",
        DOC(themachinethatgoesping, algorithms, signalprocessing, datastructures, CWSignalInfos))
        .def(py::init<>(), DOC_XYZ(CWSignalInfos))
        .def(py::init<float, float, float>(),
             DOC_XYZ(CWSignalInfos),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"))
        .def("__eq__", &CWSignalInfos::operator==, DOC_XYZ(operator_eq), py::arg("other"))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(CWSignalInfos)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(CWSignalInfos)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(CWSignalInfos)
        // end CWSignalInfos
        ;
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping