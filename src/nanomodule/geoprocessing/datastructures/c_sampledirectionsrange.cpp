// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirectionsrange.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

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

#define DOC_SampleDirectionsRange(ARG)                                                             \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleDirectionsRange,                                                                     \
        ARG)

template<size_t Dim>
void init_c_sampledirectionsrange_dim(nb::module_& m)
{
    const std::string py_class_name = "SampleDirectionsRange_" + std::to_string(Dim);

    nb::class_<SampleDirectionsRange<Dim>, SampleDirections<Dim>>(m,
                                                                  py_class_name.c_str(),
                                                                  DOC(themachinethatgoesping,
                                                                      algorithms,
                                                                      geoprocessing,
                                                                      datastructures,
                                                                      SampleDirectionsRange))
        .def(nb::init<>(), DOC_SampleDirectionsRange(SampleDirectionsRange))
        .def(nb::init<const std::array<size_t, Dim>&>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_2),
             nb::arg("shape"))
        .def(nb::init<SampleDirections<Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_3),
             nb::arg("sample_directions"),
             nb::arg("range"))
        .def(nb::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_4),
             nb::arg("alongtrack_angle"),
             nb::arg("crosstrack_angle"),
             nb::arg("range"))
        .def("__eq__",
             &SampleDirectionsRange<Dim>::operator==,
             DOC_SampleDirectionsRange(operator_eq),
             nb::arg("other"))

     .def_rw("range",
          &SampleDirectionsRange<Dim>::range,
          DOC_SampleDirectionsRange(range),
          nb::rv_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirectionsRange<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirectionsRange<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirectionsRange<Dim>)
        // end SampleDirectionsRange
        ;
}

void init_c_sampledirectionsrange(nb::module_& m)
{
    init_c_sampledirectionsrange_dim<1>(m);
    init_c_sampledirectionsrange_dim<2>(m);
    init_c_sampledirectionsrange_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping