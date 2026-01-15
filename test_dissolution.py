#!/usr/bin/env python
"""Test script for bubble dissolution module."""

import sys
sys.path.insert(0, '/home/ssd/src/themachinethatgoesping/algorithms/python')

from themachinethatgoesping.algorithms.backscattermodels.gasbubbles.bubble_dissolution import (
    BubbleEnvironment, simulate_bubble_rise, METHANE
)
import numpy as np

print("Import OK")

# Test with default environment (100% methane)
env = BubbleEnvironment()
print(f'Environment: 100% CH4, dirty bubble')
print(f'Temp profile: {env.temperature_profile(100):.2f} K ({env.temperature_profile(100)-273.15:.2f} °C)')
print(f'Pressure at 100m: {env.pressure_at_depth(100)/1e5:.2f} bar')

# Simulate a 2mm bubble from 100m
result = simulate_bubble_rise(
    initial_radius=0.002,  # 2 mm
    initial_depth=100.0,
    env=env,
    depth_resolution=10.0
)

print(f'\nSimulation result:')
print(f'  Initial radius: {result.initial_radius*1000:.2f} mm')
print(f'  Initial depth: {result.initial_depth:.0f} m')
print(f'  Reached surface: {result.reached_surface}')
print(f'  Final radius: {result.final_radius*1000:.3f} mm')
print(f'  Rise time: {result.times[-1]:.1f} s')

print(f'\nDepth profile (first 5 points):')
for i in range(min(5, len(result.depths))):
    print(f'  z={result.depths[i]:6.1f}m: r={result.radii[i]*1000:.3f}mm, u={result.rise_velocities[i]*100:.1f}cm/s')
