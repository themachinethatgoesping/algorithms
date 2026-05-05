//sourcehash: 7ade4118f839a764f7a73e599cb2183a3f53742d22e94738089a607dac776c21

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise =
R"doc(Per-beam piecewise-linear sample geometry with K segments.

Layout per axis:
  offsets : [K, n_beams] slopes  : [K, n_beams]
Shared:
  knot_sample_nrs : [K+1] monotonic, defines segment boundaries)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_BeamSampleGeometryPiecewise = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_eval_xyz =
R"doc(Evaluate (x, y, z) for a given beam and sample number.

Selects the segment k with knot_sample_nrs[k] <= sample_nr <=
knot_sample_nrs[k+1]. Outside the knot range, clamps to the first/last
segment (linear extrapolation).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_first_sample_numbers = R"doc([n_beams] first valid sample nr)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_from_layer_xyz =
R"doc(Build from a [K+1, n_beams, 3] tensor of world-frame knot xyz.

For each beam and axis, fits per-segment linear pieces to the K+1
knots:
  slopes[k, b]  = (knot_xyz[k+1, b, axis] - knot_xyz[k, b, axis])
                  / (knot_sample_nrs[k+1] - knot_sample_nrs[k])
  offsets[k, b] = knot_xyz[k, b, axis] - slopes[k, b] *
  knot_sample_nrs[k]

All three axes (x, y, z) are populated.

Args:
    first_sample_numbers: per-beam first valid sample [n_beams]
    number_of_samples: per-beam sample count [n_beams]
    knot_sample_nrs: [K+1] monotone segment boundaries
    knot_xyz: [K+1, n_beams, 3] world-frame knot xyz)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_first_sample_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_knot_sample_nrs = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_n_segments = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_number_of_samples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_offsets_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_offsets_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_offsets_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_slopes_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_slopes_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_get_slopes_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_x_2 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_y_2 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_has_z_2 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_is_single_affine =
R"doc(Returns true if all K segments collapse (within tolerance) to a
       single affine per beam — i.e. piecewise representation is
       equivalent to a plain BeamSampleGeometry.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_knot_sample_nrs = R"doc([K+1] monotone segment boundaries)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_locate_segment = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_n_segments = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_number_of_samples = R"doc([n_beams] number of samples)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_off_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_off_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_off_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_slp_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_slp_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_slp_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_to_single_affine =
R"doc(Convert to a single-segment BeamSampleGeometry by taking segment 0.

Caller is responsible for checking is_single_affine() if equivalence
matters. Otherwise this returns the first segment's affine (useful for
shallow-water / fast path).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometryPiecewise_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


