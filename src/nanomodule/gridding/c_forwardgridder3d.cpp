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

#include <themachinethatgoesping/algorithms/gridding/forwardgridder3d.hpp>

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

#define DOC_ForwardGridder3D(ARG)                                                                  \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder3D, ARG)

template<typename t_float>
void init_ForwardGridder3D_float(nb::module_& m, const std::string& suffix)
{
    using T_ForwardGridder3D     = ForwardGridder3D<t_float>;
    const std::string class_name = std::string("ForwardGridder3D") + suffix;

    nb::class_<T_ForwardGridder3D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder3D))
        .def(nb::init<t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float,
                      t_float>(),
             DOC_ForwardGridder3D(ForwardGridder3D),
             nb::arg("xres"),
             nb::arg("yres"),
             nb::arg("zres"),
             nb::arg("min_x"),
             nb::arg("max_x"),
             nb::arg("min_y"),
             nb::arg("max_y"),
             nb::arg("min_z"),
             nb::arg("max_z"),
             nb::arg("xbase") = 0.0,
             nb::arg("ybase") = 0.0,
             nb::arg("zbase") = 0.0)
        .def_static(
            "from_res",
            nb::overload_cast<t_float, t_float, t_float, t_float, t_float, t_float, t_float>(
                &T_ForwardGridder3D::from_res),
            DOC_ForwardGridder3D(from_res),
            nb::arg("res"),
            nb::arg("min_x"),
            nb::arg("max_x"),
            nb::arg("min_y"),
            nb::arg("max_y"),
            nb::arg("min_z"),
            nb::arg("max_z"))
        .def_static("from_data",
                    nb::overload_cast<t_float,
                                      const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder3D::template from_data<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder3D(from_data),
                    nb::arg("res"),
                    nb::arg("sx"),
                    nb::arg("sy"),
                    nb::arg("sz"))
        .def("get_empty_grd_images",
             &T_ForwardGridder3D::template get_empty_grd_images<xt::nanobind::pytensor<t_float, 3>>,
             DOC_ForwardGridder3D(get_empty_grd_images))
        .def("group_blocks",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder3D::template group_blocks<xtnb::pytensor<t_float, 1>>, nb::const_),
             DOC_ForwardGridder3D(group_blocks),
             nb::arg("sx").noconvert(),
             nb::arg("sy").noconvert(),
             nb::arg("sz").noconvert(),
             nb::arg("sv").noconvert())
        .def("interpolate_block_mean",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder3D::template interpolate_block_mean<xt::nanobind::pytensor<t_float, 3>,
                                                                      xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder3D(interpolate_block_mean),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("sz"),
             nb::arg("s_val"))
        .def("interpolate_block_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 3>&,
                               xtnb::pytensor<t_float, 3>&>(
                 &T_ForwardGridder3D::template interpolate_block_mean_inplace<
                     xtnb::pytensor<t_float, 3>,
                     xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder3D(interpolate_block_mean_inplace),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("sz"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
        .def("interpolate_weighted_mean",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&>(
                 &T_ForwardGridder3D::template interpolate_weighted_mean<xt::nanobind::pytensor<t_float, 3>,
                                                                         xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder3D(interpolate_weighted_mean),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("sz"),
             nb::arg("s_val"))
        .def("interpolate_weighted_mean_inplace",
             nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               const xtnb::pytensor<t_float, 1>&,
                               xtnb::pytensor<t_float, 3>&,
                               xtnb::pytensor<t_float, 3>&>(
                 &T_ForwardGridder3D::template interpolate_weighted_mean_inplace<
                     xtnb::pytensor<t_float, 3>,
                     xtnb::pytensor<t_float, 1>>,
                 nb::const_),
             DOC_ForwardGridder3D(interpolate_weighted_mean_inplace),
             nb::arg("sx"),
             nb::arg("sy"),
             nb::arg("sz"),
             nb::arg("s_val"),
             nb::arg("image_values").noconvert(),
             nb::arg("image_weights").noconvert())
        .def("get_xres", &T_ForwardGridder3D::get_xres, DOC_ForwardGridder3D(xres))
        .def("get_yres", &T_ForwardGridder3D::get_yres, DOC_ForwardGridder3D(yres))
        .def("get_zres", &T_ForwardGridder3D::get_zres, DOC_ForwardGridder3D(zres))
        .def("get_xmin", &T_ForwardGridder3D::get_xmin, DOC_ForwardGridder3D(xmin))
        .def("get_xmax", &T_ForwardGridder3D::get_xmax, DOC_ForwardGridder3D(xmax))
        .def("get_ymin", &T_ForwardGridder3D::get_ymin, DOC_ForwardGridder3D(ymin))
        .def("get_ymax", &T_ForwardGridder3D::get_ymax, DOC_ForwardGridder3D(ymax))
        .def("get_zmin", &T_ForwardGridder3D::get_zmin, DOC_ForwardGridder3D(zmin))
        .def("get_zmax", &T_ForwardGridder3D::get_zmax, DOC_ForwardGridder3D(zmax))
        .def("get_xbase", &T_ForwardGridder3D::get_xbase, DOC_ForwardGridder3D(xbase))
        .def("get_ybase", &T_ForwardGridder3D::get_ybase, DOC_ForwardGridder3D(ybase))
        .def("get_zbase", &T_ForwardGridder3D::get_zbase, DOC_ForwardGridder3D(zbase))
        .def("get_nx", &T_ForwardGridder3D::get_nx, DOC_ForwardGridder3D(nx))
        .def("get_ny", &T_ForwardGridder3D::get_ny, DOC_ForwardGridder3D(ny))
        .def("get_nz", &T_ForwardGridder3D::get_nz, DOC_ForwardGridder3D(nz))
        .def("get_border_xmin",
             &T_ForwardGridder3D::get_border_xmin,
             DOC_ForwardGridder3D(border_xmin))
        .def("get_border_xmax",
             &T_ForwardGridder3D::get_border_xmax,
             DOC_ForwardGridder3D(border_xmax))
        .def("get_border_ymin",
             &T_ForwardGridder3D::get_border_ymin,
             DOC_ForwardGridder3D(border_ymin))
        .def("get_border_ymax",
             &T_ForwardGridder3D::get_border_ymax,
             DOC_ForwardGridder3D(border_ymax))
        .def("get_border_zmin",
             &T_ForwardGridder3D::get_border_zmin,
             DOC_ForwardGridder3D(border_zmin))
        .def("get_border_zmax",
             &T_ForwardGridder3D::get_border_zmax,
             DOC_ForwardGridder3D(border_zmax))
        .def("get_x_index",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_x_index, nb::const_),
             DOC_ForwardGridder3D(get_x_index),
             nb::arg("x"))
        .def("get_x_index",
             make_vectorized_member<T_ForwardGridder3D, int, t_float>(
                 &T_ForwardGridder3D::get_x_index),
             DOC_ForwardGridder3D(get_x_index),
             nb::arg("x").noconvert())
        .def("get_y_index",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_y_index, nb::const_),
             DOC_ForwardGridder3D(get_y_index),
             nb::arg("y"))
        .def("get_y_index",
             make_vectorized_member<T_ForwardGridder3D, int, t_float>(
                 &T_ForwardGridder3D::get_y_index),
             DOC_ForwardGridder3D(get_y_index),
             nb::arg("y").noconvert())
        .def("get_z_index",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_z_index, nb::const_),
             DOC_ForwardGridder3D(get_z_index),
             nb::arg("z"))
        .def("get_z_index",
             make_vectorized_member<T_ForwardGridder3D, int, t_float>(
                 &T_ForwardGridder3D::get_z_index),
             DOC_ForwardGridder3D(get_z_index),
             nb::arg("z").noconvert())
        .def("get_x_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_x_index_fraction, nb::const_),
             DOC_ForwardGridder3D(get_x_index_fraction),
             nb::arg("x"))
        .def("get_x_index_fraction",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_x_index_fraction),
             DOC_ForwardGridder3D(get_x_index_fraction),
             nb::arg("x").noconvert())
        .def("get_y_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_y_index_fraction, nb::const_),
             DOC_ForwardGridder3D(get_y_index_fraction),
             nb::arg("y"))
        .def("get_y_index_fraction",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_y_index_fraction),
             DOC_ForwardGridder3D(get_y_index_fraction),
             nb::arg("y").noconvert())
        .def("get_z_index_fraction",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_z_index_fraction, nb::const_),
             DOC_ForwardGridder3D(get_z_index_fraction),
             nb::arg("z"))
        .def("get_z_index_fraction",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_z_index_fraction),
             DOC_ForwardGridder3D(get_z_index_fraction),
             nb::arg("z").noconvert())
        .def("get_x_value",
             nb::overload_cast<int>(&T_ForwardGridder3D::get_x_value, nb::const_),
             DOC_ForwardGridder3D(get_x_value),
             nb::arg("x_index"))
        .def("get_x_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, int>(
                 &T_ForwardGridder3D::get_x_value),
             DOC_ForwardGridder3D(get_x_value),
             nb::arg("x_index").noconvert())
        .def("get_y_value",
             nb::overload_cast<int>(&T_ForwardGridder3D::get_y_value, nb::const_),
             DOC_ForwardGridder3D(get_y_value),
             nb::arg("y_index"))
        .def("get_y_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, int>(
                 &T_ForwardGridder3D::get_y_value),
             DOC_ForwardGridder3D(get_y_value),
             nb::arg("y_index").noconvert())
        .def("get_z_value",
             nb::overload_cast<int>(&T_ForwardGridder3D::get_z_value, nb::const_),
             DOC_ForwardGridder3D(get_z_value),
             nb::arg("z_index"))
        .def("get_z_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, int>(
                 &T_ForwardGridder3D::get_z_value),
             DOC_ForwardGridder3D(get_z_value),
             nb::arg("z_index").noconvert())
        .def("get_x_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_x_grd_value, nb::const_),
             DOC_ForwardGridder3D(get_x_grd_value),
             nb::arg("x"))
        .def("get_x_grd_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_x_grd_value),
             DOC_ForwardGridder3D(get_x_grd_value),
             nb::arg("x").noconvert())
        .def("get_y_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_y_grd_value, nb::const_),
             DOC_ForwardGridder3D(get_y_grd_value),
             nb::arg("y"))
        .def("get_y_grd_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_y_grd_value),
             DOC_ForwardGridder3D(get_y_grd_value),
             nb::arg("y").noconvert())
        .def("get_z_grd_value",
             nb::overload_cast<t_float>(&T_ForwardGridder3D::get_z_grd_value, nb::const_),
             DOC_ForwardGridder3D(get_z_grd_value),
             nb::arg("z"))
        .def("get_z_grd_value",
             make_vectorized_member<T_ForwardGridder3D, t_float, t_float>(
                 &T_ForwardGridder3D::get_z_grd_value),
             DOC_ForwardGridder3D(get_z_grd_value),
             nb::arg("z").noconvert())
        .def("get_extent_x", &T_ForwardGridder3D::get_extent_x, DOC_ForwardGridder3D(get_extent_x))
        .def("get_extent_y", &T_ForwardGridder3D::get_extent_y, DOC_ForwardGridder3D(get_extent_y))
        .def("get_extent_z", &T_ForwardGridder3D::get_extent_z, DOC_ForwardGridder3D(get_extent_z))
        .def("get_extent",
             &T_ForwardGridder3D::get_extent,
             DOC_ForwardGridder3D(get_extent),
             nb::arg("axis") = "xyz")
        .def("get_x_coordinates",
             &T_ForwardGridder3D::get_x_coordinates,
             DOC_ForwardGridder3D(get_x_coordinates))
        .def("get_y_coordinates",
             &T_ForwardGridder3D::get_y_coordinates,
             DOC_ForwardGridder3D(get_y_coordinates))
        .def("get_z_coordinates",
             &T_ForwardGridder3D::get_z_coordinates,
             DOC_ForwardGridder3D(get_z_coordinates))
        .def_static("get_minmax",
                    nb::overload_cast<const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&,
                                      const xtnb::pytensor<t_float, 1>&>(
                        &T_ForwardGridder3D::template get_minmax<xtnb::pytensor<t_float, 1>>),
                    DOC_ForwardGridder3D(get_minmax),
                    nb::arg("sx"),
                    nb::arg("sy"),
                    nb::arg("sz"))
        .def("__repr__",
             [](const T_ForwardGridder3D& self) {
                 return "ForwardGridder3D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "," +
                        std::to_string(self.get_y_coordinates().size()) + "," +
                        std::to_string(self.get_z_coordinates().size()) + "])";
             })
        .def("__eq__",
             &T_ForwardGridder3D::operator==,
             DOC_ForwardGridder3D(operator_eq),
             nb::arg("other"))
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder3D)
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder3D)
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder3D)
        ;
}

void init_c_forwardgridder3d(nb::module_& m)
{
    init_ForwardGridder3D_float<double>(m, "");
    init_ForwardGridder3D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
