// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>
#include <sstream>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamtrace.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/tracebeam.hpp"

using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define TESTTAG "[beamtrace][raytracers2]"

TEST_CASE("BeamTrace stores tables and derives range/angle", TESTTAG)
{
    // Simple straight segment (two points) with a 45 deg starboard-down ray.
    // point 0 = launch (down at origin), point 1 = 1000 m deep, offset 1000 m.
    xt::xtensor<float, 1> depths             = { 0.f, 1000.f };
    xt::xtensor<float, 1> horizontal_offsets = { 0.f, 1000.f }; // +y = starboard
    xt::xtensor<float, 1> two_way            = { 0.f, 2.f };
    xt::xtensor<float, 1> cos_angles         = { std::cos(float(M_PI_4)), std::cos(float(M_PI_4)) };

    BeamTrace trace(depths, horizontal_offsets, two_way, cos_angles);

    REQUIRE(trace.get_number_of_points() == 2);

    // range at launch is 0, at the far point it is the straight chord length
    auto ranges = trace.get_ranges_in_meters();
    REQUIRE_THAT(ranges.unchecked(0), Catch::Matchers::WithinAbs(0.f, 1e-4f));
    REQUIRE_THAT(ranges.unchecked(1),
                 Catch::Matchers::WithinAbs(std::sqrt(1000.f * 1000.f + 1000.f * 1000.f), 1e-2f));

    // starboard (+y) with the given convention is a negative incident angle
    auto angles = trace.get_incident_angles_in_degrees();
    REQUIRE_THAT(angles.unchecked(0), Catch::Matchers::WithinAbs(-45.f, 1e-3f));
    REQUIRE_THAT(angles.unchecked(1), Catch::Matchers::WithinAbs(-45.f, 1e-3f));
}

TEST_CASE("trace_beam constant SVP straight down", TESTTAG)
{
    auto svp = SoundVelocityProfile::uniform(1500.f, 6000.f);

    // one-way time 1 s -> two-way 2 s; straight-down beam from 50 m
    auto trace = trace_beam(50.f, 0.f, svp, 2.f);

    REQUIRE(trace.get_number_of_points() >= 2);
    const size_t last = trace.get_number_of_points() - 1;

    // depth advance = c * t = 1500 m; no horizontal offset
    REQUIRE_THAT(trace.get_depths_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(50.f + 1500.f, 1e-1f));
    REQUIRE_THAT(trace.get_horizontal_offsets_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(0.f, 1e-2f));
    REQUIRE_THAT(trace.get_two_way_travel_times_in_seconds().unchecked(last),
                 Catch::Matchers::WithinAbs(2.f, 1e-4f));
    REQUIRE_THAT(trace.get_ranges_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(1500.f, 1e-1f));
}

TEST_CASE("trace_beam constant SVP 30 deg port", TESTTAG)
{
    auto svp = SoundVelocityProfile::uniform(1500.f, 6000.f);

    // +30 deg -> port (-y); one-way 1 s
    auto         trace = trace_beam(50.f, 30.f, svp, 2.f);
    const size_t last  = trace.get_number_of_points() - 1;

    const float expected_depth  = 50.f + 1500.f * std::cos(float(M_PI) / 6.f);
    const float expected_offset = -1500.f * std::sin(float(M_PI) / 6.f); // port is -y

    REQUIRE_THAT(trace.get_depths_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(expected_depth, 1e-1f));
    REQUIRE_THAT(trace.get_horizontal_offsets_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(expected_offset, 1e-1f));
    REQUIRE_THAT(trace.get_incident_angles_in_degrees().unchecked(last),
                 Catch::Matchers::WithinAbs(30.f, 1e-2f));
    // straight ray: range equals c * t
    REQUIRE_THAT(trace.get_ranges_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(1500.f, 1e-1f));
}

TEST_CASE("trace_beam gradient layer straight down matches analytic", TESTTAG)
{
    // single linear-gradient layer 0..2000 m, c: 1500 -> 1700 (g = 0.1)
    xt::xtensor<float, 1> z = { 0.f, 2000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1700.f };
    SoundVelocityProfile  svp(z, c);

    // straight-down beam, one-way time 0.5 s (two-way 1.0 s)
    auto         trace = trace_beam(0.f, 0.f, svp, 1.0f);
    const size_t last  = trace.get_number_of_points() - 1;

    // z(t) = (c0/g) * (exp(g t) - 1)
    const float c0 = 1500.f, g = 0.1f, t = 0.5f;
    const float expected_z = (c0 / g) * (std::exp(g * t) - 1.f);
    REQUIRE_THAT(trace.get_depths_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(expected_z, 0.5f));
    REQUIRE_THAT(trace.get_horizontal_offsets_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(0.f, 1e-2f));
}

TEST_CASE("trace_beam turns in a positive gradient", TESTTAG)
{
    // c reaches beyond the critical speed so a 60 deg beam turns inside the layer
    xt::xtensor<float, 1> z = { 0.f, 2000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1800.f }; // g = 0.15
    SoundVelocityProfile  svp(z, c);

    auto trace = trace_beam(0.f, 60.f, svp, 8.f);

    // launch + turning apex + final
    REQUIRE(trace.get_number_of_points() == 3);

    const auto& depths = trace.get_depths_in_meters();
    const auto& cosang = trace.get_cos_incident_angles();

    // turning apex: cos(angle) == 0 and it is the deepest point
    REQUIRE_THAT(cosang.unchecked(1), Catch::Matchers::WithinAbs(0.f, 1e-3f));
    const float c_turn      = 1500.f / std::sin(float(M_PI) / 3.f);
    const float turn_depth  = (c_turn - 1500.f) / 0.15f;
    REQUIRE_THAT(depths.unchecked(1), Catch::Matchers::WithinAbs(turn_depth, 2.f));

    // descends then ascends
    REQUIRE(depths.unchecked(1) > depths.unchecked(0));
    REQUIRE(depths.unchecked(2) < depths.unchecked(1));

    // incident angle at the apex is horizontal (+-90 deg)
    REQUIRE_THAT(std::abs(trace.get_incident_angles_in_degrees().unchecked(1)),
                 Catch::Matchers::WithinAbs(90.f, 1e-2f));
}

TEST_CASE("trace_beam upward beam", TESTTAG)
{
    auto svp = SoundVelocityProfile::uniform(1500.f, 6000.f);

    // 150 deg -> upward and towards port; one-way 0.5 s
    auto         trace = trace_beam(1000.f, 150.f, svp, 1.0f);
    const size_t last  = trace.get_number_of_points() - 1;

    const float expected_depth  = 1000.f + 1500.f * 0.5f * std::cos(150.f * float(M_PI) / 180.f);
    const float expected_offset = -1500.f * 0.5f * std::sin(150.f * float(M_PI) / 180.f);

    REQUIRE_THAT(trace.get_depths_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(expected_depth, 1e-1f)); // moved up
    REQUIRE(trace.get_depths_in_meters().unchecked(last) < 1000.f);
    REQUIRE_THAT(trace.get_horizontal_offsets_in_meters().unchecked(last),
                 Catch::Matchers::WithinAbs(expected_offset, 1e-1f));
    REQUIRE_THAT(trace.get_incident_angles_in_degrees().unchecked(last),
                 Catch::Matchers::WithinAbs(150.f, 1e-2f));
}

TEST_CASE("trace_beam crosses layers", TESTTAG)
{
    // three layers; a steep-ish beam crosses at least one boundary
    xt::xtensor<float, 1> z = { 0.f, 100.f, 500.f, 2000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1490.f, 1495.f, 1530.f };
    SoundVelocityProfile  svp(z, c);

    auto trace = trace_beam(0.f, 20.f, svp, 1.0f);

    // launch + at least one crossing + final
    REQUIRE(trace.get_number_of_points() >= 3);

    // depth and two-way time increase monotonically along a downward beam
    const auto& depths = trace.get_depths_in_meters();
    const auto& twtt   = trace.get_two_way_travel_times_in_seconds();
    for (size_t i = 1; i < trace.get_number_of_points(); ++i)
    {
        REQUIRE(depths.unchecked(i) >= depths.unchecked(i - 1));
        REQUIRE(twtt.unchecked(i) >= twtt.unchecked(i - 1));
    }
    // range is monotonically increasing as well
    auto ranges = trace.get_ranges_in_meters();
    for (size_t i = 1; i < ranges.size(); ++i)
        REQUIRE(ranges.unchecked(i) >= ranges.unchecked(i - 1));
}

TEST_CASE("BeamTrace binary/stream roundtrip and print", TESTTAG)
{
    auto svp   = SoundVelocityProfile::uniform(1500.f, 6000.f);
    auto trace = trace_beam(50.f, 25.f, svp, 3.f);

    // copy
    REQUIRE(trace == BeamTrace(trace));

    // stream
    std::stringstream buffer;
    trace.to_stream(buffer);
    auto restored = BeamTrace::from_stream(buffer);
    REQUIRE(trace == restored);

    // info string is non-empty
    REQUIRE(trace.info_string().size() != 0);
}

TEST_CASE("BeamTrace rejects mismatched table sizes", TESTTAG)
{
    BeamTrace trace;
    REQUIRE_THROWS(trace.set(xt::xtensor<float, 1>{ 0.f, 1.f },
                             xt::xtensor<float, 1>{ 0.f },
                             xt::xtensor<float, 1>{ 0.f, 1.f },
                             xt::xtensor<float, 1>{ 1.f, 1.f }));
}
