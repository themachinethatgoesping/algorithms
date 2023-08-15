// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

void init_c_SampleLocationLocal(pybind11::module& m);  // c_SampleLocationLocal.cpp
void init_c_samplelocationslocal(pybind11::module& m); // c_samplelocationslocal.cpp

void init_m_datastructures(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("datastructures");

    submodule.doc() =
        "Submodule that holds datastructures that hold the raytracers/georeferncing results";

    init_c_SampleLocationLocal(submodule);
    init_c_samplelocationslocal(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping