// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/sparsemaps/sparsemap.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::sparsemaps;

#define TESTTAG "[location]"

TEST_CASE("SparseMapTypes should support common functions", TESTTAG)
{

}

TEST_CASE("SparseMap should support common functions", TESTTAG)
{
    // // initialize location
    // auto location = Geolocation();

    // location.z     = 3;
    // location.yaw   = 10;
    // location.pitch = 20;
    // location.roll  = 30;

    // // initialize raytracer
    // auto raytracer = I_Raytracer(location, "I_Raytracer");

    // // test copy
    // REQUIRE(raytracer == I_Raytracer(raytracer));

    // // test binary
    // INFO(raytracer.info_string());
    // INFO(I_Raytracer(raytracer.from_binary(raytracer.to_binary())).info_string());
    // REQUIRE(raytracer == I_Raytracer(raytracer.from_binary(raytracer.to_binary())));

    // // test stream
    // std::stringstream buffer;
    // raytracer.to_stream(buffer);
    // REQUIRE(raytracer == I_Raytracer(raytracer.from_stream(buffer)));

    // // test print does not crash
    // REQUIRE(raytracer.info_string().size() != 0);

    // // test get/set
    // REQUIRE(raytracer.get_sensor_location() == location);

    // auto ypr = raytracer.get_sensor_orientation_quat_ypr();

    // //REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(location.yaw, 0.0001));
    // REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
    // REQUIRE_THAT(ypr[1], Catch::Matchers::WithinAbs(location.pitch, 0.0001));
    // REQUIRE_THAT(ypr[2], Catch::Matchers::WithinAbs(location.roll, 0.0001));
}
