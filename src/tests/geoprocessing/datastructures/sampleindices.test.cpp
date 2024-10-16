// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampleindices.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[location]"

TEST_CASE("SampleIndices should support common functions", TESTTAG)
{
    // initialize location
    auto location = SampleIndices<2>();

    location.beam_numbers   = { { 56 }, { 1 } };
    location.sample_numbers = { { 54 }, { 2 } };

    // test inequalitySampleDirections
    REQUIRE(SampleIndices<2>() != location);

    // test copy
    REQUIRE(location == SampleIndices(location));

    // test binary
    REQUIRE(location == SampleIndices(location.from_binary(location.to_binary())));

    // test stream
    std::stringstream buffer;
    location.to_stream(buffer);
    REQUIRE(location == SampleIndices(location.from_stream(buffer)));

    // test print does not crash
    REQUIRE(location.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(location.binary_hash() == 545582192469170180);
    REQUIRE(location.binary_hash() == SampleIndices(location).binary_hash());
    REQUIRE(location.binary_hash() ==
            SampleIndices(location.from_binary(location.to_binary())).binary_hash());
}
