// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

void init_c_i_raytracer(nb::module_& m);    // init_c_i_raytracer.cpp
void init_c_rtconstantsvp(nb::module_& m);  // init_c_rtconstantsvp.cpp

void init_m_raytracers(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("raytracers");

    submodule.doc() = "Submodule for raytracing echo sounder sample locations";

    init_c_i_raytracer(submodule);
    init_c_rtconstantsvp(submodule);
}

} // namespace py_raytracers
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping