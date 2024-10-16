// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/raytraceresults.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("RaytraceResults should support common functions", TESTTAG)
{
    // initialize location
    auto location = RaytraceResults<2>();

    location.x          = { { 56.000 }, { 1.000 } };
    location.y          = { { 54.192 }, { 2.000 } };
    location.z          = { { 3.1 }, { 3.000 } };
    location.true_range = { { 4.2 }, { 4.000 } };

    // test inequality
    REQUIRE(RaytraceResults<2>() != location);

    // test copy
    REQUIRE(location == RaytraceResults(location));

    // test binary
    REQUIRE(location == RaytraceResults(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == RaytraceResults(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(location.binary_hash() == 4496877549998357265);
    REQUIRE(location.binary_hash() == RaytraceResults(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            RaytraceResults(location.from_binary(location.to_binary())).binary_hash());
}
