// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "module.hpp"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pingtools {

// -- submodule declarations --
void init_c_readsamplerange(nanobind::module_& m);     // substructures/c_readsamplerange.cpp
void init_c_beamselection(nanobind::module_& m);       // substructures/c_beamselection.cpp
void init_c_beamsampleselection(nanobind::module_& m); // substructures/c_beamsampleselection.cpp

// -- create submodule --
void init_m_pingtools(nb::module_& m)
{
    // module description
    auto subm = m.def_submodule("pingtools", "Beam/sample selection primitives for ping objects.");

    // pingtool classes
    init_c_readsamplerange(subm);
    init_c_beamselection(subm);
    init_c_beamsampleselection(subm);
}

}
}
}
}