//sourcehash: 135f93b9b66996cf457a69bd2f7a0d3061cd1af32e9d0df7790018b65c445abb

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
       precomputations for use by the LayerRaytracer.

Depths are absolute (e.g. metres below the sea surface). Optional
metadata (timestamp, latitude, longitude) is stored as
``std::optional<double>``.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile_2 =
R"doc(Construct from depth/speed tables.
Args:
    z: depths (m), monotonically increasing (absolute coordinates)
    c: sound speeds (m/s))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_c = R"doc([L+1] sound speeds (m/s), corresponding to _z)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_g = R"doc(gradient (c_{i+1}-c_i)/(z_{i+1}-z_i) per layer)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_c = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_date_string =
R"doc(Format ``_timestamp`` as a date string.

Returns ``"no timestamp"`` if no timestamp is set.

Args:
    fractionalSecondsDigits: passed to
                             ``timeconv::unixtime_to_datestring``
    format: passed to ``timeconv::unixtime_to_datestring``)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_depth_in_meters = R"doc(Depth (m) at the given knot index.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_depths_in_meters = R"doc(All depth knots (m), absolute coordinates.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_g = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_inv_g = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_iso = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_latitude = R"doc(Latitude (decimal degrees, +N) where the profile was measured.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_longitude = R"doc(Longitude (decimal degrees, +E) where the profile was measured.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_n_layers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_number_of_entries = R"doc(Number of (depth, sound speed) entries.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed = R"doc(Sound speed at depth z (linear interp inside layers, clamped at ends).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed_in_meters_per_second = R"doc(Sound speed (m/s) at the given knot index.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speeds_in_meters_per_second = R"doc(All sound speeds (m/s), one per depth knot.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_timestamp =
R"doc(Unix timestamp (seconds since epoch, UTC) when the profile was
measured.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_z = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_has_location = R"doc(True iff both latitude and longitude are set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_has_timestamp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_inv_g = R"doc(1/g per layer (0.0f for iso-velocity layers))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_iso = R"doc(iso-velocity flag per layer (|g| < eps))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_latitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_longitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_read_optional = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_recompute_layer_constants = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_latitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_location = R"doc(Convenience: set both latitude and longitude at once.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_longitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_timestamp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_timestamp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_uniform = R"doc(Convenience: constant SVP from surface to z_max.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_write_optional = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_z = R"doc([L+1] depths (m), monotonically increasing, absolute)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_hash_value =
R"doc(boost::flyweight requires a free ``hash_value`` for the value type.
       We hash only the depth/sound-speed tables (see
       ``hash_content_only``) so that profiles that differ only in
       timestamp / location metadata deduplicate to the same flyweight
       entry.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


