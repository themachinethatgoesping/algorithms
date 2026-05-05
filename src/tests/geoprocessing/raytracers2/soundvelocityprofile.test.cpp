// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <sstream>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/soundvelocityprofile.hpp"

using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define TESTTAG "[svp][raytracers2]"

TEST_CASE("SoundVelocityProfile uniform construction works", TESTTAG)
{
    auto svp = SoundVelocityProfile::uniform(1500.f, 6000.f);
    REQUIRE(svp.get_n_layers() == 1);
    REQUIRE_THAT(svp.get_sound_speed(0.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(3000.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(6000.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    // iso flag should be true for uniform layer
    REQUIRE(svp.get_iso().unchecked(0) == true);
}

TEST_CASE("SoundVelocityProfile linear gradient layers", TESTTAG)
{
    xt::xtensor<float, 1> z = { 0.f, 100.f, 1000.f, 5000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1480.f, 1490.f, 1540.f };
    SoundVelocityProfile svp(z, c);
    REQUIRE(svp.get_n_layers() == 3);

    // exact at knots
    REQUIRE_THAT(svp.get_sound_speed(0.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(100.f), Catch::Matchers::WithinAbs(1480.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(1000.f), Catch::Matchers::WithinAbs(1490.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(5000.f), Catch::Matchers::WithinAbs(1540.f, 1e-3f));

    // gradient checks
    REQUIRE_THAT(svp.get_g().unchecked(0), Catch::Matchers::WithinAbs(-0.2f, 1e-4f));
    REQUIRE_THAT(svp.get_g().unchecked(1), Catch::Matchers::WithinAbs(10.f / 900.f, 1e-4f));

    // mid-layer linear interpolation
    REQUIRE_THAT(svp.get_sound_speed(50.f), Catch::Matchers::WithinAbs(1490.f, 1e-3f));
}

TEST_CASE("SoundVelocityProfile binary/stream roundtrip", TESTTAG)
{
    xt::xtensor<float, 1> z = { 0.f, 200.f, 1500.f };
    xt::xtensor<float, 1> c = { 1500.f, 1495.f, 1530.f };
    SoundVelocityProfile svp(z, c);

    // copy
    REQUIRE(svp == SoundVelocityProfile(svp));

    // stream
    std::stringstream buf;
    svp.to_stream(buf);
    auto svp2 = SoundVelocityProfile::from_stream(buf);
    REQUIRE(svp == svp2);

    // info string non-empty
    REQUIRE(svp.info_string().size() != 0);
}

TEST_CASE("SoundVelocityProfile rejects bad inputs", TESTTAG)
{
    SoundVelocityProfile svp;
    REQUIRE_THROWS(svp.set(xt::xtensor<float, 1>{ 0.f }, xt::xtensor<float, 1>{ 1500.f }));
    REQUIRE_THROWS(svp.set(xt::xtensor<float, 1>{ 0.f, 0.f }, xt::xtensor<float, 1>{ 1500.f, 1500.f }));
    REQUIRE_THROWS(svp.set(xt::xtensor<float, 1>{ 0.f, 100.f }, xt::xtensor<float, 1>{ 1500.f }));
}
