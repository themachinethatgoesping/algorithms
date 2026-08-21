// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// SoundVelocityProfile resampling / thinning
// -----------------------------------------------------------------------------
// Reduce the number of depth/sound-speed knots of a SoundVelocityProfile before
// feeding it to the raytracer, so fewer layers are traversed per beam (a direct
// speed-up) while keeping the raytracing error bounded.
//
// Two methods are provided:
//
//   A) "max_sound_speed_error": classic Douglas-Peucker line simplification on the
//      c(z) curve. Removes every knot whose removal changes the linearly
//      interpolated sound speed by less than the given tolerance. Self-contained
//      (no raytracing), O(n log n), the standard "SVP thinning" used by most
//      hydrographic software. It bounds |dc|, which only indirectly bounds the ray
//      error.
//
//   B) "max_raytrace_error": a bottom-up greedy that removes the knot whose removal
//      least increases the *raytraced* seabed position error, and stops once the
//      accumulated error would exceed the budget. The global error stays within the
//      budget because, below a kept knot, Snell's law preserves the ray angle
//      (sin(theta)=p*c and c is kept), so each simplified segment's offset/time error
//      propagates as a parallel shift and the total error is the sum of the
//      per-segment errors. Rays are launched from launch_depth_in_meters (the
//      transducer depth); the sound speed there is preserved exactly (a knot is kept
//      at that depth) so the ray parameter matches the original, and the profile above
//      the transducer - which the down-going beams never traverse - is collapsed.
//
// A "number_of_entries" target is also offered: simplify until (about) that many knots
// remain, using method B's ray-error ranking.
//
// Depths below max_depth_in_meters are never exercised by the rays and so do not
// contribute to the error; pass the deepest ensonified depth to let the deep part of
// an archived profile be collapsed aggressively.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/soundvelocityprofileresampling.doc.hpp"

#include "soundvelocityprofile.hpp"
#include "tracebeam.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>

#include <xtensor/containers/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

