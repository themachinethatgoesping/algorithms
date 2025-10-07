// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions/backwardmapping.hpp>

using namespace themachinethatgoesping::algorithms::imageprocessing::functions;

#define TESTTAG "[imageprocessing][backwardmapping]"

TEST_CASE("backward_map_nearest chooses closest samples", TESTTAG)
{
    xt::xtensor<double, 1> reference_x = { 0.0, 1.0, 2.0 };
    xt::xtensor<double, 1> reference_y = { 10.0, 20.0, 30.0 };

    xt::xtensor<double, 2> reference = {
        { 0.0, 1.0, 2.0 },
        { 10.0, 11.0, 12.0 },
        { 20.0, 21.0, 22.0 }
    };

    xt::xtensor<double, 1> new_x = { 0.2, 0.8, 1.9 };
    xt::xtensor<double, 1> new_y = { 12.0, 24.4, 28.0 };

    auto result = backward_map_nearest(reference, reference_x, reference_y, new_x, new_y);

    REQUIRE(result.shape()[0] == new_x.size());
    REQUIRE(result.shape()[1] == new_y.size());

    // Expected values use nearest indices in each axis
    xt::xtensor<double, 2> expected = {
        { reference(0, 0), reference(0, 1), reference(0, 2) },
        { reference(1, 0), reference(1, 1), reference(1, 2) },
        { reference(2, 0), reference(2, 1), reference(2, 2) }
    };

    for (size_t ix = 0; ix < new_x.size(); ++ix)
        for (size_t iy = 0; iy < new_y.size(); ++iy)
            REQUIRE(result(ix, iy) == expected(ix, iy));
}

TEST_CASE("backward_map_bilinear interpolates planar surface", TESTTAG)
{
    xt::xtensor<double, 1> reference_x = { 0.0, 1.0, 2.0 };
    xt::xtensor<double, 1> reference_y = { 0.0, 1.0, 2.0 };

    xt::xtensor<double, 2> reference = xt::zeros<double>({ reference_x.size(), reference_y.size() });

    for (size_t ix = 0; ix < reference_x.size(); ++ix)
        for (size_t iy = 0; iy < reference_y.size(); ++iy)
            reference(ix, iy) = reference_x(ix) + 2.0 * reference_y(iy);

    xt::xtensor<double, 1> new_x = { 0.0, 0.5, 1.5, 2.0 };
    xt::xtensor<double, 1> new_y = { 0.0, 0.25, 1.5, 2.0 };

    auto result = backward_map_bilinear(reference, reference_x, reference_y, new_x, new_y);

    REQUIRE(result.shape()[0] == new_x.size());
    REQUIRE(result.shape()[1] == new_y.size());

    for (size_t ix = 0; ix < new_x.size(); ++ix)
        for (size_t iy = 0; iy < new_y.size(); ++iy)
            REQUIRE(result(ix, iy) == Catch::Approx(new_x(ix) + 2.0 * new_y(iy)).margin(1e-12));
}

TEST_CASE("backward_map_nearest_add accumulates in target", TESTTAG)
{
    xt::xtensor<double, 1> reference_x = { 0.0, 1.0, 2.0 };
    xt::xtensor<double, 1> reference_y = { 10.0, 20.0, 30.0 };

    xt::xtensor<double, 2> reference = {
        { 0.0, 1.0, 2.0 },
        { 10.0, 11.0, 12.0 },
        { 20.0, 21.0, 22.0 }
    };

    xt::xtensor<double, 1> target_x = { 0.2, 0.8, 1.9 };
    xt::xtensor<double, 1> target_y = { 12.0, 24.4, 28.0 };

    xt::xtensor<double, 2> target = xt::ones<double>({ target_x.size(), target_y.size() });

    auto baseline = backward_map_nearest(reference, reference_x, reference_y, target_x, target_y);

    backward_map_nearest_add(reference, reference_x, reference_y, target, target_x, target_y);

    for (size_t ix = 0; ix < target_x.size(); ++ix)
        for (size_t iy = 0; iy < target_y.size(); ++iy)
            REQUIRE(target(ix, iy) == Catch::Approx(1.0 + baseline(ix, iy)).margin(1e-12));
}

TEST_CASE("backward_map_bilinear_add accumulates interpolated surface", TESTTAG)
{
    xt::xtensor<double, 1> reference_x = { 0.0, 1.0, 2.0 };
    xt::xtensor<double, 1> reference_y = { 0.0, 1.0, 2.0 };

    xt::xtensor<double, 2> reference = xt::zeros<double>({ reference_x.size(), reference_y.size() });

    for (size_t ix = 0; ix < reference_x.size(); ++ix)
        for (size_t iy = 0; iy < reference_y.size(); ++iy)
            reference(ix, iy) = reference_x(ix) + 3.0 * reference_y(iy);

    xt::xtensor<double, 1> target_x = { 0.0, 0.5, 1.5, 2.0 };
    xt::xtensor<double, 1> target_y = { 0.0, 0.25, 1.5, 2.0 };

    xt::xtensor<double, 2> target = xt::zeros<double>({ target_x.size(), target_y.size() });
    for (size_t ix = 0; ix < target_x.size(); ++ix)
        for (size_t iy = 0; iy < target_y.size(); ++iy)
            target(ix, iy) = static_cast<double>(ix * target_y.size() + iy);

    auto baseline = backward_map_bilinear(reference, reference_x, reference_y, target_x, target_y);

    backward_map_bilinear_add(reference, reference_x, reference_y, target, target_x, target_y);

    for (size_t ix = 0; ix < target_x.size(); ++ix)
        for (size_t iy = 0; iy < target_y.size(); ++iy)
        {
            const double expected = static_cast<double>(ix * target_y.size() + iy) + baseline(ix, iy);
            REQUIRE(target(ix, iy) == Catch::Approx(expected).margin(1e-12));
        }
}

TEST_CASE("backward_map throws when coordinate axes mismatch", TESTTAG)
{
    xt::xtensor<double, 2> reference = xt::zeros<double>({ 2, 2 });

    xt::xtensor<double, 1> reference_x = { 0.0, 1.0 };
    xt::xtensor<double, 1> reference_y = { 0.0, 1.0, 2.0 };

    xt::xtensor<double, 1> new_x = { 0.0 };
    xt::xtensor<double, 1> new_y = { 0.0 };

    REQUIRE_THROWS_AS(backward_map_nearest(reference, reference_x, reference_y, new_x, new_y), std::invalid_argument);
    REQUIRE_THROWS_AS(backward_map_bilinear(reference, reference_x, reference_y, new_x, new_y), std::invalid_argument);
    REQUIRE_THROWS_AS(
        backward_map_nearest_add(reference, reference_x, reference_y, reference, new_x, new_y), std::invalid_argument);
    REQUIRE_THROWS_AS(
        backward_map_bilinear_add(reference, reference_x, reference_y, reference, new_x, new_y), std::invalid_argument);

    xt::xtensor<double, 2> target = xt::zeros<double>({ 3, 3 });
    REQUIRE_THROWS_AS(
        backward_map_nearest_add(reference, reference_x, reference_y, target, new_x, new_y), std::invalid_argument);
    REQUIRE_THROWS_AS(
        backward_map_bilinear_add(reference, reference_x, reference_y, target, new_x, new_y), std::invalid_argument);
}
