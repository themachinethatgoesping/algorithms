// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <xtensor-python/nanobind/pytensor.hpp>

#include "../../themachinethatgoesping/algorithms/pingtools/beamsampleselection.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pingtools {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::pingtools;

#define DOC_BeamSampleSelection(arg)                                                               \
    DOC(themachinethatgoesping, algorithms, pingtools, BeamSampleSelection, arg)

void init_c_beamsampleselection(nanobind::module_& m)
{
    nb::class_<BeamSampleSelection, BeamSelection>(
        m,
        "BeamSampleSelection", 
        DOC(themachinethatgoesping, algorithms, pingtools, BeamSampleSelection))
        .def(nb::init<uint32_t>(),
             DOC_BeamSampleSelection(BeamSampleSelection),
             nb::arg("sample_step_ensemble") = 1)
        .def(nb::init<std::vector<uint32_t>, std::vector<uint32_t>, uint32_t>(),
             DOC_BeamSampleSelection(BeamSampleSelection_2),
             nb::arg("first_sample_number_per_beam"),
             nb::arg("last_sample_number_per_beam"),
             nb::arg("sample_step_ensemble") = 1)
        .def(nb::init<BeamSelection>(),
             DOC_BeamSampleSelection(BeamSampleSelection_3),
             nb::arg("beam_selection"))

        // --- get read sample range ---
        .def("get_read_sample_range",
             &BeamSampleSelection::get_read_sample_range,
             DOC_BeamSampleSelection(get_read_sample_range),
             nb::arg("beam_index"),
             nb::arg("first_sample_offset_in_beam"),
             nb::arg("number_of_samples_in_beam"))

        // --- get sample numbers ---
        .def("get_sample_numbers_ensemble_1d",
             &BeamSampleSelection::get_sample_numbers_ensemble_1d,
             DOC_BeamSampleSelection(get_sample_numbers_ensemble_1d))
        .def("get_sample_numbers_ensemble_2d",
             &BeamSampleSelection::get_sample_numbers_ensemble_2d,
             DOC_BeamSampleSelection(get_sample_numbers_ensemble_2d))
        .def("empty",
             &BeamSampleSelection::empty,
             DOC_BeamSampleSelection(empty))


        // --- add beam/samples ---
        .def("add_beam",
             &BeamSampleSelection::add_beam,
             DOC_BeamSampleSelection(add_beam),
             nb::arg("beam_number"),
             nb::arg("first_sample_number"),
             nb::arg("max_number_of_samples"))

        .def("set_sample_step_ensemble",
             &BeamSampleSelection::set_sample_step_ensemble,
             nb::arg("sample_step_ensemble"),
             DOC_BeamSampleSelection(set_sample_step_ensemble))
        .def("set_first_sample_number_ensemble",
             &BeamSampleSelection::set_first_sample_number_ensemble,
             nb::arg("first_sample_number_ensemble"),
             DOC_BeamSampleSelection(set_first_sample_number_ensemble))
        .def("set_last_sample_number_ensemble",
             &BeamSampleSelection::set_last_sample_number_ensemble,
             nb::arg("last_sample_number_ensemble"),
             DOC_BeamSampleSelection(set_last_sample_number_ensemble))

        .def("get_sample_step_ensemble",
             &BeamSampleSelection::get_sample_step_ensemble,
             DOC_BeamSampleSelection(get_sample_step_ensemble))
        .def("get_first_sample_number_ensemble",
             &BeamSampleSelection::get_first_sample_number_ensemble,
             DOC_BeamSampleSelection(get_first_sample_number_ensemble))
        .def("get_last_sample_number_ensemble",
             &BeamSampleSelection::get_last_sample_number_ensemble,
             DOC_BeamSampleSelection(get_last_sample_number_ensemble))
        .def("get_number_of_samples_ensemble",
             &BeamSampleSelection::get_number_of_samples_ensemble,
             DOC_BeamSampleSelection(get_number_of_samples_ensemble))

        // --- convenient data access ---
        .def("get_first_sample_number_per_beam",
             &BeamSampleSelection::get_first_sample_number_per_beam,
             nb::rv_policy::reference_internal,
             DOC_BeamSampleSelection(get_first_sample_number_per_beam))
        .def("get_last_sample_number_per_beam",
             &BeamSampleSelection::get_last_sample_number_per_beam,
             nb::rv_policy::reference_internal,
             DOC_BeamSampleSelection(get_last_sample_number_per_beam))

        // ----- operators -----
        .def("__eq__",
             &BeamSampleSelection::operator==,
             DOC_BeamSampleSelection(operator_eq),
             nb::arg("other"))

        // ----- nanobind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BeamSampleSelection)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BeamSampleSelection)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BeamSampleSelection);
}

}
}
}
}