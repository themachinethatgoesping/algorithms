// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

namespace themachinethatgoesping {
    namespace algorithms {
    namespace pymodule {
    namespace py_pointprocessing {
    namespace py_bubblestreams {

void init_c_zspine(pybind11::module& m);                   // zspine.cpp

void init_m_bubblestreams(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("bubblestreams");

    submodule.doc() =
        "Submodule that holds classes related to bubblestream processing";

        init_c_zspine(submodule);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping