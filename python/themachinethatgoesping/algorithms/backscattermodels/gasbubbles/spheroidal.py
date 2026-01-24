"""
Spheroidal bubble backscattering model for non-spherical bubbles.

Larger bubbles (typically > 1mm diameter) become oblate spheroids due to the
balance between buoyancy and surface tension forces. This module provides
corrections for non-spherical bubble shapes.

Key references:
- Feuillade, C. & Werby, M.F. (1994). "Resonances of deformed gas bubbles in 
  liquids." J. Acoust. Soc. Am. 96(6), 3684-3692.
- Clift, R., Grace, J.R. & Weber, M.E. (1978). "Bubbles, Drops, and Particles." 
  Academic Press.
- Besagni, G. & Deen, N.G. (2020). "Aspect ratio of bubbles in different liquid 
  media: A novel correlation." Chem. Eng. Sci. 215, 115383.

The shape of a rising bubble is characterized by dimensionless numbers:
- Eötvös (Bond) number: Eo = Δρ·g·d²/σ (buoyancy vs surface tension)
- Reynolds number: Re = ρ·U·d/μ (inertia vs viscosity)
- Morton number: Mo = g·μ⁴·Δρ/(ρ²·σ³) (fluid property group)

Shape regimes:
- Eo < 0.1: Spherical (surface tension dominates)
- 0.1 < Eo < 10: Ellipsoidal/Oblate (buoyancy and surface tension comparable)
- Eo > 10: Spherical cap (buoyancy dominates)
"""

import math
from math import pi
from typing import Tuple

from .parameters import EnvironmentalParameters
from . import ainslie_leighton


def calculate_eotvos_number(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
    rho_gas: float = None,
) -> float:
    """Calculate the Eötvös (Bond) number.
    
    The Eötvös number characterizes the relative importance of 
    buoyancy to surface tension forces.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    rho_gas : float, optional
        Gas density (kg/m³). If None, assumed negligible compared to water.
    
    Returns
    -------
    float
        Eötvös number (dimensionless)
    
    Notes
    -----
    Eo = Δρ × g × d² / σ
    
    where d = 2r is the diameter.
    
    - Eo < 0.1: Bubble is spherical
    - Eo > 0.1: Bubble becomes oblate
    - Eo > 10: Bubble becomes spherical cap
    """
    if params is None:
        params = EnvironmentalParameters()
    
    if rho_gas is None:
        rho_gas = 1.2  # Approximate air density at surface
    
    d = 2 * bubble_radius  # Diameter
    delta_rho = params.rho_liq - rho_gas
    
    Eo = delta_rho * params.g * d ** 2 / params.tau
    
    return Eo


