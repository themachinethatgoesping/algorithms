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
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        signalprocessing,                                                                          \
        datastructures,                                                                            \
        CWSignalParameters,                                                                        \
        ARG)

void init_c_cwsignalparameters(py::module& m)
{
    py::class_<CWSignalParameters, std::shared_ptr<CWSignalParameters>>(
        m,
        "CWSignalParameters",
        DOC(themachinethatgoesping, algorithms, signalprocessing, datastructures, CWSignalParameters))
        .def(py::init<>(), DOC_XYZ(CWSignalParameters))
        .def(py::init<float, float, float>(),
             DOC_XYZ(CWSignalParameters),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"))
        .def("__eq__", &CWSignalParameters::operator==, DOC_XYZ(operator_eq), py::arg("other"))

        .def_readwrite("center_frequency", &CWSignalParameters::center_frequency, DOC_XYZ(center_frequency))
        .def_readwrite("bandwidth", &CWSignalParameters::bandwidth, DOC_XYZ(bandwidth))
        .def_readwrite("effective_pulse_duration", &CWSignalParameters::effective_pulse_duration, DOC_XYZ(effective_pulse_duration) )

        .def("get_tx_signal_type", &CWSignalParameters::get_tx_signal_type, DOC_XYZ(get_tx_signal_type))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(CWSignalParameters)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(CWSignalParameters)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(CWSignalParameters)
        // end CWSignalParameters
        ;
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping