// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

#include <themachinethatgoesping/algorithms/signalprocessing/types.hpp>
#include <themachinethatgoesping/tools_pybind/enumhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_signalprocessing {
namespace py_datastructures {

void init_c_cwsignalparameters(pybind11::module& m); // c_cwsignalparameters.cpp
void init_c_fmsignalparameters(pybind11::module& m); // c_fmsignalparameters.cpp

void init_m_datastructures(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("datastructures");

    submodule.doc() =
        "Submodule that holds datastructures that hold the signal processing input/results";

    using signalprocessing::types::t_TxSignalType;

    auto pyenum_tx_signal_types =
        pybind11::enum_<t_TxSignalType>(
            submodule,
            "t_TxSignalType",
            DOC(themachinethatgoesping, algorithms, signalprocessing, types, t_TxSignalType))
            .value("CW", t_TxSignalType::CW)
            .value("FM_UP_SWEEP", t_TxSignalType::FM_UP_SWEEP)
            .value("FM_DOWN_SWEEP", t_TxSignalType::FM_DOWN_SWEEP)
            .export_values()
        //
        ;

    tools::pybind_helper::add_string_to_enum_conversion<t_TxSignalType>(pyenum_tx_signal_types);

    init_c_cwsignalparameters(submodule);
    init_c_fmsignalparameters(submodule);
}

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping