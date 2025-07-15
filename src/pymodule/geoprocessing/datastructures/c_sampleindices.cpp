// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampleindices.hpp"
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

#define DOC_SampleIndices(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices, ARG)

template<size_t Dim>
void init_c_sampleindices_dim(py::module& m)
{
    const std::string py_class_name = "SampleIndices_" + std::to_string(Dim);

    py::class_<SampleIndices<Dim>, std::shared_ptr<SampleIndices<Dim>>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices))
        .def(py::init<>(), DOC_SampleIndices(SampleIndices))
        .def(py::init<const std::array<size_t, Dim>&>(),
             DOC_SampleIndices(SampleIndices_2),
             py::arg("shape"))
        .def(py::init<xt::xtensor<uint16_t, Dim>, xt::xtensor<uint16_t, Dim>>(),
             DOC_SampleIndices(SampleIndices_3),
             py::arg("beam_numbers"),
             py::arg("sample_numbers"))
        .def("__eq__",
             &SampleIndices<Dim>::operator==,
             DOC_SampleIndices(operator_eq),
             py::arg("other"))
        .def("size", &SampleIndices<Dim>::size, DOC_SampleIndices(size))
        .def("shape", &SampleIndices<Dim>::shape, DOC_SampleIndices(shape))
        .def("check_shape", &SampleIndices<Dim>::check_shape, DOC_SampleIndices(check_shape))

        .def_readwrite("beam_numbers",
                       &SampleIndices<Dim>::beam_numbers,
                       DOC_SampleIndices(beam_numbers),
                       py::return_value_policy::reference_internal)
        .def_readwrite("sample_numbers",
                       &SampleIndices<Dim>::sample_numbers,
                       DOC_SampleIndices(sample_numbers),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleIndices<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleIndices<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleIndices<Dim>)
        // end SampleIndices
        ;
}

void init_c_sampleindices(py::module& m)
{
    init_c_sampleindices_dim<1>(m);
    init_c_sampleindices_dim<2>(m);
    init_c_sampleindices_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping