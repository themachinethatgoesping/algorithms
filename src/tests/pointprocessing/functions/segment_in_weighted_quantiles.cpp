// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/misc/xsort.hpp>
#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/functions/segment_in_weighted_quantiles.hpp>


using namespace std;
using namespace themachinethatgoesping::algorithms::pointprocessing::functions;

#define TESTTAG "[segment_in_weighted_quantiles]"

/*
    Segment in weighted quantiles function segments the data into quantiles
    based on the provided weights.
*/

TEST_CASE("Segment in Weighted Quantiles - Uniform Weights", TESTTAG)
{
    // Simple case where all weights are equal
    xt::xtensor<double, 1> values        = { 1.0, 2.0, 3.0, 4.0 };
    xt::xtensor<double, 1> weights       = { 1.0, 1.0, 1.0, 1.0 };
    int64_t                num_quantiles = 2;

    auto result = segment_in_weighted_quantiles(values, weights, num_quantiles);

    // For uniform weights and 2 quantiles, we expect two segments: [0, 1] and [2, 3]
    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == xt::xtensor<int64_t, 1>({ 0, 1 }));
    REQUIRE(result[1] == xt::xtensor<int64_t, 1>({ 2, 3 }));
}

TEST_CASE("Segment in Weighted Quantiles - Non-Uniform Weights", TESTTAG)
{
    // Larger weight on 10 skews the quantiles
    xt::xtensor<double, 1> values        = { 1.0, 2.0, 4.0, 10.0 };
    xt::xtensor<double, 1> weights       = { 1.0, 9.0, 1.0, 10.0 };
    int64_t                num_quantiles = 2;

    auto result = segment_in_weighted_quantiles(values, weights, num_quantiles);

    // Heavier weight on 10. We expect the segments to be skewed towards higher values
    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == xt::xtensor<int64_t, 1>({ 0, 1, 2 }));
    REQUIRE(result[1] == xt::xtensor<int64_t, 1>({ 3 }));
}

TEST_CASE("Segment in Weighted Quantiles - Single Value", TESTTAG)
{
    // Only one value => Single segment containing that value
    xt::xtensor<double, 1> values        = { 42.0 };
    xt::xtensor<double, 1> weights       = { 5.0 };
    int64_t                num_quantiles = 1;

    auto result = segment_in_weighted_quantiles(values, weights, num_quantiles);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == xt::xtensor<int64_t, 1>({ 0 }));
}

TEST_CASE("Segment in Weighted Quantiles - Edge Cases", TESTTAG)
{
    // Check exception if input is empty
    xt::xtensor<double, 1> empty_vals, empty_wts;
    int64_t                num_quantiles = 2;
    REQUIRE_THROWS_AS(segment_in_weighted_quantiles(empty_vals, empty_wts, num_quantiles),
                      std::invalid_argument);

    // Check size mismatch
    xt::xtensor<double, 1> vals_ok = { 1.0, 2.0 };
    xt::xtensor<double, 1> wts_mis = { 1.0 };
    REQUIRE_THROWS_AS(segment_in_weighted_quantiles(vals_ok, wts_mis, num_quantiles),
                      std::invalid_argument);
}

TEST_CASE("Segment in Weighted Quantiles - Five Quantiles", TESTTAG)
{
    // Non-uniform weights leading to segments of different sizes
    xt::xtensor<double, 1> values        = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    xt::xtensor<double, 1> weights       = { 1.0, 2.0, 1.0, 1.0, 1.0, 4.0, 1.0, 2.0, 1.0, 1.0 };
    int64_t                num_quantiles = 5;
    
    //cumsum: 1,3,4,5,6,10,11,13,14,15
    auto result = segment_in_weighted_quantiles(values, weights, num_quantiles);

    // Expect segments of different sizes due to non-uniform weights
    REQUIRE(result.size() == 5);
    REQUIRE(result[0] == xt::xtensor<int64_t, 1>({ 0, 1 }));
    REQUIRE(result[1] == xt::xtensor<int64_t, 1>({ 2, 3, 4 }));
    REQUIRE(result[2] == xt::xtensor<int64_t, 1>({ 5 }));
    REQUIRE(result[3] == xt::xtensor<int64_t, 1>({ 6, 7 }));
    REQUIRE(result[4] == xt::xtensor<int64_t, 1>({ 8, 9 }));
}