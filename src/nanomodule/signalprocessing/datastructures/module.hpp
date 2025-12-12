// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

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

void init_m_datastructures(nb::module_& m); // module.cpp

} // namespace py_datastructures
} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping