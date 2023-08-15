// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/samplelocationslocal.hpp"
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

#define DOC_SampleLocationsLocal(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        SampleLocationsLocal,                                                                      \
        ARG)

template<size_t Dim>
void init_c_samplelocationslocal_dim(py::module& m)
{
    const std::string py_class_name = "SampleLocationsLocal_" + std::to_string(Dim);

    py::class_<SampleLocationsLocal<Dim>>(m,
                                          py_class_name.c_str(),
                                          DOC(themachinethatgoesping,
                                              algorithms,
                                              geoprocessing,
                                              datastructures,
                                              SampleLocationsLocal))
        .def(py::init<>(), DOC_SampleLocationsLocal(SampleLocationsLocal))
        .def(py::init<const std::array<unsigned int, Dim>&>(),
             DOC_SampleLocationsLocal(SampleLocationsLocal_2),
             py::arg("shape"))
        .def(py::init<xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>,
                      xt::xtensor<float, Dim>>(),
             DOC_SampleLocationsLocal(SampleLocationsLocal_3),
             py::arg("x"),
             py::arg("y"),
             py::arg("z"),
             py::arg("true_range"))
        .def("__eq__",
             &SampleLocationsLocal<Dim>::operator==,
             DOC_SampleLocationsLocal(operator_eq),
             py::arg("other"))
        .def("size", &SampleLocationsLocal<Dim>::size, DOC_SampleLocationsLocal(size))

        .def_readwrite("x", &SampleLocationsLocal<Dim>::x, DOC_SampleLocationsLocal(x))
        .def_readwrite("y", &SampleLocationsLocal<Dim>::x, DOC_SampleLocationsLocal(y))
        .def_readwrite("z", &SampleLocationsLocal<Dim>::x, DOC_SampleLocationsLocal(z))
        .def_readwrite(
            "true_range", &SampleLocationsLocal<Dim>::x, DOC_SampleLocationsLocal(true_range))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(SampleLocationsLocal<Dim>)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(SampleLocationsLocal<Dim>)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(SampleLocationsLocal<Dim>)
        // end SampleLocationsLocal
        ;
}

void init_c_samplelocationslocal(py::module& m)
{
    init_c_samplelocationslocal_dim<1>(m);
    init_c_samplelocationslocal_dim<2>(m);
    init_c_samplelocationslocal_dim<3>(m);
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping