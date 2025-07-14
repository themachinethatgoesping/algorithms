// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/amplitudecorrection/functions/wcicorrection.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::amplitudecorrection::functions;

#define TESTTAG "[location]"

template<themachinethatgoesping::tools::helper::c_xtensor t_xtensor_2d,
         themachinethatgoesping::tools::helper::c_xtensor t_xtensor_1d>
void check_wci([[maybe_unused]] std::string name,
               const t_xtensor_2d&          result,
               const t_xtensor_2d&          wci,
               const t_xtensor_1d&          per_beam_offset,
               const t_xtensor_1d&          per_sample_offset,
               std::optional<size_t>        min_beam_index = std::nullopt,
               std::optional<size_t>        max_beam_index = std::nullopt)
{
    using Catch::Approx;

    size_t nsamples = 20;
    size_t nbeams   = 10;

    // max_beam_nr and min_beam_nr are allowed to be > wci.shape(0) and will be limited to the
    // actual number of beams
    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

    REQUIRE(result.dimension() == 2);
    REQUIRE(wci.dimension() == 2);
    REQUIRE(per_beam_offset.dimension() == 1);
    REQUIRE(per_sample_offset.dimension() == 1);

    REQUIRE(result.shape(0) == nbeams);
    REQUIRE(result.shape(1) == nsamples);
    REQUIRE(wci.shape(0) == nbeams);
    REQUIRE(wci.shape(1) == nsamples);
    REQUIRE(per_beam_offset.shape(0) == nbeams);
    REQUIRE(per_sample_offset.shape(0) == nsamples);

    for (size_t bn = min_beam_index.value_or(0); bn <= max_beam_nr; ++bn)
    {
        for (size_t sn = 0; sn < nsamples; ++sn)
        {
            // std::string msg = fmt::format(
            //     "ERROR[{}]: per_beam_offset({}) = {}, per_sample_offset({}) = {}, wci({},{}) =
            //     {}", name, bn, per_beam_offset(bn), sn, per_sample_offset(sn), bn, sn, wci(bn,
            //     sn));

            // if (min_beam_index.has_value())
            //     msg += fmt::format(", min_beam_index = {}", min_beam_index.value());

            // if (max_beam_index.has_value())
            //     msg += fmt::format(", max_beam_index = {}", max_beam_index.value());

            // INFO(msg);

            REQUIRE(result(bn, sn) ==
                    Approx(wci(bn, sn) + per_beam_offset(bn) + per_sample_offset(sn)));
        }
    }

    // check that the rest of the array is not touched
    if (min_beam_index.has_value())
    {
        size_t min_beam_nr = min_beam_index.value_or(0);
        if (min_beam_nr >= wci.shape(0))
            min_beam_nr = wci.shape(0);

        for (size_t bn = 0; bn < min_beam_nr; ++bn)
        {
            for (size_t sn = 0; sn < nsamples; ++sn)
            {
                // INFO(
                //     fmt::format("ERROR[{}]: Value should be untouchted (1)! bn = {}/min {}, sn =
                //     {}"
                //                 "wci({},{}) = {}",
                //                 name,
                //                 bn,
                //                 min_beam_index.value(),
                //                 sn,
                //                 bn,
                //                 sn,
                //                 wci(bn, sn)));
                REQUIRE(result(bn, sn) == 1);
            }
        }
    }

    if (max_beam_index.has_value())
        for (size_t bn = max_beam_index.value() + 1; bn < nbeams; ++bn)
        {
            for (size_t sn = 0; sn < nsamples; ++sn)
            {
                // INFO(fmt::format(
                //     "ERROR[{}]: Value should be untouchted (1)! bn = {}/max {}, sn = {}, "
                //     "wci({},{}) = {}",
                //     name,
                //     bn,
                //     max_beam_index.value(),
                //     sn,
                //     bn,
                //     sn,
                //     wci(bn, sn)));
                REQUIRE(result(bn, sn) == 1);
            }
        }
}

/**
 * @brief These tests just make sure that the return values are consistent with previous versions
 * of the functions. The values are not checked for correctness.
 * In case the functions are changed and updated to produce 'better' results, the tests should be
 * updated accordingly. However, these changes should be documented and justified und users should
 * be informed about these changes as they will affect quantification results.
 *
 */
TEST_CASE("WCICorrection functions should reproduce previously computed results", TESTTAG)
{
    using Catch::Approx;

    SECTION("wrong tensor shapes should throw")
    {
        xt::xtensor<float, 2> wci                   = xt::eval(xt::ones<float>({ 10, 20 }));
        xt::xtensor<float, 1> per_beam_offset       = xt::eval(xt::linspace<float>(-5.5, 10.5, 10));
        xt::xtensor<float, 1> per_beam_offset_wrong = xt::eval(xt::linspace<float>(-5.5, 10.5, 11));
        xt::xtensor<float, 1> per_sample_offset     = xt::eval(xt::linspace<float>(-2.5, 35.2, 20));
        xt::xtensor<float, 1> per_sample_offset_wrong =
            xt::eval(xt::linspace<float>(-2.5, 35.2, 9));

        // wrong beam/sample sizes
        REQUIRE_THROWS_AS(
            apply_beam_sample_correction(wci, per_beam_offset_wrong, per_sample_offset),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            inplace_beam_sample_correction(wci, per_beam_offset_wrong, per_sample_offset),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            apply_beam_sample_correction(wci, per_beam_offset, per_sample_offset_wrong),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            inplace_beam_sample_correction(wci, per_beam_offset, per_sample_offset_wrong),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            apply_beam_sample_correction(wci, per_beam_offset_wrong, per_sample_offset_wrong),
            std::invalid_argument);
        REQUIRE_THROWS_AS(
            inplace_beam_sample_correction(wci, per_beam_offset_wrong, per_sample_offset_wrong),
            std::invalid_argument);

        // beam sample arrays switched
        REQUIRE_THROWS_AS(apply_beam_sample_correction(wci, per_sample_offset, per_beam_offset),
                          std::invalid_argument);
        REQUIRE_THROWS_AS(inplace_beam_sample_correction(wci, per_sample_offset, per_beam_offset),
                          std::invalid_argument);

        REQUIRE_THROWS_AS(apply_beam_correction(wci, per_sample_offset), std::invalid_argument);
        REQUIRE_THROWS_AS(inplace_beam_correction(wci, per_sample_offset), std::invalid_argument);
        REQUIRE_THROWS_AS(apply_sample_correction(wci, per_beam_offset), std::invalid_argument);
        REQUIRE_THROWS_AS(inplace_sample_correction(wci, per_beam_offset), std::invalid_argument);
    }

    SECTION(
        "apply_beam_sample_correction and inplace_beam_sample_correction (applied to full array)")
    {
        xt::xtensor<float, 2> wci                 = xt::eval(xt::ones<float>({ 10, 20 }));
        xt::xtensor<float, 1> per_beam_offset     = xt::eval(xt::linspace<float>(-5.5, 10.5, 10));
        auto                  per_beam_offset_0   = xt::zeros_like(per_beam_offset);
        xt::xtensor<float, 1> per_sample_offset   = xt::eval(xt::linspace<float>(-2.5, 35.2, 20));
        auto                  per_sample_offset_0 = xt::zeros_like(per_sample_offset);

        for (size_t mp_cores : { 1, 0, 4 })
        {
            // --- apply_beam_sample_correction ---
            xt::xtensor<float, 2> result =
                xt::eval(apply_beam_sample_correction(wci, per_beam_offset, per_sample_offset));
            check_wci("apply_beam_sample_correction" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset,
                      per_sample_offset);

            result = wci;
            inplace_beam_sample_correction(
                result, per_beam_offset, per_sample_offset, std::nullopt, std::nullopt, mp_cores);
            check_wci("inplace_beam_sample_correction" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset,
                      per_sample_offset);

            // --- apply_sample_correction ---
            result = xt::eval(
                apply_beam_sample_correction(wci, per_beam_offset_0, per_sample_offset, mp_cores));
            check_wci("apply_sample_correction 1" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset_0,
                      per_sample_offset);

            result = xt::eval(apply_sample_correction(wci, per_sample_offset, mp_cores));
            check_wci("apply_sample_correction 2" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset_0,
                      per_sample_offset);

            result = wci;
            inplace_sample_correction(
                result, per_sample_offset, std::nullopt, std::nullopt, mp_cores);
            check_wci("inplace_sample_correction" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset_0,
                      per_sample_offset);

            // --- apply_beam_correction ---
            result = xt::eval(
                apply_beam_sample_correction(wci, per_beam_offset, per_sample_offset_0, mp_cores));
            check_wci("apply_beam_correction 1" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset,
                      per_sample_offset_0);

            result = xt::eval(apply_beam_correction(wci, per_beam_offset, mp_cores));
            check_wci("apply_beam_correction 2" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset,
                      per_sample_offset_0);

            result = wci;
            inplace_beam_correction(result, per_beam_offset, std::nullopt, std::nullopt, mp_cores);
            check_wci("inplace_beam_correction" + fmt::format("({} cores)", mp_cores),
                      result,
                      wci,
                      per_beam_offset,
                      per_sample_offset_0);

            // --- apply system offset ---
            result = xt::eval(apply_system_offset(wci, 12, mp_cores));
            check_wci("apply_system_offset" + fmt::format("({} cores)", mp_cores),
                      result,
                      xt::eval(wci + 12),
                      per_beam_offset_0,
                      per_sample_offset_0);

            result = wci;
            inplace_system_offset(result, 12, std::nullopt, std::nullopt, mp_cores);
            check_wci("inplace_system_offset" + fmt::format("({} cores)", mp_cores),
                      result,
                      xt::eval(wci + 12),
                      per_beam_offset_0,
                      per_sample_offset_0);
        }
    }

    SECTION("inplace_beam_sample_correction (applied to part of the array)")
    {

        xt::xtensor<float, 2> wci                 = xt::eval(xt::ones<float>({ 10, 20 }));
        xt::xtensor<float, 1> per_beam_offset     = xt::eval(xt::linspace<float>(-5.5, 10.5, 10));
        auto                  per_beam_offset_0   = xt::zeros_like(per_beam_offset);
        xt::xtensor<float, 1> per_sample_offset   = xt::eval(xt::linspace<float>(-2.5, 35.2, 20));
        auto                  per_sample_offset_0 = xt::zeros_like(per_sample_offset);

        for (size_t mp_cores : { 1, 0, 4 })
            for (std::optional<size_t> min_bn :
                 std::vector<std::optional<size_t>>{ std::nullopt, 0, 7, 10, 999999 })
                for (std::optional<size_t> max_bn :
                     std::vector<std::optional<size_t>>{ std::nullopt, 0, 3, 10, 999999 })
                {

                    // --- inplace_beam_sample_correction ---
                    xt::xtensor<float, 2> result = wci;
                    inplace_beam_sample_correction(
                        result, per_beam_offset, per_sample_offset, min_bn, max_bn, mp_cores);
                    check_wci("PARTIAL inplace_beam_sample_correction" +
                                  fmt::format("({} cores)", mp_cores),
                              result,
                              wci,
                              per_beam_offset,
                              per_sample_offset,
                              min_bn,
                              max_bn);

                    // ---inplace_sample_correction---
                    result = wci;
                    inplace_sample_correction(result, per_sample_offset, min_bn, max_bn, mp_cores);
                    check_wci("PARTIAL inplace_sample_correction" +
                                  fmt::format("({} cores)", mp_cores),
                              result,
                              wci,
                              per_beam_offset_0,
                              per_sample_offset,
                              min_bn,
                              max_bn);

                    // ---apply_beam_correction---
                    result = wci;
                    inplace_beam_correction(result, per_beam_offset, min_bn, max_bn, mp_cores);
                    check_wci("PARTIAL inplace_beam_correction" +
                                  fmt::format("({} cores)", mp_cores),
                              result,
                              wci,
                              per_beam_offset,
                              per_sample_offset_0,
                              min_bn,
                              max_bn);

                    // --- apply system offset (inplace only) ---
                    result            = wci;
                    auto modified_wci = wci;
                    xt::view(modified_wci,
                             xt::range(min_bn.value_or(0),
                                       max_bn.value_or(modified_wci.shape(0) - 1) + 1),
                             xt::all()) += 12;
                    inplace_system_offset(result, 12, min_bn, max_bn, mp_cores);
                    check_wci("PARTIAL inplace_system_offset" + fmt::format("({} cores)", mp_cores),
                              result,
                              modified_wci,
                              per_beam_offset_0,
                              per_sample_offset_0);
                }
    }
}
