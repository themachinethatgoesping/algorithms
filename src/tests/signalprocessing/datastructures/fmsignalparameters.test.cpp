// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/signalprocessing/datastructures.hpp"
#include "../../../themachinethatgoesping/algorithms/signalprocessing/types.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::signalprocessing::datastructures;
using namespace themachinethatgoesping::algorithms::signalprocessing::types;

using Catch::Approx;

#define TESTTAG "[location]"

TEST_CASE("FMSignalParameters should support common functions", TESTTAG)
{
    // initialize location
    auto txs = FMSignalParameters(123567.891f,              // center_frequency
                                  789.012f,                 // bandwidth
                                  0.00234f,                 // effective_pulse_duration
                                  t_TxSignalType::FM_UP_SWEEP // up_sweep
    );

    // test inequality
    REQUIRE(FMSignalParameters() != txs);

    // test copy
    REQUIRE(txs == FMSignalParameters(txs));

    // test binary
    REQUIRE(txs == FMSignalParameters(txs.from_binary(txs.to_binary())));

    // test stream
    std::stringstream buffer;
    txs.to_stream(buffer);
    REQUIRE(txs == FMSignalParameters(txs.from_stream(buffer)));

    // test print does not crash
    REQUIRE(txs.info_string().size() != 0);

    // test individual variables
    REQUIRE(txs.center_frequency == Approx(123567.891f));
    REQUIRE(txs.bandwidth == Approx(789.012f));
    REQUIRE(txs.effective_pulse_duration == Approx(0.00234));
    REQUIRE(txs.get_tx_signal_type() == t_TxSignalType::FM_UP_SWEEP);

    // test hash
    REQUIRE(txs.binary_hash() == 4636758341217930380);
}
