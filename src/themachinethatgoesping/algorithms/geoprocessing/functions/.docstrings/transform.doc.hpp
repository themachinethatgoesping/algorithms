//sourcehash: 33431a7f43275d590dd8b708732e29ebb34ea9e0e1e4a78406571fa6352eb363

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_functions_with_geolocation =
R"doc(Return a copy of the geometry with a Geolocation (depth +
yaw/pitch/roll) applied (non-mutating wrapper around
BeamSampleGeometry::with_geolocation).

Equivalent to with_rigid_transform(g.yaw(), g.pitch(), g.roll(), 0, 0,
g.z).

Args:
    geom: geometry to transform (taken by value, moved into the
          result)
    g: geolocation providing the depth and yaw/pitch/roll to apply

Returns:
    a new BeamSampleGeometry with the geolocation applied

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_functions_with_geolocation_2 =
R"doc(Return a copy of the geometry with a GeolocationLocal
(northing/easting/ depth + yaw/pitch/roll) applied (non-mutating
wrapper around BeamSampleGeometry::with_geolocation).

The resulting (x, y, z) are expressed as (northing, easting, depth): x
<-> northing and y <-> easting (northing-first convention).

Args:
    geom: geometry to transform (taken by value, moved into the
          result)
    g: local geolocation providing northing/easting/depth and
       yaw/pitch/roll

Returns:
    a new BeamSampleGeometry with the geolocation applied

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_functions_with_geolocation_3 =
R"doc(Return a copy of the geometry with a GeolocationUTM (UTM
northing/easting/ depth + yaw/pitch/roll) applied, subtracting a
float64 reference origin to preserve precision (non-mutating wrapper
around BeamSampleGeometry::with_geolocation).

Absolute UTM coordinates (~10^6 m) cannot be represented accurately in
the float32 affines; the reference origin is subtracted in double
precision before casting to float, so the result is expressed in
metres relative to that reference. Output convention: x <-> northing-
relative, y <-> easting-relative, z <-> depth.

Args:
    geom: geometry to transform (taken by value, moved into the
          result)
    g: UTM pose (northing/easting in m, yaw/pitch/roll in °)
    ref_northing: reference UTM northing in m, subtracted from
                  g.northing (default 0)
    ref_easting: reference UTM easting in m, subtracted from g.easting
                 (default 0)

Returns:
    a new BeamSampleGeometry with the geolocation applied

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_functions_with_offset =
R"doc(Return a copy of the geometry with a translation (dx, dy, dz) baked
into its affines (non-mutating wrapper around
BeamSampleGeometry::with_offset).

Only the per-beam offsets of set affines are modified; slopes are
untouched. Axes whose affine is unset are ignored silently.

Args:
    geom: geometry to transform (taken by value, moved into the
          result)
    dx: translation along x in m (default 0)
    dy: translation along y in m (default 0)
    dz: translation along z in m (default 0)

Returns:
    a new BeamSampleGeometry with the translation applied

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_functions_with_rigid_transform =
R"doc(Return a copy of the geometry with a rigid transform (yaw/pitch/roll
rotation + translation) baked into its affines (non-mutating wrapper
around BeamSampleGeometry::with_rigid_transform).

Requires that all three (x, y, z) affines are set, since a rotation
mixes the axes.

Args:
    geom: geometry to transform (taken by value, moved into the
          result)
    yaw_deg: yaw in °, 0° == north, 90° == east
    pitch_deg: pitch in °, positive bow up
    roll_deg: roll in °, positive port up
    tx: translation along x in m, applied AFTER rotation (default 0)
    ty: translation along y in m, applied AFTER rotation (default 0)
    tz: translation along z in m, applied AFTER rotation (default 0)

Returns:
    a new BeamSampleGeometry with the rigid transform applied

)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


