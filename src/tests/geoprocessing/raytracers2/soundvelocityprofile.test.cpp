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
    REQUIRE(svp.get_number_of_layers() == 1);
    REQUIRE_THAT(svp.get_sound_speed(0.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(3000.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(6000.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    // iso flag should be true for uniform layer
    REQUIRE(svp.get_isovelocity_flags().unchecked(0) == true);
}

TEST_CASE("SoundVelocityProfile linear gradient layers", TESTTAG)
{
    xt::xtensor<float, 1> z = { 0.f, 100.f, 1000.f, 5000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1480.f, 1490.f, 1540.f };
    SoundVelocityProfile svp(z, c);
    REQUIRE(svp.get_number_of_layers() == 3);

    // exact at knots
    REQUIRE_THAT(svp.get_sound_speed(0.f), Catch::Matchers::WithinAbs(1500.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(100.f), Catch::Matchers::WithinAbs(1480.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(1000.f), Catch::Matchers::WithinAbs(1490.f, 1e-3f));
    REQUIRE_THAT(svp.get_sound_speed(5000.f), Catch::Matchers::WithinAbs(1540.f, 1e-3f));

    // gradient checks
    REQUIRE_THAT(svp.get_sound_speed_gradients_in_per_second().unchecked(0), Catch::Matchers::WithinAbs(-0.2f, 1e-4f));
    REQUIRE_THAT(svp.get_sound_speed_gradients_in_per_second().unchecked(1), Catch::Matchers::WithinAbs(10.f / 900.f, 1e-4f));

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

TEST_CASE("SoundVelocityProfile optional metadata roundtrip", TESTTAG)
{
    xt::xtensor<float, 1> z = { 0.f, 100.f, 1000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1480.f, 1500.f };
    SoundVelocityProfile  svp(z, c);

    // unset by default
    REQUIRE_FALSE(svp.has_timestamp());
    REQUIRE_FALSE(svp.has_location());
    REQUIRE_FALSE(svp.get_timestamp().has_value());
    REQUIRE_FALSE(svp.get_latitude().has_value());
    REQUIRE_FALSE(svp.get_longitude().has_value());

    svp.set_timestamp(1700000000.5);
    svp.set_location(50.5, 4.25);
    REQUIRE(svp.has_timestamp());
    REQUIRE(svp.has_location());

    // stream roundtrip preserves metadata
    std::stringstream buf;
    svp.to_stream(buf);
    auto svp2 = SoundVelocityProfile::from_stream(buf);
    REQUIRE(svp == svp2);
    REQUIRE(svp2.get_timestamp().value() == 1700000000.5);
    REQUIRE(svp2.get_latitude().value() == 50.5);
    REQUIRE(svp2.get_longitude().value() == 4.25);

    // clear individually
    svp2.set_timestamp(std::nullopt);
    REQUIRE_FALSE(svp2.has_timestamp());
}

TEST_CASE("SoundVelocityProfile hash_value ignores optional metadata", TESTTAG)
{
    // Two SVPs with identical depth/sound-speed tables but different
    // timestamps / locations must have the same boost::hash_value (so that
    // boost::flyweight deduplicates them) -- the SSP datagrams in real
    // Kongsberg files differ in timestamp on every cast.
    xt::xtensor<float, 1> z = { 0.f, 100.f, 1000.f };
    xt::xtensor<float, 1> c = { 1500.f, 1480.f, 1500.f };
    SoundVelocityProfile  a(z, c);
    SoundVelocityProfile  b(z, c);
    a.set_timestamp(1700000000.0);
    a.set_location(50.0, 4.0);
    b.set_timestamp(1800000000.0);
    b.set_location(60.0, 5.0);

    REQUIRE(hash_value(a) == hash_value(b));
    REQUIRE(a.hash_content_only() == b.hash_content_only());
    // but full binary_hash (which includes optional metadata) must differ
    REQUIRE(a.binary_hash() != b.binary_hash());

    // different content -> different hash
    xt::xtensor<float, 1> z2 = { 0.f, 100.f, 1000.f };
    xt::xtensor<float, 1> c2 = { 1500.f, 1481.f, 1500.f };
    SoundVelocityProfile  d(z2, c2);
    REQUIRE(hash_value(a) != hash_value(d));
}
