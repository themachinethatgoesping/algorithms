// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// Tests for the per-beam true-bistatic raytracer (raytracers2).
//
// The bistatic solver traces a transmit ray from the transmit array and a receive ray
// from the receive array through the layered profile and finds the seabed point where
// the two legs meet with a combined one-way time equal to the two-way travel time. In
// an iso-velocity profile the legs are straight, so a known seabed point can be turned
// into the two steering angles and the two-way travel time the sonar would measure, run
// through the solver and required to be recovered. When the transmit and receive poses
// are identical the bistatic trace must reproduce the ordinary monostatic trace_beam
// exactly (same per-layer angles and horizontal distances).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <array>
#include <cmath>
#include <sstream>

#include <Eigen/Geometry>

#include <xtensor/containers/xtensor.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamdirections.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamtrace.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/bistaticraytracer.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/tracebeam.hpp"

using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

#define TESTTAG "[bistaticraytracer][raytracers2]"

namespace {

constexpr double RTD = 180.0 / M_PI;

const std::array<double, 3> ZERO = { 0.0, 0.0, 0.0 };

// Synthesise a single-beam bistatic problem (flat orientation) from a known seabed point
// and array positions, seed the solver with the concentric beam direction and return the
// resulting trace.
BistaticBeamTrace solve_single(const Eigen::Vector3d&      transmit_position,
                               const Eigen::Vector3d&      receive_position,
                               const Eigen::Vector3d&      seabed_point,
                               const SoundVelocityProfile& svp,
                               float                       sound_speed)
{
    const Eigen::Vector3d transmit_axis(1.0, 0.0, 0.0);
    const Eigen::Vector3d receive_axis(0.0, 1.0, 0.0);

    const Eigen::Vector3d to_seabed_tx = (seabed_point - transmit_position).normalized();
    const Eigen::Vector3d to_seabed_rx = (seabed_point - receive_position).normalized();

    const double transmit_steering = std::asin(to_seabed_tx.dot(transmit_axis)) * RTD;
    // receive_projection = -sin(receive_steering) = u_rx . receive_axis
    const double receive_steering = std::asin(-to_seabed_rx.dot(receive_axis)) * RTD;

    const double two_way_travel_time =
        ((seabed_point - transmit_position).norm() + (seabed_point - receive_position).norm()) /
        double(sound_speed);

    // concentric beam direction (both arrays collapsed to a point) as the solver seed
    xt::xtensor<float, 2> transmit_attitude = { { 0.f, 0.f, 0.f } };
    xt::xtensor<float, 2> receive_attitude  = { { 0.f, 0.f, 0.f } };
    xt::xtensor<float, 1> transmit_steer    = { float(transmit_steering) };
    xt::xtensor<float, 1> receive_steer     = { float(receive_steering) };
    auto beam_directions =
        compute_beam_directions(ZERO, ZERO, transmit_attitude, receive_attitude, transmit_steer, receive_steer, 0.0);
    const std::array<float, 3> concentric_guess = beam_directions.get_beam_direction(0);

    const std::array<double, 3> transmit_xyz = {
        transmit_position.x(), transmit_position.y(), transmit_position.z()
    };
    const std::array<double, 3> receive_xyz = {
        receive_position.x(), receive_position.y(), receive_position.z()
    };

    return trace_bistatic_beam(ZERO,
                               ZERO,
                               transmit_steering,
                               transmit_xyz,
                               ZERO,
                               ZERO,
                               receive_steering,
                               receive_xyz,
                               two_way_travel_time,
                               svp,
                               concentric_guess,
                               40,
                               1e-4f);
}

// Require two beam traces to agree point-by-point (depths, horizontal offsets, incident
// angles). Used to prove the bistatic legs share the trace_beam math exactly.
void require_beamtrace_matches(const BeamTrace& actual, const BeamTrace& expected, float margin)
{
    const auto& actual_depths     = actual.get_depths_in_meters();
    const auto& expected_depths   = expected.get_depths_in_meters();
    const auto& actual_offsets    = actual.get_horizontal_offsets_in_meters();
    const auto& expected_offsets  = expected.get_horizontal_offsets_in_meters();
    const auto  actual_incidence  = actual.get_incident_angles_in_degrees();
    const auto  expected_incidence = expected.get_incident_angles_in_degrees();

    REQUIRE(actual_depths.size() == expected_depths.size());
    for (size_t index = 0; index < expected_depths.size(); ++index)
    {
        REQUIRE_THAT(actual_depths.unchecked(index),
                     Catch::Matchers::WithinAbs(expected_depths.unchecked(index), margin));
        REQUIRE_THAT(actual_offsets.unchecked(index),
                     Catch::Matchers::WithinAbs(expected_offsets.unchecked(index), margin));
        REQUIRE_THAT(actual_incidence.unchecked(index),
                     Catch::Matchers::WithinAbs(expected_incidence.unchecked(index), margin));
    }
}

} // namespace

TEST_CASE("trace_beam_to_depth is a straight ray in an iso-velocity profile", TESTTAG)
{
    const float sound_speed = 1500.f;
    auto        svp         = SoundVelocityProfile::uniform(sound_speed, 12000.f);

    const double zenith = 30.0 * M_PI / 180.0;
    auto         leg     = trace_beam_to_depth(svp, 0.0, zenith, 50.0);

    REQUIRE(leg.reached_target);
    REQUIRE_THAT(leg.horizontal_offset_in_meters,
                 Catch::Matchers::WithinAbs(50.0 * std::tan(zenith), 1e-3)); // 50*tan30
    REQUIRE_THAT(leg.path_length_in_meters,
                 Catch::Matchers::WithinAbs(50.0 / std::cos(zenith), 1e-3)); // 50/cos30
    REQUIRE_THAT(leg.one_way_travel_time_in_seconds,
                 Catch::Matchers::WithinAbs((50.0 / std::cos(zenith)) / sound_speed, 1e-6));
    REQUIRE_THAT(leg.cos_angle_at_target, Catch::Matchers::WithinAbs(std::cos(zenith), 1e-5));
}

TEST_CASE("trace_bistatic_beam recovers the seabed point (iso-velocity, separated arrays)", TESTTAG)
{
    const float sound_speed = 1500.f;
    auto        svp         = SoundVelocityProfile::uniform(sound_speed, 12000.f);

    // separated arrays: RX 2 m forward and 0.3 m deeper than TX (ship motion + vertical offset)
    const Eigen::Vector3d transmit_position(0.0, 0.0, 0.0);
    const Eigen::Vector3d receive_position(2.0, 0.0, 0.3);

    for (const Eigen::Vector3d& seabed :
         { Eigen::Vector3d(5.0, 30.0, 50.0),
           Eigen::Vector3d(-3.0, -45.0, 60.0),
           Eigen::Vector3d(1.0, 0.0, 40.0) })
    {
        auto trace = solve_single(transmit_position, receive_position, seabed, svp, sound_speed);

        REQUIRE(trace.get_solver_residual_in_meters() < 1e-2f);
        const auto& bottom = trace.get_bottom_position();
        REQUIRE_THAT(bottom[0], Catch::Matchers::WithinAbs(float(seabed.x()), 2e-2f));
        REQUIRE_THAT(bottom[1], Catch::Matchers::WithinAbs(float(seabed.y()), 2e-2f));
        REQUIRE_THAT(bottom[2], Catch::Matchers::WithinAbs(float(seabed.z()), 2e-2f));
    }
}

TEST_CASE("trace_bistatic_beam degenerates to the monostatic case (coincident arrays)", TESTTAG)
{
    const float sound_speed = 1500.f;
    auto        svp         = SoundVelocityProfile::uniform(sound_speed, 12000.f);

    const Eigen::Vector3d common_position(0.0, 0.0, 0.0);

    for (const Eigen::Vector3d& seabed :
         { Eigen::Vector3d(0.0, 25.0, 45.0), Eigen::Vector3d(0.0, -50.0, 70.0) })
    {
        auto trace = solve_single(common_position, common_position, seabed, svp, sound_speed);

        REQUIRE(trace.get_solver_residual_in_meters() < 1e-2f);
        const auto& bottom = trace.get_bottom_position();
        REQUIRE_THAT(bottom[0], Catch::Matchers::WithinAbs(float(seabed.x()), 2e-2f));
        REQUIRE_THAT(bottom[1], Catch::Matchers::WithinAbs(float(seabed.y()), 2e-2f));
        REQUIRE_THAT(bottom[2], Catch::Matchers::WithinAbs(float(seabed.z()), 2e-2f));
    }
}

TEST_CASE("trace_bistatic_beam matches monostatic trace_beam for identical poses", TESTTAG)
{
    // A layered profile so each leg records several per-layer points (launch + crossings + end).
    xt::xtensor<float, 1> depths = { 0.f, 20.f, 60.f, 400.f };
    xt::xtensor<float, 1> speeds = { 1500.f, 1480.f, 1495.f, 1525.f };
    SoundVelocityProfile  svp(depths, speeds);

    const std::array<double, 3> position = { 0.0, 0.0, 0.0 };
    const float                 two_way_travel_time = 0.2f;

    struct Steering
    {
        double transmit;
        double receive;
    };

    // identical transmit/receive positions and orientations -> the bistatic solution must be
    // the concentric one, so both legs equal the monostatic trace exactly.
    for (const Steering& steering :
         { Steering{ 0.0, 25.0 }, Steering{ 0.0, -40.0 }, Steering{ 6.0, 35.0 }, Steering{ -4.0, -15.0 } })
    {
        // concentric beam direction and its pointing/azimuth decomposition
        xt::xtensor<float, 2> transmit_attitude = { { 0.f, 0.f, 0.f } };
        xt::xtensor<float, 2> receive_attitude  = { { 0.f, 0.f, 0.f } };
        xt::xtensor<float, 1> transmit_steer    = { float(steering.transmit) };
        xt::xtensor<float, 1> receive_steer     = { float(steering.receive) };
        auto                  beam_directions   = compute_beam_directions(
            ZERO, ZERO, transmit_attitude, receive_attitude, transmit_steer, receive_steer, 0.0);
        const std::array<float, 3> direction = beam_directions.get_beam_direction(0);
        const std::array<float, 2> pointing_azimuth =
            beam_direction_to_pointing_and_azimuth_in_degrees(direction[0], direction[1], direction[2]);
        const float pointing = pointing_azimuth[0];
        const float azimuth  = pointing_azimuth[1];

        // monostatic reference beam
        BeamTrace monostatic = trace_beam(0.f, pointing, svp, two_way_travel_time);

        // bistatic trace with identical transmit and receive poses
        auto bistatic = trace_bistatic_beam(ZERO,
                                            ZERO,
                                            steering.transmit,
                                            position,
                                            ZERO,
                                            ZERO,
                                            steering.receive,
                                            position,
                                            double(two_way_travel_time),
                                            svp,
                                            direction,
                                            40,
                                            1e-4f);

        // both legs must reproduce the monostatic per-layer angles and horizontal distances
        require_beamtrace_matches(bistatic.get_transmit_leg(), monostatic, 2e-2f);
        require_beamtrace_matches(bistatic.get_receive_leg(), monostatic, 2e-2f);

        // launch angle equals the concentric pointing angle
        REQUIRE_THAT(bistatic.get_transmit_launch_angle_in_degrees(),
                     Catch::Matchers::WithinAbs(pointing, 1e-2f));
        REQUIRE_THAT(bistatic.get_receive_launch_angle_in_degrees(),
                     Catch::Matchers::WithinAbs(pointing, 1e-2f));

        // seabed point equals the monostatic reconstruction (identical lift formula)
        const auto& monostatic_offsets = monostatic.get_horizontal_offsets_in_meters();
        const auto& monostatic_depths  = monostatic.get_depths_in_meters();
        const float last_offset        = monostatic_offsets.unchecked(monostatic_offsets.size() - 1);
        const float last_depth         = monostatic_depths.unchecked(monostatic_depths.size() - 1);
        const float azimuth_radians    = azimuth * float(M_PI / 180.0);
        const float expected_x         = -last_offset * std::sin(azimuth_radians);
        const float expected_y         = last_offset * std::cos(azimuth_radians);

        const auto& bottom = bistatic.get_bottom_position();
        REQUIRE_THAT(bottom[0], Catch::Matchers::WithinAbs(expected_x, 2e-2f));
        REQUIRE_THAT(bottom[1], Catch::Matchers::WithinAbs(expected_y, 2e-2f));
        REQUIRE_THAT(bottom[2], Catch::Matchers::WithinAbs(last_depth, 2e-2f));
    }
}

TEST_CASE("BistaticBeamTrace stream + printing roundtrip", TESTTAG)
{
    auto      svp          = SoundVelocityProfile::uniform(1500.f, 12000.f);
    BeamTrace transmit_leg = trace_beam(0.f, 20.f, svp, 0.06f);
    BeamTrace receive_leg  = trace_beam(0.f, -15.f, svp, 0.06f);

    std::array<float, 3> bottom_position = { 1.f, 40.f, 45.f };
    BistaticBeamTrace    trace(transmit_leg, receive_leg, 5.f, -3.f, bottom_position, 1e-4f);

    std::stringstream buffer;
    trace.to_stream(buffer);
    auto restored = BistaticBeamTrace::from_stream(buffer);

    REQUIRE(trace == restored);
    REQUIRE(trace.info_string().size() != 0);
}
