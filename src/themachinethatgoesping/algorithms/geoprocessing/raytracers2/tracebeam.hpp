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
#include <stdexcept>
#include <vector>

#include <fmt/format.h>

#include <xtensor/containers/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Trace a single beam through a layered sound velocity profile.
 *
 * Emits one point at launch, one at each layer crossing and turning point,
 * and a final point at the requested travel time (or when the ray exits the profile).
 *
 * @param launch_depth_in_meters         launch depth (m, positive down); must be inside the profile range.
 * @param launch_angle_in_degrees        angle from straight down (deg); 0 = down, positive = port.
 * @param sound_velocity_profile         profile to trace through.
 * @param two_way_travel_time_in_seconds two-way travel time budget (s).
 * @return BeamTrace with the launch point, layer crossings, turning points and the final point.
 */
inline BeamTrace trace_beam(float                       launch_depth_in_meters,
                            float                       launch_angle_in_degrees,
                            const SoundVelocityProfile& sound_velocity_profile,
                            float                       two_way_travel_time_in_seconds)
{
    const auto&  depths        = sound_velocity_profile.get_depths_in_meters();
    const auto&  sound_speeds  = sound_velocity_profile.get_sound_speeds_in_meters_per_second();
    const auto&  gradients     = sound_velocity_profile.get_sound_speed_gradients_in_per_second();
    const auto&  isovelocity   = sound_velocity_profile.get_isovelocity_flags();
    const size_t number_of_layers = sound_velocity_profile.get_number_of_layers();

    if (number_of_layers == 0)
        throw std::runtime_error("trace_beam: sound velocity profile is not initialized");

    const double surface_depth = depths.unchecked(0);
    const double bottom_depth   = depths.unchecked(number_of_layers);
    if (!(launch_depth_in_meters >= surface_depth) || !(launch_depth_in_meters <= bottom_depth))
        throw std::runtime_error(
            fmt::format("trace_beam: launch depth {} m is outside the profile range [{}, {}] m",
                        launch_depth_in_meters,
                        surface_depth,
                        bottom_depth));

    // --- launch setup (double precision for the integration) ---
    constexpr double deg_to_rad = M_PI / 180.0;
    const double     angle       = launch_angle_in_degrees * deg_to_rad;
    const double     time_budget = 0.5 * double(two_way_travel_time_in_seconds); // one-way

    const double c0    = sound_velocity_profile.get_sound_speed(launch_depth_in_meters);
    const double sin_a = std::sin(angle);
    const double cos_a = std::cos(angle);
    const double p     = std::abs(sin_a) / c0;                          // Snell invariant
    const double hsign = sin_a > 0.0 ? -1.0 : (sin_a < 0.0 ? 1.0 : 0.0); // athwartships travel sign

    // Locate the layer that contains the launch depth.
    size_t layer = 0;
    {
        size_t lo = 0, hi = number_of_layers;
        while (hi - lo > 1)
        {
            const size_t mid = (lo + hi) / 2;
            (double(launch_depth_in_meters) < double(depths.unchecked(mid)) ? hi : lo) = mid;
        }
        layer = lo;
    }

    // running state along the ray
    double z    = launch_depth_in_meters;
    double t    = 0.0;
    double h    = 0.0;
    double c    = c0;
    double cosm = std::sqrt(std::max(0.0, 1.0 - (p * c) * (p * c))); // |cos(theta)|

    // vertical travel direction (+1 down, -1 up). A (near) horizontal launch
    // starts at the ray apex and curves towards decreasing sound speed.
    int vdir;
    if (std::abs(cos_a) > 1e-9)
        vdir = cos_a > 0.0 ? 1 : -1;
    else
        vdir = gradients.unchecked(layer) > 0.0 ? -1 : 1;

    // output points; point 0 is the launch point
    std::vector<float> out_depths       = { float(z) };
    std::vector<float> out_offsets       = { 0.f };
    std::vector<float> out_travel_times  = { 0.f };
    std::vector<float> out_cos_angles    = { float(cos_a) };

    auto emit = [&](double depth, double offset, double one_way_time, double cos_angle) {
        out_depths.push_back(float(depth));
        out_offsets.push_back(float(offset));
        out_travel_times.push_back(float(2.0 * one_way_time));
        out_cos_angles.push_back(float(cos_angle));
    };

    // Upper bound on emitted points: launch + at most a crossing/turn per layer
    // and direction + final. Also guards against pathological non-progress.
    const size_t max_points = 8 * number_of_layers + 16;

    while (t < time_budget && out_depths.size() < max_points)
    {
        const double gradient = gradients.unchecked(layer);
        const double remaining = time_budget - t;

        const size_t boundary_knot = vdir > 0 ? layer + 1 : layer;
        const double boundary_depth = depths.unchecked(boundary_knot);
        const double boundary_speed  = sound_speeds.unchecked(boundary_knot);

        // --- iso-velocity layer: straight ray ---
        if (isovelocity.unchecked(layer))
        {
            if (cosm < 1e-9)
            {
                // horizontal ray inside an iso layer: can only run out of time
                emit(z, h + hsign * c * remaining, time_budget, 0.0);
                break;
            }

            const double time_to_boundary = std::abs(boundary_depth - z) / (c * cosm);
            if (time_to_boundary >= remaining)
            {
                const double path = c * remaining;
                emit(z + vdir * cosm * path, h + hsign * (p * c) * path, time_budget, vdir * cosm);
                break;
            }

            const double path = std::abs(boundary_depth - z) / cosm;
            z = boundary_depth;
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
        const bool turns = p > 0.0 && p * boundary_speed >= 1.0;

        double target_speed, target_cos, target_depth;
        if (turns)
        {
            target_speed = 1.0 / p;
            target_cos   = 0.0;
            target_depth = depths.unchecked(layer) + (target_speed - sound_speeds.unchecked(layer)) / gradient;
        }
        else
        {
            target_speed = boundary_speed;
            target_cos   = std::sqrt(std::max(0.0, 1.0 - (p * target_speed) * (p * target_speed)));
            target_depth = boundary_depth;
        }

        // Signed travel time to the segment end; its sign fixes the branch used
        // by the closed-form partial-step inversion below.
        const double signed_time =
            (std::log(target_speed / (1.0 + target_cos)) - std::log(c / (1.0 + cosm))) / gradient;
        const int    segment_sign = signed_time >= 0.0 ? 1 : -1;
        const double segment_time = std::abs(signed_time);

        if (segment_time >= remaining)
        {
            // ray runs out of time inside this layer -> closed-form partial step:
            // invert time -> sound speed, then evaluate the segment geometry.
            const double factor    = (c / (1.0 + cosm)) * std::exp(gradient * segment_sign * remaining);
            const double speed     = 2.0 * factor / (1.0 + (factor * p) * (factor * p));
            const double cos_end   = std::sqrt(std::max(0.0, 1.0 - (p * speed) * (p * speed)));
            const double offset    = hsign * std::abs(p * (speed * speed - c * c) / (gradient * (cosm + cos_end)));
            emit(z + (speed - c) / gradient, h + offset, time_budget, vdir * cos_end);
            break;
        }

        // advance to the segment end (turning apex or layer boundary)
        const double offset = hsign * std::abs(p * (target_speed * target_speed - c * c) /
                                               (gradient * (cosm + target_cos)));
        z = target_depth;
        h += offset;
        t += segment_time;
        c    = target_speed;
        cosm = target_cos;

        if (turns)
        {
            // apex reached: the beam reverses its vertical direction, same layer
            vdir = -vdir;
            emit(z, h, t, 0.0);
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

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
