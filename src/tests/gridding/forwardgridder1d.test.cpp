// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/generators/xrandom.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder1d.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::gridding;

#define TESTTAG "[gridding]"

template<typename t_gridder>
void test_read_write(const t_gridder& gridder, uint64_t hash)
{
    CHECK(gridder.binary_hash() == hash);

    // test copy
    {
        INFO(gridder.info_string());
        INFO(t_gridder(gridder).info_string());
        CHECK(gridder == t_gridder(gridder));
    }

    // test binary
    {
        auto gridder2 = t_gridder::from_binary(gridder.to_binary());
        INFO(gridder.info_string());
        INFO(gridder2.info_string());
        CHECK(gridder == gridder2);
    }

    // test stream
    {
        std::stringstream buffer;
        gridder.to_stream(buffer);
        auto gridder2 = t_gridder::from_stream(buffer);
        INFO(gridder.info_string());
        INFO(gridder2.info_string());
        CHECK(gridder == gridder2);
    }

    // test print does not crash
    CHECK(gridder.info_string().size() != 0);

    // test hash (should be stable if class is not changed)
    CHECK(gridder.binary_hash() == hash);
    CHECK(gridder.binary_hash() == t_gridder(gridder).binary_hash());
    CHECK(gridder.binary_hash() ==
          t_gridder(gridder.from_binary(gridder.to_binary())).binary_hash());
}

TEST_CASE("Test ForwardGridder1D constructor", TESTTAG)
{
    ForwardGridder1D<double> gridder(1.0, 0.0, 5.0);
    test_read_write(gridder, 16060525357025566167ULL);

    CHECK(gridder.get_x_value(0) == 0.0);
    CHECK(gridder.get_x_value(5) == 5.0);
}

TEST_CASE("Test ForwardGridder1D factory methods", TESTTAG)
{
    SECTION("from_res")
    {
        auto gridder = ForwardGridder1D<double>::from_res(0.5, 0.1, 4.9);
        test_read_write(gridder, 5847758235618054408ULL);

        CHECK(gridder.get_x_index(0.1) == 0);
        CHECK(gridder.get_x_index(4.9) == 10);
    }

    SECTION("from_data")
    {
        std::vector<double> x{ 0.1, 4.9, NAN };

        auto gridder = ForwardGridder1D<double>::from_data(0.5, x);
        test_read_write(gridder, 5847758235618054408ULL);

        CHECK(gridder.get_x_index(0.1) == 0);
        CHECK(gridder.get_x_index(4.9) == 10);
    }
}

TEST_CASE("Test get_empty_grd_images", TESTTAG)
{
    ForwardGridder1D<double> gridder(1.0, 0.0, 5.0);
    test_read_write(gridder, 16060525357025566167ULL);

    auto [img_vals, img_wgts] = gridder.get_empty_grd_images<xt::xtensor<double, 1>>();

    CHECK(img_vals.shape()[0] == 6);
    CHECK(img_wgts.shape()[0] == 6);

    CHECK(img_vals(0) == 0.0);
    CHECK(img_wgts(0) == 0.0);
}

TEST_CASE("Test interpolation methods", TESTTAG)
{
    ForwardGridder1D<double> gridder(1.0, 0.0, 2.0);
    test_read_write(gridder, 6387800190055886331ULL);

    std::vector<double> x{ 0.5, 1.5 };
    std::vector<double> v{ 10.0, 20.0 };

    SECTION("interpolate_block_mean")
    {
        auto [img_vals, img_wgts] =
            gridder.interpolate_block_mean<xt::xtensor<double, 1>>(x, v);

        CHECK(img_vals.shape()[0] == 3);

        CHECK(img_vals(0) == 0.0);
        CHECK(img_vals(1) == 10.0);
        CHECK(img_vals(2) == 20.0);

        CHECK(img_wgts(0) == 0.0);
        CHECK(img_wgts(1) == 1.0);
        CHECK(img_wgts(2) == 1.0);
    }

    SECTION("interpolate_weighted_mean")
    {
        auto [img_vals, img_wgts] =
            gridder.interpolate_weighted_mean<xt::xtensor<double, 1>>(x, v);

        CHECK(img_vals.shape()[0] == 3);

        // Check some weighted mean values
        // For each point we should have weights distributed to 2 surrounding grid points
        CHECK(img_vals(0) > 0.0);
        CHECK(img_vals(1) > 0.0);
        CHECK(img_vals(2) > 0.0);

        // Total weight should be equal to the number of input points
        double total_weight = 0.0;
        for (size_t i = 0; i < 3; i++)
        {
            total_weight += img_wgts(i);
        }
        CHECK(total_weight == Catch::Approx(2.0));
    }
}

TEST_CASE("Test utility methods", TESTTAG)
{
    ForwardGridder1D<double> gridder(1.0, 0.0, 5.0);
    test_read_write(gridder, 16060525357025566167ULL);

    SECTION("Index functions")
    {
        CHECK(gridder.get_x_index(2.7) == 3);
        CHECK(gridder.get_x_index_fraction(2.7) == Catch::Approx(2.7));
    }

    SECTION("Value functions")
    {
        CHECK(gridder.get_x_value(2) == 2.0);
        CHECK(gridder.get_x_grd_value(2.7) == 3.0);
    }
}

TEST_CASE("Test coordinate methods", TESTTAG)
{
    ForwardGridder1D<double> gridder(1.0, 0.0, 2.0);
    test_read_write(gridder, 6387800190055886331ULL);

    SECTION("Extent functions")
    {
        auto ext_x = gridder.get_extent_x();
        CHECK(ext_x[0] == -0.5);
        CHECK(ext_x[1] == 2.5);

        auto ext = gridder.get_extent("x");
        CHECK(ext.size() == 2);
        CHECK(ext[0] == -0.5);
        CHECK(ext[1] == 2.5);
    }

    SECTION("Coordinate functions")
    {
        auto coords_x = gridder.get_x_coordinates();
        CHECK(coords_x.size() == 3);
        CHECK(coords_x[0] == 0.0);
        CHECK(coords_x[1] == 1.0);
        CHECK(coords_x[2] == 2.0);
    }
}
