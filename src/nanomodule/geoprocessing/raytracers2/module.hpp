// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

void init_c_soundvelocityprofile(nb::module_& m); // c_soundvelocityprofile.cpp
void init_c_layerraytracer(nb::module_& m);       // c_layerraytracer.cpp

void init_m_raytracers2(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("raytracers2");

    submodule.doc() = "Layer-based closed-form Snell raytracer (next-generation "
                      "successor to raytracers/RTConstantSVP).";

    init_c_soundvelocityprofile(submodule);
    init_c_layerraytracer(submodule);
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
