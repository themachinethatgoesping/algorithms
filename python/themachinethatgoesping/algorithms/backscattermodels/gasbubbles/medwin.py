"""
Medwin (1977) / Clay & Medwin bubble backscattering model.

Implementation of the classic damped resonant bubble scattering model from:
- Medwin, H. (1977). "Counting bubbles acoustically: A review." Ultrasonics, 15, 7–13.
- Clay, C.S. & Medwin, H. (1998). Fundamentals of Acoustical Oceanography. Academic Press.
- Devin, C. Jr. (1959). "Survey of thermal, radiation, and viscous damping of 
  pulsating air bubbles in water." J. Acoust. Soc. Am., 31, 1654–1667.

This is the foundational model for bubble acoustics, treating the bubble as a
damped harmonic oscillator with three damping mechanisms:
1. Radiation damping (re-radiation of sound)
2. Thermal damping (heat conduction between gas and liquid)
3. Viscous damping (boundary layer effects)

The model is valid when ka << 1 (long wavelength approximation).
"""

import math
from math import pi

from .parameters import EnvironmentalParameters


def calculate_resonance_frequency(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate Minnaert resonance frequency of a bubble.
    
    The Minnaert (1933) frequency represents the natural pulsation frequency
    of a spherical gas bubble in a liquid.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    float
        Resonance frequency (Hz)
    
    Notes
    -----
    The Minnaert frequency is given by:
    
    f_0 = (1 / 2πa) * sqrt(3γP_0 / ρ)
    
    where:
    - a = bubble radius
    - γ = polytropic exponent (ratio of specific heats)
    - P_0 = ambient static pressure
    - ρ = liquid density
    
    References
    ----------
    Minnaert, M. (1933). "On musical air-bubbles and the sound of running water."
    Philosophical Magazine, 16(104), 235–248.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    depth = abs(waterdepth)
    
    # Static pressure at depth
    P_0 = params.P_atm + params.rho_liq * params.g * depth
    
    # Minnaert resonance frequency
    f_0 = (1 / (2 * pi * a)) * math.sqrt(3 * params.gamma * P_0 / params.rho_liq)
    
    return f_0


def calculate_damping_radiation(
    frequency: float,
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate radiation damping coefficient (dimensionless).
    
    Radiation damping arises from the re-radiation of acoustic energy
    by the pulsating bubble.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Radiation damping coefficient (dimensionless)
    
    Notes
    -----
    δ_rad = ka = ωa/c = 2πfa/c
    
    At resonance, this equals the bubble radius divided by the wavelength.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    omega = 2 * pi * frequency
    
    return omega * a / params.cw


def calculate_damping_thermal(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate thermal damping coefficient (dimensionless).
    
    Thermal damping arises from heat conduction between the compressed/expanded
    gas and the surrounding liquid during bubble oscillation.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Thermal damping coefficient (dimensionless)
    
    Notes
    -----
    Following Devin (1959):
    
    δ_th = 3(γ-1) / a * sqrt(D_g / 2ω)
    
    where D_g is the thermal diffusivity of the gas.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    depth = abs(waterdepth)
    omega = 2 * pi * frequency
    
    # Gas pressure inside bubble
    P_gas = params.P_atm + params.rho_liq * params.g * depth + 2 * params.tau / a
    
    # Gas density from ideal gas law
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    
    # Thermal diffusivity of gas
    D_g = params.K_gas / (rho_gas * params.Cp)
    
    # Thermal damping (Devin 1959)
    delta_th = 3 * (params.gamma - 1) / a * math.sqrt(D_g / (2 * omega))
    
    return delta_th


def calculate_damping_viscous(
    frequency: float,
    bubble_radius: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate viscous damping coefficient (dimensionless).
    
    Viscous damping arises from the oscillatory boundary layer at the
    bubble surface.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Viscous damping coefficient (dimensionless)
    
    Notes
    -----
    δ_vis = 4η / (ρωa²)
    
    where η is the dynamic viscosity of the liquid.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    omega = 2 * pi * frequency
    
    # Viscous damping
    delta_vis = 4 * params.eta_s / (params.rho_liq * omega * a * a)
    
    return delta_vis


def calculate_total_damping(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate total damping coefficient (dimensionless).
    
    The total damping is the sum of radiation, thermal, and viscous components.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Total damping coefficient (dimensionless)
    """
    if params is None:
        params = EnvironmentalParameters()
    
    delta_rad = calculate_damping_radiation(frequency, bubble_radius, params)
    delta_th = calculate_damping_thermal(frequency, bubble_radius, waterdepth, params)
    delta_vis = calculate_damping_viscous(frequency, bubble_radius, params)
    
    return delta_rad + delta_th + delta_vis


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Medwin (1977) model.
    
    The classic damped resonant bubble model, treating the bubble as a
    simple harmonic oscillator.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m), positive downward or absolute value used
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    float
        Backscattering cross-section (m²)
    
    Notes
    -----
    The Medwin model is:
    
    σ_bs = a² / [(f_0²/f² - 1)² + δ²]
    
    where:
    - a = bubble radius
    - f_0 = Minnaert resonance frequency
    - f = acoustic frequency
    - δ = total damping coefficient (radiation + thermal + viscous)
    
    This model assumes ka << 1 (long wavelength approximation).
    At resonance (f = f_0), σ_bs = a²/δ² which is finite due to damping.
    
    References
    ----------
    Medwin, H. (1977). "Counting bubbles acoustically: A review." Ultrasonics, 15, 7–13.
    Clay, C.S. & Medwin, H. (1998). Fundamentals of Acoustical Oceanography.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    
    # Resonance frequency
    f_0 = calculate_resonance_frequency(a, waterdepth, params)
    
    # Total damping
    delta = calculate_total_damping(frequency, a, waterdepth, params)
    
    # Frequency ratio term
    freq_term = (f_0 / frequency) ** 2 - 1
    
    # Backscattering cross-section
    denominator = freq_term ** 2 + delta ** 2
    
    if denominator < 1e-30:
        denominator = 1e-30  # Avoid division by zero
    
    sigma_bs = a ** 2 / denominator
    
    return sigma_bs


def calculate_target_strength(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate target strength in dB from the backscattering cross-section.
    
    TS = 10 * log10(σ_bs)
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Target strength (dB re 1 m²)
    """
    sigma_bs = calculate_sigma_bs(frequency, bubble_radius, waterdepth, params)
    
    if sigma_bs <= 0:
        return -float('inf')
    
    return 10 * math.log10(sigma_bs)


def calculate_quality_factor(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate the quality factor Q of the bubble resonator.
    
    The quality factor describes the sharpness of the resonance peak.
    Higher Q means sharper resonance.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Quality factor (dimensionless)
    
    Notes
    -----
    Q = 1 / δ_total
    
    where δ_total is evaluated at the resonance frequency.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Calculate resonance frequency
    f_0 = calculate_resonance_frequency(bubble_radius, waterdepth, params)
    
    # Calculate damping at resonance
    delta = calculate_total_damping(f_0, bubble_radius, waterdepth, params)
    
    if delta < 1e-30:
        return float('inf')
    
    return 1 / delta
