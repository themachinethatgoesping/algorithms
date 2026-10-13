// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/pingtools/readsamplerange.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::pingtools;
#define TESTTAG "[pingtools]"

TEST_CASE("ReadSampleRange should support common functions", TESTTAG)
{
    // initialize class structure
    auto obj = ReadSampleRange(2, 3, 4, 5);

    // test variables
    REQUIRE(obj.get_first_sample_to_read() == 2);
    REQUIRE(obj.get_number_of_samples_to_read() == 3);
    REQUIRE(obj.get_first_read_sample_offset() == 4);
    REQUIRE(obj.get_last_read_sample_offset() == 5);

    // test inequality
    REQUIRE(obj != ReadSampleRange(5, 4, 3, 2));

    // test copy
    REQUIRE(obj == ReadSampleRange(obj));

    // test binary
    REQUIRE(obj == ReadSampleRange(obj.from_binary(obj.to_binary())));

    // test stream
    std::stringstream buffer;
    obj.to_stream(buffer);
    REQUIRE(obj == ReadSampleRange(obj.from_stream(buffer)));

    // test print does not crash
    REQUIRE(obj.info_string().size() != 0);

    //--- Class concept ---
}
