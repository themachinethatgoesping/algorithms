// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- module header
// automatically gernerated using  python -m pybind11_mkdoc -o docstrings.h
// <headerfiles>

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsampleparameters.hpp"
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

#define DOC_BeamSampleParameters(ARG)                                                              \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        BeamSampleParameters,                                                                      \
        ARG)

void init_c_beamsampleparameters(py::module& m)
{
    const std::string py_class_name = "BeamSampleParameters";

    py::class_<BeamSampleParameters>(m,
                                     py_class_name.c_str(),
                                     DOC(themachinethatgoesping,
                                         algorithms,
                                         geoprocessing,
                                         datastructures,
                                         BeamSampleParameters))
        .def(py::init<unsigned int>(),
             DOC_BeamSampleParameters(BeamSampleParameters),
             py::arg("number_of_beams"))
        .def(py::init<xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<float, 1>,
                      xt::xtensor<unsigned int, 1>>(),
             DOC_BeamSampleParameters(BeamSampleParameters),
             py::arg("alongtrack_angles"),
             py::arg("crosstrack_angles"),
             py::arg("first_sample_offset"),
             py::arg("sample_interval"),
             py::arg("number_of_samples"))
        .def("__eq__",
             &BeamSampleParameters::operator==,
             DOC_BeamSampleParameters(operator_eq),
             py::arg("other"))

        //----- accessors -----
        .def("get_alongtrack_angles",
             &BeamSampleParameters::get_alongtrack_angles,
             DOC_BeamSampleParameters(get_alongtrack_angles),
             py::return_value_policy::reference_internal)
        .def("get_crosstrack_angles",
             &BeamSampleParameters::get_crosstrack_angles,
             DOC_BeamSampleParameters(get_crosstrack_angles),
             py::return_value_policy::reference_internal)
        .def("get_first_sample_offset",
             &BeamSampleParameters::get_first_sample_offset,
             DOC_BeamSampleParameters(get_first_sample_offset),
             py::return_value_policy::reference_internal)
        .def("get_sample_interval",
             &BeamSampleParameters::get_sample_interval,
             DOC_BeamSampleParameters(get_sample_interval),
             py::return_value_policy::reference_internal)
        .def("get_number_of_samples",
             &BeamSampleParameters::get_number_of_samples,
             DOC_BeamSampleParameters(get_number_of_samples),
             py::return_value_policy::reference_internal)

        // ----- setters -----
        .def("set_alongtrack_angles",
             &BeamSampleParameters::set_alongtrack_angles,
             DOC_BeamSampleParameters(set_alongtrack_angles),
             py::arg("alongtrack_angles"))
        .def("set_crosstrack_angles",
             &BeamSampleParameters::set_crosstrack_angles,
             DOC_BeamSampleParameters(set_crosstrack_angles),
             py::arg("crosstrack_angles"))
        .def("set_first_sample_offset",
             &BeamSampleParameters::set_first_sample_offset,
             DOC_BeamSampleParameters(set_first_sample_offset),
             py::arg("first_sample_offset"))
        .def("set_sample_interval",
             &BeamSampleParameters::set_sample_interval,
             DOC_BeamSampleParameters(set_sample_interval),
             py::arg("sample_interval"))
        .def("set_number_of_samples",
             &BeamSampleParameters::set_number_of_samples,
             DOC_BeamSampleParameters(set_number_of_samples),
             py::arg("number_of_samples"))

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