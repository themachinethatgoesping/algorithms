# SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""High-level tests for the new layer-based raytracer (raytracers2).

Validates:
- SoundVelocityProfile construction & queries
- LayerRaytracer.trace_at_times analytic constant-SVP result
- BeamSampleGeometryPiecewise.from_layer_xyz round-trip and auto-collapse
"""

import math
import numpy as np
from pytest import approx

import themachinethatgoesping.algorithms.geoprocessing.raytracers2 as rt2
import themachinethatgoesping.algorithms.geoprocessing.datastructures as ds
from themachinethatgoesping.navigation.datastructures import Geolocation


class TestSoundVelocityProfile:
    def test_uniform(self):
        svp = rt2.SoundVelocityProfile.uniform(1500.0, 6000.0)
        assert svp.get_number_of_layers() == 1
        assert svp.get_sound_speed(0.0) == approx(1500.0)
        assert svp.get_sound_speed(3000.0) == approx(1500.0)

    def test_layered(self):
        z = np.array([0.0, 100.0, 1000.0, 5000.0], dtype=np.float32)
        c = np.array([1500.0, 1480.0, 1490.0, 1540.0], dtype=np.float32)
        svp = rt2.SoundVelocityProfile(z, c)
        assert svp.get_number_of_layers() == 3
        # exact at knots
        assert svp.get_sound_speed(100.0) == approx(1480.0, abs=1e-3)
        assert svp.get_sound_speed(1000.0) == approx(1490.0, abs=1e-3)
        # mid-layer
        assert svp.get_sound_speed(50.0) == approx(1490.0, abs=1e-3)


class TestLayerRaytracer:
    def _make_pose(self, z=0.0):
        p = Geolocation()
        p.z = z
        p.yaw = 0.0
        p.pitch = 0.0
        p.roll = 0.0
        return p

    def test_constant_svp_straight_down(self):
        rt = rt2.LayerRaytracer(rt2.SoundVelocityProfile.uniform(1500.0, 6000.0))

        launch = np.array([[0.0, 0.0, 1.0]], dtype=np.float32)  # straight down
        knot_times = np.array([1.0, 2.0], dtype=np.float32)
        poses = [self._make_pose(0.0)] * 2

        out = rt.trace_at_times(launch, knot_times, poses)
        assert out.shape == (2, 1, 3)

        # straight down: depth = 1500 * t, x = y = 0
        assert out[0, 0, 0] == approx(0.0, abs=1e-2)
        assert out[0, 0, 1] == approx(0.0, abs=1e-2)
        assert out[0, 0, 2] == approx(1500.0, abs=1e-1)
        assert out[1, 0, 2] == approx(3000.0, abs=2e-1)

    def test_constant_svp_45deg_starboard(self):
        rt = rt2.LayerRaytracer(rt2.SoundVelocityProfile.uniform(1500.0, 6000.0))
        s = math.sin(math.pi / 4)
        launch = np.array([[0.0, s, s]], dtype=np.float32)
        knot_times = np.array([1.0], dtype=np.float32)
        poses = [self._make_pose(0.0)]

        out = rt.trace_at_times(launch, knot_times, poses)
        assert out[0, 0, 0] == approx(0.0, abs=1e-2)
        assert out[0, 0, 1] == approx(1500.0 * s, abs=1.0)
        assert out[0, 0, 2] == approx(1500.0 * s, abs=1.0)


class TestBeamSampleGeometryPiecewise:
    def test_full_pipeline_constant_svp(self):
        """End-to-end: trace -> build piecewise geom -> evaluate -> verify."""
        # Constant SVP, two beams (straight down + 45 deg)
        rt = rt2.LayerRaytracer(rt2.SoundVelocityProfile.uniform(1500.0, 6000.0))
        s = math.sin(math.pi / 4)
        launch = np.array(
            [[0.0, 0.0, 1.0],
             [0.0, s, s]],
            dtype=np.float32,
        )

        # 4 knots in time -> K=3 segments
        knot_times = np.array([0.0, 0.5, 1.0, 1.5], dtype=np.float32)
        pose = Geolocation()
        pose.z = 0.0
        poses = [pose] * len(knot_times)

        knot_xyz = rt.trace_at_times(launch, knot_times, poses)
        assert knot_xyz.shape == (4, 2, 3)

        # Map travel time -> sample number using sample interval
        sample_interval = 0.000125  # 8 kHz typical
        knot_sample_nrs = (knot_times / sample_interval).astype(np.float32)
        # ensure strictly monotone, drop the first knot if it's at t=0
        knot_sample_nrs = knot_sample_nrs.copy()
        knot_sample_nrs[0] = 0.0  # explicit
        # need strictly increasing -> shift the first to be 0 (already is) but
        # the test below requires strict monotone, so we use the array as-is
        # since 0 < 4000 < 8000 < 12000.

        first = np.array([0.0, 0.0], dtype=np.float32)
        n = np.array([12001, 12001], dtype=np.uint32)

        geom = ds.BeamSampleGeometryPiecewise.from_layer_xyz(
            first, n, knot_sample_nrs, knot_xyz.astype(np.float32))

        assert geom.get_n_beams() == 2
        assert geom.get_n_segments() == 3

        # In a constant SVP, all segments collapse to a single affine per beam.
        assert geom.is_single_affine(tol=1e-2)

        # Evaluate at knot sample numbers => match knot_xyz exactly
        for k in range(4):
            for b in range(2):
                px, py, pz = geom.eval_xyz(b, knot_sample_nrs[k])
                assert px == approx(knot_xyz[k, b, 0], abs=1e-2)
                assert py == approx(knot_xyz[k, b, 1], abs=1e-2)
                assert pz == approx(knot_xyz[k, b, 2], abs=1e-2)

    def test_layered_svp_not_collapsed(self):
        """With a non-trivial SVP the per-segment slopes differ -> NOT single-affine."""
        z = np.array([0.0, 100.0, 1000.0, 5000.0], dtype=np.float32)
        c = np.array([1500.0, 1480.0, 1490.0, 1540.0], dtype=np.float32)
        rt = rt2.LayerRaytracer(rt2.SoundVelocityProfile(z, c))

        # Oblique beams to maximise refraction
        launch = np.array(
            [[0.0, 0.5, 0.86602540],
             [0.0, 0.7, 0.71414284]],
            dtype=np.float32,
        )

        knot_times = np.array([0.5, 1.0, 1.5], dtype=np.float32)
        pose = Geolocation()
        pose.z = 0.0
        poses = [pose] * len(knot_times)

        knot_xyz = rt.trace_at_times(launch, knot_times, poses)
        assert np.all(np.isfinite(knot_xyz)), "ray turning unexpectedly"

        knot_sample_nrs = np.array([4000.0, 8000.0, 12000.0], dtype=np.float32)
        first = np.array([0.0, 0.0], dtype=np.float32)
        n = np.array([12001, 12001], dtype=np.uint32)

        geom = ds.BeamSampleGeometryPiecewise.from_layer_xyz(
            first, n, knot_sample_nrs, knot_xyz.astype(np.float32))

        assert geom.get_n_segments() == 2
        # Refraction across layers should produce non-collinear segments
        assert not geom.is_single_affine(tol=1e-2)
