// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirectionstime.hpp"
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

#define DOC_SampleDirectionsTime(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleDirectionsTime,                                                                      \
        ARG)

template<size_t Dim>
void init_c_sampledirectionstime_dim(nb::module_& m)
{
    const std::string py_class_name = "SampleDirectionsTime_" + std::to_string(Dim);

    nb::class_<SampleDirectionsTime<Dim>, SampleDirections<Dim>>(m,
                                                                 py_class_name.c_str(),
                                                                 DOC(themachinethatgoesping,
                                                                     algorithms,
                                                                     geoprocessing,
                                                                     datastructures,
                                                                     SampleDirectionsTime))
        .def(nb::init<>(), DOC_SampleDirectionsTime(SampleDirectionsTime))
        .def(nb::init<const std::array<size_t, Dim>&>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_2),
             nb::arg("shape"))
        .def(nb::init<SampleDirections<Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_3),
             nb::arg("sample_directions"),
             nb::arg("two_way_travel_time"))
        .def(nb::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsTime(SampleDirectionsTime_4),
             nb::arg("alongtrack_angle"),
             nb::arg("crosstrack_angle"),
             nb::arg("two_way_travel_time"))
        .def("__eq__",
             &SampleDirectionsTime<Dim>::operator==,
             DOC_SampleDirectionsTime(operator_eq),
             nb::arg("other"))

     .def_rw("two_way_travel_time",
          &SampleDirectionsTime<Dim>::two_way_travel_time,
          DOC_SampleDirectionsTime(two_way_travel_time),
          nb::rv_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirectionsTime<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirectionsTime<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirectionsTime<Dim>)
        // end SampleDirectionsTime
        ;
}

void init_c_sampledirectionstime(nb::module_& m)
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