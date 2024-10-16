// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsampleparameters.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[param]"

TEST_CASE("BeamSampleParameters should support common functions", TESTTAG)
{
    // initialize param
    auto param = BeamSampleParameters(2);

    param.set_alongtrack_angles({ { 54.192 }, { 2.000 } });
    param.set_crosstrack_angles({ { 3.000 }, { 2.000 } });
    param.set_first_sample_offset({ { 5.000 }, { -2.000 } });
    param.set_sample_interval({ { -20.000 }, { 4.000 } });
    param.set_number_of_samples({ { 54 }, { 12 } });
    
    // test inequality
    REQUIRE(BeamSampleParameters(2) != param);

    // test copy
    REQUIRE(param == BeamSampleParameters(param));

    // test binary
    REQUIRE(param == BeamSampleParameters(param.from_binary(param.to_binary())));

    // test stream
    std::stringstream buffer;
    param.to_stream(buffer);
    REQUIRE(param == BeamSampleParameters(param.from_stream(buffer)));

    // test print does not crash
    REQUIRE(param.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    REQUIRE(param.binary_hash() == 4652324683533098954);
    REQUIRE(param.binary_hash() == BeamSampleParameters(param).binary_hash());
    REQUIRE(param.binary_hash() ==
            BeamSampleParameters(param.from_binary(param.to_binary())).binary_hash());
}
