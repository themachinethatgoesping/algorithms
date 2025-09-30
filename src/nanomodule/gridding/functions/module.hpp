// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

namespace nb = nanobind;

void init_f_resamplingfunctions(nb::module_& m); // f_resamplingfunctions.cpp
void init_f_gridfunctions(nb::module_& m);       // f_gridfunctions.cpp

void init_m_functions(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("functions");

    submodule.doc() = "Submodule for gridding functions";

    init_f_resamplingfunctions(submodule);
    init_f_gridfunctions(submodule);
}

} // namespace py_functions
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping