// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers {

void init_i_raytracer(pybind11::module& m); // init_i_raytracer.cpp
void init_c_rtconstantsvp(pybind11::module& m); // init_c_rtconstantsvp.cpp

void init_m_raytracers(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("raytracers");

    submodule.doc() = "Submodule for raytracing echo sounder sample locations";

    init_i_raytracer(submodule);
    init_c_rtconstantsvp(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping