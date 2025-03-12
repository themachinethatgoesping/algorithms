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
    c_zspine.def_static("from_point_cloud",
                        py::overload_cast<const xt::pytensor<t_float_coord, 1>&,
                                          const xt::pytensor<t_float_coord, 1>&,
                                          const xt::pytensor<t_float_coord, 1>&,
                                          const xt::pytensor<t_float_weight, 1>&,
                                          const int64_t>(
                            &ZSpine::from_point_cloud<xt::pytensor<t_float_coord, 1>,
                                                      xt::pytensor<t_float_weight, 1>>),
                        DOC_ZSpine(from_point_cloud),
                        py::arg("x"),
                        py::arg("y"),
                        py::arg("z"),
                        py::arg("weights"),
                        py::arg("n_quantiles"));
}

template<typename t_float>
void add_template_overloads_1(auto& c_zspine)
{
    namespace py = pybind11;
    using namespace pointprocessing::bubblestreams;

    // factory functions
    c_zspine.def("get_xy",
                 py::overload_cast<const xt::pytensor<t_float, 1>&>(
                     &ZSpine::get_xy<xt::pytensor<t_float, 1>>, py::const_),
                 DOC_ZSpine(get_xy),
                 py::arg("z"));
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
            .def(py::init<>(), DOC_ZSpine(ZSpine))
            .def(py::init<std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>,
                          std::vector<ZSpine::coord_type>>(),
                 DOC_ZSpine(ZSpine_2),
                 py::arg("x"),
                 py::arg("y"),
                 py::arg("z"))
            .def("__eq__", &ZSpine::operator==, DOC_ZSpine(operator_eq), py::arg("other"))

            .def("get_spine_points", &ZSpine::get_spine_points, DOC_ZSpine(get_spine_points))
            .def("get_spine", &ZSpine::get_spine, DOC_ZSpine(get_spine), py::arg("n_points"))

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