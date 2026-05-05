// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>
#include <vector>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/layerraytracer.hpp"

using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;
using themachinethatgoesping::navigation::datastructures::Geolocation;

#define TESTTAG "[layerraytracer][raytracers2]"

TEST_CASE("LayerRaytracer constant SVP analytic check", TESTTAG)
{
    // 1500 m/s uniform from 0..6000m
    auto rt = LayerRaytracer(SoundVelocityProfile::uniform(1500.f, 6000.f));

    // Beam straight down + 45-deg starboard
    xt::xtensor<float, 2> launch_dirs = {
        { 0.f, 0.f, 1.f },                          // straight down
        { 0.f, std::sin(0.5f * (float)M_PI_2), std::cos(0.5f * (float)M_PI_2) }, // 45 deg
    };
    // knots at 1, 2, 3 seconds (one-way)
    xt::xtensor<float, 1> knot_times = { 1.f, 2.f, 3.f };

    Geolocation pose;
    pose.z = 0.f; pose.yaw = 0.f; pose.pitch = 0.f; pose.roll = 0.f;
    std::vector<Geolocation> poses(knot_times.size(), pose);

    auto out = rt.trace_at_times(launch_dirs, knot_times, poses, 1);

    REQUIRE(out.shape(0) == 3);
    REQUIRE(out.shape(1) == 2);
    REQUIRE(out.shape(2) == 3);

    // straight-down beam: x=0, y=0, z = 1500 * t
    for (size_t k = 0; k < 3; ++k)
    {
        const float t = knot_times.unchecked(k);
        REQUIRE_THAT(out(k, 0, 0), Catch::Matchers::WithinAbs(0.f, 1e-2f));
        REQUIRE_THAT(out(k, 0, 1), Catch::Matchers::WithinAbs(0.f, 1e-2f));
        REQUIRE_THAT(out(k, 0, 2), Catch::Matchers::WithinAbs(1500.f * t, 1.f));
    }

    // 45-deg starboard beam: x=0, y = 1500*t*sin45, z = 1500*t*cos45
    for (size_t k = 0; k < 3; ++k)
    {
        const float t      = knot_times.unchecked(k);
        const float expect = 1500.f * t * std::sin((float)M_PI_4);
        REQUIRE_THAT(out(k, 1, 0), Catch::Matchers::WithinAbs(0.f, 1e-2f));
        REQUIRE_THAT(out(k, 1, 1), Catch::Matchers::WithinAbs(expect, 1.f));
        REQUIRE_THAT(out(k, 1, 2), Catch::Matchers::WithinAbs(expect, 1.f));
    }
}

TEST_CASE("LayerRaytracer linear-gradient single layer", TESTTAG)
{
    // Single linear-gradient layer 0..1000m, c: 1500 -> 1480
    xt::xtensor<float, 1> z = { 0.f, 1000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1480.f };
    auto rt = LayerRaytracer(SoundVelocityProfile(z, c));

    // Straight-down beam: depth at one-way time t solved analytically.
    //   c(z) = c0 + g*z, g = -0.02
    //   For straight-down (sin theta = 0, p = 0): dz/dt = c -> dt = dz/c
    //   t = (1/g)*ln(c1/c0) -> z(t) = (c0/g)*(exp(g*t) - 1)
    xt::xtensor<float, 2> launch_dirs = { { 0.f, 0.f, 1.f } };
    xt::xtensor<float, 1> knot_times  = { 0.3f };
    Geolocation pose; pose.z = 0.f;
    std::vector<Geolocation> poses(1, pose);

    auto out = rt.trace_at_times(launch_dirs, knot_times, poses, 1);

    const float c0 = 1500.f, g = -0.02f;
    const float t  = 0.3f;
    const float expected_z = (c0 / g) * (std::exp(g * t) - 1.f);
    REQUIRE_THAT(out(0, 0, 2), Catch::Matchers::WithinAbs(expected_z, 0.5f));
}

TEST_CASE("LayerRaytracer multi-layer knot anchoring", TESTTAG)
{
    // Multi-layer SVP. Verify that tracing to a knot, then re-tracing to the
    // same knot, yields identical xyz (knot anchoring is exact).
    xt::xtensor<float, 1> z = { 0.f, 100.f, 500.f, 2000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1490.f, 1495.f, 1530.f };
    auto rt = LayerRaytracer(SoundVelocityProfile(z, c));

    xt::xtensor<float, 2> launch_dirs = {
        { 0.f, 0.f, 1.f },
        { 0.f, 0.5f, 0.86602540378f },
    };
    xt::xtensor<float, 1> knot_times = { 0.5f, 1.0f, 1.5f };
    Geolocation pose; pose.z = 0.f;
    std::vector<Geolocation> poses(3, pose);

    auto out1 = rt.trace_at_times(launch_dirs, knot_times, poses, 1);
    auto out2 = rt.trace_at_times(launch_dirs, knot_times, poses, 1);

    for (size_t k = 0; k < knot_times.size(); ++k)
        for (size_t b = 0; b < 2; ++b)
            for (size_t a = 0; a < 3; ++a)
                REQUIRE_THAT(out1(k, b, a),
                             Catch::Matchers::WithinAbs(out2(k, b, a), 1e-3f));

    // For straight-down beam in multi-layer, depth must be monotonically
    // increasing with knot time
    REQUIRE(out1(0, 0, 2) < out1(1, 0, 2));
    REQUIRE(out1(1, 0, 2) < out1(2, 0, 2));
}
