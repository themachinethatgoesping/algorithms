//sourcehash: a9aa59156c9186bf82067c7b0ce0297370770e9f13d34534affed80418a993e3

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile =
R"doc(1-D depth-dependent sound velocity profile with layered analytic
       precomputations for use by the LayerRaytracer.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile_2 =
R"doc(Construct from depth/speed tables.
Args:
    z: depths (m), monotonically increasing
    c: sound speeds (m/s))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_c = R"doc([L+1] sound speeds (m/s), corresponding to _z)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_g = R"doc(gradient (c_{i+1}-c_i)/(z_{i+1}-z_i) per layer)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_c = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_g = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_inv_g = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_iso = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_n_layers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed = R"doc(Sound speed at depth z (linear interp inside layers, clamped at ends).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_inv_g = R"doc(1/g per layer (0.0f for iso-velocity layers))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_iso = R"doc(iso-velocity flag per layer (|g| < eps))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_recompute_layer_constants = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_uniform = R"doc(Convenience: constant SVP from surface to z_max.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_z = R"doc([L+1] depths (m), monotonically increasing, z[0] >= 0)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


