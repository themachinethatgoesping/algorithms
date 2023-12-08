// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirectionstime.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("SampleDirectionsTime should support common functions", TESTTAG)
{
    // initialize location
    auto location = SampleDirectionsTime<2>();

    location.alongtrack_angle    = { { 56.000 }, { 1.000 } };
    location.crosstrack_angle   = { { 54.192 }, { 2.000 } };
    location.two_way_travel_time = { { 4.2 }, { 4.000 } };

    // test inequality
    REQUIRE(SampleDirectionsTime<2>() != location);

    // test copy
    REQUIRE(location == SampleDirectionsTime(location));

    // test binary
    REQUIRE(location == SampleDirectionsTime(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == SampleDirectionsTime(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash
    REQUIRE(location.binary_hash() == 15106317722440586394);
    REQUIRE(location.binary_hash() == SampleDirectionsTime(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            SampleDirectionsTime(location.from_binary(location.to_binary())).binary_hash());
}
