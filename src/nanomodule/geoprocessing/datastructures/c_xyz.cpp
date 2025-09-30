// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/xyz.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_XYZ(ARG)                                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, XYZ, ARG)

template<size_t Dim>
void init_c_xyz_dim(nb::module_& m)
{
    const std::string py_class_name = "XYZ_" + std::to_string(Dim);

    nb::class_<XYZ<Dim>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, XYZ))
        .def(nb::init<>(), DOC_XYZ(XYZ))
        .def(nb::init<const std::array<size_t, Dim>&>(), DOC_XYZ(XYZ_2), nb::arg("shape"))
        .def(nb::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_XYZ(XYZ_3),
             nb::arg("x"),
             nb::arg("y"),
             nb::arg("z"))
        .def("__eq__", &XYZ<Dim>::operator==, DOC_XYZ(operator_eq), nb::arg("other"))
        .def("size", &XYZ<Dim>::size, DOC_XYZ(size))
        .def("shape", &XYZ<Dim>::shape, DOC_XYZ(shape))
        .def_static("concat", &XYZ<Dim>::concat, DOC_XYZ(concat))

        .def("rotate",
             nb::overload_cast<const Eigen::Quaternionf&>(&XYZ<Dim>::rotate),
             DOC_XYZ(rotate),
             nb::arg("quat"))
        .def("rotate",
             nb::overload_cast<float, float, float>(&XYZ<Dim>::rotate),
             DOC_XYZ(rotate_2),
             nb::arg("yaw")   = 0.f,
             nb::arg("pitch") = 0.f,
             nb::arg("roll")  = 0.f)
        .def("translate",
             &XYZ<Dim>::translate,
             DOC_XYZ(translate),
             nb::arg("x") = 0.f,
             nb::arg("y") = 0.f,
             nb::arg("z") = 0.f)
        .def("to_latlon",
             &XYZ<Dim>::to_latlon,
             //DOC_XYZ(to_latlon), //TODO: pybind_mkdoc crashes on this
             nb::arg("utm_zone"),
             nb::arg("northern_hemisphere"))

     .def_rw("x", &XYZ<Dim>::x, DOC_XYZ(x), nb::rv_policy::reference_internal)
     .def_rw("y", &XYZ<Dim>::y, DOC_XYZ(y), nb::rv_policy::reference_internal)
     .def_rw("z", &XYZ<Dim>::z, DOC_XYZ(z), nb::rv_policy::reference_internal)

        .def("get_minmax_x", &XYZ<Dim>::get_minmax_x, DOC_XYZ(get_minmax_x))
        .def("get_minmax_y", &XYZ<Dim>::get_minmax_y, DOC_XYZ(get_minmax_y))
        .def("get_minmax_z", &XYZ<Dim>::get_minmax_z, DOC_XYZ(get_minmax_z))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(XYZ<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(XYZ<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(XYZ<Dim>)
        // end XYZ
        ;
}

void init_c_xyz(nb::module_& m)
{
    init_c_xyz_dim<1>(m);
    init_c_xyz_dim<2>(m);
    init_c_xyz_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping