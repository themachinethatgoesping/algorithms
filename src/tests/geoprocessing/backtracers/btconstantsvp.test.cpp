// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/xrandom.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/btconstantsvp.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::navigation::datastructures;

#define TESTTAG "[location]"

TEST_CASE("BTConstantSVP should support common functions", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 10;
    location.pitch = 20;
    location.roll  = 30;

    float c = 1450.f;

    // initialize backtracer
    auto backtracer = BTConstantSVP(location, c);

    // test copy
    {
        INFO(backtracer.info_string());
        INFO(BTConstantSVP(backtracer).info_string());
        REQUIRE(backtracer == BTConstantSVP(backtracer));
    }

    // test binary
    {
        auto backtracer2 = BTConstantSVP::from_binary(backtracer.to_binary());
        INFO(backtracer.info_string());
        INFO(backtracer2.info_string());
        REQUIRE(backtracer == backtracer2);
    }

    // test stream
    {
        std::stringstream buffer;
        backtracer.to_stream(buffer);
        auto backtracer2 = BTConstantSVP::from_stream(buffer);
        INFO(backtracer.info_string());
        INFO(backtracer2.info_string());
        REQUIRE(backtracer == backtracer2);
    }

    // test print does not crash
    REQUIRE(backtracer.info_string().size() != 0);

    // test get/set
    REQUIRE(backtracer.get_sensor_location() == location);

    auto ypr = backtracer.get_sensor_orientation_quat_ypr();

    // REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(location.yaw, 0.0001));
    REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(ypr[1], Catch::Matchers::WithinAbs(location.pitch, 0.0001));
    REQUIRE_THAT(ypr[2], Catch::Matchers::WithinAbs(location.roll, 0.0001));

    REQUIRE_THAT(backtracer.get_sound_velocity(), Catch::Matchers::WithinAbs(c, 0.0001));
}

TEST_CASE("BTConstantSVP reproduce some pre computed results (single points)", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c   = 1450.f;
    //float c_2 = c * 0.5;

    // initialize backtracer
    auto backtracer = BTConstantSVP(location, c);

    // test raytracing some single points
    {
        auto targets = backtracer.backtrace_points({ 0 }, { 0 }, { 0 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00414f, 0.0001));
    }

    {
        auto targets = backtracer.backtrace_points({ 1 }, { 0 }, { 0 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(18.43495f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00436f, 0.0001));
    }

    {
        auto targets = backtracer.backtrace_points({ 1 }, { 2 }, { 0 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(15.50136f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(-32.31153f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00516f, 0.0001));
    }

    {
        auto targets = backtracer.backtrace_points({ 1 }, { 2 }, { 3 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(26.56505f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(-63.43494f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00308f, 0.0001));
    }

    {
        auto targets = backtracer.backtrace_points({ -1 }, { 2 }, { 3 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(-26.56505f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(-63.43494f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00308f, 0.0001));
    }

    {
        auto targets = backtracer.backtrace_points({ -1 }, { 0 }, { -3 });
        CHECK_THAT(targets.alongtrack_angle[0], Catch::Matchers::WithinAbs(-9.46232f, 0.0001));
        CHECK_THAT(targets.crosstrack_angle[0], Catch::Matchers::WithinAbs(-0.0f, 0.0001));
        CHECK_THAT(targets.two_way_travel_time[0], Catch::Matchers::WithinAbs(0.00839f, 0.0001));
    }
}