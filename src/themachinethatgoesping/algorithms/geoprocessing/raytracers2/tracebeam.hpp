// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// trace_beam — closed-form single-beam raytrace through a layered SVP
// -----------------------------------------------------------------------------
// Traces one beam from a launch depth and launch angle through a
// SoundVelocityProfile until a requested two-way travel time is reached and
// returns the ray path as a BeamTrace (see beamtrace.hpp for the frame and
// sign conventions).
//
// The beam is launched in the athwartships y-z plane with direction
// (0, -sin(angle), cos(angle)); angle 0 points straight down (+z) and the beam
// may be launched in any direction (downward or upward). Inside each layer the
// path is one analytic segment: a straight line in iso-velocity layers and a
// circular arc in constant-gradient layers. A point is emitted at the launch
// position, at every layer crossing, at every interior turning point (where the
// ray becomes horizontal and reverses its vertical direction) and at the final
// position at the requested travel time. If the ray leaves the profile (top or
// bottom) before the travel time is reached, the trace stops at that exit.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/tracebeam.doc.hpp"

#include "beamtrace.hpp"
#include "soundvelocityprofile.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <vector>

#include <fmt/format.h>

#include <xtensor/containers/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

namespace tracebeam_detail {

/**
 * @brief Closed-form geometry of one straight ray segment in an iso-velocity layer.
 *
 * For a ray with Snell parameter p = sin(theta) / c in a layer of constant sound
 * speed @p sound_speed, spanning a vertical extent @p vertical_extent (m, >= 0),
 * returns the horizontal distance, one-way travel time and along-ray path length
 * of that segment. The ray angle theta is constant across an iso-velocity layer.
 * This is the shared kernel used by both trace_beam and trace_beam_to_depth.
 */
inline void layer_segment_iso(float  sound_speed,
                              float  ray_parameter,
                              float  vertical_extent,
                              float& horizontal_distance,
                              float& travel_time,
                              float& path_length)
{
    const float sine    = ray_parameter * sound_speed;
    const float cosine  = std::sqrt(std::max(0.0f, 1.0f - sine * sine));
    path_length         = vertical_extent / cosine;
    horizontal_distance = path_length * sine;
    travel_time         = path_length / sound_speed;
}

/**
 * @brief Closed-form geometry of one circular-arc ray segment in a constant-gradient layer.
 *
 * For a ray with Snell parameter p in a layer of gradient @p gradient (1/s, != 0),
 * going from sound speed @p sound_speed_1 (cosine @p cosine_1) to @p sound_speed_2
 * (cosine @p cosine_2), returns the horizontal distance (>= 0), the signed one-way
 * travel time (positive along increasing depth) and the along-ray path length.
 * This is the shared kernel used by both trace_beam and trace_beam_to_depth.
 */
inline void layer_segment_gradient(float  gradient,
                                   float  sound_speed_1,
                                   float  cosine_1,
                                   float  sound_speed_2,
                                   float  cosine_2,
                                   float  ray_parameter,
                                   float& horizontal_distance,
                                   float& signed_travel_time,
                                   float& path_length)
{
    horizontal_distance =
        std::abs(ray_parameter * (sound_speed_2 * sound_speed_2 - sound_speed_1 * sound_speed_1) /
                 (gradient * (cosine_1 + cosine_2)));
    signed_travel_time = (std::log(sound_speed_2 / (1.0f + cosine_2)) -
                          std::log(sound_speed_1 / (1.0f + cosine_1))) /
                         gradient;
    const float theta_1 = std::acos(std::clamp(cosine_1, -1.0f, 1.0f));
    const float theta_2 = std::acos(std::clamp(cosine_2, -1.0f, 1.0f));
    path_length =
        std::abs(theta_2 - theta_1) / std::max(std::abs(ray_parameter * gradient), 1e-12f);
}

} // namespace tracebeam_detail

