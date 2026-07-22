//sourcehash: fb71226b94bd8f4db2558376ca2c2de0a25a3f55fd2811cdf67d209a89aa3252

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

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile = R"doc(Construct an empty SoundVelocityProfile.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_SoundVelocityProfile_2 =
R"doc(Construct from depth/sound-speed tables.
Args:
    z: monotonically increasing depth knots (m, positive down).
    c: corresponding sound speeds (m/s, must be positive).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_depths = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_date_string =
R"doc(Format ``_timestamp`` as a date string.

Returns ``"no timestamp"`` if no timestamp is set.

Args:
    fractionalSecondsDigits: passed to
                             ``timeconv::unixtime_to_datestring``
    format: passed to ``timeconv::unixtime_to_datestring``)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_depth_in_meters = R"doc(Depth (m) at the given knot index.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_depths_in_meters = R"doc(All depth knots (m), absolute coordinates.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_inverse_sound_speed_gradients_in_seconds =
R"doc(1 / gradient (s) per layer; 0 for iso-velocity layers (size =
number_of_layers).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_isovelocity_flags =
R"doc(Per-layer iso-velocity flag: true when |gradient| < ISO_EPS (size =
number_of_layers).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_latitude =
R"doc(Latitude (decimal degrees, +N) where the profile was measured, or
std::nullopt if unset.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_longitude =
R"doc(Longitude (decimal degrees, +E) where the profile was measured, or
std::nullopt if unset.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_number_of_entries = R"doc(Number of (depth, sound speed) entries (= number of layers + 1).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_number_of_layers = R"doc(Number of layers (= number of knots − 1).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_profile_with_surface_sound_speed =
R"doc(Return a copy of this profile with a measured surface (transducer)
sound speed
       integrated at the transducer depth (Kongsberg "SHC=0"
       convention).

The returned profile replaces every knot at or above
``transducer_depth_in_meters`` with an iso-velocity segment at
``surface_sound_speed_in_meters_per_second`` (from depth 0 down to the
transducer depth) and keeps the archived knots strictly below the
transducer depth. This makes the sound speed at the transducer equal
to the real-time measured surface sound speed (SSV), which is what the
echosounder uses when forming the beams; a beam launched at the
transducer depth is then self-consistent (the Snell launch/reference
speed and the profile value at the launch depth agree, removing the
angle-dependent outer-beam depth bias that appears when the archived
profile value at the transducer differs from the measured SSV).

The measured surface sound speed is also stored as metadata on the
returned profile (get_surface_sound_speed()).

Args:
    surface_sound_speed_in_meters_per_second: measured sound speed at
                                              the transducer (m/s,
                                              >0).
    transducer_depth_in_meters: transducer depth below the surface (m,
                                >= 0).

Returns:
    SoundVelocityProfile extended with the surface sound speed.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed = R"doc(Sound speed at depth z (linear interp inside layers, clamped at ends).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed_gradients_in_per_second = R"doc(Sound-speed gradient dc/dz (s⁻¹) per layer (size = number_of_layers).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speed_in_meters_per_second = R"doc(Sound speed (m/s) at the given knot index.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_sound_speeds_in_meters_per_second = R"doc(All sound speeds (m/s), one per depth knot.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_surface_sound_speed =
R"doc(Measured transducer/surface sound speed (m/s), or std::nullopt if
unset.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_get_timestamp =
R"doc(Unix timestamp (s, UTC) when the profile was measured, or std::nullopt
if unset.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_gradients = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_has_location = R"doc(True iff both latitude and longitude are set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_has_surface_sound_speed = R"doc(True iff a surface (transducer) sound speed is set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_has_timestamp = R"doc(True iff a timestamp is set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_inverse_gradients = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_isovelocity = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_latitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_longitude = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_operator_eq = R"doc(Equality comparison (metadata is ignored).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_read_optional = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_recompute_layer_constants = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set =
R"doc(Set depth/sound-speed tables and recompute layer constants.
Args:
    z: monotonically increasing depth knots (m, positive down).
    c: corresponding sound speeds (m/s, must be positive).

Raises:
    std::runtime_error: if sizes differ, fewer than 2 entries, or non-
        monotone depths.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_latitude = R"doc(Set latitude (decimal degrees, +N); pass std::nullopt to clear.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_location =
R"doc(Set both latitude (decimal degrees, +N) and longitude (decimal
degrees, +E) at once.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_longitude = R"doc(Set longitude (decimal degrees, +E); pass std::nullopt to clear.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_surface_sound_speed =
R"doc(Set the measured transducer/surface sound speed (m/s); pass
std::nullopt to clear.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_set_timestamp = R"doc(Set the unix timestamp (s, UTC); pass std::nullopt to clear.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_sound_speeds = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_surface_sound_speed = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_timestamp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_uniform =
R"doc(Constant-velocity profile from the surface to z_max.
Args:
    c: sound speed (m/s).
    z_max: maximum depth (m); default 12 000 m.

Returns:
    SoundVelocityProfile with uniform sound speed.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_SoundVelocityProfile_write_optional = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_hash_value =
R"doc(boost::flyweight requires a free ``hash_value`` for the value type.
       We hash only the depth/sound-speed tables (see
       ``hash_content_only``) so that profiles that differ only in
       timestamp / location metadata deduplicate to the same flyweight
       entry.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


