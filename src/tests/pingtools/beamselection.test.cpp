// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/pingtools/beamselection.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::pingtools;
#define TESTTAG "[pingtools]"

TEST_CASE("BeamSelection should support common functions", TESTTAG)
{
    // initialize class structure
    auto obj = BeamSelection();
    CHECK(obj.empty());

    // add some beams
    obj.add_beam(0);
    obj.add_beam(1);
    obj.add_beam(2);
    obj.add_beam(4);

    CHECK(obj.empty() == false);

    // test variables
    CHECK(obj.get_number_of_beams() == 4);

    CHECK(obj.get_beam_numbers() == std::vector<uint32_t>{ 0, 1, 2, 4 });

    // test alternative intialization
    auto obj2 = BeamSelection(3);

    // test variables alternative
    CHECK(obj2.get_number_of_beams() == 3);
    CHECK(obj2.get_beam_numbers() == std::vector<uint32_t>{ 0, 1, 2 });

    CHECK(obj != obj2);
    obj2.add_beam(4);
    CHECK(obj == obj2);

    // test inequality
    CHECK(obj != BeamSelection());

    // test copy
    CHECK(obj == BeamSelection(obj));

    // test binary
    CHECK(obj == BeamSelection(obj.from_binary(obj.to_binary())));

    // test stream
    std::stringstream buffer;
    obj.to_stream(buffer);
    CHECK(obj == BeamSelection(obj.from_stream(buffer)));

    // test print does not crash
    CHECK(obj.info_string().size() != 0);

    //--- Class concept ---
}
