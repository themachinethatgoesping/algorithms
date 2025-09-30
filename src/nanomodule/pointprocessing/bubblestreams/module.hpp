// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pointprocessing {
namespace py_bubblestreams {

namespace nb = nanobind;

void init_c_zspine(nb::module_& m); // zspine.cpp

void init_m_bubblestreams(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("bubblestreams");

    submodule.doc() = "Submodule that holds classes related to bubblestream processing";

    init_c_zspine(submodule);
}

} // namespace py_bubblestreams
} // namespace py_pointprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping