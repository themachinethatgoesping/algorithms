// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <optional>

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>

#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_imageprocessing {
namespace py_functions {

using themachinethatgoesping::tools_nanobind::pytensor;

#define DOC_imageprocessing_functions(ARG)                                                         \
    DOC(themachinethatgoesping, algorithms, imageprocessing, functions, ARG)

template<typename t_float>
void init_find_local_maxima(nanobind::module_& m)
{
    namespace nb = nanobind;
    using namespace imageprocessing::functions;

    // find_local_maxima
    m.def("find_local_maxima",
          nb::overload_cast<const pytensor<t_float, 3>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<pytensor<t_float, 3>>),
          DOC_imageprocessing_functions(find_local_maxima),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          nb::overload_cast<const pytensor<t_float, 2>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<pytensor<t_float, 2>>),
          DOC_imageprocessing_functions(find_local_maxima_2),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          nb::overload_cast<const pytensor<t_float, 1>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<pytensor<t_float, 1>>),
          DOC_imageprocessing_functions(find_local_maxima_3),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);

    // find_local_maxima2
    m.def("find_local_maxima2",
          nb::overload_cast<const pytensor<t_float, 3>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<pytensor<t_float, 3>>),
          DOC_imageprocessing_functions(find_local_maxima2),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);

    m.def("find_local_maxima2",
          nb::overload_cast<const pytensor<t_float, 2>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<pytensor<t_float, 2>>),
          DOC_imageprocessing_functions(find_local_maxima2_2),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);

    m.def("find_local_maxima2",
          nb::overload_cast<const pytensor<t_float, 1>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<pytensor<t_float, 1>>),
          DOC_imageprocessing_functions(find_local_maxima2_3),
          nb::arg("data").noconvert(),
          nb::arg("threshold")   = std::nullopt,
          nb::arg("accept_nans") = true,
          nb::arg("mp_cores")    = 1);
}

template<typename t_region, typename t_value>
void init_grow_regions(nanobind::module_& m)
{
    namespace nb = nanobind;
    using namespace imageprocessing::functions;

    m.def("grow_regions",
          nb::overload_cast<pytensor<t_region, 3>&,
                            const pytensor<t_value, 3>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<pytensor<t_region, 3>, pytensor<t_value, 3>>),
          DOC_imageprocessing_functions(grow_regions),
          nb::arg("region_volume").noconvert(),
          nb::arg("data_volume").noconvert(),
          nb::arg("null_region")             = 0,
          nb::arg("threshold")               = std::nullopt,
          nb::arg("force_negative_gradient") = true,
          nb::arg("eat_neighbor_regions")    = false,
          nb::arg("mp_cores")                = 1);

    m.def("grow_regions",
          nb::overload_cast<pytensor<t_region, 2>&,
                            const pytensor<t_value, 2>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<pytensor<t_region, 2>, pytensor<t_value, 2>>),
          DOC_imageprocessing_functions(grow_regions_2),
          nb::arg("region_image").noconvert(),
          nb::arg("data_image").noconvert(),
          nb::arg("null_region")             = 0,
          nb::arg("threshold")               = std::nullopt,
          nb::arg("force_negative_gradient") = true,
          nb::arg("eat_neighbor_regions")    = false,
          nb::arg("mp_cores")                = 1);

    m.def("grow_regions",
          nb::overload_cast<pytensor<t_region, 1>&,
                            const pytensor<t_value, 1>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<pytensor<t_region, 1>, pytensor<t_value, 1>>),
          DOC_imageprocessing_functions(grow_regions_3),
          nb::arg("region_vector").noconvert(),
          nb::arg("data_vector").noconvert(),
          nb::arg("null_region")             = 0,
          nb::arg("threshold")               = std::nullopt,
          nb::arg("force_negative_gradient") = true,
          nb::arg("eat_neighbor_regions")    = false,
          nb::arg("mp_cores")                = 1);
}

template<typename t_value>
void init_grow_regions_value_type(nanobind::module_& m)
{
    init_grow_regions<int64_t, t_value>(m);
    init_grow_regions<int32_t, t_value>(m);
    init_grow_regions<int16_t, t_value>(m);
    init_grow_regions<int8_t, t_value>(m);
}

void init_m_functions(nanobind::module_& m)
{
    nanobind::module_ submodule = m.def_submodule("functions");

    submodule.doc() = "M that holds functions used for image processing functions";

    init_find_local_maxima<double>(submodule);
    init_find_local_maxima<float>(submodule);
    init_find_local_maxima<int64_t>(submodule);
    init_find_local_maxima<int32_t>(submodule);
    init_find_local_maxima<int16_t>(submodule);
    init_find_local_maxima<int8_t>(submodule);

    init_grow_regions_value_type<double>(submodule);
    init_grow_regions_value_type<float>(submodule);
    init_grow_regions_value_type<int64_t>(submodule);
    init_grow_regions_value_type<int32_t>(submodule);
    init_grow_regions_value_type<int16_t>(submodule);
    init_grow_regions_value_type<int8_t>(submodule);
}

} // namespace py_functions
} // namespace py_imageprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping