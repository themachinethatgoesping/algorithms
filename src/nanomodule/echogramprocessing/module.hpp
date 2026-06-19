// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_echogramprocessing {

// defined in c_bottomdetector.cpp
void init_c_bottomdetector(nanobind::module_& m);

void init_m_echogramprocessing(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("echogramprocessing");

    submodule.doc() =
        "Submodule for echogram processing (e.g. bottom detection on Sv echograms)";

    init_c_bottomdetector(submodule);
}

} // namespace py_echogramprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
