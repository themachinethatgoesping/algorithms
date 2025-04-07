// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder1d.hpp>
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {

#define DOC_ForwardGridder1D(ARG)                                                                    \
    DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder1D, ARG)

template<typename t_float>
void init_ForwardGridder1D_float(pybind11::module& m, const std::string& suffix)
{
    namespace py = pybind11;
    using namespace gridding;

    // Create the ForwardGridder1D class
    using T_ForwardGridder1D       = ForwardGridder1D<t_float>;
    const std::string class_name = std::string("ForwardGridder1D") + suffix;

    py::class_<T_ForwardGridder1D>(
        m, class_name.c_str(), DOC(themachinethatgoesping, algorithms, gridding, ForwardGridder1D))
        // Constructors
        .def(py::init<t_float,
                      t_float,
                      t_float,
                      t_float>(),
             DOC_ForwardGridder1D(ForwardGridder1D),
             py::arg("xres"),
             py::arg("min_x"),
             py::arg("max_x"),
             py::arg("xbase") = 0.0)

        // Factory methods
        .def_static("from_res",
                    &T_ForwardGridder1D::from_res,
                    DOC_ForwardGridder1D(from_res),
                    py::arg("res"),
                    py::arg("min_x"),
                    py::arg("max_x"))
        .def_static(
            "from_data",
            [](t_float                         res,
               const xt::pytensor<t_float, 1>& sx) {
                return T_ForwardGridder1D::from_data(res, sx);
            },
            DOC_ForwardGridder1D(from_data),
            py::arg("res"),
            py::arg("sx").noconvert())

        // Grid functions
        .def(
            "get_empty_grd_images",
            [](const T_ForwardGridder1D& self) {
                return self.template get_empty_grd_images<xt::pytensor<t_float, 1>>();
            },
            DOC_ForwardGridder1D(get_empty_grd_images))

        // Interpolation functions
        .def(
            "interpolate_block_mean",
            [](const T_ForwardGridder1D&         self,
               const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& s_val,
               xt::pytensor<t_float, 1>        image_values,
               xt::pytensor<t_float, 1>        image_weights) {
                return self.template interpolate_block_mean<xt::pytensor<t_float, 1>,
                                                            xt::pytensor<t_float, 1>>(
                    sx, s_val, image_values, image_weights);
            },
            DOC_ForwardGridder1D(interpolate_block_mean),
            py::arg("sx").noconvert(),
            py::arg("s_val").noconvert(),
            py::arg("image_values")  = xt::pytensor<t_float, 1>(),
            py::arg("image_weights") = xt::pytensor<t_float, 1>())

        .def(
            "interpolate_weighted_mean",
            [](const T_ForwardGridder1D&         self,
               const xt::pytensor<t_float, 1>& sx,
               const xt::pytensor<t_float, 1>& s_val,
               xt::pytensor<t_float, 1>        image_values,
               xt::pytensor<t_float, 1>        image_weights) {
                return self.template interpolate_weighted_mean<xt::pytensor<t_float, 1>,
                                                               xt::pytensor<t_float, 1>>(
                    sx, s_val, image_values, image_weights);
            },
            DOC_ForwardGridder1D(interpolate_weighted_mean),
            py::arg("sx").noconvert(),
            py::arg("s_val").noconvert(),
            py::arg("image_values")  = xt::pytensor<t_float, 1>(),
            py::arg("image_weights") = xt::pytensor<t_float, 1>())

        // Various utility methods
        .def("get_xres", &T_ForwardGridder1D::get_xres, DOC_ForwardGridder1D(xres))
        .def("get_xmin", &T_ForwardGridder1D::get_xmin, DOC_ForwardGridder1D(xmin))
        .def("get_xmax", &T_ForwardGridder1D::get_xmax, DOC_ForwardGridder1D(xmax))
        .def("get_xbase", &T_ForwardGridder1D::get_xbase, DOC_ForwardGridder1D(xbase))
        .def("get_nx", &T_ForwardGridder1D::get_nx, DOC_ForwardGridder1D(nx))
        .def("get_border_xmin", &T_ForwardGridder1D::get_border_xmin, DOC_ForwardGridder1D(border_xmin))
        .def("get_border_xmax", &T_ForwardGridder1D::get_border_xmax, DOC_ForwardGridder1D(border_xmax))

        .def("get_x_index",
             &T_ForwardGridder1D::get_x_index,
             DOC_ForwardGridder1D(get_x_index),
             py::arg("x"))

        .def("get_x_index_fraction",
             &T_ForwardGridder1D::get_x_index_fraction,
             DOC_ForwardGridder1D(get_x_index_fraction),
             py::arg("x"))

        .def("get_x_value",
             &T_ForwardGridder1D::get_x_value,
             DOC_ForwardGridder1D(get_x_value),
             py::arg("x_index"))

        .def("get_x_grd_value",
             &T_ForwardGridder1D::get_x_grd_value,
             DOC_ForwardGridder1D(get_x_grd_value),
             py::arg("x"))

        .def("get_extent_x", &T_ForwardGridder1D::get_extent_x, DOC_ForwardGridder1D(get_extent_x))
        .def("get_extent",
             &T_ForwardGridder1D::get_extent,
             DOC_ForwardGridder1D(get_extent),
             py::arg("axis") = "x")

        .def("get_x_coordinates",
             &T_ForwardGridder1D::get_x_coordinates,
             DOC_ForwardGridder1D(get_x_coordinates))

        // Static utility methods
        .def_static(
            "get_minmax",
            [](const xt::pytensor<t_float, 1>& sx) {
                return T_ForwardGridder1D::get_minmax(sx);
            },
            DOC_ForwardGridder1D(get_minmax),
            py::arg("sx").noconvert())

        // Basic string representation
        .def("__repr__",
             [](const T_ForwardGridder1D& self) {
                 return "ForwardGridder1D(resolution=[" +
                        std::to_string(self.get_x_coordinates().size()) + "])";
             })

        // ----- operators -----
        .def("__eq__",
             &T_ForwardGridder1D::operator==,
             DOC_ForwardGridder1D(operator_eq),
             py::arg("other"))
        // ----- pybind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(T_ForwardGridder1D)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(T_ForwardGridder1D)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(T_ForwardGridder1D)
        // end ForwardGridder1D
        ;
}

void init_c_forwardgridder1d(pybind11::module& m)
{
    init_ForwardGridder1D_float<double>(m, "");
    init_ForwardGridder1D_float<float>(m, "F");
}

} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
