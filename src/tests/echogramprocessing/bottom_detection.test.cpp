// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <string>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/generators/xbuilder.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/algorithms/echogramprocessing/functions/bottom_detection.hpp>

using namespace std;
using namespace themachinethatgoesping::algorithms::echogramprocessing::functions;

#define TESTTAG "[echogramprocessing]"

namespace {

constexpr int    N_SAMPLES     = 200;
constexpr int    N_PINGS       = 16;
constexpr int    BOTTOM_ONSET  = 120; // first sample of the bottom plateau
constexpr int    BOTTOM_WIDTH  = 25;  // wide enough to survive the majority filter
constexpr double RANGE_OFFSET  = 0.1; // m at sample 0 (avoid range 0 -> -inf)
constexpr double RANGE_RES     = 0.1; // m per sample
constexpr double PULSE_NS      = 3.0; // effective pulse length in samples
constexpr double BEAMWIDTH     = 7.0; // deg

// One ping: quiet background, a near-surface ringdown spike (inside the
// near-field guard so it must be ignored) and a strong wide bottom plateau.
xt::xtensor<double, 1> make_ping(bool with_bottom)
{
    xt::xtensor<double, 1> ping = xt::xtensor<double, 1>::from_shape({ N_SAMPLES });
    for (int s = 0; s < N_SAMPLES; ++s)
        ping(s) = -90.0; // quiet background (dB)

    for (int s = 2; s <= 5; ++s) // ringdown spike within 2*pulse guard
        ping(s) = -25.0;

    if (with_bottom)
        for (int s = BOTTOM_ONSET; s < BOTTOM_ONSET + BOTTOM_WIDTH; ++s)
            ping(s) = -28.0;
    return ping;
}

} // namespace

TEST_CASE("BottomDetector detects a clear bottom and ignores ringdown", TESTTAG)
{
    BottomDetector det;
    const auto     ping   = make_ping(true);
    const auto     result = det.find_bottom_in_ping(ping, RANGE_OFFSET, RANGE_RES, PULSE_NS, BEAMWIDTH);

    REQUIRE(result.valid);
    REQUIRE(result.bottom_sample > 2.0 * PULSE_NS);          // past near-field/ringdown
    REQUIRE(result.bottom_sample > BOTTOM_ONSET - 15.0);
    REQUIRE(result.bottom_sample < BOTTOM_ONSET + 15.0);
    REQUIRE(std::isfinite(result.bottom_backscatter_db));
    REQUIRE(result.bottom_backscatter_db >= det.thr_bottom);
}

TEST_CASE("BottomDetector returns invalid for a noise-only ping", TESTTAG)
{
    BottomDetector         det;
    xt::xtensor<double, 1> quiet = xt::xtensor<double, 1>::from_shape({ N_SAMPLES });
    for (int s = 0; s < N_SAMPLES; ++s)
        quiet(s) = -90.0;

    const auto result = det.find_bottom_in_ping(quiet, RANGE_OFFSET, RANGE_RES, PULSE_NS, BEAMWIDTH);
    REQUIRE_FALSE(result.valid);
    REQUIRE(std::isnan(result.bottom_sample));
}

TEST_CASE("BottomDetector batch detects the bottom for every ping", TESTTAG)
{
    BottomDetector det;

    xt::xtensor<double, 2> sv = xt::xtensor<double, 2>::from_shape({ N_PINGS, N_SAMPLES });
    for (int p = 0; p < N_PINGS; ++p)
    {
        const auto ping = make_ping(true);
        for (int s = 0; s < N_SAMPLES; ++s)
            sv(p, s) = ping(s);
    }

    xt::xtensor<double, 1> offsets    = RANGE_OFFSET * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> resolution = RANGE_RES * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> pulse      = PULSE_NS * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> beamwidth  = BEAMWIDTH * xt::ones<double>({ N_PINGS });

    const auto [bottom, bs] = det.find_bottom(sv, offsets, resolution, pulse, beamwidth, 1);

    REQUIRE(static_cast<int>(bottom.size()) == N_PINGS);
    REQUIRE(static_cast<int>(bs.size()) == N_PINGS);
    for (int p = 0; p < N_PINGS; ++p)
    {
        INFO("ping " << p << " bottom=" << bottom(p));
        REQUIRE(std::isfinite(bottom(p)));
        REQUIRE(bottom(p) > BOTTOM_ONSET - 15.0);
        REQUIRE(bottom(p) < BOTTOM_ONSET + 15.0);
    }
}

TEST_CASE("BottomDetector batch leaves a noise-only ping as NaN", TESTTAG)
{
    BottomDetector det;
    det.n_ping_smoothing = 1; // isolate the noise ping (no cross-ping fill)

    xt::xtensor<double, 2> sv = xt::xtensor<double, 2>::from_shape({ N_PINGS, N_SAMPLES });
    for (int p = 0; p < N_PINGS; ++p)
    {
        const auto ping = make_ping(p != 5); // ping 5 is pure noise
        for (int s = 0; s < N_SAMPLES; ++s)
            sv(p, s) = (p != 5) ? ping(s) : -90.0;
    }

    xt::xtensor<double, 1> offsets    = RANGE_OFFSET * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> resolution = RANGE_RES * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> pulse      = PULSE_NS * xt::ones<double>({ N_PINGS });
    xt::xtensor<double, 1> beamwidth  = BEAMWIDTH * xt::ones<double>({ N_PINGS });

    const auto [bottom, bs] = det.find_bottom(sv, offsets, resolution, pulse, beamwidth, 1);

    REQUIRE(std::isnan(bottom(5)));
    REQUIRE(std::isfinite(bottom(0)));
}

TEST_CASE("BottomDetector equality and printing", TESTTAG)
{
    BottomDetector a;
    BottomDetector b;
    REQUIRE(a == b);

    b.thr_bottom = -30.0;
    REQUIRE_FALSE(a == b);

    const std::string info = a.__printer__(2, false).create_str();
    REQUIRE(info.find("BottomDetector") != std::string::npos);
}
