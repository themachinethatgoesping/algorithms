// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

void init_c_backtracedwci(nb::module_& m); // c_backtracedwci.cpp
void init_i_backtracer(nb::module_& m);    // c_i_backtracer.cpp
void init_c_btconstantsvp(nb::module_& m); // c_btconstantsvp.cpp

void init_m_backtracers(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("backtracers");

    submodule.doc() = "Submodule for backtracing echo sounder sample locations to beamangle and "
                      "twoway traveltime";

    init_c_backtracedwci(submodule);
    init_i_backtracer(submodule);
    init_c_btconstantsvp(submodule);
}

} // namespace py_backtracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping