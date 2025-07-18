// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_imageprocessing {
namespace py_functions {

#define DOC_imageprocessing_functions(ARG)                                                         \
    DOC(themachinethatgoesping, algorithms, imageprocessing, functions, ARG)

template<typename t_float>
void init_find_local_maxima(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace imageprocessing::functions;

    // find_local_maxima
    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 3>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 3>>),
          DOC_imageprocessing_functions(find_local_maxima),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 2>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 2>>),
          DOC_imageprocessing_functions(find_local_maxima_2),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 1>>),
          DOC_imageprocessing_functions(find_local_maxima_3),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    // find_local_maxima2
    m.def("find_local_maxima2",
          py::overload_cast<const xt::pytensor<t_float, 3>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<xt::pytensor<t_float, 3>>),
          DOC_imageprocessing_functions(find_local_maxima2),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima2",
          py::overload_cast<const xt::pytensor<t_float, 2>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<xt::pytensor<t_float, 2>>),
          DOC_imageprocessing_functions(find_local_maxima2_2),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima2",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima2<xt::pytensor<t_float, 1>>),
          DOC_imageprocessing_functions(find_local_maxima2_3),
          py::arg("data").noconvert(),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);
}

template<typename t_region, typename t_value>
void init_grow_regions(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace imageprocessing::functions;

    m.def("grow_regions",
          py::overload_cast<xt::pytensor<t_region, 3>&,
                            const xt::pytensor<t_value, 3>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<xt::pytensor<t_region, 3>, xt::pytensor<t_value, 3>>),
          DOC_imageprocessing_functions(grow_regions),
          py::arg("region_volume").noconvert(),
          py::arg("data_volume").noconvert(),
          py::arg("null_region")             = 0,
          py::arg("threshold")               = std::nullopt,
          py::arg("force_negative_gradient") = true,
          py::arg("eat_neighbor_regions")    = false,
          py::arg("mp_cores")                = 1);

    m.def("grow_regions",
          py::overload_cast<xt::pytensor<t_region, 2>&,
                            const xt::pytensor<t_value, 2>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<xt::pytensor<t_region, 2>, xt::pytensor<t_value, 2>>),
          DOC_imageprocessing_functions(grow_regions_2),
          py::arg("region_volume").noconvert(),
          py::arg("data_volume").noconvert(),
          py::arg("null_region")             = 0,
          py::arg("threshold")               = std::nullopt,
          py::arg("force_negative_gradient") = true,
          py::arg("eat_neighbor_regions")    = false,
          py::arg("mp_cores")                = 1);

    m.def("grow_regions",
          py::overload_cast<xt::pytensor<t_region, 1>&,
                            const xt::pytensor<t_value, 1>&,
                            const t_region,
                            const std::optional<t_value>,
                            const bool,
                            const bool,
                            const int>(
              &grow_regions<xt::pytensor<t_region, 1>, xt::pytensor<t_value, 1>>),
          DOC_imageprocessing_functions(grow_regions_3),
          py::arg("region_volume").noconvert(),
          py::arg("data_volume").noconvert(),
          py::arg("null_region")             = 0,
          py::arg("threshold")               = std::nullopt,
          py::arg("force_negative_gradient") = true,
          py::arg("eat_neighbor_regions")    = false,
          py::arg("mp_cores")                = 1);
}

template<typename t_value>
void init_grow_regions_value_type(pybind11::module& m)
{
    init_grow_regions<int64_t, t_value>(m);
    init_grow_regions<int32_t, t_value>(m);
    init_grow_regions<int16_t, t_value>(m);
    init_grow_regions<int8_t, t_value>(m);
}

void init_m_functions(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("functions");

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