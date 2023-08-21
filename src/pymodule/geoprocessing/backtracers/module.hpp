// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

void init_i_backtracer(pybind11::module& m); // init_i_backtracer.cpp
void init_c_btconstantsvp(pybind11::module& m); // init_c_btconstantsvp.cpp

void init_m_backtracers(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("backtracers");

    submodule.doc() = "Submodule for backtracing echo sounder sample locations to beamangle and twoway traveltime";

    init_i_backtracer(submodule);
    init_c_btconstantsvp(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping