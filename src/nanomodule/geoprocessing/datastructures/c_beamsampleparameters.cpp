// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0


// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsampleparameters.hpp"
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

// -- include nanobind headers
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define DOC_BeamSampleParameters(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        BeamSampleParameters,                                                                      \
        ARG)

void init_c_beamsampleparameters(nb::module_& m)
{
    const std::string py_class_name = "BeamSampleParameters";

    nb::class_<BeamSampleParameters>(m,
                                     py_class_name.c_str(),
                                     DOC(themachinethatgoesping,
                                         algorithms,
                                         geoprocessing,
                                         datastructures,
                                         BeamSampleParameters))
        .def(nb::init<unsigned int>(),
             DOC_BeamSampleParameters(BeamSampleParameters),
             nb::arg("number_of_beams"))
        .def(nb::init<xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<unsigned int, 1>>(),
             DOC_BeamSampleParameters(BeamSampleParameters),
             nb::arg("alongtrack_angles"),
             nb::arg("crosstrack_angles"),
             nb::arg("first_sample_offset"),
             nb::arg("sample_interval"),
             nb::arg("number_of_samples"))
        .def("__eq__",
             &BeamSampleParameters::operator==,
             DOC_BeamSampleParameters(operator_eq),
             nb::arg("other"))

        //----- accessors -----
        .def("get_alongtrack_angles",
             &BeamSampleParameters::get_alongtrack_angles,
             DOC_BeamSampleParameters(get_alongtrack_angles),
             nb::rv_policy::reference_internal)
        .def("get_crosstrack_angles",
             &BeamSampleParameters::get_crosstrack_angles,
             DOC_BeamSampleParameters(get_crosstrack_angles),
             nb::rv_policy::reference_internal)
        .def("get_first_sample_offset",
             &BeamSampleParameters::get_first_sample_offset,
             DOC_BeamSampleParameters(get_first_sample_offset),
             nb::rv_policy::reference_internal)
        .def("get_sample_interval",
             &BeamSampleParameters::get_sample_interval,
             DOC_BeamSampleParameters(get_sample_interval),
             nb::rv_policy::reference_internal)
        .def("get_number_of_samples",
             &BeamSampleParameters::get_number_of_samples,
             DOC_BeamSampleParameters(get_number_of_samples),
             nb::rv_policy::reference_internal)

        // ----- setters -----
        .def("set_alongtrack_angles",
             &BeamSampleParameters::set_alongtrack_angles,
             DOC_BeamSampleParameters(set_alongtrack_angles),
             nb::arg("alongtrack_angles"))
        .def("set_crosstrack_angles",
             &BeamSampleParameters::set_crosstrack_angles,
             DOC_BeamSampleParameters(set_crosstrack_angles),
             nb::arg("crosstrack_angles"))
        .def("set_first_sample_offset",
             &BeamSampleParameters::set_first_sample_offset,
             DOC_BeamSampleParameters(set_first_sample_offset),
             nb::arg("first_sample_offset"))
        .def("set_sample_interval",
             &BeamSampleParameters::set_sample_interval,
             DOC_BeamSampleParameters(set_sample_interval),
             nb::arg("sample_interval"))
        .def("set_number_of_samples",
             &BeamSampleParameters::set_number_of_samples,
             DOC_BeamSampleParameters(set_number_of_samples),
             nb::arg("number_of_samples"))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BeamSampleParameters)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BeamSampleParameters)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BeamSampleParameters)
        // end BeamSampleParameters
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping