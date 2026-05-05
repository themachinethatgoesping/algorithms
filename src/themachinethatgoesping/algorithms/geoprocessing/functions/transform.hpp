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

/// @copydoc datastructures::BeamSampleGeometry::with_offset
inline datastructures::BeamSampleGeometry with_offset(datastructures::BeamSampleGeometry geom,
                                                      float dx = 0.f,
                                                      float dy = 0.f,
                                                      float dz = 0.f)
{
    geom.with_offset(dx, dy, dz);
    return geom;
}

/// @copydoc datastructures::BeamSampleGeometry::with_rigid_transform
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

/// @copydoc datastructures::BeamSampleGeometry::with_geolocation(const navigation::datastructures::Geolocation&)
inline datastructures::BeamSampleGeometry with_geolocation(
    datastructures::BeamSampleGeometry             geom,
    const navigation::datastructures::Geolocation& g)
{
    geom.with_geolocation(g);
    return geom;
}

/// @copydoc datastructures::BeamSampleGeometry::with_geolocation(const navigation::datastructures::GeolocationLocal&)
inline datastructures::BeamSampleGeometry with_geolocation(
    datastructures::BeamSampleGeometry                  geom,
    const navigation::datastructures::GeolocationLocal& g)
{
    geom.with_geolocation(g);
    return geom;
}

} // namespace functions
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
