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
#include <xtensor-python/pyarray.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_SampleLocationLocal(ARG)                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleLocationLocal, ARG)

void init_c_samplelocationlocal(py::module& m)
{

    py::class_<SampleLocationLocal>(
        m,
        "SampleLocationLocal",
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, SampleLocationLocal))
        .def(py::init<>(), DOC_SampleLocationLocal(SampleLocationLocal))
        .def(py::init<float, float, float, float>(),
             DOC_SampleLocationLocal(SampleLocationLocal_2),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("true_range"))
        .def("__eq__",
             &SampleLocationLocal::operator==,
             DOC_SampleLocationLocal(operator_eq),
             py::arg("other"))
        .def_readwrite("x", &SampleLocationLocal::x, DOC_SampleLocationLocal(x))
        .def_readwrite("y", &SampleLocationLocal::y, DOC_SampleLocationLocal(y))
        .def_readwrite("z", &SampleLocationLocal::z, DOC_SampleLocationLocal(z))
        .def_readwrite(
            "true_range", &SampleLocationLocal::true_range, DOC_SampleLocationLocal(true_range))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleLocationLocal)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleLocationLocal)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleLocationLocal)
        // end SampleLocationLocal
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping