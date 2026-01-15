# SPDX-FileCopyrightText: 2026 GEOMAR Helmholtz Centre for Ocean Research Kiel
#
# SPDX-License-Identifier: MPL-2.0

"""
Bubble rise velocity models.

This module implements bubble rise velocity correlations accounting for
bubble size, shape regime transitions, and surface contamination.

The main correlations are based on:
- Clift et al. (1978) "Bubbles, Drops, and Particles"
- Leifer & Patro (2002) for clean/dirty bubble distinction
- Mendelson (1967) for large bubbles

Bubble shape regimes:
- Spherical: Re < 1, small bubbles
- Ellipsoidal: 1 < Re < 500, intermediate
- Spherical cap: Re > 500, large bubbles
"""

from dataclasses import dataclass
from enum import Enum
from typing import Callable, Optional
import numpy as np

from .environment import BubbleEnvironment


class BubbleShape(Enum):
    """Bubble shape regime based on size and Reynolds number."""
    SPHERICAL = "spherical"
    ELLIPSOIDAL = "ellipsoidal"
    SPHERICAL_CAP = "spherical_cap"


@dataclass
class BubbleRiseModel:
    """Configuration for bubble rise velocity calculation.
    
    Parameters
    ----------
    model_name : str
        Name of the model (e.g., "clift", "leifer", "mendelson")
    clean_bubble : bool
        True for clean (mobile interface), False for dirty (rigid interface)
    shape_dependent : bool
        Whether to account for shape transitions
    compute_velocity : Callable
        Function: (radius_m, depth_m, env) -> velocity_m_s
    """
    model_name: str
    clean_bubble: bool = False
    shape_dependent: bool = True
    compute_velocity: Optional[Callable[[float, float, BubbleEnvironment], float]] = None


def bubble_shape_regime(
    radius: float,
    depth: float,
    env: BubbleEnvironment
) -> BubbleShape:
    """Determine bubble shape regime based on size and fluid properties.
    
    Uses Morton number (M) and Eötvös number (Eo) to classify regime.
    
    Parameters
    ----------
    radius : float
        Bubble equivalent spherical radius in meters
    depth : float  
        Water depth in meters
    env : BubbleEnvironment
        Environmental conditions
        
    Returns
    -------
    BubbleShape
        Shape regime classification
    """
    diameter = 2 * radius
    
    sigma = env.surface_tension(depth)
    rho_w = env.water_density
    rho_g = env.gas_density_at_depth(depth)
    nu = env.kinematic_viscosity(depth)
    g = env.gravity
    
    delta_rho = rho_w - rho_g
    
    # Eötvös number (Bond number): buoyancy vs surface tension
    Eo = (delta_rho * g * diameter**2) / sigma
    
    # Morton number: fluid property group
    mu = nu * rho_w  # Dynamic viscosity
    Mo = (g * mu**4 * delta_rho) / (rho_w**2 * sigma**3)
    
    # Shape classification based on Clift et al. (1978) regime map
    # Simplified criteria
    if Eo < 0.5:
        return BubbleShape.SPHERICAL
    elif Eo < 40:
        return BubbleShape.ELLIPSOIDAL
    else:
        return BubbleShape.SPHERICAL_CAP


def bubble_rise_velocity_clift(
    radius: float,
    depth: float,
    env: BubbleEnvironment,
    clean: bool = False
) -> float:
    """Calculate bubble rise velocity using Clift et al. (1978) correlations.
    
    This function accounts for different shape regimes and clean/dirty
    bubble surfaces. The correlations are valid for air bubbles in water
    and similar systems.
    
    Parameters
    ----------
    radius : float
        Bubble equivalent spherical radius in meters
    depth : float
        Water depth in meters (positive downward)
    env : BubbleEnvironment
        Environmental conditions
    clean : bool
        True for clean bubbles (mobile interface), False for dirty
        (contaminated, rigid interface)
        
    Returns
    -------
    float
        Terminal rise velocity in m/s
        
    Notes
    -----
    Clean bubbles: surface-active materials absent, internal circulation.
    Dirty bubbles: surfactants present, rigid interface like solid sphere.
    
    For most natural conditions, "dirty" is appropriate.
    
    References
    ----------
    Clift, R., Grace, J.R., Weber, M.E. (1978) "Bubbles, Drops, and Particles"
    """
    if radius <= 0:
        return 0.0
    
    diameter = 2 * radius
    
    sigma = env.surface_tension(depth)
    rho_w = env.water_density
    rho_g = env.gas_density_at_depth(depth)
    nu = env.kinematic_viscosity(depth)
    g = env.gravity
    
    delta_rho = rho_w - rho_g
    
    shape = bubble_shape_regime(radius, depth, env)
    
    if shape == BubbleShape.SPHERICAL:
        # Small spherical bubbles - Stokes or Hadamard-Rybczynski
        if clean:
            # Hadamard-Rybczynski for clean bubble (mobile surface)
            # Factor of 1.5 faster than Stokes
            u = (delta_rho * g * diameter**2) / (12 * nu * rho_w)
        else:
            # Stokes law for dirty bubble (rigid surface)
            u = (delta_rho * g * diameter**2) / (18 * nu * rho_w)
            
    elif shape == BubbleShape.ELLIPSOIDAL:
        # Intermediate regime - use Mendelson (1967) wave analogy
        # Valid for ellipsoidal bubbles
        # u = sqrt(2*sigma/(rho*d) + g*d/2)
        
        u_cap = np.sqrt(2 * sigma / (rho_w * diameter))  # Capillary term
        u_grav = np.sqrt(g * diameter / 2)  # Gravitational term
        
        u = np.sqrt(u_cap**2 + u_grav**2)
        
        if not clean:
            # Dirty bubbles rise slower - empirical factor
            u *= 0.85
            
    else:  # SPHERICAL_CAP
        # Large spherical cap bubbles - Davies & Taylor (1950)
        # u = 0.711 * sqrt(g * d)
        u = 0.711 * np.sqrt(g * diameter)
        
        if not clean:
            u *= 0.9  # Slight reduction for contamination
    
    return u


def bubble_rise_velocity_leifer(
    radius: float,
    depth: float,
    env: BubbleEnvironment,
    clean: bool = False
) -> float:
    """Calculate bubble rise velocity using Leifer et al. (2000) method.
    
    This parameterization is specifically developed for methane seep bubbles
    and distinguishes between clean and dirty bubbles based on surfactant
    loading.
    
    Parameters
    ----------
    radius : float
        Bubble equivalent spherical radius in meters
    depth : float  
        Water depth in meters
    env : BubbleEnvironment
        Environmental conditions
    clean : bool
        True for clean bubbles, False for dirty (default)
        
    Returns
    -------
    float
        Terminal rise velocity in m/s
        
    References
    ----------
    Leifer, I., Patro, R.K., Bowyer, P. (2000) "A study on the temperature
    variation of rise velocity for large clean bubbles" J. Atmos. Oceanic
    Technol., 17, 1392-1402.
    """
    if radius <= 0:
        return 0.0
    
    diameter_mm = 2 * radius * 1000  # Convert to mm
    
    # Leifer parameterization (empirical fit for seawater)
    if clean:
        # Clean bubble parameterization (faster rise)
        if diameter_mm < 0.8:
            # Very small - Stokes regime
            u = 0.033 * diameter_mm**2  # m/s
        elif diameter_mm < 6.0:
            # Intermediate - power law fit
            u = 0.139 * diameter_mm**0.70
        else:
            # Large bubbles
            u = 0.22 * diameter_mm**0.52
    else:
        # Dirty bubble parameterization (slower rise)
        if diameter_mm < 1.0:
            # Stokes regime for rigid sphere
            u = 0.022 * diameter_mm**2
        elif diameter_mm < 7.0:
            # Intermediate
            u = 0.102 * diameter_mm**0.75
        else:
            # Large bubbles
            u = 0.20 * diameter_mm**0.50
    
    # Apply temperature correction (Leifer et al. 2000)
    T = env.temperature_profile(depth)
    T_ref = 288.15  # 15°C reference
    T_factor = 1.0 + 0.02 * (T - T_ref)  # ~2% per degree
    u *= T_factor
    
    return max(u / 100, 0.001)  # Convert cm/s to m/s, minimum 1 mm/s


