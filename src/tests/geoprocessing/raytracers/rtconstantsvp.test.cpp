// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/xrandom.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/rtconstantsvp.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::navigation::datastructures;

#define TESTTAG "[location]"

TEST_CASE("RTConstantSVP should support common functions", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 10;
    location.pitch = 20;
    location.roll  = 30;

    float c   = 1450.f;
    float c_2 = c * 0.5;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    // test copy
    REQUIRE(raytracer == RTConstantSVP(raytracer));

    // test binary
    REQUIRE(raytracer == RTConstantSVP(raytracer.from_binary(raytracer.to_binary())));

    // test stream
    std::stringstream buffer;
    raytracer.to_stream(buffer);
    REQUIRE(raytracer == RTConstantSVP(raytracer.from_stream(buffer)));

    // test print does not crash
    REQUIRE(raytracer.info_string().size() != 0);

    // test get/set
    REQUIRE(raytracer.get_sensor_location() == location);

    auto ypr = raytracer.get_sensor_orientation_quat_ypr();

    // REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(location.yaw, 0.0001));
    REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(ypr[1], Catch::Matchers::WithinAbs(location.pitch, 0.0001));
    REQUIRE_THAT(ypr[2], Catch::Matchers::WithinAbs(location.roll, 0.0001));

    REQUIRE_THAT(raytracer.get_sound_velocity(), Catch::Matchers::WithinAbs(c, 0.0001));
}

TEST_CASE("RTConstantSVP reproduce some pre computed results (single points)", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c   = 1450.f;
    float c_2 = c * 0.5;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    // test raytracing some single points
    auto target = raytracer.trace_point(0, 0, 0);
    REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(3.f, 0.0001));
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(0.f, 0.0001));

    target = raytracer.trace_point(-1, 0, 0);
    REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(3.f - c_2, 0.0001));
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(-c_2, 0.0001));

    target = raytracer.trace_point(5, 45, 0);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(2563.26221f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(2566.26172f, 0.0001));
}

TEST_CASE("RTConstantSVP multi point computations should be equal to single point computations",
          TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c   = 1450.f;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    xt::random::seed(0);
    // test raytracing some single points
    xt::xtensor<float, 1> times = xt::random::rand<float>({1000});
    xt::xtensor<float, 1> along = xt::random::rand<float>({1000});
    xt::xtensor<float, 1> across = xt::random::rand<float>({1000});

    along = along * 180.f - 90.f;
    across = across * 360.f - 180.f;

    auto targets = raytracer.trace_points(times, along, across);

    for (size_t i = 0; i < times.size(); i++)
    {
        auto target = raytracer.trace_point(times[i], along[i], across[i]);

        REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(targets.x[i], 0.0001));
        REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(targets.y[i], 0.0001));
        REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(targets.z[i], 0.0001));
        REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(targets.true_range[i], 0.0001));
    }

}