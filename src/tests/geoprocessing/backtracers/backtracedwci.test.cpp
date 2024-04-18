// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <xtensor/xrandom.hpp>

#include <themachinethatgoesping/algorithms/geoprocessing/backtracers/backtracedwci.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing;

#define TESTTAG "[location]"

TEST_CASE("BacktracedWCI should support common functions", TESTTAG)
{
    using backtracers::BacktracedWCI;

    xt::xtensor<float, 2>                    wci = xt::random::rand<float>({ 2, 100 });
    datastructures::SampleDirectionsRange<1> beam_reference_directions;
    beam_reference_directions.alongtrack_angle          = { { 56.000 }, { 1.000 } };
    beam_reference_directions.crosstrack_angle          = { { 54.192 }, { 2.000 } };
    beam_reference_directions.range                     = { { 4.2 }, { 4.000 } };
    std::vector<uint16_t> beam_reference_sample_numbers = { 29, 30 };

    // initialize location
    auto BWCI = BacktracedWCI(wci, beam_reference_directions, beam_reference_sample_numbers);

    // test inequalitySampleDirections
    // REQUIRE(BacktracedWCI<2>() != location);

    // test copy
    REQUIRE(BWCI == BacktracedWCI(BWCI));

    // test binary
    REQUIRE(BWCI == BacktracedWCI(BWCI.from_binary(BWCI.to_binary())));

    // test stream
    std::stringstream buffer;
    BWCI.to_stream(buffer);
    REQUIRE(BWCI == BacktracedWCI(BWCI.from_stream(buffer)));

    // test print does not crash
    REQUIRE(BWCI.info_string().size() != 0);

    // test hash
    REQUIRE(BWCI.binary_hash() == 17203797866898865973ULL);
    REQUIRE(BWCI.binary_hash() == BacktracedWCI(BWCI).binary_hash());
    REQUIRE(BWCI.binary_hash() == BacktracedWCI(BWCI.from_binary(BWCI.to_binary())).binary_hash());
}
