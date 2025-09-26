// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_imageprocessing {

namespace py_functions {
void init_m_functions(nanobind::module_& m); // defined in functions/functions.cpp
}

void init_m_imageprocessing(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("imageprocessing");

    submodule.doc() = "Submodule for imageprocessing (absorption, tvg, calibration factors, etc.)";

    py_functions::init_m_functions(submodule);
}

} // namespace py_imageprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping