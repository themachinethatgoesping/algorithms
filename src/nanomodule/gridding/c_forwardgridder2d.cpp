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
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder2d.hpp>

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
          xt::nanobind::pytensor<ReturnType, 1> result(values.shape());
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

#define DOC_ForwardGridder2D(ARG)                                                                  \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder2D, ARG)

template<typename t_float>
void init_ForwardGridder2D_float(nb::module_& m, const std::string& suffix)
{
    using T_ForwardGridder2D     = ForwardGridder2D<t_float>;
    const std::string class_name = std::string("ForwardGridder2D") + suffix;

    nb::class_<T_ForwardGridder2D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder2D))
        .def(nb::init<t_float, t_float, t_float, t_float, t_float, t_float, t_float, t_float>(),
             DOC_ForwardGridder2D(ForwardGridder2D),
             nb::arg("xres"),
             nb::arg("yres"),
             nb::arg("min_x"),
             nb::arg("max_x"),
             nb::arg("min_y"),
             nb::arg("max_y"),
             nb::arg("xbase") = 0.0,
             nb::arg("ybase") = 0.0)
        .def_static("from_res",
                    nb::overload_cast<t_float, t_float, t_float, t_float, t_float>(
                        &T_ForwardGridder2D::from_res),
                    DOC_ForwardGridder2D(from_res),
                    nb::arg("res"),
                    nb::arg("min_x"),
                    nb::arg("max_x"),
                    nb::arg("min_y"),
                    nb::arg("max_y"))
        .def_static("from_data",
                    nb::overload_cast<t_float,
                                      const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder2D::template from_data<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder2D(from_data),
                    nb::arg("res"),
                    nb::arg("sx"),
                    nb::arg("sy"))
        .def("get_empty_grd_images",
             &T_ForwardGridder2D::template get_empty_grd_images<xt::nanobind::pytensor<t_float, 2>>,
             DOC_ForwardGridder2D(get_empty_grd_images))
        .def("group_blocks",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template group_blocks<xtnb::pytensor<t_float, 1>>, nb::const_),
             DOC_ForwardGridder2D(group_blocks),
             nb::arg("sx").noconvert(),
             nb::arg("sy").noconvert(),
             nb::arg("sv").noconvert())
        .def("interpolate_block_mean",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template interpolate_block_mean<xt::nanobind::pytensor<t_float, 2>,
                                                                      xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder2D(interpolate_block_mean),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("s_val"))
        .def("interpolate_block_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 2>&,
                               xtnb::pytensor<t_float, 2>&>(
                 &T_ForwardGridder2D::template interpolate_block_mean_inplace<xtnb::pytensor<t_float, 2>,
                                                                            xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder2D(interpolate_block_mean_inplace),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
        .def("interpolate_weighted_mean",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template interpolate_weighted_mean<xt::nanobind::pytensor<t_float, 2>,
                                                                         xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder2D(interpolate_weighted_mean),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("s_val"))
        .def("interpolate_weighted_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 2>&,
                               xtnb::pytensor<t_float, 2>&>(
                 &T_ForwardGridder2D::template interpolate_weighted_mean_inplace<
                     xtnb::pytensor<t_float, 2>,
                     xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder2D(interpolate_weighted_mean_inplace),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
        .def("get_xres", &T_ForwardGridder2D::get_xres, DOC_ForwardGridder2D(xres))
        .def("get_yres", &T_ForwardGridder2D::get_yres, DOC_ForwardGridder2D(yres))
        .def("get_xmin", &T_ForwardGridder2D::get_xmin, DOC_ForwardGridder2D(xmin))
        .def("get_xmax", &T_ForwardGridder2D::get_xmax, DOC_ForwardGridder2D(xmax))
        .def("get_ymin", &T_ForwardGridder2D::get_ymin, DOC_ForwardGridder2D(ymin))
        .def("get_ymax", &T_ForwardGridder2D::get_ymax, DOC_ForwardGridder2D(ymax))
        .def("get_xbase", &T_ForwardGridder2D::get_xbase, DOC_ForwardGridder2D(xbase))
        .def("get_ybase", &T_ForwardGridder2D::get_ybase, DOC_ForwardGridder2D(ybase))
        .def("get_nx", &T_ForwardGridder2D::get_nx, DOC_ForwardGridder2D(nx))
        .def("get_ny", &T_ForwardGridder2D::get_ny, DOC_ForwardGridder2D(ny))
        .def("get_border_xmin",
             &T_ForwardGridder2D::get_border_xmin,
             DOC_ForwardGridder2D(border_xmin))
        .def("get_border_xmax",
             &T_ForwardGridder2D::get_border_xmax,
             DOC_ForwardGridder2D(border_xmax))
        .def("get_border_ymin",
             &T_ForwardGridder2D::get_border_ymin,
             DOC_ForwardGridder2D(border_ymin))
        .def("get_border_ymax",
             &T_ForwardGridder2D::get_border_ymax,
             DOC_ForwardGridder2D(border_ymax))
        .def("get_x_index",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_x_index, nb::const_),
             DOC_ForwardGridder2D(get_x_index),
             nb::arg("x"))
        .def("get_x_index",
             make_vectorized_member<T_ForwardGridder2D, int, t_float>(
                 &T_ForwardGridder2D::get_x_index),
             DOC_ForwardGridder2D(get_x_index),
             nb::arg("x").noconvert())
        .def("get_y_index",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_y_index, nb::const_),
             DOC_ForwardGridder2D(get_y_index),
             nb::arg("y"))
        .def("get_y_index",
             make_vectorized_member<T_ForwardGridder2D, int, t_float>(
                 &T_ForwardGridder2D::get_y_index),
             DOC_ForwardGridder2D(get_y_index),
             nb::arg("y").noconvert())
        .def("get_x_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_x_index_fraction, nb::const_),
             DOC_ForwardGridder2D(get_x_index_fraction),
             nb::arg("x"))
        .def("get_x_index_fraction",
             make_vectorized_member<T_ForwardGridder2D, t_float, t_float>(
                 &T_ForwardGridder2D::get_x_index_fraction),
             DOC_ForwardGridder2D(get_x_index_fraction),
             nb::arg("x").noconvert())
        .def("get_y_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_y_index_fraction, nb::const_),
             DOC_ForwardGridder2D(get_y_index_fraction),
             nb::arg("y"))
        .def("get_y_index_fraction",
             make_vectorized_member<T_ForwardGridder2D, t_float, t_float>(
                 &T_ForwardGridder2D::get_y_index_fraction),
             DOC_ForwardGridder2D(get_y_index_fraction),
             nb::arg("y").noconvert())
        .def("get_x_value",
             nb::overload_cast<int>(&T_ForwardGridder2D::get_x_value, nb::const_),
             DOC_ForwardGridder2D(get_x_value),
             nb::arg("x_index"))
        .def("get_x_value",
             make_vectorized_member<T_ForwardGridder2D, t_float, int>(
                 &T_ForwardGridder2D::get_x_value),
             DOC_ForwardGridder2D(get_x_value),
             nb::arg("x_index").noconvert())
        .def("get_y_value",
             nb::overload_cast<int>(&T_ForwardGridder2D::get_y_value, nb::const_),
             DOC_ForwardGridder2D(get_y_value),
             nb::arg("y_index"))
        .def("get_y_value",
             make_vectorized_member<T_ForwardGridder2D, t_float, int>(
                 &T_ForwardGridder2D::get_y_value),
             DOC_ForwardGridder2D(get_y_value),
             nb::arg("y_index").noconvert())
        .def("get_x_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_x_grd_value, nb::const_),
             DOC_ForwardGridder2D(get_x_grd_value),
             nb::arg("x"))
        .def("get_x_grd_value",
             make_vectorized_member<T_ForwardGridder2D, t_float, t_float>(
                 &T_ForwardGridder2D::get_x_grd_value),
             DOC_ForwardGridder2D(get_x_grd_value),
             nb::arg("x").noconvert())
        .def("get_y_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder2D::get_y_grd_value, nb::const_),
             DOC_ForwardGridder2D(get_y_grd_value),
             nb::arg("y"))
        .def("get_y_grd_value",
             make_vectorized_member<T_ForwardGridder2D, t_float, t_float>(
                 &T_ForwardGridder2D::get_y_grd_value),
             DOC_ForwardGridder2D(get_y_grd_value),
             nb::arg("y").noconvert())
        .def("get_extent_x", &T_ForwardGridder2D::get_extent_x, DOC_ForwardGridder2D(get_extent_x))
        .def("get_extent_y", &T_ForwardGridder2D::get_extent_y, DOC_ForwardGridder2D(get_extent_y))
        .def("get_extent",
             &T_ForwardGridder2D::get_extent,
             DOC_ForwardGridder2D(get_extent),
             nb::arg("axis") = "xy")
        .def("get_x_coordinates",
             &T_ForwardGridder2D::get_x_coordinates,
             DOC_ForwardGridder2D(get_x_coordinates))
        .def("get_y_coordinates",
             &T_ForwardGridder2D::get_y_coordinates,
             DOC_ForwardGridder2D(get_y_coordinates))
        .def_static("get_minmax",
                    nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder2D::template get_minmax<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder2D(get_minmax),
                    nb::arg("sx"),
                    nb::arg("sy"))
        .def("__repr__",
             [](const T_ForwardGridder2D& self) {
                 return "ForwardGridder2D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "," +
                        std::to_string(self.get_y_coordinates().size()) + "])";
             })
        .def("__eq__",
             &T_ForwardGridder2D::operator==,
             DOC_ForwardGridder2D(operator_eq),
             nb::arg("other"))
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder2D)
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder2D)
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder2D)
        ;
}

void init_c_forwardgridder2d(nb::module_& m)
{
    init_ForwardGridder2D_float<double>(m, "");
    init_ForwardGridder2D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
