// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirections.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_SampleDirections(ARG)                                                                  \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleDirections, ARG)

template<size_t Dim>
void init_c_sampledirections_dim(nb::module_& m)
{
    const std::string py_class_name = "SampleDirections_" + std::to_string(Dim);

    nb::class_<SampleDirections<Dim>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleDirections))
       .def(nb::init<>(), DOC_SampleDirections(SampleDirections))
       .def(nb::init<const std::array<size_t, Dim>&>(),
             DOC_SampleDirections(SampleDirections_2),
           nb::arg("shape"))
       .def(nb::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirections(SampleDirections_3),
           nb::arg("alongtrack_angle"),
           nb::arg("crosstrack_angle"))
        .def("__eq__",
             &SampleDirections<Dim>::operator==,
             DOC_SampleDirections(operator_eq),
           nb::arg("other"))
        .def("size", &SampleDirections<Dim>::size, DOC_SampleDirections(size))
        .def("shape", &SampleDirections<Dim>::shape, DOC_SampleDirections(shape))
        .def("check_shape", &SampleDirections<Dim>::check_shape, DOC_SampleDirections(check_shape))

    .def_rw("alongtrack_angle",
        &SampleDirections<Dim>::alongtrack_angle,
        DOC_SampleDirections(alongtrack_angle),
        nb::rv_policy::reference_internal)
    .def_rw("crosstrack_angle",
        &SampleDirections<Dim>::crosstrack_angle,
        DOC_SampleDirections(crosstrack_angle),
        nb::rv_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirections<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirections<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirections<Dim>)
        // end SampleDirections
        ;
}

void init_c_sampledirections(nb::module_& m)
{
    init_c_sampledirections_dim<1>(m);
    init_c_sampledirections_dim<2>(m);
    init_c_sampledirections_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping