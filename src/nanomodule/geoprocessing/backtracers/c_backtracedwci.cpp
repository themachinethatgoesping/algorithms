// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/backtracedwci.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_backtracers {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_BacktracedWCI(ARG)                                                                     \
    DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BacktracedWCI, ARG)

void init_c_backtracedwci(nb::module_& m)
{

    nb::class_<BacktracedWCI>(
        m,
        "BacktracedWCI",
        DOC(themachinethatgoesping, algorithms, geoprocessing, backtracers, BacktracedWCI))
     //.def(nb::init<>(), DOC_BacktracedWCI(BacktracedWCI))
        .def(nb::init<const xt::xtensor<float, 2>&,
                      const SampleDirectionsRange<1>&,
                      const std::vector<uint16_t>&,
                      uint16_t,
                      uint16_t>(),
             DOC_BacktracedWCI(BacktracedWCI_2),
             nb::arg("wci"),
             nb::arg("beam_reference_directions"),
             nb::arg("beam_reference_sample_numbers"),
             nb::arg("wci_first_sample_number"),
             nb::arg("wci_sample_number_step") = 1)
        .def("__eq__", &BacktracedWCI::operator==, DOC_BacktracedWCI(operator_eq), nb::arg("other"))
        .def("lookup",
             &BacktracedWCI::lookup,
             DOC_BacktracedWCI(lookup),
             nb::arg("beam_angle"),
             nb::arg("range"))
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
        .def("get_wci_first_sample_number",
             &BacktracedWCI::get_wci_first_sample_number,
             DOC_BacktracedWCI(get_wci_first_sample_number))
        .def("get_wci_first_sample_number_internal",
             &BacktracedWCI::get_wci_first_sample_number_internal,
             DOC_BacktracedWCI(get_wci_first_sample_number_internal))
        .def("get_wci_sample_number_step",
             &BacktracedWCI::get_wci_sample_number_step,
             DOC_BacktracedWCI(get_wci_sample_number_step))

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