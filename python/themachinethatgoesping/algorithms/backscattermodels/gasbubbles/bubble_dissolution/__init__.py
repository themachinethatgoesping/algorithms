# SPDX-FileCopyrightText: 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""
-------
ALL CODE IN bubble_dissolution is UNCHECKED AI CODE AND TEXT AND VERY LIKELY NONESENSE
REFERENCES AND NOTES ARE PROPABLY AT LEASY PARTIALY MADE UP
-------

Bubble dissolution modeling module.

This module provides models for simulating how gas bubbles dissolve and change size
as they rise through the water column. Based on mass transfer equations from:

- Leifer & Patro (2002) - "The bubble mechanism for methane transport"
- McGinnis et al. (2006) - "Fate of rising methane bubbles in stratified waters"
- Clift et al. (1978) - "Bubbles, Drops, and Particles"

Key components:
- BubbleEnvironment: Environmental parameters (temperature, salinity, gas composition)
- BubbleRiseModel: Calculates rise velocity based on bubble size and shape
- simulate_bubble_rise: Main simulation function for single bubble trajectory
- simulate_bubble_population: Simulate multiple bubbles with size distribution

Example
-------
>>> from themachinethatgoesping.algorithms.backscattermodels.gasbubbles.bubble_dissolution import (
...     BubbleEnvironment, simulate_bubble_rise
... )
>>> env = BubbleEnvironment()  # 100% methane, ~8°C, dirty bubbles
>>> result = simulate_bubble_rise(
...     initial_radius=0.002,  # 2 mm bubble
...     initial_depth=100.0,   # Released at 100 m
...     env=env,
... )
>>> print(f"Reached surface: {result.reached_surface}")
>>> print(f"Final radius: {result.final_radius*1000:.2f} mm")
"""

from .environment import (
    BubbleEnvironment,
    GasProperties,
    METHANE,
    CO2,
    NITROGEN,
    OXYGEN,
)
from .rise_velocity import (
    BubbleRiseModel,
    BubbleShape,
    bubble_rise_velocity_clift,
    bubble_rise_velocity_leifer,
    bubble_rise_velocity_woolf,
    bubble_shape_regime,
    get_rise_velocity_model,
)
from .dissolution import (
    BubbleDissolutionResult,
    simulate_bubble_rise,
    simulate_bubble_population,
    sherwood_number,
    mass_transfer_coefficient,
)
from .bubble_shape import (
    BubbleShapeRegime,
    BubbleShapeResult,
    predict_bubble_shape,
    bubble_shape_vs_size,
    calculate_eotvos_number,
    calculate_morton_number,
    calculate_aspect_ratio_wellek,
    calculate_aspect_ratio_loth,
)

__all__ = [
    # Environment
    "BubbleEnvironment",
    "GasProperties",
    "METHANE",
    "CO2",
    "NITROGEN",
    "OXYGEN",
    # Rise velocity
    "BubbleRiseModel",
    "BubbleShape",
    "bubble_rise_velocity_clift",
    "bubble_rise_velocity_leifer",
    "bubble_rise_velocity_woolf",
    "bubble_shape_regime",
    "get_rise_velocity_model",
    # Dissolution
    "BubbleDissolutionResult",
    "simulate_bubble_rise",
    "simulate_bubble_population",
    "sherwood_number",
    "mass_transfer_coefficient",
    # Shape prediction
    "BubbleShapeRegime",
    "BubbleShapeResult",
    "predict_bubble_shape",
    "bubble_shape_vs_size",
    "calculate_eotvos_number",
    "calculate_morton_number",
    "calculate_aspect_ratio_wellek",
    "calculate_aspect_ratio_loth",
]
