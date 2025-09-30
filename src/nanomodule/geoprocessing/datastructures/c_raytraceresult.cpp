// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0


// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresult.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_RaytraceResult(ARG)                                                               \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResult, ARG)

void init_c_raytraceresult(nb::module_& m)
{

    nb::class_<RaytraceResult>(
        m,
        "RaytraceResult",
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResult))
       .def(nb::init<>(), DOC_RaytraceResult(RaytraceResult))
       .def(nb::init<float, float, float, float>(),
             DOC_RaytraceResult(RaytraceResult_2),
           nb::arg("x"),
           nb::arg("y"),
           nb::arg("z"),
           nb::arg("true_range"))
        .def("__eq__",
             &RaytraceResult::operator==,
             DOC_RaytraceResult(operator_eq),
           nb::arg("other"))
        .def_rw("x", &RaytraceResult::x, DOC_RaytraceResult(x))
        .def_rw("y", &RaytraceResult::y, DOC_RaytraceResult(y))
        .def_rw("z", &RaytraceResult::z, DOC_RaytraceResult(z))
        .def_rw(
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