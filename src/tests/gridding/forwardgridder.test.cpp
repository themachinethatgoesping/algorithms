// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/generators/xrandom.hpp>
#include <xtensor/views/xview.hpp>
#include <xtensor/containers/xadapt.hpp>

#include <themachinethatgoesping/algorithms/gridding/forwardgridder1d.hpp>
#include <themachinethatgoesping/algorithms/gridding/forwardgridder2d.hpp>
#include <themachinethatgoesping/algorithms/gridding/forwardgridder3d.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::gridding;

#define TESTTAG "[gridding][dimensional_equivalence]"

// Helper function to compare values from different dimensional arrays
template <typename T_1D, typename T_2D>
void compare_1d_2d(const T_1D& arr_1d, const T_2D& arr_2d, size_t y_index = 0)
{
    REQUIRE(arr_1d.size() == arr_2d.shape()[0]);
    for (size_t i = 0; i < arr_1d.size(); i++)
    {
        CHECK(arr_1d(i) == Catch::Approx(arr_2d(i, y_index)));
    }
}

template <typename T_1D, typename T_3D>
void compare_1d_3d(const T_1D& arr_1d, const T_3D& arr_3d, size_t y_index = 0, size_t z_index = 0)
{
    REQUIRE(arr_1d.size() == arr_3d.shape()[0]);
    for (size_t i = 0; i < arr_1d.size(); i++)
    {
        CHECK(arr_1d(i) == Catch::Approx(arr_3d(i, y_index, z_index)));
    }
}

template <typename T_2D, typename T_3D>
void compare_2d_3d(const T_2D& arr_2d, const T_3D& arr_3d, size_t z_index = 0)
{
    REQUIRE(arr_2d.shape()[0] == arr_3d.shape()[0]);
    REQUIRE(arr_2d.shape()[1] == arr_3d.shape()[1]);
    for (size_t i = 0; i < arr_2d.shape()[0]; i++)
    {
        for (size_t j = 0; j < arr_2d.shape()[1]; j++)
        {
            CHECK(arr_2d(i, j) == Catch::Approx(arr_3d(i, j, z_index)));
        }
    }
}

TEST_CASE("Test ForwardGridder1D vs ForwardGridder2D with y=0", TESTTAG)
{
    // Test data
    std::vector<double> x_values{ 0.7, 1.2, 2.5, 3.3, 4.8 };
    std::vector<double> y_zeros(x_values.size(), 0.0);
    std::vector<double> data_values{ 10.0, 20.0, 15.0, 25.0, 30.0 };
    
    // Create equivalent gridders
    auto gridder1d = ForwardGridder1D<double>::from_data(1.0, x_values);
    auto gridder2d = ForwardGridder2D<double>::from_data(1.0, x_values, y_zeros);
    
    SECTION("Check grid parameters match")
    {
        CHECK(gridder1d.get_nx() == gridder2d.get_nx());
        CHECK(gridder1d.get_xmin() == gridder2d.get_xmin());
        CHECK(gridder1d.get_xmax() == gridder2d.get_xmax());
        CHECK(gridder1d.get_xres() == gridder2d.get_xres());
        
        auto ext1d = gridder1d.get_extent_x();
        auto ext2d = gridder2d.get_extent_x();
        CHECK(ext1d[0] == ext2d[0]);
        CHECK(ext1d[1] == ext2d[1]);
    }
    
    SECTION("Compare block_mean interpolation")
    {
        auto [img_vals_1d, img_wgts_1d] = 
            gridder1d.interpolate_block_mean<xt::xtensor<double, 1>>(x_values, data_values);
            
        auto [img_vals_2d, img_wgts_2d] = 
            gridder2d.interpolate_block_mean<xt::xtensor<double, 2>>(x_values, y_zeros, data_values);
            
        // Compare results (1D vs first row of 2D)
        compare_1d_2d(img_vals_1d, img_vals_2d);
        compare_1d_2d(img_wgts_1d, img_wgts_2d);
    }
    
    SECTION("Compare weighted_mean interpolation")
    {
        auto [img_vals_1d, img_wgts_1d] = 
            gridder1d.interpolate_weighted_mean<xt::xtensor<double, 1>>(x_values, data_values);
            
        auto [img_vals_2d, img_wgts_2d] = 
            gridder2d.interpolate_weighted_mean<xt::xtensor<double, 2>>(x_values, y_zeros, data_values);
            
        // Compare results (1D vs first row of 2D)
        compare_1d_2d(img_vals_1d, img_vals_2d);
        compare_1d_2d(img_wgts_1d, img_wgts_2d);
    }
}

TEST_CASE("Test ForwardGridder2D vs ForwardGridder3D with z=0", TESTTAG)
{
    // Test data
    std::vector<double> x_values{ 0.7, 1.2, 2.5, 3.3, 4.8 };
    std::vector<double> y_values{ 0.5, 1.5, 2.5, 3.5, 4.5 };
    std::vector<double> z_zeros(x_values.size(), 0.0);
    std::vector<double> data_values{ 10.0, 20.0, 15.0, 25.0, 30.0 };
    
    // Create equivalent gridders
    auto gridder2d = ForwardGridder2D<double>::from_data(1.0, x_values, y_values);
    auto gridder3d = ForwardGridder3D<double>::from_data(1.0, x_values, y_values, z_zeros);
    
    SECTION("Check grid parameters match")
    {
        CHECK(gridder2d.get_nx() == gridder3d.get_nx());
        CHECK(gridder2d.get_ny() == gridder3d.get_ny());
        CHECK(gridder2d.get_xmin() == gridder3d.get_xmin());
        CHECK(gridder2d.get_xmax() == gridder3d.get_xmax());
        CHECK(gridder2d.get_ymin() == gridder3d.get_ymin());
        CHECK(gridder2d.get_ymax() == gridder3d.get_ymax());
        CHECK(gridder2d.get_xres() == gridder3d.get_xres());
        CHECK(gridder2d.get_yres() == gridder3d.get_yres());
        
        auto ext2d_x = gridder2d.get_extent_x();
        auto ext3d_x = gridder3d.get_extent_x();
        CHECK(ext2d_x[0] == ext3d_x[0]);
        CHECK(ext2d_x[1] == ext3d_x[1]);
        
        auto ext2d_y = gridder2d.get_extent_y();
        auto ext3d_y = gridder3d.get_extent_y();
        CHECK(ext2d_y[0] == ext3d_y[0]);
        CHECK(ext2d_y[1] == ext3d_y[1]);
    }
    
    SECTION("Compare block_mean interpolation")
    {
        auto [img_vals_2d, img_wgts_2d] = 
            gridder2d.interpolate_block_mean<xt::xtensor<double, 2>>(x_values, y_values, data_values);
            
        auto [img_vals_3d, img_wgts_3d] = 
            gridder3d.interpolate_block_mean<xt::xtensor<double, 3>>(x_values, y_values, z_zeros, data_values);
            
        // Compare results (2D vs first z-slice of 3D)
        compare_2d_3d(img_vals_2d, img_vals_3d);
        compare_2d_3d(img_wgts_2d, img_wgts_3d);
    }
    
    SECTION("Compare weighted_mean interpolation")
    {
        auto [img_vals_2d, img_wgts_2d] = 
            gridder2d.interpolate_weighted_mean<xt::xtensor<double, 2>>(x_values, y_values, data_values);
            
        auto [img_vals_3d, img_wgts_3d] = 
            gridder3d.interpolate_weighted_mean<xt::xtensor<double, 3>>(x_values, y_values, z_zeros, data_values);
            
        // Compare results (2D vs first z-slice of 3D)
        compare_2d_3d(img_vals_2d, img_vals_3d);
        compare_2d_3d(img_wgts_2d, img_wgts_3d);
    }
}

