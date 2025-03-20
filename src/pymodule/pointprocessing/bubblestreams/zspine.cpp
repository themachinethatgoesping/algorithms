// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/bubblestreams/zspine.hpp>
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pointprocessing {
namespace py_bubblestreams {

#define DOC_ZSpine(ARG)                                                                            \
    DOC(themachinethatgoesping, algorithms, pointprocessing, bubblestreams, ZSpine, ARG)

template<typename t_float_coord, typename t_float_weight>
void add_template_overloads_2(auto& c_zspine)
{
    namespace py = pybind11;
    using namespace pointprocessing::bubblestreams;

    // factory functions
    c_zspine.def_static(
        "from_point_cloud",
        py::overload_cast<const xt::pytensor<t_float_coord, 1>&,
                          const xt::pytensor<t_float_coord, 1>&,
                          const xt::pytensor<t_float_coord, 1>&,
                          const xt::pytensor<t_float_weight, 1>&,
                          const int64_t,
                          bool>(&ZSpine::from_point_cloud<xt::pytensor<t_float_coord, 1>,
                                                          xt::pytensor<t_float_weight, 1>>),
        DOC_ZSpine(from_point_cloud),
        py::arg("x"),
        py::arg("y"),
        py::arg("z"),
        py::arg("weights"),
        py::arg("n_quantiles"),
        py::arg("is_altitude") = false);
}

template<typename t_float>
void add_template_overloads_1(auto& c_zspine)
{
    namespace py = pybind11;
    using namespace pointprocessing::bubblestreams;

    // factory functions
    c_zspine.def("get_xy_vec",
                 py::overload_cast<const xt::pytensor<t_float, 1>&>(
                     &ZSpine::get_xy_vec<xt::pytensor<t_float, 1>>, py::const_),
                 DOC_ZSpine(get_xy_vec),
                 py::arg("z"));

    // displacement functions
    c_zspine
        .def("displace_points_inplace",
             py::overload_cast<xt::pytensor<t_float, 1>&,
                               xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(
                 &ZSpine::displace_points_inplace<xt::pytensor<t_float, 1>>, py::const_),
             DOC_ZSpine(displace_points_inplace),
             py::arg("x").noconvert(),
             py::arg("y").noconvert(),
             py::arg("z"),
             py::arg("bottom_z") = std::nullopt,
             py::arg("inverse")  = false,
             py::arg("mp_cores") = 1)
        .def("displace_points",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(&ZSpine::displace_points<xt::pytensor<t_float, 1>>,
                                          py::const_),
             DOC_ZSpine(displace_points),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("bottom_z") = std::nullopt,
             py::arg("inverse")  = false,
             py::arg("mp_cores") = 1)
        .def("displace_points_x",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(&ZSpine::displace_points_x<xt::pytensor<t_float, 1>>,
                                          py::const_),
             DOC_ZSpine(displace_points_x),
             py::arg("x"),
             py::arg("z"),
             py::arg("bottom_z") = std::nullopt,
             py::arg("inverse")  = false,
             py::arg("mp_cores") = 1)
        .def("displace_points_y",
             py::overload_cast<const xt::pytensor<t_float, 1>&,
                               const xt::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(&ZSpine::displace_points_y<xt::pytensor<t_float, 1>>,
                                          py::const_),
             DOC_ZSpine(displace_points_y),
             py::arg("y"),
             py::arg("z"),
             py::arg("bottom_z") = std::nullopt,
             py::arg("inverse")  = false,
             py::arg("mp_cores") = 1);
}

void init_c_zspine(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace pointprocessing::bubblestreams;

    auto c_zspine =
        py::class_<ZSpine, std::shared_ptr<ZSpine>>(
            m,
            "ZSpine",
            DOC(themachinethatgoesping, algorithms, pointprocessing, bubblestreams, ZSpine))
            .def(py::init<bool>(), DOC_ZSpine(ZSpine), py::arg("is_altitude") = false)
            .def(py::init<std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>,
                          bool>(),
                 DOC_ZSpine(ZSpine_2),
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("z"),
                 py::arg("is_altitude") = false)
            .def("__eq__", &ZSpine::operator==, DOC_ZSpine(operator_eq), py::arg("other"))

            .def("add_point",
                 &ZSpine::add_point,
                 DOC_ZSpine(add_point),
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("z"))
            .def("add_points",
                 &ZSpine::add_points,
                 DOC_ZSpine(add_points),
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("z"))

            // origin functions
            .def("reset_origin", &ZSpine::reset_origin, DOC_ZSpine(reset_origin))
            .def("set_origin",
                 &ZSpine::set_origin,
                 DOC_ZSpine(set_origin),
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("z"))
            .def("estimate_origin",
                 &ZSpine::estimate_origin,
                 DOC_ZSpine(estimate_origin),
                 py::arg("bottom_z"),
                 py::arg("slope_modifier") = 1.0)

            // get functions
            .def("get_spine_points",
                 &ZSpine::get_spine_points,
                 DOC_ZSpine(get_spine_points),
                 py::arg("with_origin") = true)
            .def("get_spine",
                 &ZSpine::get_spine,
                 DOC_ZSpine(get_spine),
                 py::arg("n_points"),
                 py::arg("with_origin") = true)

            .def("get_origin", &ZSpine::get_origin, DOC_ZSpine(get_origin))
            .def("get_is_altitude", &ZSpine::get_is_altitude, DOC_ZSpine(get_is_altitude))
            .def("get_xy", &ZSpine::get_xy, DOC_ZSpine(get_xy), py::arg("z"))

            .def("get_x_interpolator", &ZSpine::get_x_interpolator, DOC_ZSpine(get_x_interpolator))
            .def("get_y_interpolator", &ZSpine::get_y_interpolator, DOC_ZSpine(get_y_interpolator))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(ZSpine)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(ZSpine)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(ZSpine)
        // end ZSpine
        ;

    add_template_overloads_2<double, double>(c_zspine);
    add_template_overloads_2<double, float>(c_zspine);
    add_template_overloads_2<float, double>(c_zspine);
    add_template_overloads_2<float, float>(c_zspine);
    add_template_overloads_1<double>(c_zspine);
    add_template_overloads_1<float>(c_zspine);
}

} // namespace py_functions
} // namespace py_pointprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping