// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/gridding/functions/resamplingfunctions.hpp"

using namespace themachinethatgoesping::algorithms::gridding::functions;

#define TESTTAG "[gridding]"

TEST_CASE("compute_resampled_coordinates: basic functionality", TESTTAG)
{
    // Simple test data with known statistical properties
    xt::xtensor<float, 1> values_min = {10.0f, 15.0f, 20.0f, 25.0f};
    xt::xtensor<float, 1> values_max = {50.0f, 55.0f, 60.0f, 65.0f};
    xt::xtensor<float, 1> values_res = {1.0f, 1.0f, 1.0f, 1.0f};

    auto coordinates = compute_resampled_coordinates(values_min, values_max, values_res);

    REQUIRE(coordinates.size() > 50);
    REQUIRE(coordinates(0) == Catch::Approx(10.0f).epsilon(0.01f));
    REQUIRE(coordinates(coordinates.size()-1) <= 65.0f);
    
    if (coordinates.size() > 1) {
        float spacing = coordinates(1) - coordinates(0);
        REQUIRE(spacing == Catch::Approx(1.0f).epsilon(0.01f));
    }
}

TEST_CASE("compute_resampled_coordinates: explicit bounds and max_steps", TESTTAG)
{
    xt::xtensor<double, 1> values_min = {5.0, 6.0, std::numeric_limits<double>::quiet_NaN()};
    xt::xtensor<double, 1> values_max = {15.0, 16.0, 17.0};
    xt::xtensor<double, 1> values_res = {0.5, 0.5, 0.5};
    
    double explicit_min = 7.0;
    double explicit_max = 14.0;

    auto coordinates = compute_resampled_coordinates(
        values_min, values_max, values_res, explicit_min, explicit_max);

    REQUIRE(coordinates.size() >= 14);
    REQUIRE(coordinates(0) == Catch::Approx(7.0).epsilon(0.01));
    
    // Test max_steps limitation with fine resolution
    xt::xtensor<float, 1> fine_min = {0.0f, 1.0f};
    xt::xtensor<float, 1> fine_max = {100.0f, 101.0f};
    xt::xtensor<float, 1> fine_res = {0.1f, 0.1f};
    
    auto limited_coords = compute_resampled_coordinates(
        fine_min, fine_max, fine_res, 
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::quiet_NaN(),
        50);

    REQUIRE(limited_coords.size() <= 50);
    if (limited_coords.size() > 1) {
        float spacing = limited_coords(1) - limited_coords(0);
        REQUIRE(spacing >= 1.5f); // Should be coarser than 0.1
    }
}

