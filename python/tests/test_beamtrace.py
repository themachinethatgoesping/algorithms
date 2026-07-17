# SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""Tests for the single-beam raytracer (raytracers2.trace_beam / BeamTrace)."""

import math
import numpy as np
from pytest import approx

import themachinethatgoesping.algorithms.geoprocessing.raytracers2 as rt2


class TestBeamTrace:
    def test_construct_and_derive(self):
        depths = np.array([0.0, 1000.0], dtype=np.float32)
        offsets = np.array([0.0, 1000.0], dtype=np.float32)  # +y = starboard
        twtt = np.array([0.0, 2.0], dtype=np.float32)
        cos_angles = np.array(
            [math.cos(math.pi / 4), math.cos(math.pi / 4)], dtype=np.float32
        )

        trace = rt2.BeamTrace(depths, offsets, twtt, cos_angles)
        assert trace.get_number_of_points() == 2

        ranges = trace.get_ranges_in_meters()
        assert ranges[0] == approx(0.0, abs=1e-3)
        assert ranges[1] == approx(math.hypot(1000.0, 1000.0), abs=1e-2)

        # starboard (+y) is a negative incident angle by convention
        angles = trace.get_incident_angles_in_degrees()
        assert angles[1] == approx(-45.0, abs=1e-3)

    def test_size_mismatch_raises(self):
        trace = rt2.BeamTrace()
        try:
            trace.set(
                np.array([0.0, 1.0], dtype=np.float32),
                np.array([0.0], dtype=np.float32),
                np.array([0.0, 1.0], dtype=np.float32),
                np.array([1.0, 1.0], dtype=np.float32),
            )
            raised = False
        except Exception:
            raised = True
        assert raised


class TestTraceBeam:
    def test_constant_svp_straight_down(self):
        svp = rt2.SoundVelocityProfile.uniform(1500.0, 6000.0)
        trace = rt2.trace_beam(50.0, 0.0, svp, 2.0)  # one-way 1 s

        last = trace.get_number_of_points() - 1
        assert trace.get_depths_in_meters()[last] == approx(50.0 + 1500.0, abs=1e-1)
        assert trace.get_horizontal_offsets_in_meters()[last] == approx(0.0, abs=1e-2)
        assert trace.get_two_way_travel_times_in_seconds()[last] == approx(2.0, abs=1e-4)
        assert trace.get_ranges_in_meters()[last] == approx(1500.0, abs=1e-1)

    def test_constant_svp_port(self):
        svp = rt2.SoundVelocityProfile.uniform(1500.0, 6000.0)
        trace = rt2.trace_beam(50.0, 30.0, svp, 2.0)  # +30 deg -> port

        last = trace.get_number_of_points() - 1
        assert trace.get_depths_in_meters()[last] == approx(
            50.0 + 1500.0 * math.cos(math.radians(30.0)), abs=1e-1
        )
        # port is -y
        assert trace.get_horizontal_offsets_in_meters()[last] == approx(
            -1500.0 * math.sin(math.radians(30.0)), abs=1e-1
        )
        assert trace.get_incident_angles_in_degrees()[last] == approx(30.0, abs=1e-2)

    def test_turns_in_positive_gradient(self):
        z = np.array([0.0, 2000.0], dtype=np.float32)
        c = np.array([1500.0, 1800.0], dtype=np.float32)  # g = 0.15
        svp = rt2.SoundVelocityProfile(z, c)

        trace = rt2.trace_beam(0.0, 60.0, svp, 8.0)
        assert trace.get_number_of_points() == 3  # launch + apex + final

        depths = trace.get_depths_in_meters()
        cosang = trace.get_cos_incident_angles()

        assert cosang[1] == approx(0.0, abs=1e-3)  # horizontal at the apex
        c_turn = 1500.0 / math.sin(math.radians(60.0))
        assert depths[1] == approx((c_turn - 1500.0) / 0.15, abs=2.0)
        assert depths[1] > depths[0]
        assert depths[2] < depths[1]

    def test_upward_beam(self):
        svp = rt2.SoundVelocityProfile.uniform(1500.0, 6000.0)
        trace = rt2.trace_beam(1000.0, 150.0, svp, 1.0)  # upward, port

        last = trace.get_number_of_points() - 1
        assert trace.get_depths_in_meters()[last] < 1000.0
        assert trace.get_incident_angles_in_degrees()[last] == approx(150.0, abs=1e-2)

    def test_binary_roundtrip(self):
        svp = rt2.SoundVelocityProfile.uniform(1500.0, 6000.0)
        trace = rt2.trace_beam(50.0, 25.0, svp, 3.0)
        restored = rt2.BeamTrace.from_binary(trace.to_binary())
        assert trace == restored
