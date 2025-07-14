// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/misc/xsort.hpp>
#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/functions/weighted_median.hpp>


// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::pointprocessing::functions;

#define TESTTAG "[weighted_median]"


/*
  Weighted median is the smallest value for which the cumulative weight
  is >= 50% of the total.
*/

TEST_CASE("Weighted Median - Uniform Weights", TESTTAG)
{
    // Simple case where all weights are equal
    xt::xtensor<double, 1> values  = {1.0, 2.0, 3.0, 4.0};
    xt::xtensor<double, 1> weights = {1.0, 1.0, 1.0, 1.0};

    double result = weighted_median(values, weights);
    
    // For uniform weights, median of [1,2,3,4] is (2+3)/2 = 2.5
    REQUIRE(result == Catch::Approx(2.5));
}

TEST_CASE("Weighted Median - Non-Uniform Weights", TESTTAG)
{
    // Larger weight on 10 skews the median higher
    xt::xtensor<double, 1> values  = {1.0, 2.0, 4.0, 10.0};
    xt::xtensor<double, 1> weights = {1.0, 2.0, 1.0, 10.0};

    double result = weighted_median(values, weights);

    // Heavier weight on 10. We expect a weighted median close to 10.0
    REQUIRE(result == Catch::Approx(10.0));
}

TEST_CASE("Weighted Median - Single Value", TESTTAG)
{
    // Only one value => Weighted median = that value
    xt::xtensor<double, 1> values  = {42.0};
    xt::xtensor<double, 1> weights = {5.0};

    double result = weighted_median(values, weights);
    REQUIRE(result == Catch::Approx(42.0));
}

TEST_CASE("Weighted Median - Edge Cases", TESTTAG)
{
    // Check exception if input is empty
    xt::xtensor<double, 1> empty_vals, empty_wts;
    REQUIRE_THROWS_AS(weighted_median(empty_vals, empty_wts), std::invalid_argument);

    // Check size mismatch
    xt::xtensor<double, 1> vals_ok = {1.0, 2.0};
    xt::xtensor<double, 1> wts_mis = {1.0};
    REQUIRE_THROWS_AS(weighted_median(vals_ok, wts_mis), std::invalid_argument);
}