/**
 * @brief Trace a single beam through a layered sound velocity profile.
 *
 * Emits one point at launch, one at each layer crossing and turning point,
 * and a final point at the requested travel time (or when the ray exits the profile).
 *
 * The Snell ray parameter (the invariant that governs refraction) is defined by the
 * launch angle and the sound speed at which the beam was formed. For a multibeam that is
 * the measured surface/transducer sound speed (SSV). Pass it as
 * @p surface_sound_speed_in_meters_per_second whenever it differs from the profile value
 * at the launch depth (e.g. the real-time SSV differs from the archived cast); otherwise
 * the profile value at the launch depth is used, and both agree exactly when the two
 * speeds are equal. Using the wrong launch sound speed introduces an angle-dependent
 * (outer-beam) depth bias.
 *
 * @param launch_depth_in_meters         launch depth (m, positive down); must be inside the profile
 * range.
 * @param launch_angle_in_degrees        angle from straight down (deg); 0 = down, positive = port.
 * @param sound_velocity_profile         profile to trace through.
 * @param two_way_travel_time_in_seconds two-way travel time budget (s).
 * @param surface_sound_speed_in_meters_per_second sound speed (m/s) at which the beam was
 *        formed; the ray parameter is sin(angle)/this. std::nullopt (default, i.e. not provided)
 *        falls back to the profile value at the launch depth.
 * @return BeamTrace with the launch point, layer crossings, turning points and the final point.
 */
