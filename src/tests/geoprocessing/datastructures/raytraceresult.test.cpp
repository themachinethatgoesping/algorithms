// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresult.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("RaytraceResult should support common functions", TESTTAG)
{
    // initialize location
    auto location = RaytraceResult();

    location.x          = 56.000;
    location.y          = 54.192;
    location.z          = 3.1;
    location.true_range = 4.2;

    // test copy
    REQUIRE(location == RaytraceResult(location));

    // test binary
    REQUIRE(location == RaytraceResult(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == RaytraceResult(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(location.binary_hash() == 5760038646027523138);
    REQUIRE(location.binary_hash() == RaytraceResult(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            RaytraceResult(location.from_binary(location.to_binary())).binary_hash());
}
