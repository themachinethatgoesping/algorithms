// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>

#include "../../themachinethatgoesping/algorithms/pingtools/readsamplerange.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_pingtools {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::pingtools;

#define DOC_ReadSampleRange(arg)                                                                   \
    DOC(themachinethatgoesping, algorithms, pingtools, ReadSampleRange, arg)

void init_c_readsamplerange(nanobind::module_& m)
{
     nb::class_<ReadSampleRange>(
        m, "ReadSampleRange", DOC(themachinethatgoesping, algorithms, pingtools, ReadSampleRange))
          .def(nb::init<uint16_t, uint16_t, uint16_t, uint16_t>(),
             DOC_ReadSampleRange(ReadSampleRange),
                nb::arg("first_sample_to_read"),
                nb::arg("number_of_samples_to_read"),
                nb::arg("first_read_sample_offset"),
                nb::arg("last_read_sample_offset"))

        // --- data access ---
        .def("get_first_sample_to_read",
             &ReadSampleRange::get_first_sample_to_read,
             DOC_ReadSampleRange(get_first_sample_to_read))
        .def("get_number_of_samples_to_read",
             &ReadSampleRange::get_number_of_samples_to_read,
             DOC_ReadSampleRange(number_of_samples_to_read))
        .def("get_first_read_sample_offset",
             &ReadSampleRange::get_first_read_sample_offset,
             DOC_ReadSampleRange(first_read_sample_offset))
        .def("get_last_read_sample_offset",
             &ReadSampleRange::get_last_read_sample_offset,
             DOC_ReadSampleRange(last_read_sample_offset))

        // ----- operators -----
        .def("__eq__",
             &ReadSampleRange::operator==,
             DOC_ReadSampleRange(operator_eq),
             nb::arg("other"))

        // ----- nanobind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(ReadSampleRange)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(ReadSampleRange)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(ReadSampleRange);
}

}
}
}
}