inline BeamTrace trace_beam(
    float                       launch_depth_in_meters,
    float                       launch_angle_in_degrees,
    const SoundVelocityProfile& sound_velocity_profile,
    float                       two_way_travel_time_in_seconds,
    std::optional<float>        surface_sound_speed_in_meters_per_second = std::nullopt)
{
    const auto&  depths       = sound_velocity_profile.get_depths_in_meters();
    const auto&  sound_speeds = sound_velocity_profile.get_sound_speeds_in_meters_per_second();
    const auto&  gradients    = sound_velocity_profile.get_sound_speed_gradients_in_per_second();
    const auto&  isovelocity  = sound_velocity_profile.get_isovelocity_flags();
    const size_t number_of_layers = sound_velocity_profile.get_number_of_layers();

    if (number_of_layers == 0)
        throw std::runtime_error("trace_beam: sound velocity profile is not initialized");

    const float surface_depth = depths.unchecked(0);
    const float bottom_depth  = depths.unchecked(number_of_layers);
    if (!(launch_depth_in_meters >= surface_depth) || !(launch_depth_in_meters <= bottom_depth))
        throw std::runtime_error(
            fmt::format("trace_beam: launch depth {} m is outside the profile range [{}, {}] m",
                        launch_depth_in_meters,
                        surface_depth,
                        bottom_depth));

    // --- launch setup (float precision for the integration) ---
    constexpr float deg_to_rad  = 3.1415926535f / 180.0f;
    const float     angle       = launch_angle_in_degrees * deg_to_rad;
    const float     time_budget = 0.5f * two_way_travel_time_in_seconds; // one-way

    const float c0    = sound_velocity_profile.get_sound_speed(launch_depth_in_meters);
    const float sin_a = std::sin(angle);
    const float cos_a = std::cos(angle);
    // The Snell invariant is fixed by the sound speed at which the beam was formed (the
    // surface/transducer SSV when provided), not necessarily the profile value at the
    // launch depth. The ray still propagates through the profile starting at c0.
    const float launch_reference_speed = surface_sound_speed_in_meters_per_second.value_or(c0);
    const float p = std::abs(sin_a) / launch_reference_speed; // Snell invariant
    const float hsign =
        sin_a > 0.0f ? -1.0f : (sin_a < 0.0f ? 1.0f : 0.0f); // athwartships travel sign

    // Locate the layer that contains the launch depth.
    size_t layer = 0;
    {
        size_t lo = 0, hi = number_of_layers;
        while (hi - lo > 1)
        {
            const size_t mid                                           = (lo + hi) / 2;
            (launch_depth_in_meters < depths.unchecked(mid) ? hi : lo) = mid;
        }
        layer = lo;
    }

    // running state along the ray
    float z    = launch_depth_in_meters;
    float t    = 0.0f; // one-way travel time (s)
    float h    = 0.0f; // horizontal offset (m) from launch point, positive starboard
    float c    = c0;   // sound speed at z (m/s)
    float cosm = std::sqrt(std::max(
        0.0f,
        1.0f -
            (p * c) *
                (p * c))); // |cos(theta)| Theta = angle from vertical, 1 down, 0 horizontal, -1 up

    // vertical travel direction (+1 down, -1 up). A (near) horizontal launch
    // starts at the ray apex and curves towards decreasing sound speed.
    int vdir;
    if (std::abs(cos_a) > 1e-9f)
        vdir = cos_a > 0.0f ? 1 : -1;
    else
        vdir = gradients.unchecked(layer) > 0.0f ? -1 : 1;

    // output points; point 0 is the launch point
    std::vector<float> out_depths       = { z };
    std::vector<float> out_offsets      = { 0.f };
    std::vector<float> out_travel_times = { 0.f };
    std::vector<float> out_cos_angles   = { cos_a };

    auto emit = [&](float depth, float offset, float one_way_time, float cos_angle) {
        out_depths.push_back(depth);
        out_offsets.push_back(offset);
        out_travel_times.push_back(2.0f * one_way_time);
        out_cos_angles.push_back(cos_angle);
    };

    // Upper bound on emitted points: launch + at most a crossing/turn per layer
    // and direction + final. Also guards against pathological non-progress.
    const size_t max_points = 8 * number_of_layers + 16;

    while (t < time_budget && out_depths.size() < max_points)
    {
        const float gradient  = gradients.unchecked(layer);
        const float remaining = time_budget - t;

        const size_t boundary_knot  = vdir > 0 ? layer + 1 : layer;
        const float  boundary_depth = depths.unchecked(boundary_knot);
        const float  boundary_speed = sound_speeds.unchecked(boundary_knot);

        // --- iso-velocity layer: straight ray ---
        if (isovelocity.unchecked(layer))
        {
            if (cosm < 1e-9f)
            {
                // horizontal ray inside an iso layer: can only run out of time
                emit(z, h + hsign * c * remaining, time_budget, 0.0f);
                break;
            }

            const float time_to_boundary = std::abs(boundary_depth - z) / (c * cosm);
            if (time_to_boundary >= remaining)
            {
                const float path = c * remaining;
                emit(z + vdir * cosm * path, h + hsign * (p * c) * path, time_budget, vdir * cosm);
                break;
            }

            const float path = std::abs(boundary_depth - z) / cosm;
            z                = boundary_depth;
            h += hsign * (p * c) * path;
            t += time_to_boundary;
            emit(z, h, t, vdir * cosm); // c and cosm are unchanged in an iso layer

            if (boundary_knot == 0 || boundary_knot == number_of_layers)
                break; // left the profile
            layer = vdir > 0 ? layer + 1 : layer - 1;
            continue;
        }

        // --- constant-gradient layer: circular arc ---
        // The ray turns inside this layer if it would reach c = 1/p (horizontal)
        // before the boundary, i.e. the boundary sound speed is beyond critical.
        const bool turns = p > 0.0f && p * boundary_speed >= 1.0f;

        float target_speed, target_cos, target_depth;
        if (turns)
        {
            target_speed = 1.0f / p;
            target_cos   = 0.0f;
            target_depth =
                depths.unchecked(layer) + (target_speed - sound_speeds.unchecked(layer)) / gradient;
        }
        else
        {
            target_speed = boundary_speed;
            target_cos = std::sqrt(std::max(0.0f, 1.0f - (p * target_speed) * (p * target_speed)));
            target_depth = boundary_depth;
        }

        // Closed-form arc geometry to the segment end (shared kernel). Its signed
        // travel time also fixes the branch used by the partial-step inversion below.
        float seg_horizontal, signed_time, seg_path;
        tracebeam_detail::layer_segment_gradient(
            gradient, c, cosm, target_speed, target_cos, p, seg_horizontal, signed_time, seg_path);
        const int   segment_sign = signed_time >= 0.0f ? 1 : -1;
        const float segment_time = std::abs(signed_time);

        if (segment_time >= remaining)
        {
            // ray runs out of time inside this layer -> closed-form partial step:
            // invert time -> sound speed, then evaluate the segment geometry.
            const float factor =
                (c / (1.0f + cosm)) * std::exp(gradient * segment_sign * remaining);
            const float speed   = 2.0f * factor / (1.0f + (factor * p) * (factor * p));
            const float cos_end = std::sqrt(std::max(0.0f, 1.0f - (p * speed) * (p * speed)));
            const float offset =
                hsign * std::abs(p * (speed * speed - c * c) / (gradient * (cosm + cos_end)));
            emit(z + (speed - c) / gradient, h + offset, time_budget, vdir * cos_end);
            break;
        }

        // advance to the segment end (turning apex or layer boundary)
        z = target_depth;
        h += hsign * seg_horizontal;
        t += segment_time;
        c    = target_speed;
        cosm = target_cos;

        if (turns)
        {
            // apex reached: the beam reverses its vertical direction, same layer
            vdir = -vdir;
            emit(z, h, t, 0.0f);
            continue;
        }

        emit(z, h, t, vdir * cosm);
        if (boundary_knot == 0 || boundary_knot == number_of_layers)
            break; // left the profile
        layer = vdir > 0 ? layer + 1 : layer - 1;
    }

    // build the result tables
    auto to_tensor = [](const std::vector<float>& values) {
        xt::xtensor<float, 1> tensor = xt::xtensor<float, 1>::from_shape({ values.size() });
        std::copy(values.begin(), values.end(), tensor.begin());
        return tensor;
    };

    return BeamTrace(to_tensor(out_depths),
                     to_tensor(out_offsets),
                     to_tensor(out_travel_times),
                     to_tensor(out_cos_angles));
}

/**
 * @brief Endpoint of one ray leg traced down to a target depth (fast, no polyline).
 *
 * Produced by trace_beam_to_depth. The horizontal offset and path length are
 * magnitudes for the single leg between the launch depth and the target depth;
 * the caller carries the horizontal azimuth of the leg separately.
 */
struct RayToDepth
{
    /// Horizontal distance (m, >= 0) from the launch point to the target depth.
    float horizontal_offset_in_meters = 0.f;
    /// One-way travel time (s) from the launch point to the target depth.
    float one_way_travel_time_in_seconds = 0.f;
    /// Along-ray path length (m) from the launch point to the target depth.
    float path_length_in_meters = 0.f;
    /// Cosine of the ray angle from straight down at the target depth (after refraction).
    float cos_angle_at_target = 1.f;
    /// True if the ray reached the target depth (false if it turned or left the profile first).
    bool reached_target = true;
};

/**
 * @brief Trace one ray leg from a launch depth/angle down to a target depth.
 *
 * Uses the identical layered-Snell principle as trace_beam (the same
 * tracebeam_detail closed-form iso/gradient segment kernels), but integrates to a
 * target depth instead of a travel-time budget and only accumulates the endpoint
 * (no polyline). This is the fast inner step of the bistatic solver, which calls
 * it many times per beam while searching for the seabed point; once converged,
 * the full per-layer polyline of each leg is produced with trace_beam.
 *
 * The launch is downward (0 = nadir); if the ray turns (becomes horizontal) or
 * leaves the profile before the target depth, reached_target is false.
 *
 * @param sound_velocity_profile          layered profile to trace through.
 * @param launch_depth_in_meters          depth (m, positive down) of the leg origin; must be within
 * the profile.
 * @param launch_zenith_angle_in_radians  ray angle from straight down at the launch point (0 =
 * nadir).
 * @param target_depth_in_meters          depth (m, positive down) to trace to; must be > launch
 * depth and within the profile.
 * @param surface_sound_speed_in_meters_per_second sound speed (m/s) at which the beam was
 *        formed; the ray parameter is sin(zenith)/this. std::nullopt (default, i.e. not provided)
 *        falls back to the profile value at the launch depth. Must match trace_beam so
 *        mono/bistatic agree.
 * @return RayToDepth endpoint of the leg.
 */
inline RayToDepth trace_beam_to_depth(
    const SoundVelocityProfile& sound_velocity_profile,
    float                       launch_depth_in_meters,
    float                       launch_zenith_angle_in_radians,
    float                       target_depth_in_meters,
    std::optional<float>        surface_sound_speed_in_meters_per_second = std::nullopt)
{
    RayToDepth result;

    const auto&  depths      = sound_velocity_profile.get_depths_in_meters();
    const auto&  gradients   = sound_velocity_profile.get_sound_speed_gradients_in_per_second();
    const auto&  isovelocity = sound_velocity_profile.get_isovelocity_flags();
    const size_t number_of_layers = sound_velocity_profile.get_number_of_layers();

    if (number_of_layers == 0)
        throw std::runtime_error("trace_beam_to_depth: sound velocity profile is not initialized");

    const float surface_depth = depths.unchecked(0);
    const float bottom_depth  = depths.unchecked(number_of_layers);

    if (!(target_depth_in_meters > launch_depth_in_meters))
        return result; // nothing to trace (target at or above the launch depth)

    if (launch_depth_in_meters < surface_depth - 1e-3f ||
        target_depth_in_meters > bottom_depth + 1e-3f)
    {
        result.reached_target = false;
        return result; // profile does not cover the requested depth range
    }

    const float launch_sound_speed = sound_velocity_profile.get_sound_speed(launch_depth_in_meters);
    // Ray parameter fixed by the beam-forming sound speed (surface/transducer SSV when
    // provided); the leg still propagates through the profile starting at launch_sound_speed.
    const float reference_sound_speed =
        surface_sound_speed_in_meters_per_second.value_or(launch_sound_speed);
    const float ray_parameter = std::sin(launch_zenith_angle_in_radians) / reference_sound_speed;

    // locate the layer containing the launch depth
    size_t layer = 0;
    {
        size_t lo = 0, hi = number_of_layers;
        while (hi - lo > 1)
        {
            const size_t mid                                           = (lo + hi) / 2;
            (launch_depth_in_meters < depths.unchecked(mid) ? hi : lo) = mid;
        }
        layer = lo;
    }

    float depth            = launch_depth_in_meters;
    float horizontal_range = 0.0f;
    float travel_time      = 0.0f;
    float path_length      = 0.0f;
    float cos_at_depth     = std::sqrt(std::max(
        0.0f, 1.0f - (ray_parameter * launch_sound_speed) * (ray_parameter * launch_sound_speed)));

    while (depth < target_depth_in_meters - 1e-9f && layer < number_of_layers)
    {
        const float layer_bottom_depth = depths.unchecked(layer + 1);
        const float segment_bottom     = std::min(target_depth_in_meters, layer_bottom_depth);

        const float sound_speed_top = sound_velocity_profile.get_sound_speed(depth);
        const float sin_top         = ray_parameter * sound_speed_top;
        if (std::abs(sin_top) >= 1.0f)
        {
            result.reached_target = false; // ray is horizontal / turning at this depth
            break;
        }
        const float cos_top = std::sqrt(std::max(0.0f, 1.0f - sin_top * sin_top));

        float segment_horizontal, segment_time, segment_path;
        if (isovelocity.unchecked(layer))
        {
            tracebeam_detail::layer_segment_iso(sound_speed_top,
                                                ray_parameter,
                                                segment_bottom - depth,
                                                segment_horizontal,
                                                segment_time,
                                                segment_path);
            cos_at_depth = cos_top;
        }
        else
        {
            const float gradient           = gradients.unchecked(layer);
            const float sound_speed_bottom = sound_speed_top + gradient * (segment_bottom - depth);
            const float sin_bottom         = ray_parameter * sound_speed_bottom;
            if (std::abs(sin_bottom) >= 1.0f)
            {
                result.reached_target = false; // ray turns before reaching the segment bottom
                break;
            }
            const float cos_bottom = std::sqrt(std::max(0.0f, 1.0f - sin_bottom * sin_bottom));

            float signed_time;
            tracebeam_detail::layer_segment_gradient(gradient,
                                                     sound_speed_top,
                                                     cos_top,
                                                     sound_speed_bottom,
                                                     cos_bottom,
                                                     ray_parameter,
                                                     segment_horizontal,
                                                     signed_time,
                                                     segment_path);
            segment_time = std::abs(signed_time);
            cos_at_depth = cos_bottom;
        }

        horizontal_range += segment_horizontal;
        travel_time += segment_time;
        path_length += segment_path;

        depth = segment_bottom;
        if (segment_bottom >= layer_bottom_depth)
            ++layer;
    }

    if (depth < target_depth_in_meters - 1e-6f)
        result.reached_target = false;

    result.horizontal_offset_in_meters    = horizontal_range;
    result.one_way_travel_time_in_seconds = travel_time;
    result.path_length_in_meters          = path_length;
    result.cos_angle_at_target            = cos_at_depth;
    return result;
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping