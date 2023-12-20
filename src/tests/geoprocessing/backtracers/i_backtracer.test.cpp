// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/backtracers/i_backtracer.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::backtracers;
using namespace themachinethatgoesping::navigation::datastructures;

#define TESTTAG "[location]"

TEST_CASE("I_Backtracer should support common functions", TESTTAG)
{
    // initialize location
    auto location = Geolocation();

    location.z     = 3;
    location.yaw   = 10;
    location.pitch = 20;
    location.roll  = 30;

    float x = 2.1;
    float y = 55.4;

    // initialize backtracer
    auto backtracer = I_Backtracer(location, x, y, "I_Backtracer");

    // test copy
    REQUIRE(backtracer == I_Backtracer(backtracer));

    // test binary
    INFO(backtracer.info_string());
    INFO(I_Backtracer(backtracer.from_binary(backtracer.to_binary())).info_string());
    REQUIRE(backtracer == I_Backtracer(backtracer.from_binary(backtracer.to_binary())));

    // test stream
    std::stringstream buffer;
    backtracer.to_stream(buffer);
    REQUIRE(backtracer == I_Backtracer(backtracer.from_stream(buffer)));

    // test print does not crash
    REQUIRE(backtracer.info_string().size() != 0);

    // test get/set
    REQUIRE(backtracer.get_sensor_location() == location);

    auto ypr = backtracer.get_sensor_orientation_quat_ypr();

    // REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(location.yaw, 0.0001));
    REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(ypr[1], Catch::Matchers::WithinAbs(location.pitch, 0.0001));
    REQUIRE_THAT(ypr[2], Catch::Matchers::WithinAbs(location.roll, 0.0001));

    // test hash
    REQUIRE(backtracer.binary_hash() == 11759859546874707158ull);
    REQUIRE(backtracer.binary_hash() == I_Backtracer(backtracer).binary_hash());
    REQUIRE(backtracer.binary_hash() ==
            I_Backtracer(backtracer.from_binary(backtracer.to_binary())).binary_hash());    
}
