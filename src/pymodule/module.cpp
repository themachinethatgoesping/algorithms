// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>

#include "geoprocessing/module.hpp"

#define FORCE_IMPORT_ARRAY // this is needed for xtensor-python but must only be included once
#include <xtensor-python/pyarray.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {

PYBIND11_MODULE(MODULE_NAME, m)
{
    xt::import_numpy(); // import numpy for xtensor (otherwise there will be weird segfaults)

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