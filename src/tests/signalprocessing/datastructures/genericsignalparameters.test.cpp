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

TEST_CASE("GenericSignalParameters should support common functions", TESTTAG)
{
    // initialize location
    auto txs = GenericSignalParameters(123567.891f,            // center_frequency
                                       0.00223f,               // bandwidth
                                       0.0023f,                // effective_pulse_duration
                                       t_TxSignalType::UNKNOWN // up_sweep
    );

    // test inequality
    REQUIRE(GenericSignalParameters() != txs);

    // test copy
    REQUIRE(txs == GenericSignalParameters(txs));

    // test binary
    REQUIRE(txs == GenericSignalParameters(txs.from_binary(txs.to_binary())));

    // test stream
    std::stringstream buffer;
    txs.to_stream(buffer);
    REQUIRE(txs == GenericSignalParameters(txs.from_stream(buffer)));

    // test print does not crash
    REQUIRE(txs.info_string().size() != 0);

    // test individual variables
    REQUIRE(txs.center_frequency == Approx(123567.891f));
    REQUIRE(txs.bandwidth == Approx(0.00223f));
    REQUIRE(txs.effective_pulse_duration == Approx(0.0023f));
    REQUIRE(txs.get_tx_signal_type() == t_TxSignalType::UNKNOWN);

    // test hash
    REQUIRE(txs.binary_hash() == 10730480407014384156ULL);
    REQUIRE(txs.binary_hash() == GenericSignalParameters(txs).binary_hash());
    REQUIRE(txs.binary_hash() ==
            GenericSignalParameters(txs.from_binary(txs.to_binary())).binary_hash());
}
