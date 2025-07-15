// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampledirections.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("SampleDirections should support common functions", TESTTAG)
{
    // initialize location
    auto location = SampleDirections<2>();

    location.alongtrack_angle    = { { 56.000 }, { 1.000 } };
    location.crosstrack_angle   = { { 54.192 }, { 2.000 } };

    // test inequality
    REQUIRE(SampleDirections<2>() != location);

    // test copy
    REQUIRE(location == SampleDirections(location));

    // test binary
    REQUIRE(location == SampleDirections(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == SampleDirections(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(location.binary_hash() == 7946198183670543780);
    REQUIRE(location.binary_hash() == SampleDirections(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            SampleDirections(location.from_binary(location.to_binary())).binary_hash());
}
