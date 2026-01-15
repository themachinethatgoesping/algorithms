# SPDX-FileCopyrightText: 2026 GEOMAR Helmholtz Centre for Ocean Research Kiel
#
# SPDX-License-Identifier: MPL-2.0

"""
Bubble shape prediction models.

This module provides models to predict bubble shape (spherical, ellipsoidal,
spherical cap) and aspect ratio as a function of bubble size and fluid properties.

The main approach uses the Clift et al. (1978) regime diagram based on:
- Eötvös number (Eo): buoyancy vs surface tension
- Morton number (Mo): fluid property group
- Reynolds number (Re): inertia vs viscosity

Bubble Shape Regimes:
--------------------
1. SPHERICAL (small bubbles, Eo < 0.5):
   - Surface tension dominates, bubble stays spherical
   - Aspect ratio E ≈ 1.0
   - Typical for r < 0.5 mm in water

2. ELLIPSOIDAL (intermediate, 0.5 < Eo < 40):
   - Balance between surface tension and buoyancy
   - Oblate spheroid shape (flattened)
   - Aspect ratio E = minor/major axis, typically 0.5-1.0
   - Typical for r = 0.5-5 mm in water

3. SPHERICAL_CAP (large bubbles, Eo > 40):
   - Buoyancy dominates, flat bottom
   - Mushroom-like shape
   - Typical for r > 5 mm in water

4. WOBBLING (intermediate-large):
   - Unstable shape, oscillating
   - Path instabilities

References
----------
- Clift, R., Grace, J.R., Weber, M.E. (1978) "Bubbles, Drops, and Particles"
- Loth, E. (2008) "Quasi-steady shape and drag of deformable bubbles and drops"
- Myint et al. (2007) "Shapes of rising bubbles"
"""

from dataclasses import dataclass
from enum import Enum
from typing import Optional
import numpy as np

from .environment import BubbleEnvironment


class BubbleShapeRegime(Enum):
    """Bubble shape regime classification."""
    SPHERICAL = "spherical"
    ELLIPSOIDAL = "ellipsoidal"
    WOBBLING = "wobbling"
    SPHERICAL_CAP = "spherical_cap"


@dataclass
class BubbleShapeResult:
    """Complete bubble shape characterization.
    
    Attributes
    ----------
    regime : BubbleShapeRegime
        Shape regime classification
    aspect_ratio : float
        E = minor axis / major axis (1.0 = sphere, <1 = oblate)
    equivalent_radius : float
        Radius of sphere with same volume (m)
    semi_major_axis : float
        Semi-major axis a (horizontal, m) for ellipsoid
    semi_minor_axis : float
        Semi-minor axis b (vertical, m) for ellipsoid
    eotvos_number : float
        Eo = Δρ g d² / σ
    morton_number : float
        Mo = g μ⁴ Δρ / (ρ² σ³)
    reynolds_number : float
        Re = ρ u d / μ (if velocity known)
    surface_area : float
        Bubble surface area (m²)
    cross_section_area : float
        Projected area normal to rise direction (m²)
    """
    regime: BubbleShapeRegime
    aspect_ratio: float
    equivalent_radius: float
    semi_major_axis: float
    semi_minor_axis: float
    eotvos_number: float
    morton_number: float
    reynolds_number: Optional[float] = None
    surface_area: Optional[float] = None
    cross_section_area: Optional[float] = None


def calculate_eotvos_number(
    radius: float,
    depth: float,
    env: BubbleEnvironment
) -> float:
    """Calculate Eötvös (Bond) number.
    
    Eo = Δρ g d² / σ
    
    Measures ratio of buoyancy to surface tension forces.
    
    Parameters
    ----------
    radius : float
        Bubble equivalent radius (m)
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
        
    Returns
    -------
    float
        Eötvös number (dimensionless)
    """
    diameter = 2 * radius
    sigma = env.surface_tension(depth)
    rho_w = env.water_density
    rho_g = env.gas_density_at_depth(depth)
    delta_rho = rho_w - rho_g
    g = env.gravity
    
    return delta_rho * g * diameter**2 / sigma


def calculate_morton_number(
    depth: float,
    env: BubbleEnvironment
) -> float:
    """Calculate Morton number.
    
    Mo = g μ⁴ Δρ / (ρ² σ³)
    
    Fluid property group that characterizes the system.
    For water at ~20°C: Mo ≈ 2.5e-11
    
    Parameters
    ----------
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
        
    Returns
    -------
    float
        Morton number (dimensionless)
    """
    sigma = env.surface_tension(depth)
    rho_w = env.water_density
    rho_g = env.gas_density_at_depth(depth)
    delta_rho = rho_w - rho_g
    nu = env.kinematic_viscosity(depth)
    mu = nu * rho_w  # Dynamic viscosity
    g = env.gravity
    
    return g * mu**4 * delta_rho / (rho_w**2 * sigma**3)


def calculate_aspect_ratio_wellek(eotvos: float) -> float:
    """Calculate bubble aspect ratio using Wellek et al. (1966) correlation.
    
    E = 1 / (1 + 0.163 * Eo^0.757)
    
    Valid for clean bubbles in the ellipsoidal regime.
    
    Parameters
    ----------
    eotvos : float
        Eötvös number
        
    Returns
    -------
    float
        Aspect ratio E = b/a (minor/major axis)
    """
    if eotvos <= 0:
        return 1.0
    
    E = 1.0 / (1.0 + 0.163 * eotvos**0.757)
    return max(0.1, min(1.0, E))


def calculate_aspect_ratio_loth(eotvos: float, morton: float) -> float:
    """Calculate bubble aspect ratio using Loth (2008) correlation.
    
    More accurate for wider range of conditions than Wellek.
    Accounts for Morton number effects.
    
    Parameters
    ----------
    eotvos : float
        Eötvös number
    morton : float
        Morton number
        
    Returns
    -------
    float
        Aspect ratio E = b/a
    """
    if eotvos <= 0.1:
        return 1.0
    
    # Loth (2008) correlation for aspect ratio
    # E = 1 / (1 + 0.15 * Eo^0.75 * Mo^-0.065)
    # For water (low Mo), this simplifies
    
    Mo_factor = max(morton, 1e-15)**(-0.065) if morton > 0 else 1.0
    E = 1.0 / (1.0 + 0.15 * eotvos**0.75 * Mo_factor)
    
    return max(0.1, min(1.0, E))


def calculate_aspect_ratio_myint(eotvos: float, weber: float = None) -> float:
    """Calculate aspect ratio using Myint et al. (2007) correlation.
    
    Based on experimental data for air bubbles in water.
    
    Parameters
    ----------
    eotvos : float
        Eötvös number
    weber : float, optional
        Weber number (if known)
        
    Returns
    -------
    float
        Aspect ratio E = b/a
    """
    if eotvos <= 0.1:
        return 1.0
    
    # Myint correlation for clean bubbles in water
    # Fit to experimental data
    if eotvos < 1:
        E = 1.0 - 0.02 * eotvos
    elif eotvos < 10:
        E = 0.98 - 0.06 * (eotvos - 1)
    elif eotvos < 40:
        E = 0.44 - 0.008 * (eotvos - 10)
    else:
        E = 0.2  # Spherical cap regime
    
    return max(0.15, min(1.0, E))


def predict_bubble_shape(
    radius: float,
    depth: float,
    env: BubbleEnvironment,
    rise_velocity: float = None,
    aspect_ratio_model: str = "loth"
) -> BubbleShapeResult:
    """Predict bubble shape and aspect ratio.
    
    Uses the Clift et al. (1978) regime diagram approach combined with
    empirical correlations for aspect ratio.
    
    Parameters
    ----------
    radius : float
        Bubble equivalent spherical radius (m)
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
    rise_velocity : float, optional
        Rise velocity (m/s), used for Reynolds number calculation
    aspect_ratio_model : str
        Model for aspect ratio: "wellek", "loth", or "myint"
        
    Returns
    -------
    BubbleShapeResult
        Complete shape characterization
        
    Examples
    --------
    >>> env = BubbleEnvironment()
    >>> shape = predict_bubble_shape(0.002, 50.0, env)  # 2mm at 50m
    >>> print(f"Regime: {shape.regime.value}")
    >>> print(f"Aspect ratio: {shape.aspect_ratio:.2f}")
    """
    # Calculate dimensionless numbers
    Eo = calculate_eotvos_number(radius, depth, env)
    Mo = calculate_morton_number(depth, env)
    
    # Reynolds number if velocity known
    Re = None
    if rise_velocity is not None and rise_velocity > 0:
        diameter = 2 * radius
        nu = env.kinematic_viscosity(depth)
        Re = rise_velocity * diameter / nu
    
    # Determine shape regime based on Eo
    # Using Clift et al. (1978) criteria
    if Eo < 0.5:
        regime = BubbleShapeRegime.SPHERICAL
    elif Eo < 4:
        regime = BubbleShapeRegime.ELLIPSOIDAL
    elif Eo < 40:
        # Check for wobbling regime based on Re
        if Re is not None and Re > 200:
            regime = BubbleShapeRegime.WOBBLING
        else:
            regime = BubbleShapeRegime.ELLIPSOIDAL
    else:
        regime = BubbleShapeRegime.SPHERICAL_CAP
    
    # Calculate aspect ratio based on model
    if aspect_ratio_model == "wellek":
        E = calculate_aspect_ratio_wellek(Eo)
    elif aspect_ratio_model == "loth":
        E = calculate_aspect_ratio_loth(Eo, Mo)
    elif aspect_ratio_model == "myint":
        E = calculate_aspect_ratio_myint(Eo)
    else:
        raise ValueError(f"Unknown model: {aspect_ratio_model}")
    
    # Force spherical for very small Eo
    if regime == BubbleShapeRegime.SPHERICAL:
        E = 1.0
    
    # For spherical cap, use different geometry
    if regime == BubbleShapeRegime.SPHERICAL_CAP:
        E = min(E, 0.3)  # Cap bubbles are very flat
    
    # Calculate ellipsoid dimensions from aspect ratio
    # For oblate spheroid: V = (4/3) π a² b, where a > b
    # E = b/a, so b = E*a
    # V = (4/3) π a³ E
    # a = (3V / (4π E))^(1/3)
    V = (4/3) * np.pi * radius**3
    semi_major = (3 * V / (4 * np.pi * E))**(1/3)
    semi_minor = E * semi_major
    
    # Surface area of oblate spheroid
    if E < 0.9999:
        e = np.sqrt(1 - E**2)  # Eccentricity
        surface_area = 2 * np.pi * semi_major**2 * (1 + (E**2 / e) * np.arctanh(e))
    else:
        surface_area = 4 * np.pi * radius**2  # Sphere
    
    # Cross-sectional area (horizontal plane)
    cross_section = np.pi * semi_major**2
    
    return BubbleShapeResult(
        regime=regime,
        aspect_ratio=E,
        equivalent_radius=radius,
        semi_major_axis=semi_major,
        semi_minor_axis=semi_minor,
        eotvos_number=Eo,
        morton_number=Mo,
        reynolds_number=Re,
        surface_area=surface_area,
        cross_section_area=cross_section,
    )


def bubble_shape_vs_size(
    radius_range: tuple[float, float],
    depth: float,
    env: BubbleEnvironment,
    n_points: int = 50,
    aspect_ratio_model: str = "loth"
) -> dict:
    """Calculate bubble shape properties over a range of sizes.
    
    Parameters
    ----------
    radius_range : tuple[float, float]
        (min_radius, max_radius) in meters
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
    n_points : int
        Number of points
    aspect_ratio_model : str
        Aspect ratio model name
        
    Returns
    -------
    dict
        Dictionary with arrays:
        - "radii": bubble radii (m)
        - "diameters_mm": bubble diameters (mm)
        - "aspect_ratios": E = b/a
        - "eotvos": Eo numbers
        - "regimes": shape regime strings
    """
    radii = np.linspace(radius_range[0], radius_range[1], n_points)
    
    aspect_ratios = []
    eotvos_numbers = []
    regimes = []
    
    for r in radii:
        result = predict_bubble_shape(r, depth, env, aspect_ratio_model=aspect_ratio_model)
        aspect_ratios.append(result.aspect_ratio)
        eotvos_numbers.append(result.eotvos_number)
        regimes.append(result.regime.value)
    
    return {
        "radii": radii,
        "diameters_mm": radii * 2000,  # Convert to mm
        "aspect_ratios": np.array(aspect_ratios),
        "eotvos": np.array(eotvos_numbers),
        "regimes": regimes,
    }
