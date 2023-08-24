// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresults.hpp"
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

#define DOC_RaytraceResults(ARG)                                                                   \
    DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResults, ARG)

template<size_t Dim>
void init_c_raytraceresults_dim(py::module& m)
{
    const std::string py_class_name = "RaytraceResults_" + std::to_string(Dim);

    py::class_<RaytraceResults<Dim>, XYZ<Dim>, std::shared_ptr<RaytraceResults<Dim>>>(
        m,
        py_class_name.c_str(),
        DOC(themachinethatgoesping, algorithms, geoprocessing, datastructures, RaytraceResults))
        .def(py::init<>(), DOC_RaytraceResults(RaytraceResults))
        .def(py::init<const std::array<unsigned int, Dim>&>(),
             DOC_RaytraceResults(RaytraceResults_2),
             py::arg("shape"))
        .def(py::init<xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>>(),
             DOC_RaytraceResults(RaytraceResults_3),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("true_range"))
        .def_readwrite("true_range",
                       &RaytraceResults<Dim>::true_range,
                       DOC_RaytraceResults(true_range),
                       py::return_value_policy::reference_internal)

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(RaytraceResults<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(RaytraceResults<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(RaytraceResults<Dim>)
        // end RaytraceResults
        ;
}

void init_c_raytraceresults(py::module& m)
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