// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

#include "datastructures/module.hpp"
#include "raytracers/module.hpp"
#include "backtracers/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {

void init_m_geoprocessing(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("geoprocessing");

    submodule.doc() = "Submodule for geoprocessing (raytracers and "
                      "georefencing) echosounder samples";

    py_datastructures::init_m_datastructures(submodule);
    py_raytracers::init_m_raytracers(submodule);
    py_backtracers::init_m_backtracers(submodule);
}

} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping