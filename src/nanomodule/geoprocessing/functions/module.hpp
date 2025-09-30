// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_functions {

void init_f_to_raypoints(nb::module_& m); // init_f_to_raypoints.cpp

void init_m_functions(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("functions");

    submodule.doc() = "Submodule for geoprocessing functions";

    init_f_to_raypoints(submodule);
}

} // namespace py_functions
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping