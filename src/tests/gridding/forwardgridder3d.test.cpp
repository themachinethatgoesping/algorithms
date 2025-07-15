// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/generators/xrandom.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder3d.hpp>

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

TEST_CASE("Test ForwardGridder3D constructor", TESTTAG)
{
    ForwardGridder3D<double> gridder(1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 5.0, 0.0, 5.0);
    test_read_write(gridder, 15531528956952703043ULL);

    CHECK(gridder.get_x_value(0) == 0.0);
    CHECK(gridder.get_x_value(5) == 5.0);
    CHECK(gridder.get_y_value(0) == 0.0);
    CHECK(gridder.get_y_value(5) == 5.0);
    CHECK(gridder.get_z_value(0) == 0.0);
    CHECK(gridder.get_z_value(5) == 5.0);
}

TEST_CASE("Test ForwardGridder3D factory methods", TESTTAG)
{
    SECTION("from_res")
    {
        auto gridder = ForwardGridder3D<double>::from_res(0.5, 0.1, 4.9, 0.1, 4.9, 0.1, 4.9);
        test_read_write(gridder, 5296434962229713227ULL);

        CHECK(gridder.get_x_index(0.1) == 0);
        CHECK(gridder.get_x_index(4.9) == 10);
        CHECK(gridder.get_y_index(0.1) == 0);
        CHECK(gridder.get_y_index(4.9) == 10);
        CHECK(gridder.get_z_index(0.1) == 0);
        CHECK(gridder.get_z_index(4.9) == 10);
    }

    SECTION("from_data")
    {
        std::vector<double> x{ 0.1, 4.9, NAN };
        std::vector<double> y{ 0.1, 4.9, NAN };
        std::vector<double> z{ 0.1, 4.9, NAN };

        auto gridder = ForwardGridder3D<double>::from_data(0.5, x, y, z);
        test_read_write(gridder, 5296434962229713227ULL);

        CHECK(gridder.get_x_index(0.1) == 0);
        CHECK(gridder.get_x_index(4.9) == 10);
        CHECK(gridder.get_y_index(0.1) == 0);
        CHECK(gridder.get_y_index(4.9) == 10);
        CHECK(gridder.get_z_index(0.1) == 0);
        CHECK(gridder.get_z_index(4.9) == 10);
    }
}

TEST_CASE("Test get_empty_grd_images", TESTTAG)
{
    ForwardGridder3D<double> gridder(1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 5.0, 0.0, 5.0);
    test_read_write(gridder, 15531528956952703043ULL);

    auto [img_vals, img_wgts] = gridder.get_empty_grd_images<xt::xtensor<double, 3>>();

    CHECK(img_vals.shape()[0] == 6);
    CHECK(img_vals.shape()[1] == 6);
    CHECK(img_vals.shape()[2] == 6);
    CHECK(img_wgts.shape()[0] == 6);
    CHECK(img_wgts.shape()[1] == 6);
    CHECK(img_wgts.shape()[2] == 6);

    CHECK(img_vals(0, 0, 0) == 0.0);
    CHECK(img_wgts(0, 0, 0) == 0.0);
}

TEST_CASE("Test interpolation methods", TESTTAG)
{
    ForwardGridder3D<double> gridder(1.0, 1.0, 1.0, 0.0, 2.0, 0.0, 2.0, 0.0, 2.0);
    test_read_write(gridder, 5725600377409992654ULL);

    std::vector<double> x{ 0.5, 1.5 };
    std::vector<double> y{ 0.5, 1.5 };
    std::vector<double> z{ 0.5, 1.5 };
    std::vector<double> v{ 10.0, 20.0 };

    SECTION("interpolate_block_mean")
    {
        auto [img_vals, img_wgts] =
            gridder.interpolate_block_mean<xt::xtensor<double, 3>>(x, y, z, v);

        CHECK(img_vals.shape()[0] == 3);
        CHECK(img_vals.shape()[1] == 3);
        CHECK(img_vals.shape()[2] == 3);

        CHECK(img_vals(0, 0, 0) == 0.0);
        CHECK(img_vals(0, 0, 1) == 0.0);
        CHECK(img_vals(0, 1, 0) == 0.0);
        CHECK(img_vals(1, 0, 0) == 0.0);
        CHECK(img_vals(0, 1, 1) == 0.0);
        CHECK(img_vals(1, 0, 1) == 0.0);
        CHECK(img_vals(1, 1, 0) == 0.0);
        CHECK(img_vals(1, 1, 1) == 10.0);

        CHECK(img_wgts(1, 1, 1) == 1.0);
    }

    SECTION("interpolate_weighted_mean")
    {
        auto [img_vals, img_wgts] =
            gridder.interpolate_weighted_mean<xt::xtensor<double, 3>>(x, y, z, v);

        CHECK(img_vals.shape()[0] == 3);
        CHECK(img_vals.shape()[1] == 3);
        CHECK(img_vals.shape()[2] == 3);

        // Check some weighted mean values
        // For each point we should have weights distributed to 8 surrounding grid points
        CHECK(img_vals(0, 0, 0) > 0.0);
        CHECK(img_vals(0, 0, 1) > 0.0);
        CHECK(img_vals(0, 1, 0) > 0.0);
        CHECK(img_vals(1, 0, 0) > 0.0);
        CHECK(img_vals(1, 1, 1) > 0.0);

        // Total weight should be equal to the number of input points
        double total_weight = 0.0;
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                for (size_t k = 0; k < 3; k++)
                {
                    total_weight += img_wgts(i, j, k);
                }
            }
        }
        CHECK(total_weight == Catch::Approx(2.0));
    }
    test_read_write(gridder, 5725600377409992654ULL);
}

TEST_CASE("Test utility methods", TESTTAG)
{
    ForwardGridder3D<double> gridder(1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 5.0, 0.0, 5.0);
    test_read_write(gridder, 15531528956952703043ULL);

    SECTION("Index functions")
    {
        CHECK(gridder.get_x_index(2.7) == 3);
        CHECK(gridder.get_y_index(3.2) == 3);
        CHECK(gridder.get_z_index(4.9) == 5);

        CHECK(gridder.get_x_index_fraction(2.7) == Catch::Approx(2.7));
        CHECK(gridder.get_y_index_fraction(3.2) == Catch::Approx(3.2));
        CHECK(gridder.get_z_index_fraction(4.9) == Catch::Approx(4.9));
    }

    SECTION("Value functions")
    {
        CHECK(gridder.get_x_value(2) == 2.0);
        CHECK(gridder.get_y_value(3) == 3.0);
        CHECK(gridder.get_z_value(4) == 4.0);

        CHECK(gridder.get_x_grd_value(2.7) == 3.0);
        CHECK(gridder.get_y_grd_value(3.2) == 3.0);
        CHECK(gridder.get_z_grd_value(4.9) == 5.0);
    }
    test_read_write(gridder, 15531528956952703043ULL);
}

TEST_CASE("Test coordinate methods", TESTTAG)
{
    ForwardGridder3D<double> gridder(1.0, 1.0, 1.0, 0.0, 2.0, 0.0, 2.0, 0.0, 2.0);
    test_read_write(gridder, 5725600377409992654ULL);

    SECTION("Extent functions")
    {
        auto ext_x = gridder.get_extent_x();
        CHECK(ext_x[0] == -0.5);
        CHECK(ext_x[1] == 2.5);

        auto ext_y = gridder.get_extent_y();
        CHECK(ext_y[0] == -0.5);
        CHECK(ext_y[1] == 2.5);

        auto ext_z = gridder.get_extent_z();
        CHECK(ext_z[0] == -0.5);
        CHECK(ext_z[1] == 2.5);

        auto ext_xyz = gridder.get_extent("xyz");
        CHECK(ext_xyz.size() == 6);
        CHECK(ext_xyz[0] == -0.5);
        CHECK(ext_xyz[1] == 2.5);
        CHECK(ext_xyz[2] == -0.5);
        CHECK(ext_xyz[3] == 2.5);
        CHECK(ext_xyz[4] == -0.5);
        CHECK(ext_xyz[5] == 2.5);

        auto ext_xz = gridder.get_extent("xz");
        CHECK(ext_xz.size() == 4);
        CHECK(ext_xz[0] == -0.5);
        CHECK(ext_xz[1] == 2.5);
        CHECK(ext_xz[2] == -0.5);
        CHECK(ext_xz[3] == 2.5);
    }

    SECTION("Coordinate functions")
    {
        auto coords_x = gridder.get_x_coordinates();
        CHECK(coords_x.size() == 3);
        CHECK(coords_x[0] == 0.0);
        CHECK(coords_x[1] == 1.0);
        CHECK(coords_x[2] == 2.0);

        auto coords_y = gridder.get_y_coordinates();
        CHECK(coords_y.size() == 3);
        CHECK(coords_y[0] == 0.0);
        CHECK(coords_y[1] == 1.0);
        CHECK(coords_y[2] == 2.0);

        auto coords_z = gridder.get_z_coordinates();
        CHECK(coords_z.size() == 3);
        CHECK(coords_z[0] == 0.0);
        CHECK(coords_z[1] == 1.0);
        CHECK(coords_z[2] == 2.0);
    }
    test_read_write(gridder, 5725600377409992654ULL);
}
