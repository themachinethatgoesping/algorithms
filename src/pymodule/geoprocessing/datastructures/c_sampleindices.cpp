// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0


// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampleindices.hpp"
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

#define DOC_SampleIndices(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices, ARG)

void init_c_sampleindices(py::module& m)
{
    const std::string py_class_name = "SampleIndices";

    py::class_<SampleIndices>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleIndices))
        .def(py::init<>(), DOC_SampleIndices(SampleIndices))
        .def(py::init<std::map<unsigned int, std::vector<unsigned int>>>(),
             DOC_SampleIndices(SampleIndices_2),
             py::arg("beam_sample_map"))
        .def("__eq__",
             &SampleIndices::operator==,
             DOC_SampleIndices(operator_eq),
             py::arg("other"))

        .def_readwrite("beam_sample_map",
                       &SampleIndices::beam_sample_map,
                       DOC_SampleIndices(beam_sample_map),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleIndices)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleIndices)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleIndices)
        // end SampleIndices
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping