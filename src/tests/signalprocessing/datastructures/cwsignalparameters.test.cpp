// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/signalprocessing/datastructures.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::signalprocessing::datastructures;
using namespace themachinethatgoesping::algorithms::signalprocessing::types;

using Catch::Approx;

#define TESTTAG "[location]"

TEST_CASE("CWSignalParameters should support common functions", TESTTAG)
{
    // initialize location
    auto txs = CWSignalParameters(123567.891f, // center_frequency
                                  789.012f,    // bandwidth
                                  0.00234f     // effective_pulse_duration
    );

    // test inequality
    // REQUIRE(CWSignalParameters() != txs);

    // test copy
    REQUIRE(txs == CWSignalParameters(txs));

    // test binary
    REQUIRE(txs == CWSignalParameters(txs.from_binary(txs.to_binary())));

    // test stream
    std::stringstream buffer;
    txs.to_stream(buffer);
    REQUIRE(txs == CWSignalParameters(txs.from_stream(buffer)));

    // test print does not crash
    REQUIRE(txs.info_string().size() != 0);

    // test individual variables
    REQUIRE(txs.get_center_frequency() == Approx(123567.891f));
    REQUIRE(txs.get_bandwidth() == Approx(789.012f));
    REQUIRE(txs.get_effective_pulse_duration() == Approx(0.00234f));
    REQUIRE(txs.get_tx_signal_type() == t_TxSignalType::CW);

    // test hash (should be stable if class is not changed)
    REQUIRE(txs.binary_hash() == 6748335302485371792);
    REQUIRE(txs.binary_hash() == CWSignalParameters(txs).binary_hash());
    REQUIRE(txs.binary_hash() ==
            CWSignalParameters(txs.from_binary(txs.to_binary())).binary_hash());
}
