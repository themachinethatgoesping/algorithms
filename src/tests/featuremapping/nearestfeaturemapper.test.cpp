// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/featuremapping/nearestfeaturemapper.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::featuremapping;

#define TESTTAG "[nearestfeaturemapper]"

TEST_CASE("NearestFeatureMapper should support common functions", TESTTAG)
{
    // initialize converter
    std::vector<double> ping_times     = { 0.0, 1.0, 2.0, 3.0, 4.0 };
    std::vector<double> ping_numbers   = { 100.0, 200.0, 300.0, 400.0, 500.0 };
    std::vector<double> ping_distances = { 0.0, 10.5, 21.0, 31.5, 42.0 };

    auto converter = NearestFeatureMapper({
        {"ping_times", ping_times},
        {"ping_numbers", ping_numbers},
        {"ping_distances", ping_distances}
    });

    CHECK(converter.binary_hash() == 15667435390590270911ULL);

    // test copy
    CHECK(converter == NearestFeatureMapper(converter));

    // test binary
    CHECK(converter == NearestFeatureMapper(converter.from_binary(converter.to_binary())));

    // test feature_to_index (nearest interpolation)
    SECTION("feature_to_index should return nearest index")
    {
        // Test exact matches
        CHECK(converter.feature_to_index("ping_times", 0.0) == 0);
        CHECK(converter.feature_to_index("ping_times", 1.0) == 1);
        CHECK(converter.feature_to_index("ping_times", 4.0) == 4);
        
        // Test nearest neighbor interpolation
        CHECK(converter.feature_to_index("ping_times", 0.4) == 0); // closer to 0.0
        CHECK(converter.feature_to_index("ping_times", 0.6) == 1); // closer to 1.0
        CHECK(converter.feature_to_index("ping_times", 1.4) == 1); // closer to 1.0
        CHECK(converter.feature_to_index("ping_times", 1.6) == 2); // closer to 2.0
        CHECK(converter.feature_to_index("ping_times", 3.7) == 4); // closer to 4.0
        
        // Test with ping_numbers
        CHECK(converter.feature_to_index("ping_numbers", 149.0) == 0); // closer to 100.0
        CHECK(converter.feature_to_index("ping_numbers", 249.0) == 1); // closer to 200.0
        CHECK(converter.feature_to_index("ping_numbers", 349.0) == 2); // closer to 300.0
        
        // Test boundary values
        CHECK(converter.feature_to_index("ping_times", -1.0) == 0); // extrapolation to first
        CHECK(converter.feature_to_index("ping_times", 10.0) == 4); // extrapolation to last
    }

    SECTION("index_to_feature should return correct values")
    {
        // Test all indices for ping_times
        CHECK(converter.index_to_feature("ping_times", 0) == Catch::Approx(0.0));
        CHECK(converter.index_to_feature("ping_times", 1) == Catch::Approx(1.0));
        CHECK(converter.index_to_feature("ping_times", 2) == Catch::Approx(2.0));
        CHECK(converter.index_to_feature("ping_times", 3) == Catch::Approx(3.0));
        CHECK(converter.index_to_feature("ping_times", 4) == Catch::Approx(4.0));
        
        // Test all indices for ping_numbers
        CHECK(converter.index_to_feature("ping_numbers", 0) == Catch::Approx(100.0));
        CHECK(converter.index_to_feature("ping_numbers", 1) == Catch::Approx(200.0));
        CHECK(converter.index_to_feature("ping_numbers", 2) == Catch::Approx(300.0));
        CHECK(converter.index_to_feature("ping_numbers", 3) == Catch::Approx(400.0));
        CHECK(converter.index_to_feature("ping_numbers", 4) == Catch::Approx(500.0));
        
        // Test all indices for ping_distances
        CHECK(converter.index_to_feature("ping_distances", 0) == Catch::Approx(0.0));
        CHECK(converter.index_to_feature("ping_distances", 1) == Catch::Approx(10.5));
        CHECK(converter.index_to_feature("ping_distances", 2) == Catch::Approx(21.0));
        CHECK(converter.index_to_feature("ping_distances", 3) == Catch::Approx(31.5));
        CHECK(converter.index_to_feature("ping_distances", 4) == Catch::Approx(42.0));
    }

    SECTION("feature_to_feature should convert between features correctly")
    {
        // Test conversion from ping_times to ping_numbers
        CHECK(converter.feature_to_feature("ping_times", "ping_numbers", 0.0) == Catch::Approx(100.0));
        CHECK(converter.feature_to_feature("ping_times", "ping_numbers", 2.0) == Catch::Approx(300.0));
        CHECK(converter.feature_to_feature("ping_times", "ping_numbers", 4.0) == Catch::Approx(500.0));
        
        // Test with nearest neighbor behavior
        CHECK(converter.feature_to_feature("ping_times", "ping_numbers", 0.4) == Catch::Approx(100.0)); // index 0
        CHECK(converter.feature_to_feature("ping_times", "ping_numbers", 1.6) == Catch::Approx(300.0)); // index 2
        
        // Test conversion from ping_numbers to ping_distances
        CHECK(converter.feature_to_feature("ping_numbers", "ping_distances", 100.0) == Catch::Approx(0.0));
        CHECK(converter.feature_to_feature("ping_numbers", "ping_distances", 300.0) == Catch::Approx(21.0));
        CHECK(converter.feature_to_feature("ping_numbers", "ping_distances", 500.0) == Catch::Approx(42.0));
        
        // Test with nearest neighbor behavior for ping_numbers
        CHECK(converter.feature_to_feature("ping_numbers", "ping_distances", 149.0) == Catch::Approx(0.0)); // closer to 100.0, index 0
        CHECK(converter.feature_to_feature("ping_numbers", "ping_distances", 349.0) == Catch::Approx(21.0)); // closer to 300.0, index 2
        
        // Test reverse conversion (ping_distances to ping_times)
        CHECK(converter.feature_to_feature("ping_distances", "ping_times", 0.0) == Catch::Approx(0.0));
        CHECK(converter.feature_to_feature("ping_distances", "ping_times", 21.0) == Catch::Approx(2.0));
        CHECK(converter.feature_to_feature("ping_distances", "ping_times", 42.0) == Catch::Approx(4.0));
        
        // Test with nearest neighbor for distances
        CHECK(converter.feature_to_feature("ping_distances", "ping_times", 5.0) == Catch::Approx(0.0)); // closer to 0.0, index 0
        CHECK(converter.feature_to_feature("ping_distances", "ping_times", 15.0) == Catch::Approx(1.0)); // closer to 10.5, index 1
    }

    SECTION("error handling for invalid features")
    {
        CHECK_THROWS_AS(converter.feature_to_index("invalid_feature", 1.0), std::runtime_error);
        CHECK_THROWS_AS(converter.index_to_feature("invalid_feature", 0), std::runtime_error);
        CHECK_THROWS_AS(converter.feature_to_feature("invalid_feature", "ping_times", 1.0), std::runtime_error);
        CHECK_THROWS_AS(converter.feature_to_feature("ping_times", "invalid_feature", 1.0), std::runtime_error);
    }

    SECTION("vectorized feature_to_index should match individual calls")
    {
        // Test with ping_times
        xt::xtensor<float, 1> input_values = {0.0f, 0.4f, 1.6f, 3.7f, 10.0f};
        auto result_indices = converter.feature_to_index<xt::xtensor<size_t, 1>>("ping_times", input_values);
        
        CHECK(result_indices.size() == input_values.size());
        CHECK(result_indices(0) == converter.feature_to_index("ping_times", 0.0));
        CHECK(result_indices(1) == converter.feature_to_index("ping_times", 0.4));
        CHECK(result_indices(2) == converter.feature_to_index("ping_times", 1.6));
        CHECK(result_indices(3) == converter.feature_to_index("ping_times", 3.7));
        CHECK(result_indices(4) == converter.feature_to_index("ping_times", 10.0));
        
        // Test with ping_numbers
        xt::xtensor<float, 1> ping_number_values = {149.0f, 249.0f, 349.0f};
        auto ping_number_indices = converter.feature_to_index<xt::xtensor<float, 1>, xt::xtensor<size_t, 1>>("ping_numbers", ping_number_values);
        
        CHECK(ping_number_indices.size() == ping_number_values.size());
        CHECK(ping_number_indices(0) == converter.feature_to_index("ping_numbers", 149.0));
        CHECK(ping_number_indices(1) == converter.feature_to_index("ping_numbers", 249.0));
        CHECK(ping_number_indices(2) == converter.feature_to_index("ping_numbers", 349.0));
    }

    SECTION("vectorized index_to_feature should match individual calls")
    {
        // Test with ping_times
        xt::xtensor<size_t, 1> input_indices = {0, 1, 2, 3, 4};
        auto result_values = converter.index_to_feature<xt::xtensor<size_t, 1>, xt::xtensor<float, 1>>("ping_times", input_indices);
        
        CHECK(result_values.size() == input_indices.size());
        CHECK(result_values(0) == Catch::Approx(converter.index_to_feature("ping_times", 0)));
        CHECK(result_values(1) == Catch::Approx(converter.index_to_feature("ping_times", 1)));
        CHECK(result_values(2) == Catch::Approx(converter.index_to_feature("ping_times", 2)));
        CHECK(result_values(3) == Catch::Approx(converter.index_to_feature("ping_times", 3)));
        CHECK(result_values(4) == Catch::Approx(converter.index_to_feature("ping_times", 4)));
        
        // Test with ping_distances
        xt::xtensor<size_t, 1> subset_indices = {0, 2, 4};
        auto distance_values = converter.index_to_feature<xt::xtensor<size_t, 1>, xt::xtensor<float, 1>>("ping_distances", subset_indices);
        
        CHECK(distance_values.size() == subset_indices.size());
        CHECK(distance_values(0) == Catch::Approx(converter.index_to_feature("ping_distances", 0)));
        CHECK(distance_values(1) == Catch::Approx(converter.index_to_feature("ping_distances", 2)));
        CHECK(distance_values(2) == Catch::Approx(converter.index_to_feature("ping_distances", 4)));
    }

    SECTION("vectorized feature_to_feature should match individual calls")
    {
        // Test conversion from ping_times to ping_numbers
        xt::xtensor<float, 1> time_values = {0.0f, 0.4f, 2.0f, 1.6f, 4.0f};
        auto number_values = converter.feature_to_feature("ping_times", "ping_numbers", time_values);
        
        CHECK(number_values.size() == time_values.size());
        CHECK(number_values(0) == Catch::Approx(converter.feature_to_feature("ping_times", "ping_numbers", 0.0)));
        CHECK(number_values(1) == Catch::Approx(converter.feature_to_feature("ping_times", "ping_numbers", 0.4)));
        CHECK(number_values(2) == Catch::Approx(converter.feature_to_feature("ping_times", "ping_numbers", 2.0)));
        CHECK(number_values(3) == Catch::Approx(converter.feature_to_feature("ping_times", "ping_numbers", 1.6)));
        CHECK(number_values(4) == Catch::Approx(converter.feature_to_feature("ping_times", "ping_numbers", 4.0)));
        
        // Test conversion from ping_numbers to ping_distances
        xt::xtensor<float, 1> number_input = {100.0f, 349.0f, 500.0f};
        auto distance_output = converter.feature_to_feature("ping_numbers", "ping_distances", number_input);
        
        CHECK(distance_output.size() == number_input.size());
        CHECK(distance_output(0) == Catch::Approx(converter.feature_to_feature("ping_numbers", "ping_distances", 100.0)));
        CHECK(distance_output(1) == Catch::Approx(converter.feature_to_feature("ping_numbers", "ping_distances", 349.0)));
        CHECK(distance_output(2) == Catch::Approx(converter.feature_to_feature("ping_numbers", "ping_distances", 500.0)));
    }

    // test stream
    std::stringstream buffer;
    converter.to_stream(buffer);
    CHECK(converter == NearestFeatureMapper(converter.from_stream(buffer)));

    // test print does not crash
    CHECK(converter.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    CHECK(converter.binary_hash() == NearestFeatureMapper(converter).binary_hash());
    CHECK(converter.binary_hash() ==
          NearestFeatureMapper(converter.from_binary(converter.to_binary())).binary_hash());

    CHECK(converter.binary_hash() == 15667435390590270911ULL);
}
