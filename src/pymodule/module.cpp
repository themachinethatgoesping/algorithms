// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

#include "geoprocessing/module.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {

PYBIND11_MODULE(MODULE_NAME, m)
{
    pybind11::add_ostream_redirect(m, "ostream_redirect");

    m.doc()               = "Python module process ping data, e.g. apply absorption, spreading "
                            "loss, compute "
                            "range/depth, raytrace ...";
    m.attr("__version__") = MODULE_VERSION;

    py_geoprocessing::init_m_geoprocessing(m);
}

} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping