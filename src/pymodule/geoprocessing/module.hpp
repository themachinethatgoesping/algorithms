// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

#include "datastructures/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {


void init_m_geoprocessing(pybind11::module &m) {
  pybind11::module submodule = m.def_submodule("geoprocessing");

  submodule.doc() = "Submodule for geoprocessing (raytracing and "
                           "georefencing) echosounder samples";

  py_datastructures::init_m_datastructures(submodule);
}

} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping