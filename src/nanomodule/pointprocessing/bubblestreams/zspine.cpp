// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/bubblestreams/zspine.hpp>
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

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
    namespace nb = nanobind;
    using namespace pointprocessing::bubblestreams;

    // factory functions
    c_zspine.def_static("from_point_cloud",
                        nb::overload_cast<const xt::nanobind::pytensor<t_float_coord, 1>&,
                                          const xt::nanobind::pytensor<t_float_coord, 1>&,
                                          const xt::nanobind::pytensor<t_float_coord, 1>&,
                                          const xt::nanobind::pytensor<t_float_weight, 1>&,
                                          const int64_t,
                                          bool>(
                            &ZSpine::from_point_cloud<xt::nanobind::pytensor<t_float_coord, 1>,
                                                      xt::nanobind::pytensor<t_float_weight, 1>>),
                        DOC_ZSpine(from_point_cloud),
                        nb::arg("x"),
                        nb::arg("y"),
                        nb::arg("z"),
                        nb::arg("weights"),
                        nb::arg("n_quantiles"),
                        nb::arg("is_altitude") = false);
}

template<typename t_float>
void add_template_overloads_1(auto& c_zspine)
{
    namespace nb = nanobind;
    using namespace pointprocessing::bubblestreams;

    // factory functions
    c_zspine.def("get_xy_vec",
                 nb::overload_cast<const xt::nanobind::pytensor<t_float, 1>&>(
                     &ZSpine::get_xy_vec<xt::nanobind::pytensor<t_float, 1>>, nb::const_),
                 DOC_ZSpine(get_xy_vec),
                 nb::arg("z"));

    // displacement functions
    c_zspine
        .def("displace_points_inplace",
             nb::overload_cast<xt::nanobind::pytensor<t_float, 1>&,
                               xt::nanobind::pytensor<t_float, 1>&,
                               const xt::nanobind::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(
                 &ZSpine::displace_points_inplace<xt::nanobind::pytensor<t_float, 1>>, nb::const_),
             DOC_ZSpine(displace_points_inplace),
             nb::arg("x").noconvert(),
             nb::arg("y").noconvert(),
             nb::arg("z"),
             nb::arg("bottom_z") = std::nullopt,
             nb::arg("inverse")  = false,
             nb::arg("mp_cores") = 1)
        .def("displace_points",
             nb::overload_cast<const xt::nanobind::pytensor<t_float, 1>&,
                               const xt::nanobind::pytensor<t_float, 1>&,
                               const xt::nanobind::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(
                 &ZSpine::displace_points<xt::nanobind::pytensor<t_float, 1>>, nb::const_),
             DOC_ZSpine(displace_points),
             nb::arg("x"),
             nb::arg("y"),
             nb::arg("z"),
             nb::arg("bottom_z") = std::nullopt,
             nb::arg("inverse")  = false,
             nb::arg("mp_cores") = 1)
        .def("displace_points_x",
             nb::overload_cast<const xt::nanobind::pytensor<t_float, 1>&,
                               const xt::nanobind::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(
                 &ZSpine::displace_points_x<xt::nanobind::pytensor<t_float, 1>>, nb::const_),
             DOC_ZSpine(displace_points_x),
             nb::arg("x"),
             nb::arg("z"),
             nb::arg("bottom_z") = std::nullopt,
             nb::arg("inverse")  = false,
             nb::arg("mp_cores") = 1)
        .def("displace_points_y",
             nb::overload_cast<const xt::nanobind::pytensor<t_float, 1>&,
                               const xt::nanobind::pytensor<t_float, 1>&,
                               std::optional<ZSpine::coord_type>,
                               const bool,
                               const int>(
                 &ZSpine::displace_points_y<xt::nanobind::pytensor<t_float, 1>>, nb::const_),
             DOC_ZSpine(displace_points_y),
             nb::arg("y"),
             nb::arg("z"),
             nb::arg("bottom_z") = std::nullopt,
             nb::arg("inverse")  = false,
             nb::arg("mp_cores") = 1);
}

void init_c_zspine(nanobind::module_& m)
{
    namespace nb = nanobind;
    using namespace pointprocessing::bubblestreams;

    auto c_zspine =
        nb::class_<ZSpine>(
            m,
            "ZSpine",
            DOC(themachinethatgoesping, algorithms, pointprocessing, bubblestreams, ZSpine))
            .def(nb::init<bool>(), DOC_ZSpine(ZSpine), nb::arg("is_altitude") = false)
            .def(nb::init<std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>,
                          bool>(),
                 DOC_ZSpine(ZSpine_2),
                 nb::arg("x"),
                 nb::arg("y"),
                 nb::arg("z"),
                 nb::arg("is_altitude") = false)
            .def("__eq__", &ZSpine::operator==, DOC_ZSpine(operator_eq), nb::arg("other"))

            .def("add_point",
                 &ZSpine::add_point,
                 DOC_ZSpine(add_point),
                 nb::arg("x"),
                 nb::arg("y"),
                 nb::arg("z"))
            .def("add_points",
                 &ZSpine::add_points,
                 DOC_ZSpine(add_points),
                 nb::arg("x"),
                 nb::arg("y"),
                 nb::arg("z"))

            // origin functions
            .def("reset_origin", &ZSpine::reset_origin, DOC_ZSpine(reset_origin))
            .def("set_origin",
                 &ZSpine::set_origin,
                 DOC_ZSpine(set_origin),
                 nb::arg("x"),
                 nb::arg("y"),
                 nb::arg("z"))
            .def("estimate_origin",
                 &ZSpine::estimate_origin,
                 DOC_ZSpine(estimate_origin),
                 nb::arg("bottom_z"),
                 nb::arg("slope_modifier") = 1.0)

            // get functions
            .def("get_spine_points",
                 &ZSpine::get_spine_points,
                 DOC_ZSpine(get_spine_points),
                 nb::arg("with_origin") = true)
            .def("get_spine",
                 &ZSpine::get_spine,
                 DOC_ZSpine(get_spine),
                 nb::arg("n_points"),
                 nb::arg("with_origin") = true)

            .def("get_origin", &ZSpine::get_origin, DOC_ZSpine(get_origin))
            .def("get_is_altitude", &ZSpine::get_is_altitude, DOC_ZSpine(get_is_altitude))
            .def("get_xy", &ZSpine::get_xy, DOC_ZSpine(get_xy), nb::arg("z"))

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