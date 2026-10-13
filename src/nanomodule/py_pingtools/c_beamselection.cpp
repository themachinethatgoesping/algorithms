// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include "../../themachinethatgoesping/algorithms/pingtools/beamselection.hpp"
#include "../../themachinethatgoesping/algorithms/pingtools/beamsampleselection.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pingtools {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::pingtools;

#define DOC_BeamSelection(arg)                                                                     \
    DOC(themachinethatgoesping, algorithms, pingtools, BeamSelection, arg)

void init_c_beamselection(nanobind::module_& m)
{
    nb::class_<BeamSelection>(
        m, "BeamSelection", DOC(themachinethatgoesping, algorithms, pingtools, BeamSelection))
        .def(nb::init<>(), DOC_BeamSelection(BeamSelection))
        .def(nb::init<const BeamSampleSelection&>(),
             DOC_BeamSelection(BeamSelection),
             nb::arg("beam_sample_selection"))
        .def(nb::init<uint32_t>(),
             DOC_BeamSelection(BeamSelection_2),
             nb::arg("number_of_beams"))
        .def(nb::init<std::vector<uint32_t>>(),
             DOC_BeamSelection(BeamSelection_3),
             nb::arg("beam_numbers"))

        // --- add beam/samples ---
        .def("add_beam",
             &BeamSelection::add_beam,
             DOC_BeamSelection(add_beam),
             nb::arg("beam_number"))

        // --- convenient data access ---
        .def("get_number_of_beams",
             &BeamSelection::get_number_of_beams,
             DOC_BeamSelection(get_number_of_beams))
        .def("empty",
             &BeamSelection::empty,
             DOC_BeamSelection(empty))
        .def("get_beam_numbers",
             &BeamSelection::get_beam_numbers,
             nb::rv_policy::reference_internal,
             DOC_BeamSelection(get_beam_numbers))

        // ----- operators -----
        .def("__eq__",
             &BeamSelection::operator==,
             DOC_BeamSelection(operator_eq),
             nb::arg("other"))

        // ----- nanobind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BeamSelection)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BeamSelection)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BeamSelection);
}

}
}
}
}