TEST_CASE("Test ForwardGridder1D vs ForwardGridder3D with y=0, z=0", TESTTAG)
{
    // Test data
    std::vector<double> x_values{ 0.7, 1.2, 2.5, 3.3, 4.8 };
    std::vector<double> y_zeros(x_values.size(), 0.0);
    std::vector<double> z_zeros(x_values.size(), 0.0);
    std::vector<double> data_values{ 10.0, 20.0, 15.0, 25.0, 30.0 };
    
    // Create equivalent gridders
    auto gridder1d = ForwardGridder1D<double>::from_data(1.0, x_values);
    auto gridder3d = ForwardGridder3D<double>::from_data(1.0, x_values, y_zeros, z_zeros);
    
    SECTION("Check grid parameters match")
    {
        CHECK(gridder1d.get_nx() == gridder3d.get_nx());
        CHECK(gridder1d.get_xmin() == gridder3d.get_xmin());
        CHECK(gridder1d.get_xmax() == gridder3d.get_xmax());
        CHECK(gridder1d.get_xres() == gridder3d.get_xres());
        
        auto ext1d = gridder1d.get_extent_x();
        auto ext3d = gridder3d.get_extent_x();
        CHECK(ext1d[0] == ext3d[0]);
        CHECK(ext1d[1] == ext3d[1]);
    }
    
    SECTION("Compare block_mean interpolation")
    {
        auto [img_vals_1d, img_wgts_1d] = 
            gridder1d.interpolate_block_mean<xt::xtensor<double, 1>>(x_values, data_values);
            
        auto [img_vals_3d, img_wgts_3d] = 
            gridder3d.interpolate_block_mean<xt::xtensor<double, 3>>(x_values, y_zeros, z_zeros, data_values);
            
        // Compare results (1D vs specific slice of 3D)
        compare_1d_3d(img_vals_1d, img_vals_3d);
        compare_1d_3d(img_wgts_1d, img_wgts_3d);
    }
    
    SECTION("Compare weighted_mean interpolation")
    {
        auto [img_vals_1d, img_wgts_1d] = 
            gridder1d.interpolate_weighted_mean<xt::xtensor<double, 1>>(x_values, data_values);
            
        auto [img_vals_3d, img_wgts_3d] = 
            gridder3d.interpolate_weighted_mean<xt::xtensor<double, 3>>(x_values, y_zeros, z_zeros, data_values);
            
        // Compare results (1D vs specific slice of 3D)
        compare_1d_3d(img_vals_1d, img_vals_3d);
        compare_1d_3d(img_wgts_1d, img_wgts_3d);
    }
}

TEST_CASE("Test ForwardGridder performance consistency", TESTTAG)
{
    // Generate larger test data to verify equivalence with random data
    size_t n = 1000;
    xt::xtensor<double, 1> x_values = xt::random::rand<double>({n}, 0.0, 5.0);
    xt::xtensor<double, 1> y_values = xt::random::rand<double>({n}, 0.0, 5.0);
    xt::xtensor<double, 1> z_values = xt::random::rand<double>({n}, 0.0, 5.0);
    xt::xtensor<double, 1> data_values = xt::random::rand<double>({n}, 1.0, 100.0);
    
    auto z_zeros = xt::zeros_like(x_values);
    
    // Create gridders
    auto gridder2d = ForwardGridder2D<double>::from_res(0.5, 0.0, 5.0, 0.0, 5.0);
    auto gridder3d_zero_z = ForwardGridder3D<double>::from_res(0.5, 0.0, 5.0, 0.0, 5.0, 0.0, 0.0);
    
    SECTION("2D and 3D with z=0 should produce equivalent results")
    {
        auto [img_vals_2d, img_wgts_2d] = 
            gridder2d.interpolate_block_mean<xt::xtensor<double, 2>>(
                x_values, y_values, data_values);
            
        auto [img_vals_3d, img_wgts_3d] = 
            gridder3d_zero_z.interpolate_block_mean<xt::xtensor<double, 3>>(
                x_values, y_values, z_zeros, data_values);
        
        // Check that 2D results match the z=0 slice of 3D results
        compare_2d_3d(img_vals_2d, img_vals_3d);
        compare_2d_3d(img_wgts_2d, img_wgts_3d);
    }
}
