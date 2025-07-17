// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder2d.hpp>
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {

#define DOC_ForwardGridder2D(ARG)                                                                  \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder2D, ARG)

template<typename t_float>
void init_ForwardGridder2D_float(pybind11::module& m, const std::string& suffix)
{
    namespace py = pybind11;
    using namespace gridding;

    // Create the ForwardGridder2D class
    using T_ForwardGridder2D     = ForwardGridder2D<t_float>;
    const std::string class_name = std::string("ForwardGridder2D") + suffix;

    py::class_<T_ForwardGridder2D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder2D))
        // Constructors
        .def(py::init<t_float, t_float, t_float, t_float, t_float, t_float, t_float, t_float>(),
             DOC_ForwardGridder2D(ForwardGridder2D),
             py::arg("xres"),
             py::arg("yres"),
             py::arg("min_x"),
             py::arg("max_x"),
             py::arg("min_y"),
             py::arg("max_y"),
             py::arg("xbase") = 0.0,
             py::arg("ybase") = 0.0)

        // Factory methods
        .def_static("from_res",
                    py::overload_cast<t_float, t_float, t_float, t_float, t_float>(
                        &T_ForwardGridder2D::from_res),
                    DOC_ForwardGridder2D(from_res),
                    py::arg("res"),
                    py::arg("min_x"),
                    py::arg("max_x"),
                    py::arg("min_y"),
                    py::arg("max_y"))
        
        // from_data with t_float
        .def_static("from_data",
                    py::overload_cast<t_float,
                                      const xt::pytensor<t_float, 1>&,
                                      const xt::pytensor<t_float, 1>&>(
                        &T_ForwardGridder2D::template from_data<xt::pytensor<t_float, 1>>),
                    DOC_ForwardGridder2D(from_data),
                    py::arg("res"),
                    py::arg("sx"),
                    py::arg("sy"))
     

        // Grid functions with t_float and double variants
        .def("get_empty_grd_images",
             &T_ForwardGridder2D::template get_empty_grd_images<xt::pytensor<t_float, 2>>,
             DOC_ForwardGridder2D(get_empty_grd_images))

        .def("group_blocks",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template group_blocks<xt::pytensor<t_float, 1>>, py::const_),
             DOC_ForwardGridder2D(group_blocks),
             py::arg("sx").noconvert(),
             py::arg("sy").noconvert(),
             py::arg("sv").noconvert())

        // Interpolation functions - block mean (returns new arrays)
        .def("interpolate_block_mean",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template interpolate_block_mean<xt::pytensor<t_float, 2>,
                                                                      xt::pytensor<t_float, 1>>,
                 py::const_),
             DOC_ForwardGridder2D(interpolate_block_mean),
             py::arg("sx"),
             py::arg("sy"),
             py::arg("s_val"))

        // Interpolation functions - block mean inplace (modifies existing arrays)
        .def("interpolate_block_mean_inplace",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               xt::pytensor<t_float, 2>&,
                               xt::pytensor<t_float, 2>&>(
                 &T_ForwardGridder2D::template interpolate_block_mean_inplace<xt::pytensor<t_float, 2>,
                                                                            xt::pytensor<t_float, 1>>,
                 py::const_),
             DOC_ForwardGridder2D(interpolate_block_mean_inplace),
             py::arg("sx"),
             py::arg("sy"),
             py::arg("s_val"),
             py::arg("image_values").noconvert(),
             py::arg("image_weights").noconvert())

        // Weighted mean interpolation (returns new arrays)
        .def("interpolate_weighted_mean",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&>(
                 &T_ForwardGridder2D::template interpolate_weighted_mean<xt::pytensor<t_float, 2>,
                                                                         xt::pytensor<t_float, 1>>,
                 py::const_),
             DOC_ForwardGridder2D(interpolate_weighted_mean),
             py::arg("sx"),
             py::arg("sy"),
             py::arg("s_val"))

        // Weighted mean interpolation inplace (modifies existing arrays)
        .def("interpolate_weighted_mean_inplace",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               xt::pytensor<t_float, 2>&,
                               xt::pytensor<t_float, 2>&>(
                 &T_ForwardGridder2D::template interpolate_weighted_mean_inplace<xt::pytensor<t_float, 2>,
                                                                               xt::pytensor<t_float, 1>>,
                 py::const_),
             DOC_ForwardGridder2D(interpolate_weighted_mean_inplace),
             py::arg("sx"),
             py::arg("sy"),
             py::arg("s_val"),
             py::arg("image_values").noconvert(),
             py::arg("image_weights").noconvert())

        // Various utility methods
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
             py::vectorize(&T_ForwardGridder2D::get_x_index),
             DOC_ForwardGridder2D(get_x_index),
             py::arg("x"))
        .def("get_y_index",
             py::vectorize(&T_ForwardGridder2D::get_y_index),
             DOC_ForwardGridder2D(get_y_index),
             py::arg("y"))

        .def("get_x_index_fraction",
             py::vectorize(&T_ForwardGridder2D::get_x_index_fraction),
             DOC_ForwardGridder2D(get_x_index_fraction),
             py::arg("x"))
        .def("get_y_index_fraction",
             py::vectorize(&T_ForwardGridder2D::get_y_index_fraction),
             DOC_ForwardGridder2D(get_y_index_fraction),
             py::arg("y"))

        .def("get_x_value",
             py::vectorize(&T_ForwardGridder2D::get_x_value),
             DOC_ForwardGridder2D(get_x_value),
             py::arg("x_index"))
        .def("get_y_value",
             py::vectorize(&T_ForwardGridder2D::get_y_value),
             DOC_ForwardGridder2D(get_y_value),
             py::arg("y_index"))

        .def("get_x_grd_value",
             py::vectorize(&T_ForwardGridder2D::get_x_grd_value),
             DOC_ForwardGridder2D(get_x_grd_value),
             py::arg("x"))
        .def("get_y_grd_value",
             py::vectorize(&T_ForwardGridder2D::get_y_grd_value),
             DOC_ForwardGridder2D(get_y_grd_value),
             py::arg("y"))

        .def("get_extent_x", &T_ForwardGridder2D::get_extent_x, DOC_ForwardGridder2D(get_extent_x))
        .def("get_extent_y", &T_ForwardGridder2D::get_extent_y, DOC_ForwardGridder2D(get_extent_y))
        .def("get_extent",
             &T_ForwardGridder2D::get_extent,
             DOC_ForwardGridder2D(get_extent),
             py::arg("axis") = "xy")

        .def("get_x_coordinates",
             &T_ForwardGridder2D::get_x_coordinates,
             DOC_ForwardGridder2D(get_x_coordinates))
        .def("get_y_coordinates",
             &T_ForwardGridder2D::get_y_coordinates,
             DOC_ForwardGridder2D(get_y_coordinates))

        // Static utility methods - t_float version
        .def_static("get_minmax",
                    py::overload_cast<const xt::pytensor<t_float, 1>&, const xt::pytensor<t_float, 1>&>(
                        &T_ForwardGridder2D::template get_minmax<xt::pytensor<t_float, 1>>),
                    DOC_ForwardGridder2D(get_minmax),
                    py::arg("sx"),
                    py::arg("sy"))


        // Basic string representation
        .def("__repr__",
             [](const T_ForwardGridder2D& self) {
                 return "ForwardGridder2D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "," +
                        std::to_string(self.get_y_coordinates().size()) + "])";
             })

        // ----- operators -----
        .def("__eq__",
             &T_ForwardGridder2D::operator==,
             DOC_ForwardGridder2D(operator_eq),
             py::arg("other"))
        // ----- pybind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder2D)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder2D)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder2D)
        // end ForwardGridder2D
        ;
}

void init_c_forwardgridder2d(pybind11::module& m)
{
    init_ForwardGridder2D_float<double>(m, "");
    init_ForwardGridder2D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