def bubble_rise_velocity_woolf(
    radius: float,
    depth: float,
    env: BubbleEnvironment,
    clean: bool = False
) -> float:
    """Calculate bubble rise velocity using Woolf & Thorpe (1991) method.
    
    Simple parameterization suitable for bubble plume modeling.
    
    Parameters
    ----------
    radius : float
        Bubble radius in meters
    depth : float
        Water depth in meters  
    env : BubbleEnvironment
        Environmental conditions
    clean : bool
        True for clean, False for dirty bubbles
        
    Returns
    -------
    float
        Rise velocity in m/s
        
    References
    ----------
    Woolf, D.K., Thorpe, S.A. (1991) "Bubbles and the air-sea exchange
    of gases in near-saturation conditions" J. Mar. Res., 49, 435-466.
    """
    if radius <= 0:
        return 0.0
    
    diameter = 2 * radius
    sigma = env.surface_tension(depth)
    rho_w = env.water_density
    g = env.gravity
    
    # Woolf & Thorpe parameterization
    # Combines terminal velocity regimes
    
    r_mm = radius * 1000  # radius in mm
    
    if r_mm < 0.1:
        # Stokes regime
        nu = env.kinematic_viscosity(depth)
        u = (2 * rho_w * g * radius**2) / (9 * nu * rho_w)
        if clean:
            u *= 1.5
    elif r_mm < 0.6:
        # Transition regime - linear interpolation
        u_stokes = (2 * g * radius**2) / (9 * env.kinematic_viscosity(depth))
        u_mendelson = np.sqrt(2 * sigma / (rho_w * diameter) + g * diameter / 2)
        frac = (r_mm - 0.1) / 0.5
        u = u_stokes * (1 - frac) + u_mendelson * frac
    else:
        # Mendelson regime
        u = np.sqrt(2 * sigma / (rho_w * diameter) + g * diameter / 2)
    
    if not clean:
        u *= 0.85
        
    return u


# Factory function to get rise velocity model
def get_rise_velocity_model(
    model_name: str = "clift"
) -> BubbleRiseModel:
    """Get a configured rise velocity model by name.
    
    Parameters
    ----------
    model_name : str
        Model name: "clift", "leifer", or "woolf"
        
    Returns
    -------
    BubbleRiseModel
        Configured model object
        
    Examples
    --------
    >>> model = get_rise_velocity_model("leifer")
    >>> env = BubbleEnvironment()
    >>> velocity = model.compute_velocity(0.002, 100, env)  # 2mm bubble at 100m
    """
    models = {
        "clift": BubbleRiseModel(
            model_name="clift",
            shape_dependent=True,
            compute_velocity=lambda r, d, e: bubble_rise_velocity_clift(
                r, d, e, clean=(e.bubble_surface_condition == "clean")
            )
        ),
        "leifer": BubbleRiseModel(
            model_name="leifer",
            shape_dependent=False,
            compute_velocity=lambda r, d, e: bubble_rise_velocity_leifer(
                r, d, e, clean=(e.bubble_surface_condition == "clean")
            )
        ),
        "woolf": BubbleRiseModel(
            model_name="woolf",
            shape_dependent=True,
            compute_velocity=lambda r, d, e: bubble_rise_velocity_woolf(
                r, d, e, clean=(e.bubble_surface_condition == "clean")
            )
        ),
    }
    
    if model_name not in models:
        raise ValueError(
            f"Unknown model '{model_name}'. Available: {list(models.keys())}"
        )
    
    return models[model_name]
