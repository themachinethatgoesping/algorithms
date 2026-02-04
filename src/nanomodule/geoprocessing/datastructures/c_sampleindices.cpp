// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampleindices.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_SampleIndices(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices, ARG)

template<size_t Dim>
void init_c_sampleindices_dim(nb::module_& m)
{
    const std::string py_class_name = "SampleIndices_" + std::to_string(Dim);

    nb::class_<SampleIndices<Dim>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices))
       .def(nb::init<>(), DOC_SampleIndices(SampleIndices))
       .def(nb::init<const std::array<size_t, Dim>&>(),
             DOC_SampleIndices(SampleIndices_2),
           nb::arg("shape"))
       .def(nb::init<xt::xtensor<uint16_t, Dim>, xt::xtensor<uint16_t, Dim>>(),
             DOC_SampleIndices(SampleIndices_3),
           nb::arg("beam_numbers"),
           nb::arg("sample_numbers"))
        .def("__eq__",
             &SampleIndices<Dim>::operator==,
             DOC_SampleIndices(operator_eq),
           nb::arg("other"))
        .def("size", &SampleIndices<Dim>::size, DOC_SampleIndices(size))
        .def("shape", &SampleIndices<Dim>::shape, DOC_SampleIndices(shape))
        .def("check_shape", &SampleIndices<Dim>::check_shape, DOC_SampleIndices(check_shape))

    .def_rw("beam_numbers",
        &SampleIndices<Dim>::beam_numbers,
        DOC_SampleIndices(beam_numbers),
        nb::rv_policy::reference_internal)
    .def_rw("sample_numbers",
        &SampleIndices<Dim>::sample_numbers,
        DOC_SampleIndices(sample_numbers),
        nb::rv_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleIndices<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleIndices<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleIndices<Dim>)
        // end SampleIndices
        ;
}

void init_c_sampleindices(nb::module_& m)
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