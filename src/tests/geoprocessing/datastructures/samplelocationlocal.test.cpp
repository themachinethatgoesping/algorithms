// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/samplelocationlocal.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("SampleLocationLocal should support common functions", TESTTAG)
{
    // initialize location
    auto location = SampleLocationLocal();

    location.x          = 56.000;
    location.y          = 54.192;
    location.z          = 3.1;
    location.true_range = 4.2;

    // test copy
    REQUIRE(location == SampleLocationLocal(location));

    // test binary
    REQUIRE(location == SampleLocationLocal(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == SampleLocationLocal(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);
}
