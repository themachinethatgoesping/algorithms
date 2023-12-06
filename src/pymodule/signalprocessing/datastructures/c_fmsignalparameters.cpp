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
        FMSignalParameters,                                                                        \
        ARG)

void init_c_fmsignalparameters(py::module& m)
{
    py::class_<FMSignalParameters, std::shared_ptr<FMSignalParameters>>(m,
                                                                        "FMSignalParameters",
                                                                        DOC(themachinethatgoesping,
                                                                            algorithms,
                                                                            signalprocessing,
                                                                            datastructures,
                                                                            FMSignalParameters))
        .def(py::init<>(), DOC_XYZ(FMSignalParameters))
        .def(py::init<float, float, float, bool>(),
             DOC_XYZ(FMSignalParameters),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"),
             py::arg("up_sweep"))
        .def(py::init<float, float, float, signalprocessing::types::t_TxSignalType>(),
             DOC_XYZ(FMSignalParameters),
             py::arg("center_frequency"),
             py::arg("bandwidth"),
             py::arg("effective_pulse_duration"),
             py::arg("tx_signal_type"))
        .def("__eq__", &FMSignalParameters::operator==, DOC_XYZ(operator_eq), py::arg("other"))

        .def_readwrite(
            "center_frequency", &FMSignalParameters::center_frequency, DOC_XYZ(center_frequency))
        .def_readwrite("bandwidth", &FMSignalParameters::bandwidth, DOC_XYZ(bandwidth))
        .def_readwrite("effective_pulse_duration",
                       &FMSignalParameters::effective_pulse_duration,
                       DOC_XYZ(effective_pulse_duration))
        .def_readwrite("up_sweep", &FMSignalParameters::up_sweep, DOC_XYZ(up_sweep))
        .def("get_tx_signal_type",
             &FMSignalParameters::get_tx_signal_type,
             DOC_XYZ(get_tx_signal_type))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(FMSignalParameters)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(FMSignalParameters)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(FMSignalParameters)
        // end FMSignalParameters
        ;
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping