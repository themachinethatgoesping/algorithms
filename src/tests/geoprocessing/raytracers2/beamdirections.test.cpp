// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// Tests for compute_beam_directions / BeamDirections (raytracers2).
//
// The beam direction is the downward intersection of the transmit and receive
// steering cones (Mills cross): a unit vector d in the ping frame
// (x = forward, y = starboard, z = down) with
//     d . transmit_axis = sin(transmit_steering)    (transmit_axis = forward)
//     d . receive_axis  = -sin(receive_steering)    (receive_axis  = starboard)
// with receive steering positive to port. This is the exact physics and is
// verified directly below.
//
// It is also cross-checked against MB-System's mb_beaudoin (mb_angle.c). That
// routine shares the ping frame and the yaw/pitch/roll convention (R = Rz(yaw) *
// Ry(pitch) * Rx(roll)), so its inputs are fed directly with NO coordinate swap.
// mb_beaudoin is exact when the transmit steering is zero; for non-zero transmit
// steering its non-orthonormal basis construction is only a small-angle
// approximation of the exact cone intersection, so that regime is compared
// loosely.

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <sstream>
#include <vector>

#include <Eigen/Geometry>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/beamdirections.hpp"
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;
using themachinethatgoesping::tools::rotationfunctions::quaternion_from_ypr;

#define TESTTAG "[beamdirections][raytracers2]"

namespace {

constexpr double DTR = M_PI / 180.0;
constexpr double RTD = 180.0 / M_PI;

// ---------------------------------------------------------------------------
// Reference: MB-System mb_beaudoin (mb_angle.c), transcribed in the ping frame
// (forward, starboard, down). Inputs are ping (yaw, pitch, roll);
// mb_beaudoin_unrotate applies R = Rz(heading) Ry(pitch) Rx(roll), identical to
// quaternion_from_ypr, so the arrays are used directly with no coordinate swap.
// The returned geographic beam vector is normalized.
// ---------------------------------------------------------------------------
struct Vec3
{
    double forward, starboard, down;
};

Vec3 mb_unrotate(Vec3 o, double yaw, double pitch, double roll)
{
    const double sr = std::sin(roll * DTR), cr = std::cos(roll * DTR);
    const double sp = std::sin(pitch * DTR), cp = std::cos(pitch * DTR);
    const double sy = std::sin(yaw * DTR), cy = std::cos(yaw * DTR);
    Vec3         f;
    f.forward   = cp * cy * o.forward + (sr * sp * cy - cr * sy) * o.starboard +
                (cr * sp * cy + sr * sy) * o.down;
    f.starboard = cp * sy * o.forward + (sr * sp * sy + cr * cy) * o.starboard +
                  (cr * sp * sy - sr * cy) * o.down;
    f.down = -sp * o.forward + sr * cp * o.starboard + cr * cp * o.down;
    return f;
}

// mb_beaudoin geographic beam vector in the ping frame (normalized).
std::array<double, 3> mb_beaudoin_ping(const std::array<double, 3>& transmit_installation_ypr,
                                       const std::array<double, 3>& receive_installation_ypr,
                                       const std::array<double, 3>& transmit_attitude_ypr,
                                       const std::array<double, 3>& receive_attitude_ypr,
                                       double                       transmit_steering,
                                       double                       receive_steering)
{
    auto unrot = [](Vec3 v, const std::array<double, 3>& ypr) {
        return mb_unrotate(v, ypr[0], ypr[1], ypr[2]);
    };

    const Vec3 tx = unrot(unrot(Vec3{ 1, 0, 0 }, transmit_installation_ypr), transmit_attitude_ypr);
    const Vec3 rx = unrot(unrot(Vec3{ 0, 1, 0 }, receive_installation_ypr), receive_attitude_ypr);

    const double dot = tx.forward * rx.forward + tx.starboard * rx.starboard + tx.down * rx.down;
    const double non_ortho = std::acos(dot) * RTD - 90.0;

    const double sin_tx = std::sin(transmit_steering * DTR);
    const double y1     = std::sin(-receive_steering * DTR) / std::cos(non_ortho * DTR);
    const double y2     = sin_tx * std::tan(non_ortho * DTR);
    const double radial = std::sqrt((y1 + y2) * (y1 + y2) + sin_tx * sin_tx);

    Vec3 rel;
    if (radial <= 1.0)
        rel = Vec3{ sin_tx, y1 + y2, std::sqrt(1.0 - radial * radial) };
    else
        rel = Vec3{ sin_tx, std::sqrt(std::max(0.0, 1.0 - sin_tx * sin_tx)), 0.0 };

    const Vec3 xp = tx;
    const Vec3 zp{ tx.starboard * rx.down - tx.down * rx.starboard,
                   tx.down * rx.forward - tx.forward * rx.down,
                   tx.forward * rx.starboard - tx.starboard * rx.forward };
    const Vec3 yp{ zp.starboard * xp.down - zp.down * xp.starboard,
                   zp.down * xp.forward - zp.forward * xp.down,
                   zp.forward * xp.starboard - zp.starboard * xp.forward };

    std::array<double, 3> g{
        rel.forward * xp.forward + rel.starboard * yp.forward + rel.down * zp.forward,
        rel.forward * xp.starboard + rel.starboard * yp.starboard + rel.down * zp.starboard,
        rel.forward * xp.down + rel.starboard * yp.down + rel.down * zp.down
    };
    const double norm = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);
    return { g[0] / norm, g[1] / norm, g[2] / norm };
}

// World-frame array long axis used by compute_beam_directions (attitude * installation * local).
Eigen::Vector3d array_axis(const std::array<double, 3>& installation_ypr,
                           const std::array<double, 3>& attitude_ypr,
                           const Eigen::Vector3d&       local_axis)
{
    const Eigen::Quaterniond q =
        quaternion_from_ypr<double>(attitude_ypr[0], attitude_ypr[1], attitude_ypr[2], true) *
        quaternion_from_ypr<double>(
            installation_ypr[0], installation_ypr[1], installation_ypr[2], true);
    return q * local_axis;
}

// Single-beam wrapper around compute_beam_directions.
std::array<float, 3> one_beam(const std::array<double, 3>& transmit_installation_ypr,
                              const std::array<double, 3>& receive_installation_ypr,
                              const std::array<double, 3>& transmit_attitude_ypr,
                              const std::array<double, 3>& receive_attitude_ypr,
                              double                       transmit_steering,
                              double                       receive_steering,
                              double                       reference_heading = 0.0)
{
    xt::xtensor<float, 2> tx_att = { { float(transmit_attitude_ypr[0]),
                                       float(transmit_attitude_ypr[1]),
                                       float(transmit_attitude_ypr[2]) } };
    xt::xtensor<float, 2> rx_att = { { float(receive_attitude_ypr[0]),
                                       float(receive_attitude_ypr[1]),
                                       float(receive_attitude_ypr[2]) } };
    xt::xtensor<float, 1> tx_steer = { float(transmit_steering) };
    xt::xtensor<float, 1> rx_steer = { float(receive_steering) };

    auto bd = compute_beam_directions(transmit_installation_ypr,
                                      receive_installation_ypr,
                                      tx_att,
                                      rx_att,
                                      tx_steer,
                                      rx_steer,
                                      reference_heading);
    const auto& d = bd.get_directions();
    return { d(0, 0), d(0, 1), d(0, 2) };
}

} // namespace

TEST_CASE("compute_beam_directions points straight down for a flat, unsteered beam", TESTTAG)
{
    const auto d = one_beam({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 0.0, 0.0);
    REQUIRE_THAT(d[0], Catch::Matchers::WithinAbs(0.f, 1e-6f)); // forward
    REQUIRE_THAT(d[1], Catch::Matchers::WithinAbs(0.f, 1e-6f)); // starboard
    REQUIRE_THAT(d[2], Catch::Matchers::WithinAbs(1.f, 1e-6f)); // down

    xt::xtensor<float, 2> att   = { { 0.f, 0.f, 0.f } };
    xt::xtensor<float, 1> steer = { 0.f };
    auto bd = compute_beam_directions({ 0, 0, 0 }, { 0, 0, 0 }, att, att, steer, steer, 0.0);
    REQUIRE_THAT(bd.get_beam_pointing_angles_in_degrees()(0),
                 Catch::Matchers::WithinAbs(0.f, 1e-3f));
    REQUIRE_THAT(bd.get_beam_takeoff_angles_in_degrees()(0),
                 Catch::Matchers::WithinAbs(0.f, 1e-3f));
}

TEST_CASE("compute_beam_directions satisfies the transmit/receive steering cones exactly", TESTTAG)
{
    std::mt19937                           rng(2024);
    std::uniform_real_distribution<double> inst(-8, 8), att_yaw(-40, 40), att_rp(-20, 20),
        tx_tilt(-12, 12), rx_cross(-65, 65);

    int checked = 0;
    for (int trial = 0; trial < 800; ++trial)
    {
        const std::array<double, 3> tx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> rx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> tx_att  = { att_yaw(rng), att_rp(rng), att_rp(rng) };
        const std::array<double, 3> rx_att  = { att_yaw(rng), att_rp(rng), att_rp(rng) };
        const double                tx_steer = tx_tilt(rng);
        const double                rx_steer = rx_cross(rng);

        const auto      d3 = one_beam(tx_inst, rx_inst, tx_att, rx_att, tx_steer, rx_steer);
        Eigen::Vector3d d(d3[0], d3[1], d3[2]);
        if (d.z() < 0.1) // physical (downward) beams only
            continue;

        const Eigen::Vector3d u = array_axis(tx_inst, tx_att, Eigen::Vector3d(1, 0, 0));
        const Eigen::Vector3d v = array_axis(rx_inst, rx_att, Eigen::Vector3d(0, 1, 0));

        REQUIRE_THAT(d.norm(), Catch::Matchers::WithinAbs(1.0, 1e-5));
        REQUIRE_THAT(d.dot(u), Catch::Matchers::WithinAbs(std::sin(tx_steer * DTR), 1e-5));
        REQUIRE_THAT(d.dot(v), Catch::Matchers::WithinAbs(-std::sin(rx_steer * DTR), 1e-5));
        ++checked;
    }
    REQUIRE(checked > 200);
}

TEST_CASE("compute_beam_directions matches mb_beaudoin exactly for zero transmit steering", TESTTAG)
{
    std::mt19937                           rng(99);
    std::uniform_real_distribution<double> inst(-8, 8), att_yaw(-30, 30), att_rp(-15, 15),
        rx_cross(-65, 65);

    int compared = 0;
    for (int trial = 0; trial < 3000 && compared < 400; ++trial)
    {
        const std::array<double, 3> tx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> rx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> tx_att  = { att_yaw(rng), att_rp(rng), att_rp(rng) };
        const std::array<double, 3> rx_att  = { att_yaw(rng), att_rp(rng), att_rp(rng) };
        const double                rx_steer = rx_cross(rng);

        const auto ref = mb_beaudoin_ping(tx_inst, rx_inst, tx_att, rx_att, 0.0, rx_steer);
        const auto d   = one_beam(tx_inst, rx_inst, tx_att, rx_att, 0.0, rx_steer);
        // Compare only clearly-downward beams: near the horizon the downward-root
        // pick and mb's fixed rel.z >= 0 branch can select opposite hemispheres.
        if (ref[2] < 0.2 || d[2] < 0.2)
            continue;
        REQUIRE_THAT(d[0], Catch::Matchers::WithinAbs(float(ref[0]), 1e-4f));
        REQUIRE_THAT(d[1], Catch::Matchers::WithinAbs(float(ref[1]), 1e-4f));
        REQUIRE_THAT(d[2], Catch::Matchers::WithinAbs(float(ref[2]), 1e-4f));
        ++compared;
    }
    REQUIRE(compared > 100);
}

TEST_CASE("compute_beam_directions matches mb_beaudoin to its small-angle approximation", TESTTAG)
{
    // With non-zero transmit steering mb_beaudoin uses a non-orthonormal basis and
    // is only a small-angle approximation of the exact cone intersection; the two
    // therefore agree to O(sin(tx) * non_ortho^2). This documents that bound.
    std::mt19937                           rng(5);
    std::uniform_real_distribution<double> inst(-4, 4), att(-10, 10), tx_tilt(-10, 10),
        rx_cross(-55, 55);

    double worst    = 0.0;
    int    compared = 0;
    for (int trial = 0; trial < 4000 && compared < 500; ++trial)
    {
        const std::array<double, 3> tx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> rx_inst = { inst(rng), inst(rng), inst(rng) };
        const std::array<double, 3> tx_att  = { att(rng), att(rng), att(rng) };
        const std::array<double, 3> rx_att  = { att(rng), att(rng), att(rng) };
        const double                tx_steer = tx_tilt(rng);
        const double                rx_steer = rx_cross(rng);

        const auto ref = mb_beaudoin_ping(tx_inst, rx_inst, tx_att, rx_att, tx_steer, rx_steer);
        if (ref[2] < 0.2)
            continue;
        const auto d = one_beam(tx_inst, rx_inst, tx_att, rx_att, tx_steer, rx_steer);
        worst        = std::max({ worst,
                           std::abs(double(d[0]) - ref[0]),
                           std::abs(double(d[1]) - ref[1]),
                           std::abs(double(d[2]) - ref[2]) });
        ++compared;
    }
    REQUIRE(compared > 100);
    REQUIRE(worst < 6e-2);
}

TEST_CASE("compute_beam_directions handles a reverse-mounted receive array", TESTTAG)
{
    // A reverse-mounted RX array is only a ~180 deg installation yaw; the quaternion
    // rotation of the array axis carries the reversed steering sign through, so the
    // beam still satisfies the steering cones with no manual sign flips. (The verbatim
    // mb_beaudoin has no downward pick and returns an upward beam for a reverse mount,
    // so we verify the cone constraints directly instead of comparing to it.)
    const std::array<double, 3> tx_inst = { 0.0, 0.5, -0.3 };
    const std::array<double, 3> rx_inst = { 180.0, 1.2, 0.8 };

    std::mt19937                           rng(777);
    std::uniform_real_distribution<double> att_rp(-10, 10), rx_cross(-60, 60);

    int checked = 0;
    for (int trial = 0; trial < 600 && checked < 50; ++trial)
    {
        const std::array<double, 3> tx_att   = { 0.0, att_rp(rng), att_rp(rng) };
        const std::array<double, 3> rx_att   = { 0.0, att_rp(rng), att_rp(rng) };
        const double                rx_steer = rx_cross(rng);

        const auto      d3 = one_beam(tx_inst, rx_inst, tx_att, rx_att, 0.0, rx_steer);
        Eigen::Vector3d d(d3[0], d3[1], d3[2]);
        if (d.z() < 0.1) // downward beams only
            continue;

        const Eigen::Vector3d u = array_axis(tx_inst, tx_att, Eigen::Vector3d(1, 0, 0));
        const Eigen::Vector3d v = array_axis(rx_inst, rx_att, Eigen::Vector3d(0, 1, 0));

        REQUIRE_THAT(d.norm(), Catch::Matchers::WithinAbs(1.0, 1e-5));
        REQUIRE_THAT(d.dot(u), Catch::Matchers::WithinAbs(0.0, 1e-5)); // transmit steering = 0
        REQUIRE_THAT(d.dot(v), Catch::Matchers::WithinAbs(-std::sin(rx_steer * DTR), 1e-5));
        ++checked;
    }
    REQUIRE(checked > 10);
}

TEST_CASE("compute_beam_directions reference heading rotates the directions", TESTTAG)
{
    const std::array<double, 3> tx_inst = { 0.0, 0.0, 0.0 };
    const std::array<double, 3> rx_inst = { 0.0, 0.0, 0.0 };
    xt::xtensor<float, 2>       att      = { { 5.f, 2.f, -3.f }, { -10.f, 1.f, 4.f } };
    xt::xtensor<float, 1>       tx_steer = { 3.f, -2.f };
    xt::xtensor<float, 1>       rx_steer = { 40.f, -55.f };

    auto bd0 = compute_beam_directions(tx_inst, rx_inst, att, att, tx_steer, rx_steer, 0.0);

    const double delta = 37.0;
    auto bdd = compute_beam_directions(tx_inst, rx_inst, att, att, tx_steer, rx_steer, delta);

    const auto& d0 = bd0.get_directions();
    const auto& dd = bdd.get_directions();

    const double cD = std::cos(delta * DTR);
    const double sD = std::sin(delta * DTR);
    for (size_t i = 0; i < 2; ++i)
    {
        // Rz(-delta) applied to the heading-0 direction
        const float ex = float(cD * d0(i, 0) + sD * d0(i, 1));
        const float ey = float(-sD * d0(i, 0) + cD * d0(i, 1));
        REQUIRE_THAT(dd(i, 0), Catch::Matchers::WithinAbs(ex, 1e-5f));
        REQUIRE_THAT(dd(i, 1), Catch::Matchers::WithinAbs(ey, 1e-5f));
        REQUIRE_THAT(dd(i, 2), Catch::Matchers::WithinAbs(d0(i, 2), 1e-5f));
    }
}

TEST_CASE("BeamDirections pointing angle and azimuth reconstruct the direction", TESTTAG)
{
    std::mt19937                           rng(31);
    std::uniform_real_distribution<double> inst(-6, 6), att_yaw(-30, 30), att_rp(-15, 15),
        tx_tilt(-12, 12), rx_cross(-60, 60);

    const std::array<double, 3> tx_inst = { inst(rng), inst(rng), inst(rng) };
    const std::array<double, 3> rx_inst = { inst(rng), inst(rng), inst(rng) };

    const size_t          n      = 300;
    xt::xtensor<float, 2> tx_att = xt::xtensor<float, 2>::from_shape({ n, 3 });
    xt::xtensor<float, 2> rx_att = xt::xtensor<float, 2>::from_shape({ n, 3 });
    xt::xtensor<float, 1> tx_steer = xt::xtensor<float, 1>::from_shape({ n });
    xt::xtensor<float, 1> rx_steer = xt::xtensor<float, 1>::from_shape({ n });
    for (size_t i = 0; i < n; ++i)
    {
        tx_att(i, 0)  = float(att_yaw(rng));
        tx_att(i, 1)  = float(att_rp(rng));
        tx_att(i, 2)  = float(att_rp(rng));
        rx_att(i, 0)  = float(att_yaw(rng));
        rx_att(i, 1)  = float(att_rp(rng));
        rx_att(i, 2)  = float(att_rp(rng));
        tx_steer(i)   = float(tx_tilt(rng));
        rx_steer(i)   = float(rx_cross(rng));
    }

    auto        bd       = compute_beam_directions(
        tx_inst, rx_inst, tx_att, rx_att, tx_steer, rx_steer, 0.0);
    const auto& d        = bd.get_directions();
    const auto  pointing = bd.get_beam_pointing_angles_in_degrees();
    const auto  azimuth  = bd.get_beam_azimuth_angles_in_degrees();
    const auto  takeoff  = bd.get_beam_takeoff_angles_in_degrees();

    for (size_t i = 0; i < n; ++i)
    {
        const double beta = pointing(i) * DTR;
        const double psi  = azimuth(i) * DTR;
        // d = R_down(azimuth) * (0, -sin(pointing), cos(pointing))
        REQUIRE_THAT(d(i, 0),
                     Catch::Matchers::WithinAbs(float(std::sin(beta) * std::sin(psi)), 1e-4f));
        REQUIRE_THAT(d(i, 1),
                     Catch::Matchers::WithinAbs(float(-std::sin(beta) * std::cos(psi)), 1e-4f));
        REQUIRE_THAT(d(i, 2), Catch::Matchers::WithinAbs(float(std::cos(beta)), 1e-4f));

        REQUIRE(azimuth(i) > -90.001f);
        REQUIRE(azimuth(i) <= 90.001f);
        REQUIRE_THAT(takeoff(i), Catch::Matchers::WithinAbs(std::abs(pointing(i)), 1e-3f));
    }
}

TEST_CASE("BeamDirections pointing angle uses the port-positive across-track convention", TESTTAG)
{
    for (double rx : { 30.0, -30.0, 55.0, -60.0 })
    {
        xt::xtensor<float, 2> att      = { { 0.f, 0.f, 0.f } };
        xt::xtensor<float, 1> tx_steer = { 0.f };
        xt::xtensor<float, 1> rx_steer = { float(rx) };
        auto bd = compute_beam_directions({ 0, 0, 0 }, { 0, 0, 0 }, att, att, tx_steer, rx_steer, 0.0);
        REQUIRE_THAT(bd.get_beam_pointing_angles_in_degrees()(0),
                     Catch::Matchers::WithinAbs(float(rx), 1e-3f));
        REQUIRE_THAT(bd.get_beam_azimuth_angles_in_degrees()(0),
                     Catch::Matchers::WithinAbs(0.f, 1e-3f));
    }
}

TEST_CASE("correct_steering_angles_for_surface_sound_speed applies Snell's law", TESTTAG)
{
    const float           c_used = 1500.f, c_true = 1470.f;
    xt::xtensor<float, 1> angles = { 0.f, 10.f, -25.f, 60.f };

    const auto  corrected = correct_steering_angles_for_surface_sound_speed(angles, c_used, c_true);
    const float ratio     = c_true / c_used;
    for (size_t i = 0; i < angles.size(); ++i)
    {
        const float expected =
            std::asin(std::clamp(ratio * std::sin(angles(i) * float(DTR)), -1.f, 1.f)) * float(RTD);
        REQUIRE_THAT(corrected(i), Catch::Matchers::WithinAbs(expected, 1e-4f));
        REQUIRE_THAT(correct_steering_angle_for_surface_sound_speed(angles(i), c_used, c_true),
                     Catch::Matchers::WithinAbs(expected, 1e-4f));
    }

    // 0 deg is unchanged; a slower true speed reduces the magnitude.
    REQUIRE_THAT(correct_steering_angle_for_surface_sound_speed(0.f, c_used, c_true),
                 Catch::Matchers::WithinAbs(0.f, 1e-6f));
    REQUIRE(std::abs(correct_steering_angle_for_surface_sound_speed(60.f, c_used, c_true)) < 60.f);

    // beyond-horizon clamp: large angle with a faster true speed saturates at 90 deg.
    REQUIRE_THAT(correct_steering_angle_for_surface_sound_speed(80.f, 1450.f, 1550.f),
                 Catch::Matchers::WithinAbs(90.f, 1e-3f));
}

TEST_CASE("BeamDirections stream + printing roundtrip", TESTTAG)
{
    xt::xtensor<float, 2> dirs = { { 0.f, 0.f, 1.f }, { 0.f, 0.70710677f, 0.70710677f } };
    BeamDirections        bd(dirs);

    std::stringstream buf;
    bd.to_stream(buf);
    auto bd2 = BeamDirections::from_stream(buf);
    REQUIRE(bd == bd2);
    REQUIRE(bd.info_string().size() != 0);
}
