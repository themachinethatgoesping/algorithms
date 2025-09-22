// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirectionstime.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_SampleDirectionsTime(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleDirectionsTime,                                                                      \
        ARG)

template<size_t Dim>
void init_c_sampledirectionstime_dim(py::module& m)
{
    const std::string py_class_name = "SampleDirectionsTime_" + std::to_string(Dim);

    py::classh<SampleDirectionsTime<Dim>, SampleDirections<Dim>>(m,
                                                                 py_class_name.c_str(),
                                                                 DOC(themachinethatgoesping,
                                                                     algorithms,
                                                                     geoprocessing,
                                                                     datastructures,
                                                                     SampleDirectionsTime))
        .def(py::init<>(), DOC_SampleDirectionsTime(SampleDirectionsTime))
        .def(py::init<const std::array<size_t, Dim>&>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_2),
             py::arg("shape"))
        .def(py::init<SampleDirections<Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_3),
             py::arg("sample_directions"),
             py::arg("two_way_travel_time"))
        .def(py::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_4),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"),
             py::arg("two_way_travel_time"))
        .def("__eq__",
             &SampleDirectionsTime<Dim>::operator==,
             DOC_SampleDirectionsTime(operator_eq),
             py::arg("other"))

        .def_readwrite("two_way_travel_time",
                       &SampleDirectionsTime<Dim>::two_way_travel_time,
                       DOC_SampleDirectionsTime(two_way_travel_time),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirectionsTime<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirectionsTime<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirectionsTime<Dim>)
        // end SampleDirectionsTime
        ;
}

void init_c_sampledirectionstime(py::module& m)
{
    init_c_sampledirectionstime_dim<1>(m);
    init_c_sampledirectionstime_dim<2>(m);
    init_c_sampledirectionstime_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping