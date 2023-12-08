// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ctime>

#include <xtensor/xrandom.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers/rtconstantsvp.hpp"

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers;
using namespace themachinethatgoesping::navigation::datastructures;

#define TESTTAG "[location]"

TEST_CASE("RTConstantSVP should support common functions", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 10;
    location.pitch = 20;
    location.roll  = 30;

    float c = 1450.f;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    // test copy
    {
        INFO(raytracer.info_string());
        INFO(RTConstantSVP(raytracer).info_string());
        REQUIRE(raytracer == RTConstantSVP(raytracer));
    }

    // test binary
    {
        auto raytracer2 = RTConstantSVP::from_binary(raytracer.to_binary());
        INFO(raytracer.info_string());
        INFO(raytracer2.info_string());
        REQUIRE(raytracer == raytracer2);
    }

    // test stream
    {
        std::stringstream buffer;
        raytracer.to_stream(buffer);
        auto raytracer2 = RTConstantSVP::from_stream(buffer);
        INFO(raytracer.info_string());
        INFO(raytracer2.info_string());
        REQUIRE(raytracer == raytracer2);
    }

    // test print does not crash
    REQUIRE(raytracer.info_string().size() != 0);

    // test get/set
    REQUIRE(raytracer.get_sensor_location() == location);

    auto ypr = raytracer.get_sensor_orientation_quat_ypr();

    // REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(location.yaw, 0.0001));
    REQUIRE_THAT(ypr[0], Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(ypr[1], Catch::Matchers::WithinAbs(location.pitch, 0.0001));
    REQUIRE_THAT(ypr[2], Catch::Matchers::WithinAbs(location.roll, 0.0001));

    REQUIRE_THAT(raytracer.get_sound_velocity(), Catch::Matchers::WithinAbs(c, 0.0001));

    // test hash
    REQUIRE(raytracer.binary_hash() == 16667922773826043259);
    REQUIRE(raytracer.binary_hash() == RTConstantSVP(raytracer).binary_hash());
    REQUIRE(raytracer.binary_hash() ==
            RTConstantSVP(raytracer.from_binary(raytracer.to_binary())).binary_hash());
}

TEST_CASE("RTConstantSVP reproduce some pre computed results (single points)", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c   = 1450.f;
    float c_2 = c * 0.5;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    // test raytracing some single points
    auto target = raytracer.trace_point(0, 0, 0);
    REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(3.f, 0.0001));
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(-1, 0, 0);
    REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(3.f - c_2, 0.0001));
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(-c_2, 0.0001));
    REQUIRE_THAT(std::fabs(target.true_range),
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, 45, 0);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(2563.26221f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(0.f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(2566.26172f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, 0, 45);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(0.f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(-2563.26221f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(2566.26172f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, 45, 45);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(1812.5f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(-2563.26221f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(1815.49976f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, -45, -45);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(-1812.5f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(2563.26221f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(1815.49976f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, -45, 45);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(-1812.5f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(-2563.26221f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(1815.49976f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));

    target = raytracer.trace_point(5, 45, -45);
    REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(c_2 * 5.f, 0.0001));
    CHECK_THAT(target.x, Catch::Matchers::WithinAbs(1812.5f, 0.0001));
    CHECK_THAT(target.y, Catch::Matchers::WithinAbs(2563.26221f, 0.0001));
    CHECK_THAT(target.z, Catch::Matchers::WithinAbs(1815.49976f, 0.0001));
    REQUIRE_THAT(target.true_range,
                 Catch::Matchers::WithinAbs(std::sqrt(target.x * target.x + target.y * target.y +
                                                      (target.z - 3.f) * (target.z - 3.f)),
                                            0.001));
}

TEST_CASE("RTConstantSVP multi point computations should be equal to single point computations",
          TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c = 1450.f;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    xt::random::seed(0);
    // test raytracing some single points
    xt::xtensor<float, 1> times  = xt::random::rand<float>({ 1000 });
    xt::xtensor<float, 1> along  = xt::random::rand<float>({ 1000 });
    xt::xtensor<float, 1> across = xt::random::rand<float>({ 1000 });

    along  = along * 180.f - 90.f;
    across = across * 360.f - 180.f;

    auto targets = raytracer.trace_points(times, along, across);

    for (size_t i = 0; i < times.size(); i++)
    {
        auto target = raytracer.trace_point(times[i], along[i], across[i]);

        REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(targets.x[i], 0.0001));
        REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(targets.y[i], 0.0001));
        REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(targets.z[i], 0.0001));
        REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(targets.true_range[i], 0.0001));
    }
}

TEST_CASE("RTConstantSVP beam computations should be equal to single point computations", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c = 1450.f;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    xt::random::seed(0);
    // test raytracing some single points
    xt::xtensor<float, 1> sample_numbers       = xt::arange<unsigned int>(0, 100);
    xt::xtensor<float, 1> along                = xt::linspace<float>(-90, 90, 42);
    xt::xtensor<float, 1> across               = xt::linspace<float>(-90, 90, 42);
    float                 sampling_time        = 0.1;
    float                 sampling_time_offset = 0.05;

    // along  = along * 180.f - 90.f;
    // across = across * 360.f - 180.f;

    xt::xtensor<float, 1> times = sample_numbers * sampling_time + sampling_time_offset;

    for (unsigned int bn = 0; bn < along.size(); ++bn)
    {
        auto beam = raytracer.trace_beam(
            sample_numbers, sampling_time, sampling_time_offset, along[bn], across[bn]);

        for (size_t sn : sample_numbers)
        {
            auto target = raytracer.trace_point(times[sn], along[bn], across[bn]);

            INFO(fmt::format("bn/sn {}/{} along: {}, across: {}, time: {}",
                             bn,
                             sn,
                             along[bn],
                             across[bn],
                             times[sn]));
            INFO(fmt::format("beam {}", beam.info_string()));
            INFO(fmt::format("target {}", target.info_string()));

            REQUIRE_THAT(target.x, Catch::Matchers::WithinRel(beam.x[sn], 0.0001f));
            REQUIRE_THAT(target.y, Catch::Matchers::WithinRel(beam.y[sn], 0.0001f));
            REQUIRE_THAT(target.z, Catch::Matchers::WithinRel(beam.z[sn], 0.0001f));
            REQUIRE_THAT(target.true_range,
                         Catch::Matchers::WithinRel(beam.true_range[sn], 0.0001f));

            REQUIRE_THAT(target.x, Catch::Matchers::WithinAbs(beam.x[sn], 0.01f));
            REQUIRE_THAT(target.y, Catch::Matchers::WithinAbs(beam.y[sn], 0.01f));
            REQUIRE_THAT(target.z, Catch::Matchers::WithinAbs(beam.z[sn], 0.01f));
            REQUIRE_THAT(target.true_range, Catch::Matchers::WithinAbs(beam.true_range[sn], 0.01f));
        }
    }
}

TEST_CASE("RTConstantSVP swath computations should be equal to beam computations", TESTTAG)
{
    // initialize location
    auto location = GeoLocation();

    location.z     = 3;
    location.yaw   = 0;
    location.pitch = 0;
    location.roll  = 0;

    float c = 1450.f;

    // initialize raytracer
    auto raytracer = RTConstantSVP(location, c);

    xt::random::seed(0);

    const auto num_beams   = 13;
    const auto num_samples = 13;
    const auto num_along   = 13;
    // test raytracing some single points
    xt::xtensor<float, 1> sample_numbers_per_beam = xt::arange<unsigned int>(0, num_samples);
    xt::xtensor<float, 2> sample_numbers =
        xt::xtensor<float, 2>::from_shape({ num_beams, num_samples });
    xt::xtensor<float, 1> along                = xt::linspace<float>(-90, 90, num_along);
    xt::xtensor<float, 1> across               = xt::linspace<float>(-90, 90, num_beams);
    float                 sampling_time        = 0.1;
    float                 sampling_time_offset = 0.05;

    for (unsigned int bn = 0; bn < sample_numbers.shape()[0]; ++bn)
    {
        xt::view(sample_numbers, bn, xt::all()) = sample_numbers_per_beam;
    }

    xt::xtensor<float, 1> times = sample_numbers_per_beam * sampling_time + sampling_time_offset;

    for (unsigned int an = 0; an < along.size(); ++an)
    {
        auto along_angles = xt::empty<float>({ num_beams });
        along_angles.fill(along[an]);

        auto swath = raytracer.trace_swath(
            sample_numbers, sampling_time, sampling_time_offset, along_angles, across);

        for (unsigned int bn = 0; bn < across.size(); ++bn)
        {
            auto beam = raytracer.trace_beam(sample_numbers_per_beam,
                                             sampling_time,
                                             sampling_time_offset,
                                             along_angles[bn],
                                             across[bn]);

            for (unsigned int sn = 0; sn < sample_numbers_per_beam.size(); ++sn)
            {
                INFO(fmt::format(
                    "an/bn/sn {}/{}/{} along: {}, across: {}", an, bn, sn, along[an], across[bn]));
                INFO(fmt::format("beam {}", beam.info_string()));
                INFO(fmt::format("swath {}", swath.info_string()));

                REQUIRE_THAT(swath.x(bn, sn), Catch::Matchers::WithinRel(beam.x[sn], 0.0001f));
                REQUIRE_THAT(swath.y(bn, sn), Catch::Matchers::WithinRel(beam.y[sn], 0.0001f));
                REQUIRE_THAT(swath.z(bn, sn), Catch::Matchers::WithinRel(beam.z[sn], 0.0001f));
                REQUIRE_THAT(swath.true_range(bn, sn),
                             Catch::Matchers::WithinRel(beam.true_range[sn], 0.0001f));

                REQUIRE_THAT(swath.x(bn, sn), Catch::Matchers::WithinAbs(beam.x[sn], 0.01f));
                REQUIRE_THAT(swath.y(bn, sn), Catch::Matchers::WithinAbs(beam.y[sn], 0.01f));
                REQUIRE_THAT(swath.z(bn, sn), Catch::Matchers::WithinAbs(beam.z[sn], 0.01f));
                REQUIRE_THAT(swath.true_range(bn, sn),
                             Catch::Matchers::WithinAbs(beam.true_range[sn], 0.01f));
            }
        }
    }
}