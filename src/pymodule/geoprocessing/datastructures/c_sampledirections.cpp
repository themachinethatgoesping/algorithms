// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirections.hpp"
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

#define DOC_SampleDirections(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleDirections,                                                                      \
        ARG)

template<size_t Dim>
void init_c_sampledirections_dim(py::module& m)
{
    const std::string py_class_name = "SampleDirections_" + std::to_string(Dim);

    py::class_<SampleDirections<Dim>>(m,
                                          py_class_name.c_str(),
                                          DOC(themachinethatgoesping,
                                              algorithms,
                                              geoprocessing,
                                              datastructures,
                                              SampleDirections))
        .def(py::init<>(), DOC_SampleDirections(SampleDirections))
        .def(py::init<const std::array<unsigned int, Dim>&>(),
             DOC_SampleDirections(SampleDirections_2),
             py::arg("shape"))
        .def(py::init<xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>>(),
             DOC_SampleDirections(SampleDirections_3),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"),
             py::arg("two_way_travel_time"))
        .def("__eq__",
             &SampleDirections<Dim>::operator==,
             DOC_SampleDirections(operator_eq),
             py::arg("other"))
        .def("size",
             &SampleDirections<Dim>::size,
             DOC_SampleDirections(size))

        .def_readwrite("alongtrack_angle",
                       &SampleDirections<Dim>::alongtrack_angle,
                       DOC_SampleDirections(alongtrack_angle),
                       py::return_value_policy::reference_internal)
        .def_readwrite("crosstrack_angle",
                       &SampleDirections<Dim>::crosstrack_angle,
                       DOC_SampleDirections(crosstrack_angle),
                       py::return_value_policy::reference_internal)
        .def_readwrite("two_way_travel_time",
                       &SampleDirections<Dim>::two_way_travel_time,
                       DOC_SampleDirections(two_way_travel_time),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirections<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirections<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirections<Dim>)
        // end SampleDirections
        ;
}

void init_c_sampledirections(py::module& m)
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