namespace svpresampling_detail {

/// @brief Build a SoundVelocityProfile from the kept knots of (z, c), copying metadata from @p meta.
inline SoundVelocityProfile subset(const std::vector<float>&   z,
                                   const std::vector<float>&   c,
                                   const std::vector<char>&    keep,
                                   const SoundVelocityProfile& meta)
{
    std::size_t n_keep = 0;
    for (char k : keep)
        n_keep += k ? 1 : 0;

    xt::xtensor<float, 1> zk = xt::xtensor<float, 1>::from_shape({ n_keep });
    xt::xtensor<float, 1> ck = xt::xtensor<float, 1>::from_shape({ n_keep });
    std::size_t           j  = 0;
    for (std::size_t i = 0; i < z.size(); ++i)
        if (keep[i])
        {
            zk.unchecked(j) = z[i];
            ck.unchecked(j) = c[i];
            ++j;
        }

    SoundVelocityProfile out(std::move(zk), std::move(ck));
    out.set_timestamp(meta.get_timestamp());
    out.set_location(meta.get_latitude(), meta.get_longitude());
    if (meta.has_surface_sound_speed())
        out.set_surface_sound_speed(float(*meta.get_surface_sound_speed()));
    return out;
}

/// @brief Douglas-Peucker on c(z): keep[i]=true for knots to retain (endpoints always kept).
inline std::vector<char> douglas_peucker_c(const std::vector<float>& z,
                                            const std::vector<float>& c,
                                            float                     max_sound_speed_error)
{
    const std::size_t n = z.size();
    std::vector<char> keep(n, 0);
    keep.front() = 1;
    keep.back()  = 1;
    std::vector<std::pair<std::size_t, std::size_t>> stack;
    stack.emplace_back(0, n - 1);
    while (!stack.empty())
    {
        const auto [i, j] = stack.back();
        stack.pop_back();
        if (j <= i + 1)
            continue;
        const float dz    = z[j] - z[i];
        std::size_t k_max = i;
        float       d_max = 0.f;
        for (std::size_t k = i + 1; k < j; ++k)
        {
            const float t         = dz > 0.f ? (z[k] - z[i]) / dz : 0.f;
            const float c_interp  = c[i] + t * (c[j] - c[i]);
            const float deviation = std::abs(c[k] - c_interp);
            if (deviation > d_max)
            {
                d_max = deviation;
                k_max = k;
            }
        }
        if (d_max > max_sound_speed_error)
        {
            keep[k_max] = 1;
            stack.emplace_back(i, k_max);
            stack.emplace_back(k_max, j);
        }
    }
    return keep;
}

/// @brief Raytracing error (m) introduced by approximating knots [L..R] with the straight segment
///        (L,R). Traces from z[L] over a spread of ray parameters through the exact profile vs the
///        two-knot linear segment down to min(z[R], max_depth) and returns the worst of
///        |d horizontal offset| and |d one-way-time| * (sound speed at the target). Segments that a
///        beam launched at @p launch_depth never traverses (entirely above it) contribute nothing.
inline float segment_error(const SoundVelocityProfile& full,
                           const std::vector<float>&   z,
                           const std::vector<float>&   c,
                           std::size_t                 L,
                           std::size_t                 R,
                           float                       launch_depth,
                           float                       max_depth,
                           const std::vector<float>&   ray_parameters,
                           float                       sound_speed_at_max_depth)
{
    if (R <= L + 1 || z[L] >= max_depth || z[R] <= launch_depth)
        return 0.f;

    const float           target = std::min(z[R], max_depth);
    xt::xtensor<float, 1> za     = { z[L], z[R] };
    xt::xtensor<float, 1> ca     = { c[L], c[R] };
    SoundVelocityProfile  linear(std::move(za), std::move(ca));

    float error = 0.f;
    for (const float p : ray_parameters)
    {
        const double sin_zenith = double(p) * double(c[L]);
        if (sin_zenith >= 1.0)
            continue; // ray already turned above this segment
        const auto exact = tracebeam_detail::trace_to_depth_impl<false>(
            full, double(z[L]), sin_zenith, double(target), std::nullopt);
        const auto approx = tracebeam_detail::trace_to_depth_impl<false>(
            linear, double(z[L]), sin_zenith, double(target), std::nullopt);
        if (!exact.reached_target || !approx.reached_target)
            continue;
        const float d_horizontal =
            std::abs(approx.horizontal_offset_in_meters - exact.horizontal_offset_in_meters);
        const float d_depth = std::abs(approx.one_way_travel_time_in_seconds -
                                       exact.one_way_travel_time_in_seconds) *
                              sound_speed_at_max_depth;
        error = std::max(error, std::max(d_horizontal, d_depth));
    }
    return error;
}

/// @brief Bottom-up greedy knot removal ranked by raytracing error. The knots at index 0,
///        @p protected_index (the launch-depth knot) and n-1 are never removed. Stops when removing
///        the next knot would push the summed error past @p max_error, or when @p target_entries
///        knots remain. @p max_error <= 0 disables the error stop; @p target_entries == 0 disables
///        the count stop.
inline std::vector<char> greedy_raytrace(const SoundVelocityProfile& full,
                                         const std::vector<float>&   z,
                                         const std::vector<float>&   c,
                                         float                       max_error,
                                         std::size_t                 target_entries,
                                         std::size_t                 protected_index,
                                         float                       launch_depth,
                                         float                       max_depth,
                                         float                       max_launch_angle_in_degrees,
                                         int                         number_of_test_angles)
{
    const std::size_t n = z.size();
    std::vector<char> alive(n, 1);
    if (n <= 2)
        return alive;

    std::vector<std::size_t> prev(n), next(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        prev[i] = i == 0 ? 0 : i - 1;
        next[i] = i + 1; // next[n-1] == n acts as an end sentinel
    }

    constexpr float    deg_to_rad = 3.14159265358979f / 180.f;
    const float        launch_c   = full.get_sound_speed(launch_depth);
    std::vector<float> ray_parameters;
    const int          na = std::max(1, number_of_test_angles);
    for (int a = 0; a < na; ++a)
    {
        const float angle =
            2.f + (max_launch_angle_in_degrees - 2.f) * (na == 1 ? 0.f : float(a) / float(na - 1));
        ray_parameters.push_back(std::sin(angle * deg_to_rad) / launch_c);
    }
    const float cz = full.get_sound_speed(max_depth);

    auto seg = [&](std::size_t L, std::size_t R) {
        return segment_error(full, z, c, L, R, launch_depth, max_depth, ray_parameters, cz);
    };
    auto removal_cost = [&](std::size_t i) -> float {
        return seg(prev[i], next[i]) - seg(prev[i], i) - seg(i, next[i]);
    };
    auto removable = [&](std::size_t i) {
        return i > 0 && i + 1 < n && i != protected_index;
    };

    using Item = std::pair<float, std::size_t>; // (cost, knot index)
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> heap;
    for (std::size_t i = 1; i + 1 < n; ++i)
        if (removable(i))
            heap.emplace(removal_cost(i), i);

    std::size_t alive_count = n;
    float       total       = 0.f;
    const bool  use_error   = max_error > 0.f;
    const bool  use_count   = target_entries > 0;

    while (!heap.empty())
    {
        if (use_count && alive_count <= target_entries)
            break;
        const auto [cost_snapshot, i] = heap.top();
        heap.pop();
        if (!alive[i])
            continue;
        const float cost_now = removal_cost(i);
        if (cost_now > cost_snapshot + 1e-9f)
        {
            heap.emplace(cost_now, i); // stale entry, re-rank
            continue;
        }
        if (use_error && total + cost_now > max_error)
            break;

        total += cost_now;
        alive[i] = 0;
        --alive_count;
        const std::size_t L = prev[i], R = next[i];
        next[L] = R;
        prev[R] = L;
        if (removable(L))
            heap.emplace(removal_cost(L), L);
        if (removable(R))
            heap.emplace(removal_cost(R), R);
    }
    return alive;
}

inline std::vector<float> to_vector(const xt::xtensor<float, 1>& t)
{
    return std::vector<float>(t.begin(), t.end());
}

} // namespace svpresampling_detail

