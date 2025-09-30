// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

void init_c_xyz(nb::module_& m);                   // c_xyz.cpp
void init_c_raytraceresults(nb::module_& m);       // c_raytraceresults.cpp
void init_c_raytraceresult(nb::module_& m);        // c_raytraceresult.cpp
void init_c_sampleindices(nb::module_& m);         // c_sampleindices.cpp
void init_c_sampledirections(nb::module_& m);      // c_sampledirections.cpp
void init_c_sampledirectionsrange(nb::module_& m); // c_sampledirectionsrange.cpp
void init_c_sampledirectionstime(nb::module_& m);  // c_sampledirectionstime.cpp
void init_c_beamsampleparameters(nb::module_& m);  // c_beamsampleparameters.cpp

void init_m_datastructures(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("datastructures");

    submodule.doc() =
        "Submodule that holds datastructures that hold the raytracers/georeferncing results";

    init_c_xyz(submodule);
    init_c_sampledirections(submodule);
    init_c_sampledirectionsrange(submodule);
    init_c_sampledirectionstime(submodule);
    init_c_raytraceresult(submodule);
    init_c_raytraceresults(submodule);
    init_c_sampleindices(submodule);
    init_c_beamsampleparameters(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping