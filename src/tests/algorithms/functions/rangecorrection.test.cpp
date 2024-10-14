// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/algorithms/amplitudecorrection/functions/rangecorrection.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::amplitudecorrection::functions;

#define TESTTAG "[location]"

/**
 * @brief These tests just make sure that the return values are consistent with previous versions
 * of the functions. The values are not checked for correctness.
 * In case the functions are changed and updated to produce 'better' results, the tests should be
 * updated accordingly. However, these changes should be documented and justified und users should
 * be informed about these changes as they will affect quantification results.
 *
 */
TEST_CASE("Rangecorrection functions should reproduce previously computed results", TESTTAG)
{
    using Catch::Approx;

    SECTION("get_sample_numbers_plus_half")
    {
        auto sample_numbers = get_sample_numbers_plus_half<xt::xtensor<double, 1>>(0, 10, 1);
        REQUIRE(sample_numbers.size() == 11);
        CHECK(sample_numbers(0) == Approx(0.5));
        CHECK(sample_numbers(1) == Approx(1.5));
        CHECK(sample_numbers(2) == Approx(2.5));
        CHECK(sample_numbers(3) == Approx(3.5));
        CHECK(sample_numbers(4) == Approx(4.5));
        CHECK(sample_numbers(5) == Approx(5.5));
        CHECK(sample_numbers(6) == Approx(6.5));
        CHECK(sample_numbers(7) == Approx(7.5));
        CHECK(sample_numbers(8) == Approx(8.5));
        CHECK(sample_numbers(9) == Approx(9.5));
        CHECK(sample_numbers(10) == Approx(10.5));
    }

    SECTION("approximate_range_factor")
    {
        CHECK(approximate_range_factor(0.0001, 1500.0) == Approx(0.075));
        CHECK(approximate_range_factor(0.0001f, 1500.0f) == Approx(0.075f));
        CHECK(approximate_range_factor(0.0001, 1480.0) == Approx(0.074));
        CHECK(approximate_range_factor(0.0001f, 1480.0f) == Approx(0.074f));
        CHECK(approximate_range_factor(0.0001, 1600.0) == Approx(0.08));
        CHECK(approximate_range_factor(0.0001f, 1600.0f) == Approx(0.08f));
        CHECK(approximate_range_factor(0.0001, 1400.0) == Approx(0.07));
        CHECK(approximate_range_factor(0.0001f, 1400.0f) == Approx(0.07f));
    }

    SECTION("approximate_ranges_1")
    {
        auto ranges = approximate_ranges<xt::xtensor<double, 1>>(0.0001, 1450.0, 0, 10, 1);
        REQUIRE(ranges.size() == 11);
        CHECK(ranges(0) == Approx(0.03625));
        CHECK(ranges(1) == Approx(0.10875000000000001));
        CHECK(ranges(2) == Approx(0.18125000000000002));
        CHECK(ranges(3) == Approx(0.25375000000000003));
        CHECK(ranges(4) == Approx(0.32625000000000004));
        CHECK(ranges(5) == Approx(0.39875000000000005));
        CHECK(ranges(6) == Approx(0.47125000000000006));
        CHECK(ranges(7) == Approx(0.54375000000000007));
        CHECK(ranges(8) == Approx(0.61625000000000008));
        CHECK(ranges(9) == Approx(0.68875000000000008));
        CHECK(ranges(10) == Approx(0.76125000000000009));
    }

    SECTION("approximate_ranges_2")
    {
        auto ranges2 = approximate_ranges<xt::xtensor<float, 1>>(0.001, 1450.0, 100, 300, 4);
        auto ranges1 = get_sample_numbers_plus_half<xt::xtensor<float, 1>>(100, 300, 4) *
                       approximate_range_factor(0.001, 1450.0);

        REQUIRE(ranges1.size() == ranges2.size());
        for (size_t i = 0; i < ranges1.size(); ++i)
            CHECK(ranges2(i) == Approx(ranges1(i)));
    }

    SECTION("compute_cw_range_correction")
    {
        auto ranges = xt::eval(xt::linspace<float>(0.5, 10.5, 10));

        for (const std::optional<float>& absorption :
             std::vector<std::optional<float>>{ std::nullopt, 0.f, 0.0124f })
        {
            for (const std::optional<float>& tvg :
                 std::vector<std::optional<float>>{ std::nullopt, -13.f, 0.f, 14.f })
            {
                auto correction = compute_cw_range_correction(ranges, absorption, tvg);
                REQUIRE(correction.size() == 10);

                for (size_t i = 0; i < 10; ++i)
                {
                    INFO(fmt::format("i = {}, ranges(i) = {}", i, ranges(i)));
                    REQUIRE(correction(i) ==
                            Approx(ranges(i) * absorption.value_or(0) * 2 + tvg.value_or(0) * std::log10(ranges(i))));
                }
            }
        }
    }
}
