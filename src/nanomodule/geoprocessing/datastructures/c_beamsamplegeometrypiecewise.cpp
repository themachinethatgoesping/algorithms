// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsamplegeometrypiecewise.hpp"

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/array.h>

#include <xtensor-python/nanobind/pytensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_datastructures {

namespace nb = nanobind;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

void init_c_beamsamplegeometrypiecewise(nb::module_& m)
{
    nb::class_<BeamSampleGeometryPiecewise>(
        m,
        "BeamSampleGeometryPiecewise",
        "Per-beam piecewise-linear sample geometry with K shared sample-number knots. "
        "coord_axis[beam](sample_nr) = offsets[seg, beam] + slopes[seg, beam] * sample_nr, "
        "where seg is the segment containing sample_nr.")

        .def(nb::init<>(), "Construct an empty BeamSampleGeometryPiecewise")
        .def("__eq__", &BeamSampleGeometryPiecewise::operator==, nb::arg("other"))

        // accessors
        .def("get_n_beams", &BeamSampleGeometryPiecewise::get_n_beams)
        .def("get_n_segments", &BeamSampleGeometryPiecewise::get_n_segments)
        .def("get_first_sample_numbers",
             &BeamSampleGeometryPiecewise::get_first_sample_numbers,
             nb::rv_policy::reference_internal)
        .def("get_number_of_samples",
             &BeamSampleGeometryPiecewise::get_number_of_samples,
             nb::rv_policy::reference_internal)
        .def("get_knot_sample_nrs",
             &BeamSampleGeometryPiecewise::get_knot_sample_nrs,
             nb::rv_policy::reference_internal)
        .def("has_x", &BeamSampleGeometryPiecewise::has_x)
        .def("has_y", &BeamSampleGeometryPiecewise::has_y)
        .def("has_z", &BeamSampleGeometryPiecewise::has_z)
        .def("get_offsets_x", &BeamSampleGeometryPiecewise::get_offsets_x, nb::rv_policy::reference_internal)
        .def("get_slopes_x",  &BeamSampleGeometryPiecewise::get_slopes_x,  nb::rv_policy::reference_internal)
        .def("get_offsets_y", &BeamSampleGeometryPiecewise::get_offsets_y, nb::rv_policy::reference_internal)
        .def("get_slopes_y",  &BeamSampleGeometryPiecewise::get_slopes_y,  nb::rv_policy::reference_internal)
        .def("get_offsets_z", &BeamSampleGeometryPiecewise::get_offsets_z, nb::rv_policy::reference_internal)
        .def("get_slopes_z",  &BeamSampleGeometryPiecewise::get_slopes_z,  nb::rv_policy::reference_internal)

        .def_static("from_layer_xyz",
                    [](xt::nanobind::pytensor<float, 1>        first_sample_numbers,
                       xt::nanobind::pytensor<unsigned int, 1> number_of_samples,
                       xt::nanobind::pytensor<float, 1>        knot_sample_nrs,
                       const xt::nanobind::pytensor<float, 3>& knot_xyz) {
                        return BeamSampleGeometryPiecewise::from_layer_xyz(
                            first_sample_numbers, number_of_samples, knot_sample_nrs, knot_xyz);
                    },
                    "Build from a [K+1, n_beams, 3] tensor of world-frame knot xyz "
                    "(produced by LayerRaytracer.trace_at_times).",
                    nb::arg("first_sample_numbers"),
                    nb::arg("number_of_samples"),
                    nb::arg("knot_sample_nrs"),
                    nb::arg("knot_xyz"))

        .def("eval_xyz",
             &BeamSampleGeometryPiecewise::eval_xyz,
             "Evaluate (x, y, z) at (beam, sample_nr).",
             nb::arg("beam"),
             nb::arg("sample_nr"))

        .def("is_single_affine",
             &BeamSampleGeometryPiecewise::is_single_affine,
             "Return True if all segments collapse to a single affine per beam.",
             nb::arg("tol") = 1e-3f)

        .def("to_single_affine",
             &BeamSampleGeometryPiecewise::to_single_affine,
             "Collapse to a single-segment BeamSampleGeometry (uses segment 0).")

        __PYCLASS_DEFAULT_COPY__(BeamSampleGeometryPiecewise)
        __PYCLASS_DEFAULT_BINARY__(BeamSampleGeometryPiecewise)
        __PYCLASS_DEFAULT_PRINTING__(BeamSampleGeometryPiecewise)
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
