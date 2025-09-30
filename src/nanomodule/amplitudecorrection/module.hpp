// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_amplitudecorrection {

namespace py_functions {
void init_m_functions(nb::module_& m); // defined in functions/functions.cpp
}

void init_m_amplitudecorrection(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("amplitudecorrection");

    submodule.doc() =
        "Submodule for amplitudecorrection (absorption, tvg, calibration factors, etc.)";

    py_functions::init_m_functions(submodule);
}

} // namespace py_amplitudecorrection
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping