// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

#include "functions/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {

namespace nb = nanobind;

void init_c_forwardgridder1d(nb::module_& m); // c_forwardgridder1d.cpp
void init_c_forwardgridder2d(nb::module_& m); // c_forwardgridder2d.cpp
void init_c_forwardgridder3d(nb::module_& m); // c_forwardgridder3d.cpp

void init_m_gridding(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("gridding");

    submodule.doc() = "Submodule for gridding (raytracers and georefencing) echosounder samples";

    py_functions::init_m_functions(submodule);
    init_c_forwardgridder1d(submodule);
    init_c_forwardgridder2d(submodule);
    init_c_forwardgridder3d(submodule);
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping