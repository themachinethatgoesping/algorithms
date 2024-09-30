// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/backtracedwci.hpp"
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp> // Numpy bindings

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

namespace py = pybind11;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_BacktracedWCI(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BacktracedWCI, ARG)

void init_c_backtracedwci(py::module& m)
{

    py::class_<BacktracedWCI, std::shared_ptr<BacktracedWCI>>(
        m,
        "BacktracedWCI",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BacktracedWCI))
        //.def(py::init<>(), DOC_BacktracedWCI(BacktracedWCI))
        .def(py::init<const xt::xtensor<float, 2>&,
                      const SampleDirectionsRange<1>&,
                      const std::vector<uint16_t>&>(),
             DOC_BacktracedWCI(BacktracedWCI_2),
             py::arg("wci"),
             py::arg("beam_reference_directions"),
             py::arg("beam_reference_sample_numbers"))
        .def("__eq__", &BacktracedWCI::operator==, DOC_BacktracedWCI(operator_eq), py::arg("other"))
        .def("lookup",
             &BacktracedWCI::lookup,
             DOC_BacktracedWCI(lookup),
             py::arg("beam_angle"),
             py::arg("range"))
        .def("lookup_const",
             &BacktracedWCI::lookup_const,
             DOC_BacktracedWCI(lookup_const),
             py::arg("beam_angle"),
             py::arg("range"))
        .def("size", &BacktracedWCI::size, DOC_BacktracedWCI(size))
        .def("shape", &BacktracedWCI::shape, DOC_BacktracedWCI(shape))
        .def("get_wci", &BacktracedWCI::get_wci, DOC_BacktracedWCI(get_wci))
        .def("get_angle_beamnumber_interpolator",
             &BacktracedWCI::get_angle_beamnumber_interpolator,
             DOC_BacktracedWCI(get_angle_beamnumber_interpolator))
        .def("get_range_samplenumber_interpolators",
             &BacktracedWCI::get_range_samplenumber_interpolators,
             DOC_BacktracedWCI(get_range_samplenumber_interpolators))
        .def("get_min_angle", &BacktracedWCI::get_min_angle, DOC_BacktracedWCI(get_min_angle))
        .def("get_max_angle", &BacktracedWCI::get_max_angle, DOC_BacktracedWCI(get_max_angle))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BacktracedWCI)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BacktracedWCI)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BacktracedWCI)
        // end BacktracedWCI
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping