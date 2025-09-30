// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

#include "datastructures/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_signalprocessing {

void init_m_signalprocessing(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("signalprocessing");

    submodule.doc() = "Submodule for signalprocessing (volume scattering, etc.)";

    py_datastructures::init_m_datastructures(submodule);
}

} // namespace py_signalprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping