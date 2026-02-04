// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <xtensor-python/nanobind/pytensor.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pointprocessing {
namespace py_functions {

#define DOC_pointprocessing_functions(ARG) DOC(themachinethatgoesping, algorithms, pointprocessing, functions, ARG)

template<typename t_float_val, typename t_float_weight>
void init_weighted_median(nanobind::module_& m)
{
    namespace nb  = nanobind;
    namespace xnb = xt::nanobind;
    using namespace pointprocessing::functions;

    // weighted_median
    m.def("weighted_median",
          nb::overload_cast<const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_weight, 1>&>(
              &weighted_median<xnb::pytensor<t_float_val, 1>, xnb::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median),
          nb::arg("values"),
          nb::arg("weights"));

    m.def("weighted_median",
          nb::overload_cast<const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_weight, 1>&>(
              &weighted_median<xnb::pytensor<t_float_val, 1>, xnb::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median_2),
          nb::arg("values_x"),
          nb::arg("values_y"),
          nb::arg("weights"));

    m.def("weighted_median",
          nb::overload_cast<const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_weight, 1>&>(
              &weighted_median<xnb::pytensor<t_float_val, 1>, xnb::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(weighted_median_3),
          nb::arg("values_x"),
          nb::arg("values_y"),
          nb::arg("values_z"),
          nb::arg("weights"));

    // weighted_median
    m.def("segment_in_weighted_quantiles",
          nb::overload_cast<const xnb::pytensor<t_float_val, 1>&,
                            const xnb::pytensor<t_float_weight, 1>&,
                            const int64_t,
                            const bool>(&segment_in_weighted_quantiles<
                                     xnb::pytensor<t_float_val, 1>,
                                     xnb::pytensor<t_float_weight, 1>>),
          DOC_pointprocessing_functions(segment_in_weighted_quantiles),
          nb::arg("values"),
          nb::arg("weights"),
          nb::arg("n_quantiles"),
          nb::arg("return_empty_segments") = false);
}

void init_m_functions(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("functions");

    submodule.doc() = "Module that holds functions used for amplitude corrections";

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