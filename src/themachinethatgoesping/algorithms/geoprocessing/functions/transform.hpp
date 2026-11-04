// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// transform.hpp — free-function wrappers around BeamSampleGeometry's
// rigid-transform member methods. Convenient for value-style chaining:
//
//   auto world = with_geolocation(sensor_geom, sensor_pose);
//   auto shifted = with_offset(geom, 0.f, 0.f, depth);
//
// The actual implementation lives in BeamSampleGeometry's members (with_offset,
// with_rigid_transform, with_geolocation). These wrappers take the geometry by
// value to enable move semantics and chained, non-mutating composition.
//
// Lat/lon transforms are NOT supported — convert to a local tangent
// (UTM / GeolocationLocal) first, then apply with_geolocation.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/transform.doc.hpp"

#include "../datastructures/beamsamplegeometry.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace functions {

/**
 * @brief Return a copy of the geometry with a translation (dx, dy, dz) baked into
 * its affines (non-mutating wrapper around BeamSampleGeometry::with_offset).
 *
 * Only the per-beam offsets of set affines are modified; slopes are untouched.
 * Axes whose affine is unset are ignored silently.
 *
 * @param geom geometry to transform (taken by value, moved into the result)
 * @param dx translation along x in m (default 0)
 * @param dy translation along y in m (default 0)
 * @param dz translation along z in m (default 0)
 * @return a new BeamSampleGeometry with the translation applied
 */
inline datastructures::BeamSampleGeometry with_offset(datastructures::BeamSampleGeometry geom,
                                                      float dx = 0.f,
                                                      float dy = 0.f,
                                                      float dz = 0.f)
{
    geom.with_offset(dx, dy, dz);
    return geom;
}

/**
 * @brief Return a copy of the geometry with a rigid transform (yaw/pitch/roll
 * rotation + translation) baked into its affines (non-mutating wrapper around
 * BeamSampleGeometry::with_rigid_transform).
 *
 * Requires that all three (x, y, z) affines are set, since a rotation mixes the
 * axes.
 *
 * @param geom geometry to transform (taken by value, moved into the result)
 * @param yaw_deg yaw in °, 0° == north, 90° == east
 * @param pitch_deg pitch in °, positive bow up
 * @param roll_deg roll in °, positive port up
 * @param tx translation along x in m, applied AFTER rotation (default 0)
 * @param ty translation along y in m, applied AFTER rotation (default 0)
 * @param tz translation along z in m, applied AFTER rotation (default 0)
 * @return a new BeamSampleGeometry with the rigid transform applied
 */
inline datastructures::BeamSampleGeometry with_rigid_transform(
    datastructures::BeamSampleGeometry geom,
    float                              yaw_deg,
    float                              pitch_deg,
    float                              roll_deg,
    float                              tx = 0.f,
    float                              ty = 0.f,
    float                              tz = 0.f)
{
    geom.with_rigid_transform(yaw_deg, pitch_deg, roll_deg, tx, ty, tz);
    return geom;
}

/**
 * @brief Return a copy of the geometry with a Geolocation (depth + yaw/pitch/roll)
 * applied (non-mutating wrapper around BeamSampleGeometry::with_geolocation).
 *
 * Equivalent to with_rigid_transform(g.yaw(), g.pitch(), g.roll(), 0, 0, g.z).
 *
 * @param geom geometry to transform (taken by value, moved into the result)
 * @param g geolocation providing the depth and yaw/pitch/roll to apply
 * @return a new BeamSampleGeometry with the geolocation applied
 */
inline datastructures::BeamSampleGeometry with_geolocation(
    datastructures::BeamSampleGeometry             geom,
    const navigation::datastructures::Geolocation& g)
{
    geom.with_geolocation(g);
    return geom;
}

/**
 * @brief Return a copy of the geometry with a GeolocationLocal (northing/easting/
 * depth + yaw/pitch/roll) applied (non-mutating wrapper around
 * BeamSampleGeometry::with_geolocation).
 *
 * The resulting (x, y, z) are expressed as (northing, easting, depth):
 * x <-> northing and y <-> easting (northing-first convention).
 *
 * @param geom geometry to transform (taken by value, moved into the result)
 * @param g local geolocation providing northing/easting/depth and yaw/pitch/roll
 * @return a new BeamSampleGeometry with the geolocation applied
 */
inline datastructures::BeamSampleGeometry with_geolocation(
    datastructures::BeamSampleGeometry                  geom,
    const navigation::datastructures::GeolocationLocal& g)
{
    geom.with_geolocation(g);
    return geom;
}

/**
 * @brief Return a copy of the geometry with a GeolocationUTM (UTM northing/easting/
 * depth + yaw/pitch/roll) applied, subtracting a float64 reference origin to
 * preserve precision (non-mutating wrapper around
 * BeamSampleGeometry::with_geolocation).
 *
 * Absolute UTM coordinates (~10^6 m) cannot be represented accurately in the
 * float32 affines; the reference origin is subtracted in double precision before
 * casting to float, so the result is expressed in metres relative to that
 * reference. Output convention: x <-> northing-relative, y <-> easting-relative,
 * z <-> depth.
 *
 * @param geom geometry to transform (taken by value, moved into the result)
 * @param g UTM pose (northing/easting in m, yaw/pitch/roll in °)
 * @param ref_northing reference UTM northing in m, subtracted from g.northing (default 0)
 * @param ref_easting reference UTM easting in m, subtracted from g.easting (default 0)
 * @return a new BeamSampleGeometry with the geolocation applied
 */
inline datastructures::BeamSampleGeometry with_geolocation(
    datastructures::BeamSampleGeometry                geom,
    const navigation::datastructures::GeolocationUTM& g,
    double                                            ref_northing = 0.0,
    double                                            ref_easting  = 0.0)
{
    geom.with_geolocation(g, ref_northing, ref_easting);
    return geom;
}

} // namespace functions
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
