// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_functions {

void init_f_to_raypoints(pybind11::module& m); // init_f_to_raypoints.cpp

void init_m_functions(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("functions");

    submodule.doc() = "Submodule for geoprocessing functions";

    init_f_to_raypoints(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping