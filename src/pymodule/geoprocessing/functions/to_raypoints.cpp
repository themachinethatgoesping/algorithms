// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/geoprocessing/functions/to_raypoints.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_functions {

#define DOC_functions(ARG)                                                                         \
    DOC(themachinethatgoesping, algorithms, geoprocessing, functions, ARG)

template<typename t_float>
void init_to_raypoints(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace geoprocessing::functions;

    m.def("to_raypoints",
          py::overload_cast<const t_float,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            int>(
              &to_raypoints<xt::pytensor<t_float, 2>, xt::pytensor<t_float, 1>, t_float>),
          DOC_functions(to_raypoints),
          py::arg("base_location"),
          py::arg("end_locations").noconvert(),
          py::arg("base_scale_value"),
          py::arg("end_scale_values").noconvert(),
          py::arg("ray_scale_values").noconvert(),
          py::arg("mp_cores") = 1);
}

void init_f_to_raypoints(pybind11::module& m)
{
    init_to_raypoints<float>(m);
    init_to_raypoints<double>(m);
}

} // namespace py_functions
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping