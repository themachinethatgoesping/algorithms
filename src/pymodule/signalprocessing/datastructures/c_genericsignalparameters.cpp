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
        GenericSignalParameters,                                                                   \
        ARG)

void init_c_genericsignalparameters(py::module& m)
{
    py::class_<GenericSignalParameters, std::shared_ptr<GenericSignalParameters>>(
        m,
        "GenericSignalParameters",
        DOC(themachinethatgoesping,
            algorithms,
            signalprocessing,
            datastructures,
            GenericSignalParameters))
        .def(py::init<>(), DOC_XYZ(GenericSignalParameters))
        .def(py::init<float, float, float, signalprocessing::types::t_TxSignalType>(),
             DOC_XYZ(GenericSignalParameters),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"),
             py::arg("tx_signal_type"))
        .def("__eq__", &GenericSignalParameters::operator==, DOC_XYZ(operator_eq), py::arg("other"))

        .def_readwrite("center_frequency",
                       &GenericSignalParameters::center_frequency,
                       DOC_XYZ(center_frequency))
        .def_readwrite("bandwidth", &GenericSignalParameters::bandwidth, DOC_XYZ(bandwidth))
        .def_readwrite("effective_pulse_duration",
                       &GenericSignalParameters::effective_pulse_duration,
                       DOC_XYZ(effective_pulse_duration))
        .def_readwrite("signal_type", &GenericSignalParameters::signal_type, DOC_XYZ(signal_type))
        .def("get_tx_signal_type",
             &GenericSignalParameters::get_tx_signal_type,
             DOC_XYZ(get_tx_signal_type))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(GenericSignalParameters)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(GenericSignalParameters)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(GenericSignalParameters)
        // end GenericSignalParameters
        ;
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping