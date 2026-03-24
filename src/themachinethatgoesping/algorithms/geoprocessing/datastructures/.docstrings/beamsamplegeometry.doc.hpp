//sourcehash: 77dda45bf30262a5a4ddcdff1f4da8c897d110e1be805b08a3a7cf9accaff066

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__
#define __DOCSTRINGS_HPP__

#define MKD_EXPAND(x)                                      x
#define MKD_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...)  COUNT
#define MKD_VA_SIZE(...)                                   MKD_EXPAND(MKD_COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define MKD_CAT1(a, b)                                     a ## b
#define MKD_CAT2(a, b)                                     MKD_CAT1(a, b)
#define MKD_DOC1(n1)                                       mkd_doc_##n1
#define MKD_DOC2(n1, n2)                                   mkd_doc_##n1##_##n2
#define MKD_DOC3(n1, n2, n3)                               mkd_doc_##n1##_##n2##_##n3
#define MKD_DOC4(n1, n2, n3, n4)                           mkd_doc_##n1##_##n2##_##n3##_##n4
#define MKD_DOC5(n1, n2, n3, n4, n5)                       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5
#define MKD_DOC6(n1, n2, n3, n4, n5, n6)                   mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define MKD_DOC7(n1, n2, n3, n4, n5, n6, n7)               mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define MKD_DOC8(n1, n2, n3, n4, n5, n6, n7, n8)           mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define MKD_DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define MKD_DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...)                                           MKD_EXPAND(MKD_EXPAND(MKD_CAT2(MKD_DOC, MKD_VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#endif // __DOCSTRINGS_HPP__
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry =
R"doc(Stores per-ping beam/sample geometry as linear affines.

Each optional affine maps sample_nr → a coordinate (x, y, or z) per
beam:
  coord[beam] = affine.offsets[beam] + affine.slopes[beam] * sample_nr

The user can set only the affines they need (e.g. z-only for depth
images). Transformation functions will throw if the required affine is
not set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_BeamSampleGeometry = R"doc(Construct an empty BeamSampleGeometry)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_BeamSampleGeometry_2 =
R"doc(Construct a BeamSampleGeometry with beam metadata only (no affines
set)

Args:
    first_sample_numbers: first valid sample number per beam [n_beams]
    number_of_samples: number of samples per beam [n_beams])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_x = R"doc(sample_nr → x per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_y = R"doc(sample_nr → y per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_z = R"doc(sample_nr → z per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_check_affine_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_first_sample_numbers = R"doc([n_beams] first valid sample nr)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_bottom_xyz =
R"doc(Create a BeamSampleGeometry from a base XYZ location and per-beam
bottom XYZ locations with their corresponding sample numbers.

Computes affines such that:
  coord(sample_nr) = base_coord + (bottom_coord - base_coord) /
  bottom_sample_nr *
sample_nr

Args:
    base_xyz: base location (single point, e.g. transducer position)
    bottom_xyz: per-beam bottom locations [n_beams]
    bottom_sample_numbers: per-beam sample number at the bottom
                           [n_beams]
    first_sample_numbers: per-beam first valid sample number [n_beams]
    number_of_samples: per-beam number of samples [n_beams]

Returns:
    BeamSampleGeometry with x, y, z affines set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_bottom_z =
R"doc(Create a BeamSampleGeometry with z affine only, from a base depth and
per-beam bottom depths at known sample numbers.

Args:
    base_z: base depth (e.g. transducer depth)
    bottom_depths: per-beam bottom depths [n_beams]
    bottom_sample_numbers: per-beam sample number at the bottom
                           [n_beams]
    first_sample_numbers: per-beam first valid sample number [n_beams]
    number_of_samples: per-beam number of samples [n_beams]

Returns:
    BeamSampleGeometry with z affine set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_first_sample_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_last_sample_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_number_of_samples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_number_of_samples = R"doc([n_beams] number of samples)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_xyz_affines = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


