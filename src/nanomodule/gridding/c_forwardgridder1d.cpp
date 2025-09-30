// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

#include <cstddef>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <themachinethatgoesping/tools_nanobind/nanobind_xtensor.hpp>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder1d.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {

namespace nb   = nanobind;
namespace xtnb = xt::nanobind;
using namespace themachinethatgoesping::algorithms::gridding;

template<typename TClass, typename ReturnType, typename InputType>
auto make_vectorized_member(ReturnType (TClass::*method)(InputType) const)
{
     return [method](const TClass& self, const xtnb::pytensor<InputType, 1>& values) {
          xt::xtensor<ReturnType, 1> result(values.shape());
          auto*                  result_data = result.data();
          const InputType*       input_data  = values.data();
          const std::size_t      count       = static_cast<std::size_t>(values.size());

          for (std::size_t idx = 0; idx < count; ++idx)
          {
               result_data[idx] = (self.*method)(input_data[idx]);
          }

          return result;
     };
}

#define DOC_ForwardGridder1D(ARG)                                                                  \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder1D, ARG)

template<typename t_float>
void init_ForwardGridder1D_float(nb::module_& m, const std::string& suffix)
{
    using T_ForwardGridder1D     = ForwardGridder1D<t_float>;
    const std::string class_name = std::string("ForwardGridder1D") + suffix;

    nb::class_<T_ForwardGridder1D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder1D))
        .def(nb::init<t_float, t_float, t_float, t_float>(),
             DOC_ForwardGridder1D(ForwardGridder1D),
             nb::arg("xres"),
             nb::arg("min_x"),
             nb::arg("max_x"),
             nb::arg("xbase") = 0.0)
        .def_static("from_res",
                    nb::overload_cast<t_float, t_float, t_float>(&T_ForwardGridder1D::from_res),
                    DOC_ForwardGridder1D(from_res),
                    nb::arg("res"),
                    nb::arg("min_x"),
                    nb::arg("max_x"))
        .def_static("from_data",
                    nb::overload_cast<t_float, const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder1D::template from_data<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder1D(from_data),
                    nb::arg("res"),
                    nb::arg("sx"))
        .def("get_empty_grd_images",
             &T_ForwardGridder1D::template get_empty_grd_images<xt::xtensor<t_float, 1>>,
             DOC_ForwardGridder1D(get_empty_grd_images))
        .def("group_blocks",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&, const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder1D::template group_blocks<xtnb::pytensor<t_float, 1>>, nb::const_),
             DOC_ForwardGridder1D(group_blocks),
             nb::arg("sx").noconvert(),
             nb::arg("sv").noconvert())
          .def("interpolate_block_mean",
                nb::overload_cast<const xtnb::pytensor<t_float, 1>&, const xtnb::pytensor<t_float, 1>&>(
                     &T_ForwardGridder1D::template interpolate_block_mean<xt::xtensor<t_float, 1>,
                                                                                       xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder1D(interpolate_block_mean),
             nb::arg("sx"),
             nb::arg("s_val"))
        .def("interpolate_block_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder1D::template interpolate_block_mean_inplace<
                     xtnb::pytensor<t_float, 1>,
                     xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder1D(interpolate_block_mean_inplace),
             nb::arg("sx"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
          .def("interpolate_weighted_mean",
                nb::overload_cast<const xtnb::pytensor<t_float, 1>&, const xtnb::pytensor<t_float, 1>&>(
                     &T_ForwardGridder1D::template interpolate_weighted_mean<xt::xtensor<t_float, 1>,
                                                                                           xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder1D(interpolate_weighted_mean),
             nb::arg("sx"),
             nb::arg("s_val"))
        .def("interpolate_weighted_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder1D::template interpolate_weighted_mean_inplace<
                     xtnb::pytensor<t_float, 1>,
                     xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder1D(interpolate_weighted_mean_inplace),
             nb::arg("sx"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
        .def("get_xres", &T_ForwardGridder1D::get_xres, DOC_ForwardGridder1D(xres))
        .def("get_xmin", &T_ForwardGridder1D::get_xmin, DOC_ForwardGridder1D(xmin))
        .def("get_xmax", &T_ForwardGridder1D::get_xmax, DOC_ForwardGridder1D(xmax))
        .def("get_xbase", &T_ForwardGridder1D::get_xbase, DOC_ForwardGridder1D(xbase))
        .def("get_nx", &T_ForwardGridder1D::get_nx, DOC_ForwardGridder1D(nx))
        .def("get_border_xmin",
             &T_ForwardGridder1D::get_border_xmin,
             DOC_ForwardGridder1D(border_xmin))
        .def("get_border_xmax",
             &T_ForwardGridder1D::get_border_xmax,
             DOC_ForwardGridder1D(border_xmax))
        .def("get_x_index",
             nb::overload_cast<t_float>(&T_ForwardGridder1D::get_x_index, nb::const_),
             DOC_ForwardGridder1D(get_x_index),
             nb::arg("x"))
        .def("get_x_index",
             make_vectorized_member<T_ForwardGridder1D, int, t_float>(&T_ForwardGridder1D::get_x_index),
             DOC_ForwardGridder1D(get_x_index),
             nb::arg("x").noconvert())
        .def("get_x_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder1D::get_x_index_fraction, nb::const_),
             DOC_ForwardGridder1D(get_x_index_fraction),
             nb::arg("x"))
        .def("get_x_index_fraction",
             make_vectorized_member<T_ForwardGridder1D, t_float, t_float>(
                 &T_ForwardGridder1D::get_x_index_fraction),
             DOC_ForwardGridder1D(get_x_index_fraction),
             nb::arg("x").noconvert())
        .def("get_x_value",
             nb::overload_cast<int>(&T_ForwardGridder1D::get_x_value, nb::const_),
             DOC_ForwardGridder1D(get_x_value),
             nb::arg("x_index"))
        .def("get_x_value",
             make_vectorized_member<T_ForwardGridder1D, t_float, int>(&T_ForwardGridder1D::get_x_value),
             DOC_ForwardGridder1D(get_x_value),
             nb::arg("x_index").noconvert())
        .def("get_x_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder1D::get_x_grd_value, nb::const_),
             DOC_ForwardGridder1D(get_x_grd_value),
             nb::arg("x"))
        .def("get_x_grd_value",
             make_vectorized_member<T_ForwardGridder1D, t_float, t_float>(
                 &T_ForwardGridder1D::get_x_grd_value),
             DOC_ForwardGridder1D(get_x_grd_value),
             nb::arg("x").noconvert())
        .def("get_extent_x", &T_ForwardGridder1D::get_extent_x, DOC_ForwardGridder1D(get_extent_x))
        .def("get_extent",
             &T_ForwardGridder1D::get_extent,
             DOC_ForwardGridder1D(get_extent),
             nb::arg("axis") = "x")
        .def("get_x_coordinates",
             &T_ForwardGridder1D::get_x_coordinates,
             DOC_ForwardGridder1D(get_x_coordinates))
        .def_static("get_minmax",
                    nb::overload_cast<const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder1D::template get_minmax<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder1D(get_minmax),
                    nb::arg("sx"))
        .def("__repr__",
             [](const T_ForwardGridder1D& self) {
                 return "ForwardGridder1D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "])";
             })
        .def("__eq__",
             &T_ForwardGridder1D::operator==,
             DOC_ForwardGridder1D(operator_eq),
             nb::arg("other"))
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder1D)
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder1D)
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder1D)
        ;
}

void init_c_forwardgridder1d(nb::module_& m)
{
    init_ForwardGridder1D_float<double>(m, "");
    init_ForwardGridder1D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
