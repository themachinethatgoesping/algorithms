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

#define DOC_GenericSignalParameters(ARG)                                                           \
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
        //.def(py::init<>(), DOC_GenericSignalParameters(GenericSignalParameters))
        .def(py::init<float, float, float, signalprocessing::types::t_TxSignalType>(),
             DOC_GenericSignalParameters(GenericSignalParameters),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"),
             py::arg("tx_signal_type"))
        .def("__eq__",
             &GenericSignalParameters::operator==,
             DOC_GenericSignalParameters(operator_eq),
             py::arg("other"))

        .def("get_center_frequency",
             &GenericSignalParameters::get_center_frequency,
             DOC_GenericSignalParameters(center_frequency))
        .def("get_bandwidth",
             &GenericSignalParameters::get_bandwidth,
             DOC_GenericSignalParameters(bandwidth))
        .def("get_effective_pulse_duration",
             &GenericSignalParameters::get_effective_pulse_duration,
             DOC_GenericSignalParameters(effective_pulse_duration))
        .def("get_tx_signal_type",
             &GenericSignalParameters::get_tx_signal_type,
             DOC_GenericSignalParameters(get_tx_signal_type))

        .def("set_center_frequency",
             &GenericSignalParameters::set_center_frequency,
             DOC_GenericSignalParameters(center_frequency),
             py::arg("center_frequency"))
        .def("set_bandwidth",
             &GenericSignalParameters::set_bandwidth,
             DOC_GenericSignalParameters(bandwidth),
             py::arg("bandwidth"))
        .def("set_effective_pulse_duration",
             &GenericSignalParameters::set_effective_pulse_duration,
             DOC_GenericSignalParameters(effective_pulse_duration),
             py::arg("effective_pulse_duration"))
        .def("set_tx_signal_type",
             &GenericSignalParameters::set_tx_signal_type,
             DOC_GenericSignalParameters(set_tx_signal_type),
             py::arg("tx_signal_type"))

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