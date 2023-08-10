// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/samplelocationlocal.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_SampleLocationLocal(ARG)                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SamplelocationLocal, ARG)

void init_c_samplelocationlocal(py::module& m)
{

    py::class_<SamplelocationLocal>(
        m,
        "SamplelocationLocal",
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SamplelocationLocal))
        .def(py::init<>(), DOC_SampleLocationLocal(SamplelocationLocal))
        .def(py::init<float, float, float, float>(),
             DOC_SampleLocationLocal(SamplelocationLocal_2),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("true_range"))
        .def("__eq__",
             &SamplelocationLocal::operator==,
             DOC_SampleLocationLocal(operator_eq),
             py::arg("other"))
        .def_readwrite("x", &SamplelocationLocal::x, DOC_SampleLocationLocal(x))
        .def_readwrite("y", &SamplelocationLocal::x, DOC_SampleLocationLocal(y))
        .def_readwrite("z", &SamplelocationLocal::x, DOC_SampleLocationLocal(z))
        .def_readwrite("true_range", &SamplelocationLocal::x, DOC_SampleLocationLocal(true_range))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SamplelocationLocal)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SamplelocationLocal)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SamplelocationLocal)
        // end SamplelocationLocal
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping