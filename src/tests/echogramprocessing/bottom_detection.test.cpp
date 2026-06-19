// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>
#include <string>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/generators/xbuilder.hpp>

#include <themachinethatgoesping/algorithms/echogramprocessing/bottomdetector.hpp>

using namespace std;
using namespace themachinethatgoesping::algorithms::echogramprocessing;

#define TESTTAG "[echogramprocessing]"

namespace {

constexpr int   N_SAMPLES    = 200;
constexpr int   N_PINGS      = 16;
constexpr int   BOTTOM_ONSET = 120;  // first sample of the bottom plateau
constexpr int   BOTTOM_WIDTH = 25;   // wide enough to survive the majority filter
constexpr float RANGE_OFFSET = 0.1f; // m at sample 0 (avoid range 0 -> -inf)
constexpr float RANGE_RES    = 0.1f; // m per sample
constexpr float PULSE_NS     = 3.0f; // effective pulse length in samples
constexpr float BEAMWIDTH    = 7.0f; // deg

// One ping: quiet background, a near-surface ringdown spike (inside the
// near-field guard so it must be ignored) and a strong wide bottom plateau.
xt::xtensor<float, 1> make_ping(bool with_bottom)
{
    xt::xtensor<float, 1> ping = xt::xtensor<float, 1>::from_shape({ N_SAMPLES });
    for (int s = 0; s < N_SAMPLES; ++s)
        ping(s) = -90.0f; // quiet background (dB)

    for (int s = 2; s <= 5; ++s) // ringdown spike within the 2*pulse guard
        ping(s) = -25.0f;

    if (with_bottom)
        for (int s = BOTTOM_ONSET; s < BOTTOM_ONSET + BOTTOM_WIDTH; ++s)
            ping(s) = -28.0f;
    return ping;
}

// Add `n_pings` identical bottom pings (optionally turning ping 5 into noise).
void add_block(BottomDetector& det, bool noise_ping_5 = false, int n_pings = N_PINGS)
{
    for (int p = 0; p < n_pings; ++p)
    {
        const bool with_bottom = !(noise_ping_5 && p == 5);
        det.add_ping(make_ping(with_bottom), RANGE_OFFSET, RANGE_RES, PULSE_NS, BEAMWIDTH);
    }
}

} // namespace

TEST_CASE("BottomDetector detects a clear bottom and ignores ringdown", TESTTAG)
{
    BottomDetector det;
    det.add_ping(make_ping(true), RANGE_OFFSET, RANGE_RES, PULSE_NS, BEAMWIDTH);

    REQUIRE(det.size() == 1u);

    const auto& bottom = det.get_bottom();
    const auto& bs     = det.get_bottom_backscatter();

    REQUIRE(bottom.size() == 1u);
    REQUIRE(std::isfinite(bottom(0)));
    REQUIRE(bottom(0) > 2.0f * PULSE_NS); // past near-field / ringdown
    REQUIRE(bottom(0) > BOTTOM_ONSET - 15.0f);
    REQUIRE(bottom(0) < BOTTOM_ONSET + 15.0f);
    REQUIRE(std::isfinite(bs(0)));
    REQUIRE(bs(0) >= det.thr_bottom);
}

TEST_CASE("BottomDetector returns NaN for a noise-only ping", TESTTAG)
{
    BottomDetector det;
    det.add_ping(make_ping(false), RANGE_OFFSET, RANGE_RES, PULSE_NS, BEAMWIDTH);

    const auto& bottom = det.get_bottom();
    const auto& bs     = det.get_bottom_backscatter();

    REQUIRE(bottom.size() == 1u);
    REQUIRE(std::isnan(bottom(0)));
    REQUIRE(std::isnan(bs(0)));
}

TEST_CASE("BottomDetector detects the bottom for every ping in a block", TESTTAG)
{
    BottomDetector det;
    add_block(det);

    const auto& bottom = det.get_bottom();
    const auto& bs     = det.get_bottom_backscatter();

    REQUIRE(static_cast<int>(bottom.size()) == N_PINGS);
    REQUIRE(static_cast<int>(bs.size()) == N_PINGS);
    for (int p = 0; p < N_PINGS; ++p)
    {
        INFO("ping " << p << " bottom=" << bottom(p));
        REQUIRE(std::isfinite(bottom(p)));
        REQUIRE(bottom(p) > BOTTOM_ONSET - 15.0f);
        REQUIRE(bottom(p) < BOTTOM_ONSET + 15.0f);
    }
}

TEST_CASE("BottomDetector leaves a noise-only ping as NaN (smoothing off)", TESTTAG)
{
    BottomDetector det;
    det.n_ping_smoothing = 1; // isolate the noise ping (no cross-ping fill)
    add_block(det, /*noise_ping_5=*/true);

    const auto& bottom = det.get_bottom();
    REQUIRE(std::isnan(bottom(5)));
    REQUIRE(std::isfinite(bottom(0)));
}

TEST_CASE("BottomDetector recompiles after more pings are added", TESTTAG)
{
    BottomDetector det;
    add_block(det, /*noise_ping_5=*/false, /*n_pings=*/8);
    REQUIRE(static_cast<int>(det.get_bottom().size()) == 8);

    add_block(det, /*noise_ping_5=*/false, /*n_pings=*/8);
    REQUIRE(static_cast<int>(det.get_bottom().size()) == 16);

    // reset clears all state
    det.reset();
    REQUIRE(det.size() == 0u);
    REQUIRE(det.get_bottom().size() == 0u);
    REQUIRE(det.get_bottom_backscatter().size() == 0u);
}

TEST_CASE("BottomDetector constructor parameters and printing", TESTTAG)
{
    using Catch::Approx;

    // default vs named-parameter constructor
    BottomDetector a;
    BottomDetector b(-30.0f, -35.0f, -1.0f, 1.0f, 0.0f, std::numeric_limits<float>::infinity(),
                     0.0f, true, 10.0f, 5, 0.7f, false, 7, 3.0f, false, 10);
    REQUIRE(a.thr_bottom == Approx(-35.0f));
    REQUIRE(b.thr_bottom == Approx(-30.0f));
    REQUIRE_FALSE(a == b);

    const std::string info = a.__printer__(2, false).create_str();
    REQUIRE(info.find("BottomDetector") != std::string::npos);
}