/**
 * @brief Return a copy of @p svp with fewer depth/sound-speed knots, chosen so that raytracing
 *        through the simplified profile stays close to the original while traversing fewer layers.
 *
 * Exactly one of the three targets must be set:
 *   - @p max_sound_speed_error_in_meters_per_second : Douglas-Peucker on c(z); keeps the
 *     interpolated sound speed within this tolerance (self-contained, fastest).
 *   - @p max_raytrace_error_in_meters : ray-error greedy; guarantees the seabed position error
 *     (horizontal and depth) stays within this budget for launch angles up to
 *     @p max_launch_angle_in_degrees, for beams launched at @p launch_depth_in_meters down to
 *     @p max_depth_in_meters (fewest layers for a given accuracy).
 *   - @p number_of_entries : simplify until about this many knots remain (ray-error ranking).
 *
 * The sound speed at @p launch_depth_in_meters (the transducer depth) is always preserved exactly so
 * a beam launched there keeps the original ray parameter; set this to the transducer depth used by
 * the raytracer.
 *
 * @param svp source profile.
 * @param max_sound_speed_error_in_meters_per_second sound-speed tolerance (m/s), or std::nullopt.
 * @param max_raytrace_error_in_meters seabed-position error budget (m), or std::nullopt.
 * @param number_of_entries target knot count (>= 2), or std::nullopt.
 * @param launch_depth_in_meters transducer/launch depth whose sound speed is preserved (default 0).
 * @param max_depth_in_meters only bound the error down to this depth; <= 0 uses the profile bottom.
 * @param max_launch_angle_in_degrees widest launch angle considered for the error (default 75).
 * @param number_of_test_angles number of launch angles sampled for the error (default 8).
 * @return simplified SoundVelocityProfile (metadata preserved).
 */
inline SoundVelocityProfile resample_soundvelocityprofile(
    const SoundVelocityProfile& svp,
    std::optional<float>        max_sound_speed_error_in_meters_per_second = std::nullopt,
    std::optional<float>        max_raytrace_error_in_meters               = std::nullopt,
    std::optional<std::size_t>  number_of_entries                          = std::nullopt,
    float                       launch_depth_in_meters                     = 0.f,
    float                       max_depth_in_meters                        = -1.f,
    float                       max_launch_angle_in_degrees                = 75.f,
    int                         number_of_test_angles                      = 8)
{
    const int n_targets = int(max_sound_speed_error_in_meters_per_second.has_value()) +
                          int(max_raytrace_error_in_meters.has_value()) +
                          int(number_of_entries.has_value());
    if (n_targets != 1)
        throw std::runtime_error(
            "resample_soundvelocityprofile: set exactly one of max_sound_speed_error_in_meters_"
            "per_second, max_raytrace_error_in_meters or number_of_entries");

    if (svp.get_number_of_entries() <= 2)
        return svp; // nothing to simplify

    std::vector<float> z = svpresampling_detail::to_vector(svp.get_depths_in_meters());
    std::vector<float> c = svpresampling_detail::to_vector(svp.get_sound_speeds_in_meters_per_second());

    // Keep a knot at the launch depth so the launch sound speed (hence the ray parameter) is exact.
    std::size_t protected_index = 0;
    if (launch_depth_in_meters > z.front() + 1e-3f && launch_depth_in_meters < z.back() - 1e-3f)
    {
        const std::size_t pos =
            std::size_t(std::lower_bound(z.begin(), z.end(), launch_depth_in_meters) - z.begin());
        if (pos < z.size() && std::abs(z[pos] - launch_depth_in_meters) <= 1e-3f)
            protected_index = pos;
        else if (pos > 0 && std::abs(z[pos - 1] - launch_depth_in_meters) <= 1e-3f)
            protected_index = pos - 1;
        else
        {
            z.insert(z.begin() + pos, launch_depth_in_meters);
            c.insert(c.begin() + pos, svp.get_sound_speed(launch_depth_in_meters));
            protected_index = pos;
        }
    }

    const std::size_t n         = z.size();
    float             max_depth = max_depth_in_meters;
    if (!(max_depth > 0.f) || max_depth > z.back())
        max_depth = z.back();

    std::vector<char> keep_flags;

    if (max_sound_speed_error_in_meters_per_second.has_value())
    {
        if (!(*max_sound_speed_error_in_meters_per_second > 0.f))
            throw std::runtime_error("resample_soundvelocityprofile: max_sound_speed_error must be > 0");
        keep_flags = svpresampling_detail::douglas_peucker_c(
            z, c, *max_sound_speed_error_in_meters_per_second);
        if (protected_index)
            keep_flags[protected_index] = 1;
    }
    else if (max_raytrace_error_in_meters.has_value())
    {
        if (!(*max_raytrace_error_in_meters > 0.f))
            throw std::runtime_error("resample_soundvelocityprofile: max_raytrace_error must be > 0");
        keep_flags = svpresampling_detail::greedy_raytrace(svp,
                                                           z,
                                                           c,
                                                           *max_raytrace_error_in_meters,
                                                           0,
                                                           protected_index,
                                                           launch_depth_in_meters,
                                                           max_depth,
                                                           max_launch_angle_in_degrees,
                                                           number_of_test_angles);
    }
    else
    {
        const std::size_t target = std::max<std::size_t>(2, *number_of_entries);
        if (target >= n)
            return svp;
        keep_flags = svpresampling_detail::greedy_raytrace(svp,
                                                           z,
                                                           c,
                                                           -1.f,
                                                           target,
                                                           protected_index,
                                                           launch_depth_in_meters,
                                                           max_depth,
                                                           max_launch_angle_in_degrees,
                                                           number_of_test_angles);
    }

    return svpresampling_detail::subset(z, c, keep_flags, svp);
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
