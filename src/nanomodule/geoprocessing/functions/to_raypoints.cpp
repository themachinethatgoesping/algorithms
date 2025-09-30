// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

#include <themachinethatgoesping/tools_nanobind/nanobind_xtensor.hpp>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/geoprocessing/functions/to_raypoints.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_functions {

namespace nb = nanobind;

#define DOC_raypoint_functions(ARG)                                                                         \
    DOC(themachinethatgoesping, algorithms, geoprocessing, functions, ARG)

template<typename t_float>
void init_to_raypoints(nb::module_& m)
{
    using namespace geoprocessing::functions;

    m.def(
        "to_raypoints",
        [](const t_float                         base_location,
           const xt::nanobind::pytensor<t_float, 1>& end_locations,
           const t_float                         base_scale_value,
           const xt::nanobind::pytensor<t_float, 1>& end_scale_values,
           const xt::nanobind::pytensor<t_float, 1>& ray_scale_values,
           int                                     mp_cores) {
            xt::xtensor<t_float, 1> end_locations_xt(end_locations.shape());
            xt::xtensor<t_float, 1> end_scale_values_xt(end_scale_values.shape());
            xt::xtensor<t_float, 1> ray_scale_values_xt(ray_scale_values.shape());

            end_locations_xt    = end_locations;
            end_scale_values_xt = end_scale_values;
            ray_scale_values_xt = ray_scale_values;

            return to_raypoints<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>, t_float>(
                base_location,
                end_locations_xt,
                base_scale_value,
                end_scale_values_xt,
                ray_scale_values_xt,
                mp_cores);
        },
        DOC_raypoint_functions(to_raypoints),
        nb::arg("base_location"),
        nb::arg("end_locations").noconvert(),
        nb::arg("base_scale_value"),
        nb::arg("end_scale_values").noconvert(),
        nb::arg("ray_scale_values").noconvert(),
        nb::arg("mp_cores") = 1);
}

void init_f_to_raypoints(nb::module_& m)
{
    init_to_raypoints<double>(m);
    init_to_raypoints<float>(m);
}

} // namespace py_functions
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping