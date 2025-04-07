// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder3d.hpp>
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {

#define DOC_ForwardGridder3D(ARG)                                                                    \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder3D, ARG)

template<typename t_float>
void init_ForwardGridder3D_float(pybind11::module& m, const std::string& suffix)
{
    namespace py = pybind11;
    using namespace gridding;

    // Create the ForwardGridder3D class
    using T_ForwardGridder3D       = ForwardGridder3D<t_float>;
    const std::string class_name = std::string("ForwardGridder3D") + suffix;

    py::class_<T_ForwardGridder3D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder3D))
        // Constructors
        .def(py::init<t_float,
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
             py::arg("xres"),
             py::arg("yres"),
             py::arg("zres"),
             py::arg("min_x"),
             py::arg("max_x"),
             py::arg("min_y"),
             py::arg("max_y"),
             py::arg("min_z"),
             py::arg("max_z"),
             py::arg("xbase") = 0.0,
             py::arg("ybase") = 0.0,
             py::arg("zbase") = 0.0)

        // Factory methods
        .def_static("from_res",
                    &T_ForwardGridder3D::from_res,
                    DOC_ForwardGridder3D(from_res),
                    py::arg("res"),
                    py::arg("min_x"),
                    py::arg("max_x"),
                    py::arg("min_y"),
                    py::arg("max_y"),
                    py::arg("min_z"),
                    py::arg("max_z"))
        .def_static(
            "from_data",
            [](t_float                         res,
               const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& sy,
               const xt::pytensor<t_float, 1>& sz) {
                return T_ForwardGridder3D::from_data(res, sx, sy, sz);
            },
            DOC_ForwardGridder3D(from_data),
            py::arg("res"),
            py::arg("sx").noconvert(),
            py::arg("sy").noconvert(),
            py::arg("sz").noconvert())

        // Grid functions
        .def(
            "get_empty_grd_images",
            [](const T_ForwardGridder3D& self) {
                return self.template get_empty_grd_images<xt::pytensor<t_float, 3>>();
            },
            DOC_ForwardGridder3D(get_empty_grd_images))

        // Interpolation functions
        .def(
            "interpolate_block_mean",
            [](const T_ForwardGridder3D&         self,
               const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& sy,
               const xt::pytensor<t_float, 1>& sz,
               const xt::pytensor<t_float, 1>& s_val,
               xt::pytensor<t_float, 3>        image_values,
               xt::pytensor<t_float, 3>        image_weights) {
                return self.template interpolate_block_mean<xt::pytensor<t_float, 3>,
                                                            xt::pytensor<t_float, 1>>(
                    sx, sy, sz, s_val, image_values, image_weights);
            },
            DOC_ForwardGridder3D(interpolate_block_mean),
            py::arg("sx").noconvert(),
            py::arg("sy").noconvert(),
            py::arg("sz").noconvert(),
            py::arg("s_val").noconvert(),
            py::arg("image_values")  = xt::pytensor<t_float, 3>(),
            py::arg("image_weights") = xt::pytensor<t_float, 3>())

        .def(
            "interpolate_weighted_mean",
            [](const T_ForwardGridder3D&         self,
               const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& sy,
               const xt::pytensor<t_float, 1>& sz,
               const xt::pytensor<t_float, 1>& s_val,
               xt::pytensor<t_float, 3>        image_values,
               xt::pytensor<t_float, 3>        image_weights) {
                return self.template interpolate_weighted_mean<xt::pytensor<t_float, 3>,
                                                               xt::pytensor<t_float, 1>>(
                    sx, sy, sz, s_val, image_values, image_weights);
            },
            DOC_ForwardGridder3D(interpolate_weighted_mean),
            py::arg("sx").noconvert(),
            py::arg("sy").noconvert(),
            py::arg("sz").noconvert(),
            py::arg("s_val").noconvert(),
            py::arg("image_values")  = xt::pytensor<t_float, 3>(),
            py::arg("image_weights") = xt::pytensor<t_float, 3>())

        // Various utility methods
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
        .def("get_border_xmin", &T_ForwardGridder3D::get_border_xmin, DOC_ForwardGridder3D(border_xmin))
        .def("get_border_xmax", &T_ForwardGridder3D::get_border_xmax, DOC_ForwardGridder3D(border_xmax))
        .def("get_border_ymin", &T_ForwardGridder3D::get_border_ymin, DOC_ForwardGridder3D(border_ymin))
        .def("get_border_ymax", &T_ForwardGridder3D::get_border_ymax, DOC_ForwardGridder3D(border_ymax))
        .def("get_border_zmin", &T_ForwardGridder3D::get_border_zmin, DOC_ForwardGridder3D(border_zmin))
        .def("get_border_zmax", &T_ForwardGridder3D::get_border_zmax, DOC_ForwardGridder3D(border_zmax))

        .def("get_x_index",
             &T_ForwardGridder3D::get_x_index,
             DOC_ForwardGridder3D(get_x_index),
             py::arg("x"))
        .def("get_y_index",
             &T_ForwardGridder3D::get_y_index,
             DOC_ForwardGridder3D(get_y_index),
             py::arg("y"))
        .def("get_z_index",
             &T_ForwardGridder3D::get_z_index,
             DOC_ForwardGridder3D(get_z_index),
             py::arg("z"))

        .def("get_x_index_fraction",
             &T_ForwardGridder3D::get_x_index_fraction,
             DOC_ForwardGridder3D(get_x_index_fraction),
             py::arg("x"))
        .def("get_y_index_fraction",
             &T_ForwardGridder3D::get_y_index_fraction,
             DOC_ForwardGridder3D(get_y_index_fraction),
             py::arg("y"))
        .def("get_z_index_fraction",
             &T_ForwardGridder3D::get_z_index_fraction,
             DOC_ForwardGridder3D(get_z_index_fraction),
             py::arg("z"))

        .def("get_x_value",
             &T_ForwardGridder3D::get_x_value,
             DOC_ForwardGridder3D(get_x_value),
             py::arg("x_index"))
        .def("get_y_value",
             &T_ForwardGridder3D::get_y_value,
             DOC_ForwardGridder3D(get_y_value),
             py::arg("y_index"))
        .def("get_z_value",
             &T_ForwardGridder3D::get_z_value,
             DOC_ForwardGridder3D(get_z_value),
             py::arg("z_index"))

        .def("get_x_grd_value",
             &T_ForwardGridder3D::get_x_grd_value,
             DOC_ForwardGridder3D(get_x_grd_value),
             py::arg("x"))
        .def("get_y_grd_value",
             &T_ForwardGridder3D::get_y_grd_value,
             DOC_ForwardGridder3D(get_y_grd_value),
             py::arg("y"))
        .def("get_z_grd_value",
             &T_ForwardGridder3D::get_z_grd_value,
             DOC_ForwardGridder3D(get_z_grd_value),
             py::arg("z"))

        .def("get_extent_x", &T_ForwardGridder3D::get_extent_x, DOC_ForwardGridder3D(get_extent_x))
        .def("get_extent_y", &T_ForwardGridder3D::get_extent_y, DOC_ForwardGridder3D(get_extent_y))
        .def("get_extent_z", &T_ForwardGridder3D::get_extent_z, DOC_ForwardGridder3D(get_extent_z))
        .def("get_extent",
             &T_ForwardGridder3D::get_extent,
             DOC_ForwardGridder3D(get_extent),
             py::arg("axis") = "xyz")

        .def("get_x_coordinates",
             &T_ForwardGridder3D::get_x_coordinates,
             DOC_ForwardGridder3D(get_x_coordinates))
        .def("get_y_coordinates",
             &T_ForwardGridder3D::get_y_coordinates,
             DOC_ForwardGridder3D(get_y_coordinates))
        .def("get_z_coordinates",
             &T_ForwardGridder3D::get_z_coordinates,
             DOC_ForwardGridder3D(get_z_coordinates))

        // Static utility methods
        .def_static(
            "get_minmax",
            [](const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& sy,
               const xt::pytensor<t_float, 1>& sz) {
                return T_ForwardGridder3D::get_minmax(sx, sy, sz);
            },
            DOC_ForwardGridder3D(get_minmax),
            py::arg("sx").noconvert(),
            py::arg("sy").noconvert(),
            py::arg("sz").noconvert())

        // Basic string representation
        .def("__repr__",
             [](const T_ForwardGridder3D& self) {
                 return "ForwardGridder3D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "," +
                        std::to_string(self.get_y_coordinates().size()) + "," +
                        std::to_string(self.get_z_coordinates().size()) + "])";
             })

        // ----- operators -----
        .def("__eq__",
             &T_ForwardGridder3D::operator==,
             DOC_ForwardGridder3D(operator_eq),
             py::arg("other"))
        // ----- pybind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder3D)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder3D)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder3D)
        // end LinearInterpolator
        ;
    ;
}

void init_c_forwardgridder3d(pybind11::module& m)
{
    init_ForwardGridder3D_float<double>(m, "");
    init_ForwardGridder3D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
