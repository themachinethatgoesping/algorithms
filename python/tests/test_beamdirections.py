# SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""Tests for the beam pointing-angle computation (raytracers2.compute_beam_directions)."""

import math
import numpy as np
from pytest import approx

import themachinethatgoesping.algorithms.geoprocessing.raytracers2 as rt2


def _call(tx_install, rx_install, tx_att, rx_att, tx_steer, rx_steer, heading=0.0):
    return rt2.compute_beam_directions(
        np.array(tx_install, dtype=float),
        np.array(rx_install, dtype=float),
        np.array(tx_att, dtype=np.float32),
        np.array(rx_att, dtype=np.float32),
        np.array(tx_steer, dtype=np.float32),
        np.array(rx_steer, dtype=np.float32),
        heading,
    )


class TestComputeBeamDirections:
    def test_straight_down(self):
        bd = _call([0, 0, 0], [0, 0, 0], [[0, 0, 0]], [[0, 0, 0]], [0.0], [0.0])

        assert bd.get_number_of_beams() == 1
        d = bd.get_directions()
        assert d[0, 0] == approx(0.0, abs=1e-5)  # forward
        assert d[0, 1] == approx(0.0, abs=1e-5)  # starboard
        assert d[0, 2] == approx(1.0, abs=1e-5)  # down
        assert bd.get_beam_pointing_angles_in_degrees()[0] == approx(0.0, abs=1e-3)
        assert bd.get_beam_takeoff_angles_in_degrees()[0] == approx(0.0, abs=1e-3)

    def test_directions_consistent_with_angles(self):
        rng = np.random.default_rng(42)
        n = 64
        tx_att = rng.uniform(-15, 15, size=(n, 3)).astype(np.float32)
        rx_att = rng.uniform(-15, 15, size=(n, 3)).astype(np.float32)
        tx_steer = rng.uniform(-10, 10, size=n).astype(np.float32)
        rx_steer = rng.uniform(-60, 60, size=n).astype(np.float32)

        bd = _call([0, 1, -0.5], [0.2, 0.3, 0.1], tx_att, rx_att, tx_steer, rx_steer)
        d = np.asarray(bd.get_directions())
        pointing = np.radians(np.asarray(bd.get_beam_pointing_angles_in_degrees()))
        az = np.radians(np.asarray(bd.get_beam_azimuth_angles_in_degrees()))

        # reconstruct the unit vector: d = R_down(azimuth) * (0, -sin(pointing), cos(pointing))
        fwd = np.sin(pointing) * np.sin(az)
        stbd = -np.sin(pointing) * np.cos(az)
        down = np.cos(pointing)

        assert d[:, 0] == approx(fwd, abs=1e-4)
        assert d[:, 1] == approx(stbd, abs=1e-4)
        assert d[:, 2] == approx(down, abs=1e-4)
        # azimuth is the principal fore-aft rotation in (-90, 90]
        az_deg = np.asarray(bd.get_beam_azimuth_angles_in_degrees())
        assert np.all(az_deg > -90.001) and np.all(az_deg <= 90.001)
        # unit vectors
        assert np.linalg.norm(d, axis=1) == approx(np.ones(n), abs=1e-5)

    def test_pointing_angle_is_port_positive(self):
        # flat vessel, no transmit steering: the pointing angle equals the
        # port-positive receive steering, and the azimuth is ~0.
        for rx in (30.0, -30.0, 55.0, -60.0):
            bd = _call([0, 0, 0], [0, 0, 0], [[0, 0, 0]], [[0, 0, 0]], [0.0], [rx])
            assert bd.get_beam_pointing_angles_in_degrees()[0] == approx(rx, abs=1e-3)
            assert bd.get_beam_azimuth_angles_in_degrees()[0] == approx(0.0, abs=1e-3)

    def test_reference_heading_rotation(self):
        att = [[5, 2, -3], [-10, 1, 4]]
        tx_steer = [3.0, -2.0]
        rx_steer = [40.0, -55.0]
        d0 = np.asarray(_call([0, 0, 0], [0, 0, 0], att, att, tx_steer, rx_steer, 0.0).get_directions())

        delta = 37.0
        dd = np.asarray(_call([0, 0, 0], [0, 0, 0], att, att, tx_steer, rx_steer, delta).get_directions())

        cD, sD = math.cos(math.radians(delta)), math.sin(math.radians(delta))
        ex = cD * d0[:, 0] + sD * d0[:, 1]
        ey = -sD * d0[:, 0] + cD * d0[:, 1]
        assert dd[:, 0] == approx(ex, abs=1e-5)
        assert dd[:, 1] == approx(ey, abs=1e-5)
        assert dd[:, 2] == approx(d0[:, 2], abs=1e-5)


class TestBeamDirectionsClass:
    def test_construct_and_roundtrip(self):
        dirs = np.array([[0, 0, 1], [0, 0.70710677, 0.70710677]], dtype=np.float32)
        bd = rt2.BeamDirections(dirs)
        assert bd.get_number_of_beams() == 2
        assert bd == rt2.BeamDirections(bd.get_directions())
        assert len(bd.info_string()) != 0


class TestSurfaceSoundSpeedCorrection:
    def test_snell_correction(self):
        c_used, c_true = 1500.0, 1470.0
        angles = np.array([0.0, 10.0, -25.0, 60.0], dtype=np.float32)
        corrected = np.asarray(
            rt2.correct_steering_angles_for_surface_sound_speed(angles, c_used, c_true)
        )
        expected = np.degrees(
            np.arcsin(np.clip((c_true / c_used) * np.sin(np.radians(angles)), -1.0, 1.0))
        )
        assert corrected == approx(expected, abs=1e-4)
        # 0 deg unchanged; a slower true speed reduces the magnitude
        assert rt2.correct_steering_angle_for_surface_sound_speed(0.0, c_used, c_true) == approx(
            0.0, abs=1e-6
        )
        assert abs(rt2.correct_steering_angle_for_surface_sound_speed(60.0, c_used, c_true)) < 60.0
        # beyond-horizon clamp saturates at 90 deg
        assert rt2.correct_steering_angle_for_surface_sound_speed(80.0, 1450.0, 1550.0) == approx(
            90.0, abs=1e-3
        )
