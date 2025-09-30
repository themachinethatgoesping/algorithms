// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

#include "bubblestreams/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pointprocessing {

namespace nb = nanobind;

namespace py_functions {
void init_m_functions(nb::module_& m); // defined in functions/functions.cpp
} // namespace py_functions

void init_m_pointprocessing(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("pointprocessing");

    submodule.doc() = "Submodule for pointprocessing (weighted_median, bubble_displacement, ...)";

    py_functions::init_m_functions(submodule);

    py_bubblestreams::init_m_bubblestreams(submodule);
}

} // namespace py_pointprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping