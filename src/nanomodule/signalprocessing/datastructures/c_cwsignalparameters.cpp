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

#define DOC_CWSignalParameters(ARG)                                                                \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        signalprocessing,                                                                          \
        datastructures,                                                                            \
        CWSignalParameters,                                                                        \
        ARG)

void init_c_cwsignalparameters(nb::module_& m)
{
    const std::string py_class_name = "CWSignalParameters";

    nb::class_<CWSignalParameters>(m,
                                   py_class_name.c_str(),
                                   DOC(themachinethatgoesping,
                                       algorithms,
                                       signalprocessing,
                                       datastructures,
                                       CWSignalParameters))
        //.def(nb::init<>(), DOC_CWSignalParameters(CWSignalParameters))
        .def(nb::init<float, float, float>(),
             DOC_CWSignalParameters(CWSignalParameters),
             nb::arg("center_frequency"),
             nb::arg("bandwidth"),
             nb::arg("effective_pulse_duration"))
        .def("__eq__",
             &CWSignalParameters::operator==,
             DOC_CWSignalParameters(operator_eq),
             nb::arg("other"))

        .def("get_center_frequency",
             &CWSignalParameters::get_center_frequency,
             DOC_CWSignalParameters(center_frequency))
        .def("get_bandwidth", &CWSignalParameters::get_bandwidth, DOC_CWSignalParameters(bandwidth))
        .def("get_effective_pulse_duration",
             &CWSignalParameters::get_effective_pulse_duration,
             DOC_CWSignalParameters(effective_pulse_duration))
        .def("get_tx_signal_type",
             &CWSignalParameters::get_tx_signal_type,
             DOC_CWSignalParameters(get_tx_signal_type))

        .def("set_center_frequency",
             &CWSignalParameters::set_center_frequency,
             DOC_CWSignalParameters(set_center_frequency),
             nb::arg("center_frequency"))
        .def("set_bandwidth",
             &CWSignalParameters::set_bandwidth,
             DOC_CWSignalParameters(set_bandwidth),
             nb::arg("bandwidth"))
        .def("set_effective_pulse_duration",
             &CWSignalParameters::set_effective_pulse_duration,
             DOC_CWSignalParameters(set_effective_pulse_duration),
             nb::arg("effective_pulse_duration"))

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