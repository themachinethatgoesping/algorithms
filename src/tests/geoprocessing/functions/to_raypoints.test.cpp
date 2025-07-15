// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/generators/xrandom.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/functions/to_raypoints.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing;

#define TESTTAG "[geoprocessing]"

TEST_CASE("to_raypoints should reproduce precomputed results", TESTTAG) {}

TEST_CASE("to_raypoints basic interpolation check", TESTTAG)
{
    double base_location    = 0.0;
    double base_scale_value = 0.0;

    auto end_locations    = xt::xtensor<double, 1>({ 10.0, 20.0 });
    auto end_scale_values = xt::xtensor<double, 1>({ 100.0, 150.0 });
    auto ray_scale_values = xt::xtensor<double, 1>({ 0.0, 0.5, 1.0, 100.0, 200.0, 300.0 });

    auto output = functions::to_raypoints<xt::xtensor<double, 2>>(
        base_location, end_locations, base_scale_value, end_scale_values, ray_scale_values);

    REQUIRE(output.shape()[0] == 2);
    REQUIRE(output.shape()[1] == 6);
    //ray one
    CHECK(output(0, 0) == Catch::Approx(0.0));
    CHECK(output(0, 1) == Catch::Approx(0.05));
    CHECK(output(0, 2) == Catch::Approx(0.1));
    CHECK(output(0, 3) == Catch::Approx(10.0));
    CHECK(output(0, 4) == Catch::Approx(20.0));
    CHECK(output(0, 5) == Catch::Approx(30.0));

    //ray two
    CHECK(output(1, 0) == Catch::Approx(0.0));
    CHECK(output(1, 1) == Catch::Approx(0.066666666666).scale(0.0001));
    CHECK(output(1, 2) == Catch::Approx(0.133333333333).scale(0.0001));
    CHECK(output(1, 3) == Catch::Approx(13.33333333333).scale(0.0001));
    CHECK(output(1, 4) == Catch::Approx(26.66666666666).scale(0.0001));
    CHECK(output(1, 5) == Catch::Approx(40.0));
}
