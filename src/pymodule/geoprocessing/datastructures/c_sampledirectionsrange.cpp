// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirectionsrange.hpp"
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

#define DOC_SampleDirectionsRange(ARG)                                                             \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleDirectionsRange,                                                                     \
        ARG)

template<size_t Dim>
void init_c_sampledirectionsrange_dim(py::module& m)
{
    const std::string py_class_name = "SampleDirectionsRange_" + std::to_string(Dim);

    py::class_<SampleDirectionsRange<Dim>,
               SampleDirections<Dim>,
               std::shared_ptr<SampleDirectionsRange<Dim>>>(m,
                                                            py_class_name.c_str(),
                                                            DOC(themachinethatgoesping,
                                                                algorithms,
                                                                geoprocessing,
                                                                datastructures,
                                                                SampleDirectionsRange))
        .def(py::init<>(), DOC_SampleDirectionsRange(SampleDirectionsRange))
        .def(py::init<const std::array<size_t, Dim>&>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_2),
             py::arg("shape"))
        .def(py::init<SampleDirections<Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_3),
             py::arg("sample_directions"),
             py::arg("range"))
        .def(py::init<xt::xtensor<float, Dim>, xt::xtensor<float, Dim>, xt::xtensor<float, Dim>>(),
             DOC_SampleDirectionsRange(SampleDirectionsRange_4),
             py::arg("alongtrack_angle"),
             py::arg("crosstrack_angle"),
             py::arg("range"))
        .def("__eq__",
             &SampleDirectionsRange<Dim>::operator==,
             DOC_SampleDirectionsRange(operator_eq),
             py::arg("other"))

        .def_readwrite("range",
                       &SampleDirectionsRange<Dim>::range,
                       DOC_SampleDirectionsRange(range),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleDirectionsRange<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleDirectionsRange<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleDirectionsRange<Dim>)
        // end SampleDirectionsRange
        ;
}

void init_c_sampledirectionsrange(py::module& m)
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