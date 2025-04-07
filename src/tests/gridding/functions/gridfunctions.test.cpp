// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/xrandom.hpp>

#include "../../../themachinethatgoesping/algorithms/gridding/functions/gridfunctions.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::gridding::functions;

#define TESTTAG "[gridding]"

TEST_CASE("Test get_minmax", TESTTAG)
{
    std::vector<double> x{ 1.0, 2.0, NAN }, y{ 2.0, 3.0, NAN }, z{ 3.0, 4.0, NAN };
    auto [minx, maxx, miny, maxy, minz, maxz] = get_minmax(x, y, z);
    CHECK(minx == 1.0);
    CHECK(maxx == 2.0);
    CHECK(miny == 2.0);
    CHECK(maxy == 3.0);
    CHECK(minz == 3.0);
    CHECK(maxz == 4.0);
}

TEST_CASE("Test get_index", TESTTAG)
{
    CHECK(get_index(5.0, 0.0, 1.0) == 5);
}

TEST_CASE("Test get_index_fraction", TESTTAG)
{
    CHECK(get_index_fraction(5.5, 0.0, 1.0) == Catch::Approx(5.5));
}

TEST_CASE("Test get_value", TESTTAG)
{
    CHECK(get_value(3.0, 0.0, 1.0) == Catch::Approx(3.0));
}

TEST_CASE("Test get_grd_value", TESTTAG)
{
    CHECK(get_grd_value(3.3, 0.0, 1.0) == 3.0);
}

TEST_CASE("Test get_index_weights", TESTTAG)
{
    SECTION("3D")
    {
        auto [xs, ys, zs, ws] = get_index_weights(1.25, 2.5, 0.75);
        CHECK(xs.size() == 8);
        CHECK(ys.size() == 8);
        CHECK(zs.size() == 8);
        CHECK(ws.size() == 8);
    }
    SECTION("2D")
    {
        auto [xs, ys, ws] = get_index_weights(1.25, 2.5);
        CHECK(xs.size() == 4);
        CHECK(ys.size() == 4);
        CHECK(ws.size() == 4);
    }
    SECTION("1D")
    {
        auto [xs, ws] = get_index_weights(1.25);
        CHECK(xs.size() == 2);
        CHECK(ws.size() == 2);
    }
}

TEST_CASE("Test grd_weighted_mean", TESTTAG)
{
    std::vector<double> x{ 0.25 }, y{ 0.25 }, z{ 0.25 }, v{ 10.0 };

    SECTION("3D")
    {
        xt::xtensor<double, 3> img_vals = xt::zeros<double>({ 2, 2, 2 });
        xt::xtensor<double, 3> img_wgts = xt::zeros<double>({ 2, 2, 2 });
        grd_weighted_mean(x, y, z, v, 0.0, 1.0, 2, 0.0, 1.0, 2, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(0, 0, 0) == Catch::Approx(4.21875).epsilon(0.01));
        CHECK(img_wgts(0, 0, 0) == Catch::Approx(0.421875).epsilon(0.01));
    }
    SECTION("2D")
    {
        xt::xtensor<double, 2> img_vals = xt::zeros<double>({ 2, 2 });
        xt::xtensor<double, 2> img_wgts = xt::zeros<double>({ 2, 2 });
        grd_weighted_mean(x, y, v, 0.0, 1.0, 2, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(0, 0) == Catch::Approx(5.625).epsilon(0.01));
        CHECK(img_wgts(0, 0) == Catch::Approx(0.5625).epsilon(0.01));
    }
    SECTION("1D")
    {
        xt::xtensor<double, 1> img_vals = xt::zeros<double>({ 2 });
        xt::xtensor<double, 1> img_wgts = xt::zeros<double>({ 2 });
        grd_weighted_mean(x, v, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(0) == Catch::Approx(7.5).epsilon(0.01));
        CHECK(img_wgts(0) == Catch::Approx(0.75).epsilon(0.01));
    }
}

TEST_CASE("Test grd_block_mean", TESTTAG)
{
    std::vector<double> x{ 1.25 }, y{ 1.25 }, z{ 0.8 }, v{ 5.0 };

    SECTION("3D")
    {
        xt::xtensor<double, 3> img_vals = xt::zeros<double>({ 2, 2, 2 });
        xt::xtensor<double, 3> img_wgts = xt::zeros<double>({ 2, 2, 2 });
        grd_block_mean(x, y, z, v, 0.0, 1.0, 2, 0.0, 1.0, 2, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(1, 1, 0) == Catch::Approx(0.0));
        CHECK(img_wgts(1, 1, 0) == Catch::Approx(0.0));
    }
    SECTION("2D")
    {
        xt::xtensor<double, 2> img_vals = xt::zeros<double>({ 2, 2 });
        xt::xtensor<double, 2> img_wgts = xt::zeros<double>({ 2, 2 });
        grd_block_mean(x, y, v, 0.0, 1.0, 2, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(1, 1) == Catch::Approx(5.0));
        CHECK(img_wgts(1, 1) == Catch::Approx(1.0));
    }
    SECTION("1D")
    {
        xt::xtensor<double, 1> img_vals = xt::zeros<double>({ 2 });
        xt::xtensor<double, 1> img_wgts = xt::zeros<double>({ 2 });
        grd_block_mean(x, v, 0.0, 1.0, 2, img_vals, img_wgts);
        CHECK(img_vals(1) == Catch::Approx(5.0));
        CHECK(img_wgts(1) == Catch::Approx(1.0));
    }
}
