// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "module.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_featuremapping {

#define DOC_M_Featuremapping(ARG)                                                                  \
    DOC(themachinethatgoesping, pingprocessing, watercolumn, featuremapping, ARG)

void init_c_nearestfeaturemapper(py::module& m);

// -- create submodule --
void init_m_featuremapping(py::module& m)
{
    // module description
    auto subm = m.def_submodule("featuremapping", "Functions and classes for featuremapping and interpolation");

    init_c_nearestfeaturemapper(subm);
}

}
}
}
}