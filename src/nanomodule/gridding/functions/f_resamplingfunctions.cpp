// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <limits>

#include <cstddef>

#include <nanobind/nanobind.h>
#include <xtensor-python/nanobind/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/functions/resamplingfunctions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

#define DOC_gridding_functions(ARG)                                                                \
    DOC(themachinethatgoesping, algorithms, gridding, functions, ARG)

template<typename t_float>
void init_resamplingfunctions(nanobind::module_& m)
{
    namespace nb = nanobind;
    namespace xnb = xt::nanobind;
    using namespace gridding::functions;

    m.def(
        "compute_resampled_coordinates",
        [](const xnb::pytensor<t_float, 1>& values_min,
           const xnb::pytensor<t_float, 1>& values_max,
           const xnb::pytensor<t_float, 1>& values_res,
           const t_float                      grid_min,
           const t_float                      grid_max,
           const std::size_t                  max_steps) {
            auto values_min_xt = xt::xtensor<t_float, 1>::from_shape(values_min.shape());
            auto values_max_xt = xt::xtensor<t_float, 1>::from_shape(values_max.shape());
            auto values_res_xt = xt::xtensor<t_float, 1>::from_shape(values_res.shape());

            values_min_xt = values_min;
            values_max_xt = values_max;
            values_res_xt = values_res;

            return compute_resampled_coordinates<xt::xtensor<t_float, 1>>(values_min_xt,
                                                                          values_max_xt,
                                                                          values_res_xt,
                                                                          grid_min,
                                                                          grid_max,
                                                                          max_steps);
        },
        DOC_gridding_functions(compute_resampled_coordinates),
        nb::arg("values_min").noconvert(),
        nb::arg("values_max").noconvert(),
        nb::arg("values_res").noconvert(),
        nb::arg("grid_min")  = std::numeric_limits<t_float>::quiet_NaN(),
        nb::arg("grid_max")  = std::numeric_limits<t_float>::quiet_NaN(),
        nb::arg("max_steps") = 1024);
}

void init_f_resamplingfunctions(nanobind::module_& m)
{
    init_resamplingfunctions<float>(m);
    init_resamplingfunctions<double>(m);
}

} // namespace py_functions
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping