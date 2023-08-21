// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/sampleindices.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[object]"

TEST_CASE("SampleIndices should support common functions", TESTTAG)
{
    // initialize object
    auto object = SampleIndices();

    object.beam_sample_map = { { 56, { 1, 3, 5 } }, { 81, { 2, 3, 5 } }, { 21, { 3, 4, 7 } } };

    // test inequality
    REQUIRE(SampleIndices() != object);

    // test copy
    REQUIRE(object == SampleIndices(object));

    // test binary
    {
        INFO(object.info_string());
        INFO(object.from_binary(object.to_binary()).info_string());
        REQUIRE(object == object.from_binary(object.to_binary()));
    }

    // test stream
    std::stringstream buffer;
    object.to_stream(buffer);
    REQUIRE(object == SampleIndices(object.from_stream(buffer)));

    // test print does not crash
    REQUIRE(object.info_string().size() != 0);
}
