// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresult.hpp"
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

#define DOC_RaytraceResult(ARG)                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResult, ARG)

void init_c_raytraceresult(py::module& m)
{

    py::class_<RaytraceResult>(
        m,
        "RaytraceResult",
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResult))
        .def(py::init<>(), DOC_RaytraceResult(RaytraceResult))
        .def(py::init<float, float, float, float>(),
             DOC_RaytraceResult(RaytraceResult_2),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("true_range"))
        .def("__eq__",
             &RaytraceResult::operator==,
             DOC_RaytraceResult(operator_eq),
             py::arg("other"))
        .def_readwrite("x", &RaytraceResult::x, DOC_RaytraceResult(x))
        .def_readwrite("y", &RaytraceResult::y, DOC_RaytraceResult(y))
        .def_readwrite("z", &RaytraceResult::z, DOC_RaytraceResult(z))
        .def_readwrite(
            "true_range", &RaytraceResult::true_range, DOC_RaytraceResult(true_range))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(RaytraceResult)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(RaytraceResult)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(RaytraceResult)
        // end RaytraceResult
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping