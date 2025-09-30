// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/signalprocessing/datastructures.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_signalprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::signalprocessing::datastructures;

#define DOC_FMSignalParameters(ARG)                                                                               \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        signalprocessing,                                                                          \
        datastructures,                                                                            \
        FMSignalParameters,                                                                        \
        ARG)

void init_c_fmsignalparameters(nb::module_& m)
{
    const std::string py_class_name = "FMSignalParameters";

    nb::class_<FMSignalParameters>(m,
                                   py_class_name.c_str(),
                                   DOC(themachinethatgoesping,
                                       algorithms,
                                       signalprocessing,
                                       datastructures,
                                       FMSignalParameters))
        //.def(nb::init<>(), DOC_FMSignalParameters(FMSignalParameters))
        .def(nb::init<float, float, float, bool>(),
             DOC_FMSignalParameters(FMSignalParameters),
             nb::arg("center_frequency"),
             nb::arg("bandwidth"),
             nb::arg("effective_pulse_duration"),
             nb::arg("up_sweep"))
        .def(nb::init<float, float, float, signalprocessing::types::t_TxSignalType>(),
             DOC_FMSignalParameters(FMSignalParameters),
             nb::arg("center_frequency"),
             nb::arg("bandwidth"),
             nb::arg("effective_pulse_duration"),
             nb::arg("tx_signal_type"))
        .def("__eq__",
             &FMSignalParameters::operator==,
             DOC_FMSignalParameters(operator_eq),
             nb::arg("other"))

        .def("get_center_frequency",
             &FMSignalParameters::get_center_frequency,
             DOC_FMSignalParameters(center_frequency))
        .def("get_bandwidth", &FMSignalParameters::get_bandwidth, DOC_FMSignalParameters(bandwidth))
        .def("get_effective_pulse_duration",
             &FMSignalParameters::get_effective_pulse_duration,
             DOC_FMSignalParameters(effective_pulse_duration))
        .def("get_up_sweep", &FMSignalParameters::get_up_sweep, DOC_FMSignalParameters(up_sweep))
        .def("get_tx_signal_type",
             &FMSignalParameters::get_tx_signal_type,
             DOC_FMSignalParameters(get_tx_signal_type))

        .def("set_center_frequency",
             &FMSignalParameters::set_center_frequency,
             DOC_FMSignalParameters(set_center_frequency),
             nb::arg("center_frequency"))
        .def("set_bandwidth",
             &FMSignalParameters::set_bandwidth,
             DOC_FMSignalParameters(set_bandwidth),
             nb::arg("bandwidth"))
        .def("set_effective_pulse_duration",
             &FMSignalParameters::set_effective_pulse_duration,
             DOC_FMSignalParameters(set_effective_pulse_duration),
             nb::arg("effective_pulse_duration"))
        .def("set_up_sweep",
             &FMSignalParameters::set_up_sweep,
             DOC_FMSignalParameters(set_up_sweep),
             nb::arg("up_sweep"))

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