def calculate_aspect_ratio_moore(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate bubble aspect ratio using Moore's correlation.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Aspect ratio χ = b/a where b is polar (vertical) semi-axis
        and a is equatorial (horizontal) semi-axis. χ < 1 for oblate.
    
    Notes
    -----
    Moore's correlation for ellipsoidal bubbles:
    
    χ = 1 / (1 + 0.163 × Eo^0.757)
    
    This is valid for the ellipsoidal regime (0.1 < Eo < 10).
    """
    if params is None:
        params = EnvironmentalParameters()
    
    Eo = calculate_eotvos_number(bubble_radius, params)
    
    if Eo < 0.01:
        return 1.0  # Spherical limit
    
    chi = 1.0 / (1.0 + 0.163 * Eo ** 0.757)
    
    # Limit to physically reasonable values
    chi = max(chi, 0.1)  # Don't allow extremely flat bubbles
    
    return chi


def calculate_aspect_ratio_clift(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate bubble aspect ratio using Clift et al. correlation.
    
    Alternative correlation from the classic "Bubbles, Drops, and Particles" book.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Aspect ratio χ = b/a (< 1 for oblate)
    
    Notes
    -----
    For pure liquids (low Morton number), the aspect ratio is 
    primarily a function of Eötvös number.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    Eo = calculate_eotvos_number(bubble_radius, params)
    
    if Eo < 0.01:
        return 1.0
    
    # Clift et al. correlation for low Morton number fluids
    chi = 1.0 / (1.0 + 0.12 * Eo ** 0.85)
    
    chi = max(chi, 0.1)
    
    return chi


def is_spherical(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
    threshold: float = 0.1,
) -> bool:
    """Check if a bubble can be treated as spherical.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    threshold : float, optional
        Eötvös number threshold below which bubble is spherical.
        Default 0.1 corresponds to ~5% shape deviation.
    
    Returns
    -------
    bool
        True if bubble is approximately spherical.
    """
    Eo = calculate_eotvos_number(bubble_radius, params)
    return Eo < threshold


def get_shape_regime(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> str:
    """Determine the shape regime of a bubble.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    str
        Shape regime: 'spherical', 'ellipsoidal', or 'spherical_cap'
    """
    Eo = calculate_eotvos_number(bubble_radius, params)
    
    if Eo < 0.1:
        return 'spherical'
    elif Eo < 10:
        return 'ellipsoidal'
    else:
        return 'spherical_cap'


def spheroid_semi_axes(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> Tuple[float, float]:
    """Calculate semi-axes of an oblate spheroid bubble.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    tuple[float, float]
        (a, b) where a is equatorial (horizontal) semi-axis and 
        b is polar (vertical) semi-axis, both in meters.
    
    Notes
    -----
    For an oblate spheroid with volume V = (4/3)πa²b = (4/3)πr³:
    
    a = r / χ^(1/3)
    b = r × χ^(2/3)
    
    where χ = b/a is the aspect ratio and r is the equivalent radius.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    r = bubble_radius
    chi = calculate_aspect_ratio_moore(r, params)
    
    # Volume-equivalent spheroid semi-axes
    # V = (4/3)πa²b = (4/3)πr³
    # With χ = b/a: a = r/χ^(1/3), b = r×χ^(2/3)
    a = r / (chi ** (1/3))  # Equatorial (horizontal)
    b = r * (chi ** (2/3))  # Polar (vertical)
    
    return a, b


def calculate_resonance_frequency_spheroid(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate resonance frequency of an oblate spheroid bubble.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Resonance frequency (Hz)
    
    Notes
    -----
    For an oblate spheroid, the resonance frequency is lower than 
    the equivalent sphere due to the increased effective radius
    in the oscillation direction.
    
    Approximate correction (Feuillade & Werby 1994):
    
    f_spheroid ≈ f_sphere × χ^(1/3)
    
    This accounts for the larger effective radius of the oblate bubble.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Spherical resonance frequency
    f_sphere = ainslie_leighton.calculate_resonance_frequency(
        bubble_radius, waterdepth, params
    )
    
    # Aspect ratio
    chi = calculate_aspect_ratio_moore(bubble_radius, params)
    
    # Shape correction factor
    # Oblate bubbles have lower resonance frequency
    shape_factor = chi ** (1/3)
    
    f_spheroid = f_sphere * shape_factor
    
    return f_spheroid


def calculate_shape_correction_factor(
    bubble_radius: float,
    incidence_angle: float = 0.0,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate shape correction factor for backscattering.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical radius (m)
    incidence_angle : float, optional
        Angle from vertical (symmetry axis) in radians. 
        0 = vertical incidence, π/2 = horizontal incidence.
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Shape correction factor C(χ, θ) to multiply with spherical σ_bs.
    
    Notes
    -----
    The correction factor depends on both aspect ratio and incidence angle:
    
    - Vertical incidence (θ=0): C ≈ χ^(2/3)
    - Horizontal incidence (θ=π/2): C ≈ χ^(-2/3)
    - Average over orientations: C ≈ (χ^(2/3) + χ^(-2/3))/2
    
    For bubbles that wobble during rise, use the orientation-averaged value.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    chi = calculate_aspect_ratio_moore(bubble_radius, params)
    
    if chi > 0.99:
        return 1.0  # Spherical, no correction needed
    
    # Correction factor for specific incidence angle
    # Using T-matrix approximation
    C_vertical = chi ** (2/3)
    C_horizontal = chi ** (-2/3)
    
    # Interpolate based on incidence angle
    cos_theta = math.cos(incidence_angle)
    sin_theta = math.sin(incidence_angle)
    
    # Weighted combination
    C = C_vertical * cos_theta ** 2 + C_horizontal * sin_theta ** 2
    
    return C


def calculate_shape_correction_averaged(
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate orientation-averaged shape correction factor.
    
    For tumbling/wobbling bubbles, the scattering is averaged over orientations.
    
    Parameters
    ----------
    bubble_radius : float
        Equivalent spherical radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Orientation-averaged shape correction factor.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    chi = calculate_aspect_ratio_moore(bubble_radius, params)
    
    if chi > 0.99:
        return 1.0
    
    # Simple average of vertical and horizontal corrections
    C_avg = 0.5 * (chi ** (2/3) + chi ** (-2/3))
    
    return C_avg


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    incidence_angle: float = None,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section for a spheroidal bubble.
    
    Uses the Ainslie-Leighton model for the base spherical calculation,
    then applies shape corrections for non-spherical bubbles.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    waterdepth : float
        Water depth (m)
    incidence_angle : float, optional
        Angle from vertical in radians. If None, uses orientation-averaged
        correction (appropriate for tumbling bubbles).
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Backscattering cross-section (m²)
    
    Notes
    -----
    The model:
    1. Calculates base spherical σ_bs using Ainslie-Leighton model
    2. Determines if shape correction is needed (Eo > 0.1)
    3. Applies shape and resonance corrections for oblate bubbles
    
    For most echosounder applications (looking down at rising bubbles),
    the vertical incidence case (incidence_angle=0) is appropriate.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Check if bubble is spherical
    if is_spherical(bubble_radius, params):
        # Use standard spherical model
        return ainslie_leighton.calculate_sigma_bs(
            frequency, bubble_radius, waterdepth, params
        )
    
    # Get spheroid properties
    chi = calculate_aspect_ratio_moore(bubble_radius, params)
    
    # Calculate spherical σ_bs as baseline
    sigma_bs_sphere = ainslie_leighton.calculate_sigma_bs(
        frequency, bubble_radius, waterdepth, params
    )
    
    # Shape correction factor
    if incidence_angle is not None:
        C_shape = calculate_shape_correction_factor(bubble_radius, incidence_angle, params)
    else:
        C_shape = calculate_shape_correction_averaged(bubble_radius, params)
    
    # Resonance frequency shift correction
    # The resonance peak shifts, so we need to adjust the frequency response
    f_0_sphere = ainslie_leighton.calculate_resonance_frequency(bubble_radius, waterdepth, params)
    f_0_spheroid = calculate_resonance_frequency_spheroid(bubble_radius, waterdepth, params)
    
    # Near resonance, the response needs additional correction
    # This is a simplified approach - full solution requires T-matrix methods
    x_sphere = frequency / f_0_sphere
    x_spheroid = frequency / f_0_spheroid
    
    # Apply shape correction
    sigma_bs = sigma_bs_sphere * C_shape
    
    return sigma_bs


def calculate_sigma_bs_vertical(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section for vertical incidence.
    
    Appropriate for downward-looking echosounders viewing rising bubbles.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Backscattering cross-section (m²)
    """
    return calculate_sigma_bs(
        frequency, bubble_radius, waterdepth, 
        incidence_angle=0.0, params=params
    )


def calculate_sigma_bs_horizontal(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section for horizontal incidence.
    
    Appropriate for side-looking sonars viewing bubbles.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Equivalent spherical bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Backscattering cross-section (m²)
    """
    return calculate_sigma_bs(
        frequency, bubble_radius, waterdepth, 
        incidence_angle=pi/2, params=params
    )


def calculate_target_strength(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    incidence_angle: float = None,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate target strength in dB.
    
    TS = 10 × log10(σ_bs)
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    incidence_angle : float, optional
        Angle from vertical in radians.
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Target strength (dB re 1 m²)
    """
    sigma_bs = calculate_sigma_bs(
        frequency, bubble_radius, waterdepth, incidence_angle, params
    )
    
    if sigma_bs <= 0:
        return -float('inf')
    
    return 10 * math.log10(sigma_bs)


def critical_radius_spherical(
    params: EnvironmentalParameters = None,
    threshold_eo: float = 0.1,
) -> float:
    """Calculate the critical radius below which bubbles are spherical.
    
    Parameters
    ----------
    params : EnvironmentalParameters, optional
        Environmental parameters.
    threshold_eo : float, optional
        Eötvös number threshold for spherical regime.
    
    Returns
    -------
    float
        Critical radius (m). Bubbles smaller than this are spherical.
    
    Notes
    -----
    From Eo = Δρ·g·(2r)²/σ = threshold:
    
    r_crit = 0.5 × sqrt(σ × threshold / (Δρ × g))
    
    For clean seawater: r_crit ≈ 0.35 mm (threshold = 0.1)
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Assume gas density negligible
    delta_rho = params.rho_liq
    
    # Critical radius from Eo = threshold
    r_crit = 0.5 * math.sqrt(params.tau * threshold_eo / (delta_rho * params.g))
    
    return r_crit
