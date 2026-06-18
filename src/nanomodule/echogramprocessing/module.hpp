// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_echogramprocessing {

namespace py_functions {
void init_m_functions(nanobind::module_& m); // defined in functions/functions.cpp
}

void init_m_echogramprocessing(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("echogramprocessing");

    submodule.doc() =
        "Submodule for echogram processing (e.g. bottom detection on Sv echograms)";

    py_functions::init_m_functions(submodule);
}

} // namespace py_echogramprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
