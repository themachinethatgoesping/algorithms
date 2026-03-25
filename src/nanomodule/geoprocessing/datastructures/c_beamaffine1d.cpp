// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamaffine1d.hpp"
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

#define DOC_BeamAffine1D(ARG)                                                                      \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        BeamAffine1D,                                                                              \
        ARG)

void init_c_beamaffine1d(nb::module_& m)
{
    nb::class_<BeamAffine1D>(m,
                             "BeamAffine1D",
                             DOC(themachinethatgoesping,
                                 algorithms,
                                 geoprocessing,
                                 datastructures,
                                 BeamAffine1D))
        // constructors
        .def(nb::init<>(), "Construct an empty BeamAffine1D")
        .def(nb::init<size_t>(),
             DOC_BeamAffine1D(BeamAffine1D_2),
             nb::arg("n_beams"))
        .def(nb::init<xt::xtensor<float, 1>, xt::xtensor<float, 1>>(),
             DOC_BeamAffine1D(BeamAffine1D_3),
             nb::arg("offsets"),
             nb::arg("slopes"))
        .def("__eq__",
             &BeamAffine1D::operator==,
             DOC_BeamAffine1D(operator_eq),
             nb::arg("other"))

        // public members
        .def_rw("offsets", &BeamAffine1D::offsets, DOC_BeamAffine1D(offsets))
        .def_rw("slopes", &BeamAffine1D::slopes, DOC_BeamAffine1D(slopes))

        // methods
        .def("size", &BeamAffine1D::size, DOC_BeamAffine1D(size))
        .def("forward",
             &BeamAffine1D::forward,
             DOC_BeamAffine1D(forward),
             nb::arg("beam_index"),
             nb::arg("sample_nr"))
        .def("inverse",
             &BeamAffine1D::inverse,
             DOC_BeamAffine1D(inverse),
             nb::arg("beam_index"),
             nb::arg("value"))

        // factory
        .def_static("from_base_and_endpoints",
                     [](float base_value,
                        const xt::nanobind::pytensor<float, 1>& end_values,
                        const xt::nanobind::pytensor<float, 1>& end_sample_numbers) {
                         return BeamAffine1D::from_base_and_endpoints(
                             base_value, end_values, end_sample_numbers);
                     },
                     DOC_BeamAffine1D(from_base_and_endpoints),
                     nb::arg("base_value"),
                     nb::arg("end_values"),
                     nb::arg("end_sample_numbers"))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BeamAffine1D)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BeamAffine1D)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BeamAffine1D)
        // end BeamAffine1D
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
