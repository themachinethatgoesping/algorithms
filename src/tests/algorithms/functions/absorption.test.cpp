// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../../themachinethatgoesping/algorithms/amplitudecorrection/functions/absorption.hpp"

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
TEST_CASE("Absorption functions should reproduce previously computed results", TESTTAG)
{
    using Catch::Approx;

    SECTION("its90_to_its68")
    {
        CHECK(its90_to_its68(0.0) == Approx(0.0));
        CHECK(its90_to_its68(0.0f) == Approx(0.0f));
        CHECK(its90_to_its68(1.0) == Approx(1.00024));
        CHECK(its90_to_its68(1.0f) == Approx(1.00024f));
        CHECK(its90_to_its68(10.0) == Approx(10.0024));
        CHECK(its90_to_its68(10.0f) == Approx(10.0024f));
        CHECK(its90_to_its68(-100.0) == Approx(-100.024));
        CHECK(its90_to_its68(-100.0f) == Approx(-100.024f));
    }

    SECTION("calc_sound_velocity")
    {
        CHECK(calc_sound_velocity(0.0, 0.0, 0.0) == Approx(1402.40099361970374048));
        CHECK(calc_sound_velocity(0.0, 0.0, 35.0) == Approx(1449.02536060550323782));
        CHECK(calc_sound_velocity(0.0, 10.0, 35.0) == Approx(1489.78894590650997998));
        CHECK(calc_sound_velocity(100.0, 10.0, 35.0) == Approx(1491.42729295043295679));
        CHECK(calc_sound_velocity(100.0, 10.0, 35.0, 45.0, 0.0) == Approx(1491.43205948529271154));
        CHECK(calc_sound_velocity(100.0, 10.0, 35.0, 45.0, 40.0) == Approx(1491.43029656977273589));
    }

    SECTION("calc_absorption_coefficient_db_m")
    {
        CHECK(calc_absorption_coefficient_db_m(1000.0, 0.0, 1500.0, 10.0, 35.0) == Approx(0.0000595565729641));
        CHECK(calc_absorption_coefficient_db_m(10000.0, 100.0, 1500.0, 3.0, 35.0) == Approx(0.00114356087217127));
        CHECK(calc_absorption_coefficient_db_m(100000.0, 0.0, 1400.0, 10.0, 35.0, 8.0) == Approx(0.03550226534746687));
        CHECK(calc_absorption_coefficient_db_m(1000000.0, 100.0, 1500.0, 4.0, 35.0, 8.0) == Approx(0.43118748483875202));
        CHECK(calc_absorption_coefficient_db_m(1000.0, 0.0, 1500.0, 10.0, 35.0, 12.0) == Approx(0.06725602852617596));
        CHECK(calc_absorption_coefficient_db_m(1000.0, 100.0, 1500.0, 7.0, 35.0, 12.0) == Approx(0.06766628550920389));
        CHECK(calc_absorption_coefficient_db_m(10000.0, 0.0, 1500.0, 10.0, 35.0, 12.0) == Approx(0.1500729200888182));
    }
}
