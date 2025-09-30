// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <themachinethatgoesping/algorithms/signalprocessing/types.hpp>
#include <themachinethatgoesping/tools_nanobind/enumhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_signalprocessing {
namespace py_datastructures {

namespace nb = nanobind;

void init_c_genericsignalparameters(nb::module_& m); // c_genericsignalparameters.cpp
void init_c_cwsignalparameters(nb::module_& m);      // c_cwsignalparameters.cpp
void init_c_fmsignalparameters(nb::module_& m);      // c_fmsignalparameters.cpp

void init_m_datastructures(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("datastructures");

    submodule.doc() =
        "Submodule that holds datastructures that hold the signal processing input/results";

    using signalprocessing::types::t_TxSignalType;
    using o_TxSignalType = themachinethatgoesping::tools::classhelper::Option<t_TxSignalType>;

    auto pyenum_tx_signal_types =
        nb::enum_<t_TxSignalType>(
            submodule,
            "t_TxSignalType",
            DOC(themachinethatgoesping, algorithms, signalprocessing, types, t_TxSignalType))
            .value("CW", t_TxSignalType::CW)
            .value("FM_UP_SWEEP", t_TxSignalType::FM_UP_SWEEP)
            .value("FM_DOWN_SWEEP", t_TxSignalType::FM_DOWN_SWEEP)
            .value("UNKNOWN", t_TxSignalType::UNKNOWN)
        //
        ;

    tools::nanobind_helper::make_option_class<o_TxSignalType>(submodule, "o_TxSignalType");

    init_c_genericsignalparameters(submodule);
    init_c_cwsignalparameters(submodule);
    init_c_fmsignalparameters(submodule);
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping