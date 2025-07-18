// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pointprocessing {
namespace py_functions {

#define DOC_pointprocessing_functions(ARG) DOC(themachinethatgoesping, algorithms, pointprocessing, functions, ARG)

template<typename t_float_val, typename t_float_weight>
void init_weighted_median(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace pointprocessing::functions;

    // weighted_median
    m.def("weighted_median",
          py::overload_cast<const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_weight, 1>&>(
              &weighted_median<xt::pytensor<t_float_val, 1>, xt::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median),
          py::arg("values"),
          py::arg("weights"));

    m.def("weighted_median",
          py::overload_cast<const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_weight, 1>&>(
              &weighted_median<xt::pytensor<t_float_val, 1>, xt::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median_2),
          py::arg("values_x"),
          py::arg("values_y"),
          py::arg("weights"));

    m.def("weighted_median",
          py::overload_cast<const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_weight, 1>&>(
              &weighted_median<xt::pytensor<t_float_val, 1>, xt::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median_3),
          py::arg("values_x"),
          py::arg("values_y"),
          py::arg("values_z"),
          py::arg("weights"));

    // weighted_median
    m.def("weighted_median",
          py::overload_cast<const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_weight, 1>&>(
              &weighted_median<xt::pytensor<t_float_val, 1>, xt::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median),
          py::arg("values"),
          py::arg("weights"));

    // weighted_median
    m.def("segment_in_weighted_quantiles",
          py::overload_cast<const xt::pytensor<t_float_val, 1>&,
                            const xt::pytensor<t_float_weight, 1>&,
                            const int64_t,
                            const bool>(
              &segment_in_weighted_quantiles<xt::pytensor<t_float_val, 1>,
                                             xt::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(segment_in_weighted_quantiles),
          py::arg("values"),
          py::arg("weights"),
          py::arg("n_quantiles"),
          py::arg("return_empty_segments") = false);
}

void init_m_functions(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("functions");

    submodule.doc() = "M that holds functions used for amplitude corrections";

    init_weighted_median<double, double>(submodule);
    init_weighted_median<double, float>(submodule);
    init_weighted_median<float, double>(submodule);
    init_weighted_median<float, float>(submodule);
}

} // namespace py_functions
} // namespace py_pointprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping