// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/xyz.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pyarray.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_XYZ(ARG)                                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, XYZ, ARG)

template<size_t Dim>
void init_c_xyz_dim(py::module& m)
{
    const std::string py_class_name = "XYZ_" + std::to_string(Dim);

    py::class_<XYZ<Dim>, std::shared_ptr<XYZ<Dim>>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, XYZ))
        .def(py::init<>(), DOC_XYZ(XYZ))
        .def(py::init<const std::array<size_t, Dim>&>(), DOC_XYZ(XYZ_2), py::arg("shape"))
        .def(py::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_XYZ(XYZ_3),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"))
        .def("__eq__", &XYZ<Dim>::operator==, DOC_XYZ(operator_eq), py::arg("other"))
        .def("size", &XYZ<Dim>::size, DOC_XYZ(size))
        .def("shape", &XYZ<Dim>::shape, DOC_XYZ(shape))

        .def_readwrite("x", &XYZ<Dim>::x, DOC_XYZ(x), py::return_value_policy::reference_internal)
        .def_readwrite("y", &XYZ<Dim>::y, DOC_XYZ(y), py::return_value_policy::reference_internal)
        .def_readwrite("z", &XYZ<Dim>::z, DOC_XYZ(z), py::return_value_policy::reference_internal)

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

void init_c_xyz(py::module& m)
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