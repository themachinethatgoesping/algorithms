//// filepath:
////home/ssd/src/themachinethatgoesping/themachinethatgoesping/subprojects/algorithms/src/tests/pointprocessing/bubblestreams/zspine.test.cpp
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

#include <themachinethatgoesping/algorithms/pointprocessing/bubblestreams/zspine.hpp>

using namespace std;
using namespace themachinethatgoesping::algorithms::pointprocessing::bubblestreams;

#define TESTTAG "[zspine]"

TEST_CASE("ZSpine from_point_cloud and interpolation", TESTTAG)
{
    // Synthetic input data
    // Z-values increase roughly linearly; x and y vary with them
    xt::xtensor<double, 1> x = { 0.0, 1.0, 1.0, 2.0, 3.0 };
    xt::xtensor<double, 1> y = { 2.0, 1.5, 2.0, 1.0, 0.0 };
    xt::xtensor<double, 1> z = { 5.0, 6.0, 7.0, 9.0, 10.0 };
    xt::xtensor<double, 1> w = { 1.0, 2.0, 1.0, 3.0, 1.0 }; // weights

    // Build the spine using ~2 segments
    // (this is just a small test set, so 2 quantiles)
    auto spine = ZSpine::from_point_cloud(x, y, z, w, 2);

    SECTION("Check internal spine points")
    {
        // Should produce a small “spine” with quantiles + weighted medians
        auto [xvals, yvals, zvals] = spine.get_spine_points();

        // We expect 2 or 3 spine points depending on data segmentation
        REQUIRE(zvals.size() >= 2);
        REQUIRE(xvals.size() == zvals.size());
        REQUIRE(yvals.size() == zvals.size());

        // Check the first and last points
        CHECK(xvals[0] == Catch::Approx(1.0).margin(0.1));
        CHECK(xvals[zvals.size() - 1] == Catch::Approx(2.0).margin(0.1));
        CHECK(yvals[0] == Catch::Approx(1.7).margin(0.1));
        CHECK(yvals[zvals.size() - 1] == Catch::Approx(1.0).margin(0.1));
        CHECK(zvals[0] == Catch::Approx(6.0).margin(0.1));
        CHECK(zvals[zvals.size() - 1] == Catch::Approx(9.0).margin(0.1));
    }

    SECTION("Check interpolation at specific Z values")
    {
        // Interpolate at a few new points
        xt::xtensor<double, 1> query_z   = { 6.0, 7.5, 9.0 };
        auto                   result_xy = spine.get_xy_vec(query_z);

        // result_xy has shape [3,2], i.e. [ (x,y), (x,y), (x,y) ]
        REQUIRE(result_xy.shape(0) == 3);
        REQUIRE(result_xy.shape(1) == 2);

        // Check the interpolated values
        CHECK(result_xy(0, 0) == Catch::Approx(1.0).margin(0.1));
        CHECK(result_xy(0, 1) == Catch::Approx(1.7).margin(0.1));
        CHECK(result_xy(1, 0) == Catch::Approx(1.5).margin(0.1));
        CHECK(result_xy(1, 1) == Catch::Approx(1.3).margin(0.1));
        CHECK(result_xy(2, 0) == Catch::Approx(2.0).margin(0.1));
        CHECK(result_xy(2, 1) == Catch::Approx(1.0).margin(0.1));
    }

    SECTION("Check spine sampling at uniform intervals")
    {
        // get_spine(5) => 5 evenly spaced z-values from min to max
        auto sampled_xyz = spine.get_spine(5);

        REQUIRE(sampled_xyz.shape(0) == 3);
        REQUIRE(sampled_xyz.shape(1) == 5);

        // Quick approximate check
        // The first row ~ x,y for z=5.0
        REQUIRE(sampled_xyz(0, 0) == Catch::Approx(1.0).margin(0.6));
        // The last row ~ x,y for z=10.0
        REQUIRE(sampled_xyz(0, 4) == Catch::Approx(2.0).margin(0.6));
    }

    SECTION("Check origin setting and resetting")
    {
        // Set an origin
        spine.set_origin(1.0, 1.0, 1.0);

        // Check the origin
        auto [x, y, z] = spine.get_origin().value();
        CHECK(x == Catch::Approx(1.0).margin(0.1));
        CHECK(y == Catch::Approx(1.0).margin(0.1));
        CHECK(z == Catch::Approx(1.0).margin(0.1));
        CHECK(spine.get_is_altitude() == false);

        // Reset the origin
        spine.reset_origin();
        CHECK_FALSE(spine.get_origin().has_value());
    }

    SECTION("CHECK origin estimation")
    {
        // Estimate origin from the spine
        spine.estimate_origin(12, 0.9);

        // Check the origin
        auto [x, y, z] = spine.get_origin().value();
        CHECK(x == Catch::Approx(2.9).margin(0.05));
        CHECK(y == Catch::Approx(0.4).margin(0.05));
        CHECK(z == Catch::Approx(12.0).margin(0.05));
        CHECK(spine.get_is_altitude() == false);
    }

    SECTION("Check basic i/o")
    {
        // test inequality
        REQUIRE(ZSpine() != spine);

        // test copy
        REQUIRE(spine == ZSpine(spine));

        // test binary
        REQUIRE(spine == ZSpine(spine.from_binary(spine.to_binary())));

        // test stream
        std::stringstream buffer;
        spine.to_stream(buffer);
        REQUIRE(spine == ZSpine(spine.from_stream(buffer)));

        // test print does not crash
        REQUIRE(spine.info_string().size() != 0);

        // test hash (should be stable if class is not changed)
        REQUIRE(spine.binary_hash() == 2126609994950921350ULL);
        REQUIRE(spine.binary_hash() == ZSpine(spine).binary_hash());
        REQUIRE(spine.binary_hash() == ZSpine(spine.from_binary(spine.to_binary())).binary_hash());
    }
}