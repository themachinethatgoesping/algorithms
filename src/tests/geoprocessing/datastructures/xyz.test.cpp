// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/xyz.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("XYZ should support common functions", TESTTAG)
{
    // initialize location
    auto location = XYZ<2>();

    location.x          = { { 56.000 }, { 1.000 } };
    location.y          = { { 54.192 }, { 2.000 } };
    location.z          = { { 3.1 }, { 3.000 } };

    // test inequality
    REQUIRE(XYZ<2>() != location);

    // test copy
    REQUIRE(location == XYZ(location));

    // test binary
    REQUIRE(location == XYZ(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == XYZ(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(location.binary_hash() == 1848392524124629315);
    REQUIRE(location.binary_hash() == XYZ(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            XYZ(location.from_binary(location.to_binary())).binary_hash());
}
