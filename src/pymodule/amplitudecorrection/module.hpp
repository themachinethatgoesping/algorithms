// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_amplitudecorrection {

namespace py_functions {
void init_m_functions(pybind11::module& m); // defined in functions/functions.cpp
}

void init_m_amplitudecorrection(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("amplitudecorrection");

    submodule.doc() =
        "Submodule for amplitudecorrection (absorption, tvg, calibration factors, etc.)";

    py_functions::init_m_functions(submodule);
}

} // namespace py_amplitudecorrection
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping