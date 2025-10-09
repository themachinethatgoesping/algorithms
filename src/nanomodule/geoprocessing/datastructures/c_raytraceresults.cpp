// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresults.hpp"
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

#define DOC_RaytraceResults(ARG)                                                                   \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResults, ARG)

template<size_t Dim>
void init_c_raytraceresults_dim(nb::module_& m)
{
    const std::string py_class_name = "RaytraceResults_" + std::to_string(Dim);

    nb::class_<RaytraceResults<Dim>, XYZ<Dim>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResults))
       .def(nb::init<>(), DOC_RaytraceResults(RaytraceResults))
       .def(nb::init<const std::array<size_t, Dim>&>(),
             DOC_RaytraceResults(RaytraceResults_2),
           nb::arg("shape"))
       .def(nb::init<xt::xtensor<float, Dim>,
                  xt::xtensor<float, Dim>,
                  xt::xtensor<float, Dim>,
                  xt::xtensor<float, Dim>>(),
             DOC_RaytraceResults(RaytraceResults_3),
           nb::arg("x"),
           nb::arg("y"),
           nb::arg("z"),
           nb::arg("true_range"))
    .def_rw("true_range",
        &RaytraceResults<Dim>::true_range,
        DOC_RaytraceResults(true_range),
        nb::rv_policy::reference_internal)

        .def_static("concat", &RaytraceResults<Dim>::concat, DOC_RaytraceResults(concat))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(RaytraceResults<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(RaytraceResults<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(RaytraceResults<Dim>)
        // end RaytraceResults
        ;
}

void init_c_raytraceresults(nb::module_& m)
{
    init_c_raytraceresults_dim<1>(m);
    init_c_raytraceresults_dim<2>(m);
    init_c_raytraceresults_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping