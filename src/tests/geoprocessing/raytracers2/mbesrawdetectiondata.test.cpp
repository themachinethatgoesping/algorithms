// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/mbesrawdetectiondata.hpp"

using namespace std;
using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;
namespace pingtools = themachinethatgoesping::algorithms::pingtools;
using themachinethatgoesping::navigation::datastructures::SensorPose;

#define TESTTAG "[raytracers2]"

namespace {
MBESRawDetectionData make_example()
{
    MBESRawDetectionData data;
    data.set_ref_pingtime(1234567.5);
    data.set_ref_pose(SensorPose("ref", 0.1f, 0.2f, 0.3f, 10.f, 1.f, 2.f));
    data.set_surface_sound_speed(1500.f);

    data.set_sector_beam_numbers(xt::xtensor<size_t, 1>({ 0, 128 }));
    data.set_sector_tx_time_delays(xt::xtensor<float, 1>({ 0.f, 0.001f }));
    data.set_sector_tx_steering_angles(xt::xtensor<float, 1>({ -1.f, 1.f }));
    data.set_sector_tx_offsets(
        { SensorPose("tx0", 1.f, 2.f, 3.f, 0.f, 0.f, 0.f),
          SensorPose("tx1", 1.f, 2.f, 3.f, 0.f, 0.f, 0.f) });
    data.set_rx_offsets(SensorPose("rx", 4.f, 5.f, 6.f, 0.f, 0.f, 0.f));

    data.set_number_of_soundings(4);
    data.set_soundings_two_way_travel_times(xt::xtensor<float, 1>({ 0.1f, 0.2f, 0.3f, 0.4f }));
    data.set_soundings_rx_steering_angles(xt::xtensor<float, 1>({ -30.f, -10.f, 10.f, 30.f }));
    data.set_soundings_sector_numbers(xt::xtensor<size_t, 1>({ 0, 0, 1, 1 }));
    return data;
}
}

TEST_CASE("MBESRawDetectionData should store and return data", TESTTAG)
{
    auto data = make_example();

    SECTION("getters return stored values")
    {
        CHECK(data.get_ref_pingtime() == Catch::Approx(1234567.5));
        CHECK(data.get_surface_sound_speed() == Catch::Approx(1500.f));
        CHECK(data.get_number_of_sectors() == 2);
        CHECK(data.get_number_of_soundings() == 4);
        CHECK(data.get_soundings_two_way_travel_times().size() == 4);
    }

    SECTION("has_ reflects availability")
    {
        CHECK(data.has_ref_pingtime());
        CHECK(data.has_soundings_two_way_travel_times());

        MBESRawDetectionData empty;
        CHECK_FALSE(empty.has_ref_pingtime());
        CHECK_FALSE(empty.has_soundings_two_way_travel_times());
    }

    SECTION("derived getters")
    {
        // sector tx times = ref ping time + delays
        auto tx_times = data.get_sector_tx_times();
        CHECK(tx_times.size() == 2);
        CHECK(tx_times(0) == Catch::Approx(1234567.5));
        CHECK(tx_times(1) == Catch::Approx(1234567.501));

        // rx times = sector tx time + twtt
        auto rx_times = data.get_soundings_rx_times();
        CHECK(rx_times.size() == 4);
        CHECK(rx_times(0) == Catch::Approx(1234567.5 + 0.1));
        CHECK(rx_times(2) == Catch::Approx(1234567.501 + 0.3));

        // beam numbers per sector
        auto bnps = data.get_beam_numbers_per_tx_sector();
        REQUIRE(bnps.size() == 2);
        CHECK(bnps[0] == std::vector<size_t>{ 0, 1 });
        CHECK(bnps[1] == std::vector<size_t>{ 2, 3 });
    }

    SECTION("beam selection subsets")
    {
        pingtools::BeamSelection bs(std::vector<uint32_t>{ 1, 3 });
        auto                     twtt = data.get_soundings_two_way_travel_times(bs);
        REQUIRE(twtt.size() == 2);
        CHECK(twtt(0) == Catch::Approx(0.2f));
        CHECK(twtt(1) == Catch::Approx(0.4f));

        auto sectors = data.get_soundings_sector_numbers(bs);
        REQUIRE(sectors.size() == 2);
        CHECK(sectors(0) == 0);
        CHECK(sectors(1) == 1);
    }
}

TEST_CASE("MBESRawDetectionData should support copy and binary round-trip", TESTTAG)
{
    auto data = make_example();

    // copy / equality
    REQUIRE(data == MBESRawDetectionData(data));
    REQUIRE(MBESRawDetectionData() != data);

    // binary round-trip
    REQUIRE(data == MBESRawDetectionData(data.from_binary(data.to_binary())));

    // empty round-trip
    MBESRawDetectionData empty;
    REQUIRE(empty == MBESRawDetectionData(empty.from_binary(empty.to_binary())));
}

TEST_CASE("MBESRawDetectionData get_mbesrawdetectiondata / get_mbesrawdetectioninfos", TESTTAG)
{
    auto data = make_example();

    // downcast reference (no copy): calls still dispatch to the concrete class
    I_MBESRawDetectionInfos& infos = data.get_mbesrawdetectioninfos();
    CHECK(infos.get_number_of_soundings() == 4);

    // snapshot copy via the interface method
    MBESRawDetectionData snapshot = data.get_mbesrawdetectiondata();
    REQUIRE(snapshot == data);

    // incomplete data must throw a clear exception
    MBESRawDetectionData incomplete;
    incomplete.set_ref_pingtime(1.0);
    CHECK_THROWS(incomplete.get_mbesrawdetectiondata());
}
