// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -- c++ library headers
#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsamplegeometry.hpp"
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

#define DOC_BeamSampleGeometry(ARG)                                                                \
    DOC(themachinethatgoesping,                                                                    \
        algorithms,                                                                                \
        geoprocessing,                                                                             \
        datastructures,                                                                            \
        BeamSampleGeometry,                                                                        \
        ARG)

void init_c_beamsamplegeometry(nb::module_& m)
{
    nb::class_<BeamSampleGeometry>(m,
                                   "BeamSampleGeometry",
                                   DOC(themachinethatgoesping,
                                       algorithms,
                                       geoprocessing,
                                       datastructures,
                                       BeamSampleGeometry))
        // constructors
        .def(nb::init<>(), "Construct an empty BeamSampleGeometry")
        .def(nb::init<xt::xtensor<float, 1>, xt::xtensor<unsigned int, 1>>(),
             DOC_BeamSampleGeometry(BeamSampleGeometry_2),
             nb::arg("first_sample_numbers"),
             nb::arg("number_of_samples"))
        .def("__eq__",
             &BeamSampleGeometry::operator==,
             DOC_BeamSampleGeometry(operator_eq),
             nb::arg("other"))

        // --- accessors ---
        .def("get_n_beams",
             &BeamSampleGeometry::get_n_beams,
             DOC_BeamSampleGeometry(get_n_beams))
        .def("get_first_sample_numbers",
             &BeamSampleGeometry::get_first_sample_numbers,
             DOC_BeamSampleGeometry(get_first_sample_numbers),
             nb::rv_policy::reference_internal)
        .def("get_number_of_samples",
             &BeamSampleGeometry::get_number_of_samples,
             DOC_BeamSampleGeometry(get_number_of_samples),
             nb::rv_policy::reference_internal)
        .def("get_last_sample_numbers",
             &BeamSampleGeometry::get_last_sample_numbers,
             DOC_BeamSampleGeometry(get_last_sample_numbers))

        // --- affine queries ---
        .def("has_x_affine",
             &BeamSampleGeometry::has_x_affine,
             DOC_BeamSampleGeometry(has_x_affine))
        .def("has_y_affine",
             &BeamSampleGeometry::has_y_affine,
             DOC_BeamSampleGeometry(has_y_affine))
        .def("has_z_affine",
             &BeamSampleGeometry::has_z_affine,
             DOC_BeamSampleGeometry(has_z_affine))

        // --- affine getters ---
        .def("get_x_affine",
             &BeamSampleGeometry::get_x_affine,
             DOC_BeamSampleGeometry(get_x_affine),
             nb::rv_policy::reference_internal)
        .def("get_y_affine",
             &BeamSampleGeometry::get_y_affine,
             DOC_BeamSampleGeometry(get_y_affine),
             nb::rv_policy::reference_internal)
        .def("get_z_affine",
             &BeamSampleGeometry::get_z_affine,
             DOC_BeamSampleGeometry(get_z_affine),
             nb::rv_policy::reference_internal)

        // --- affine setters ---
        .def("set_x_affine",
             &BeamSampleGeometry::set_x_affine,
             DOC_BeamSampleGeometry(set_x_affine),
             nb::arg("affine"))
        .def("set_y_affine",
             &BeamSampleGeometry::set_y_affine,
             DOC_BeamSampleGeometry(set_y_affine),
             nb::arg("affine"))
        .def("set_z_affine",
             &BeamSampleGeometry::set_z_affine,
             DOC_BeamSampleGeometry(set_z_affine),
             nb::arg("affine"))
        .def("set_xyz_affines",
             &BeamSampleGeometry::set_xyz_affines,
             DOC_BeamSampleGeometry(set_xyz_affines),
             nb::arg("affine_x"),
             nb::arg("affine_y"),
             nb::arg("affine_z"))

        // --- factory methods ---
        .def_static("from_bottom_xyz",
                     [](const XYZ<1>& bottom_xyz,
                        float base_x, float base_y, float base_z,
                        const xt::nanobind::pytensor<float, 1>& bottom_sample_numbers,
                        xt::nanobind::pytensor<float, 1> first_sample_numbers,
                        xt::nanobind::pytensor<unsigned int, 1> number_of_samples) {
                         return BeamSampleGeometry::from_bottom_xyz(
                             bottom_xyz, base_x, base_y, base_z,
                             bottom_sample_numbers, first_sample_numbers, number_of_samples);
                     },
                     DOC_BeamSampleGeometry(from_bottom_xyz),
                     nb::arg("bottom_xyz"),
                     nb::arg("base_x"),
                     nb::arg("base_y"),
                     nb::arg("base_z"),
                     nb::arg("bottom_sample_numbers"),
                     nb::arg("first_sample_numbers"),
                     nb::arg("number_of_samples"))
        .def_static("from_bottom_z",
                     [](float base_z,
                        const xt::nanobind::pytensor<float, 1>& bottom_depths,
                        const xt::nanobind::pytensor<float, 1>& bottom_sample_numbers,
                        xt::nanobind::pytensor<float, 1> first_sample_numbers,
                        xt::nanobind::pytensor<unsigned int, 1> number_of_samples) {
                         return BeamSampleGeometry::from_bottom_z(
                             base_z, bottom_depths, bottom_sample_numbers,
                             first_sample_numbers, number_of_samples);
                     },
                     DOC_BeamSampleGeometry(from_bottom_z),
                     nb::arg("base_z"),
                     nb::arg("bottom_depths"),
                     nb::arg("bottom_sample_numbers"),
                     nb::arg("first_sample_numbers"),
                     nb::arg("number_of_samples"))
        .def_static("from_angle_and_range",
                     [](const xt::nanobind::pytensor<float, 1>& crosstrack_angles,
                        const xt::nanobind::pytensor<float, 1>& ranges,
                        const xt::nanobind::pytensor<float, 1>& range_sample_numbers,
                        xt::nanobind::pytensor<float, 1> first_sample_numbers,
                        xt::nanobind::pytensor<unsigned int, 1> number_of_samples) {
                         return BeamSampleGeometry::from_angle_and_range(
                             crosstrack_angles, ranges, range_sample_numbers,
                             first_sample_numbers, number_of_samples);
                     },
                     DOC_BeamSampleGeometry(from_angle_and_range),
                     nb::arg("crosstrack_angles"),
                     nb::arg("ranges"),
                     nb::arg("range_sample_numbers"),
                     nb::arg("first_sample_numbers"),
                     nb::arg("number_of_samples"))
        .def_static("from_angles_and_range",
                     [](const xt::nanobind::pytensor<float, 1>& alongtrack_angles,
                        const xt::nanobind::pytensor<float, 1>& crosstrack_angles,
                        const xt::nanobind::pytensor<float, 1>& ranges,
                        const xt::nanobind::pytensor<float, 1>& range_sample_numbers,
                        xt::nanobind::pytensor<float, 1> first_sample_numbers,
                        xt::nanobind::pytensor<unsigned int, 1> number_of_samples) {
                         return BeamSampleGeometry::from_angles_and_range(
                             alongtrack_angles, crosstrack_angles, ranges,
                             range_sample_numbers, first_sample_numbers, number_of_samples);
                     },
                     DOC_BeamSampleGeometry(from_angles_and_range),
                     nb::arg("alongtrack_angles"),
                     nb::arg("crosstrack_angles"),
                     nb::arg("ranges"),
                     nb::arg("range_sample_numbers"),
                     nb::arg("first_sample_numbers"),
                     nb::arg("number_of_samples"))

        // --- forward transformations: flat overloads ---
        .def("forward_x",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<float, 1>& sample_numbers) {
                 return self.forward_x(beam_indices, sample_numbers);
             },
             "Compute x for (beam_index, sample_number) pairs. "
             "Sorted beam_indices yield best SIMD throughput.",
             nb::arg("beam_indices"),
             nb::arg("sample_numbers"))
        .def("forward_y",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<float, 1>& sample_numbers) {
                 return self.forward_y(beam_indices, sample_numbers);
             },
             "Compute y for (beam_index, sample_number) pairs.",
             nb::arg("beam_indices"),
             nb::arg("sample_numbers"))
        .def("forward_z",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<float, 1>& sample_numbers) {
                 return self.forward_z(beam_indices, sample_numbers);
             },
             "Compute z for (beam_index, sample_number) pairs.",
             nb::arg("beam_indices"),
             nb::arg("sample_numbers"))

        // --- forward transformations: range overloads ---
        .def("forward_x",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<uint32_t, 1>& first_sample_numbers,
                const xt::nanobind::pytensor<uint32_t, 1>& last_sample_numbers,
                uint32_t sample_step) {
                 return self.forward_x(beam_indices, first_sample_numbers,
                                       last_sample_numbers, sample_step);
             },
             "Compute x for sample ranges per beam. "
             "Returns 2D [n_beams x max_samples], NaN-padded.",
             nb::arg("beam_indices"),
             nb::arg("first_sample_numbers"),
             nb::arg("last_sample_numbers"),
             nb::arg("sample_step") = 1)
        .def("forward_y",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<uint32_t, 1>& first_sample_numbers,
                const xt::nanobind::pytensor<uint32_t, 1>& last_sample_numbers,
                uint32_t sample_step) {
                 return self.forward_y(beam_indices, first_sample_numbers,
                                       last_sample_numbers, sample_step);
             },
             "Compute y for sample ranges per beam. "
             "Returns 2D [n_beams x max_samples], NaN-padded.",
             nb::arg("beam_indices"),
             nb::arg("first_sample_numbers"),
             nb::arg("last_sample_numbers"),
             nb::arg("sample_step") = 1)
        .def("forward_z",
             [](const BeamSampleGeometry& self,
                const xt::nanobind::pytensor<uint32_t, 1>& beam_indices,
                const xt::nanobind::pytensor<uint32_t, 1>& first_sample_numbers,
                const xt::nanobind::pytensor<uint32_t, 1>& last_sample_numbers,
                uint32_t sample_step) {
                 return self.forward_z(beam_indices, first_sample_numbers,
                                       last_sample_numbers, sample_step);
             },
             "Compute z for sample ranges per beam. "
             "Returns 2D [n_beams x max_samples], NaN-padded.",
             nb::arg("beam_indices"),
             nb::arg("first_sample_numbers"),
             nb::arg("last_sample_numbers"),
             nb::arg("sample_step") = 1)

        // --- flat-index helpers ---
        .def("get_flat_offsets",
             &BeamSampleGeometry::get_flat_offsets,
             DOC_BeamSampleGeometry(get_flat_offsets))
        .def("get_total_samples",
             &BeamSampleGeometry::get_total_samples,
             DOC_BeamSampleGeometry(get_total_samples))

        // --- full flat coordinate arrays ---
        .def("forward_x_flat",
             &BeamSampleGeometry::forward_x_flat,
             DOC_BeamSampleGeometry(forward_x_flat))
        .def("forward_y_flat",
             &BeamSampleGeometry::forward_y_flat,
             DOC_BeamSampleGeometry(forward_y_flat))
        .def("forward_z_flat",
             &BeamSampleGeometry::forward_z_flat,
             DOC_BeamSampleGeometry(forward_z_flat))

        // default copy functions
        __PYCLASS_DEFAULT_COPY__(BeamSampleGeometry)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(BeamSampleGeometry)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(BeamSampleGeometry)
        // end BeamSampleGeometry
        ;
}

} // namespace py_datastructures
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
