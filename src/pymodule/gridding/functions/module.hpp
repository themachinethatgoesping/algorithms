// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

void init_f_resamplingfunctions(pybind11::module& m); // f_resamplingfunctions.cpp
void init_f_gridfunctions(pybind11::module& m);       // f_gridfunctions.cpp

void init_m_functions(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("functions");

    submodule.doc() = "Submodule for gridding functions";

    init_f_resamplingfunctions(submodule);
    init_f_gridfunctions(submodule);
}

} // namespace py_datastructures
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping