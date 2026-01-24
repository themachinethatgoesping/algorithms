"""
Stanton (1989) high-pass bubble backscattering model.

Implementation of the heuristic high-pass model from:
Stanton, T.K. (1989). "Simple approximate formulas for backscattering of sound 
by spherical and elongated objects." J. Acoust. Soc. Am., 86(4), 1499–1510.
DOI: 10.1121/1.398711

The high-pass model combines low-frequency (Rayleigh) and high-frequency 
(geometric) scattering limits into a simple closed-form expression. It provides
reasonable accuracy across a wide frequency range without requiring detailed
knowledge of material properties.

For gas bubbles, this model captures the transition from resonant monopole
scattering at low frequencies to geometric scattering at high frequencies.
"""

import math
from math import pi

from .parameters import EnvironmentalParameters


def calculate_resonance_frequency(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate Minnaert resonance frequency.
    
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
        Resonance frequency (Hz)
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


def calculate_damping_total(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate total dimensionless damping coefficient.
    
    Combines radiation, thermal, and viscous damping following Devin (1959).
    
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
        Total dimensionless damping coefficient
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    depth = abs(waterdepth)
    omega = 2 * pi * frequency
    
    # Wavenumber
    k = omega / params.cw
    ka = k * a
    
    # Gas properties
    P_gas = params.P_atm + params.rho_liq * params.g * depth + 2 * params.tau / a
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    D_g = params.K_gas / (rho_gas * params.Cp)
    
    # Resonance frequency for reference
    f_0 = calculate_resonance_frequency(a, waterdepth, params)
    omega_0 = 2 * pi * f_0
    
    # Radiation damping (Stanton formulation)
    delta_rad = ka
    
    # Thermal damping (Devin 1959)
    delta_th = 3 * (params.gamma - 1) / a * math.sqrt(D_g / (2 * omega_0))
    
    # Viscous damping
    delta_vis = 4 * params.eta_s / (params.rho_liq * omega_0 * a * a)
    
    # Total damping
    delta_total = delta_rad + delta_th + delta_vis
    
    return delta_total


def calculate_form_function(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> complex:
    """Calculate the backscattering form function using high-pass model.
    
    The form function f_bs relates backscattering amplitude to geometric size.
    
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
    complex
        Backscattering form function (dimensionless)
    
    Notes
    -----
    Stanton's high-pass model uses:
    
    f_bs = f_LF × f_HF / sqrt(f_LF² + f_HF²)
    
    For gas bubbles near resonance, the monopole dominates with:
    
    f_bs ≈ a / [(f_0/f)² - 1 + iδ]
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    omega = 2 * pi * frequency
    k = omega / params.cw
    ka = k * a
    
    # Resonance frequency
    f_0 = calculate_resonance_frequency(a, waterdepth, params)
    
    # Frequency ratio
    x = frequency / f_0
    
    # Damping
    delta = calculate_damping_total(frequency, a, waterdepth, params)
    
    # Form function denominator
    # Real part: (1 - x²) from resonance
    # Imaginary part: δx from damping
    denominator = complex((1 / (x * x)) - 1, delta)
    
    # Form function (normalized to radius)
    f_bs = 1.0 / denominator
    
    return f_bs


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Stanton high-pass model.
    
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
    The backscattering cross-section is:
    
    σ_bs = a² |f_bs|²
    
    where f_bs is the form function from the high-pass model.
    
    For a resonant gas bubble:
    
    σ_bs = a² / [(f_0²/f² - 1)² + δ²]
    
    This is equivalent to the Medwin model but derived from
    Stanton's more general high-pass framework.
    
    References
    ----------
    Stanton, T.K. (1989). J. Acoust. Soc. Am. 86(4), 1499-1510.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    
    # Form function
    f_bs = calculate_form_function(frequency, a, waterdepth, params)
    
    # Backscattering cross-section
    sigma_bs = a * a * abs(f_bs) ** 2
    
    return sigma_bs


def calculate_sigma_bs_high_frequency(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate high-frequency limit of backscattering cross-section.
    
    At high frequencies (f >> f_0), the bubble acts as a weak scatterer
    with cross-section approaching the geometric limit.
    
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
        Backscattering cross-section (m²)
    
    Notes
    -----
    At high frequencies (ka >> 1, f >> f_0):
    
    σ_bs → (a/2)² × |R|²
    
    where R is the reflection coefficient at the bubble surface.
    For a gas bubble, |R| ≈ 1, giving σ_bs → a²/4.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    
    # High-frequency (geometric) limit
    # Reflection coefficient magnitude for gas-water interface ≈ 1
    R_mag = 1.0
    
    sigma_bs_hf = (a / 2) ** 2 * R_mag ** 2
    
    return sigma_bs_hf


def calculate_sigma_bs_combined(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering using Stanton's heuristic combination.
    
    This smoothly interpolates between resonant and geometric scattering.
    
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
        Backscattering cross-section (m²)
    
    Notes
    -----
    Stanton's high-pass combination:
    
    σ_bs = σ_LF × σ_HF / (σ_LF + σ_HF)
    
    This ensures smooth transition and correct asymptotic behavior.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Resonant (low-frequency dominant) contribution
    sigma_lf = calculate_sigma_bs(frequency, bubble_radius, waterdepth, params)
    
    # Geometric (high-frequency) contribution  
    sigma_hf = calculate_sigma_bs_high_frequency(frequency, bubble_radius, waterdepth, params)
    
    # Heuristic combination (harmonic mean style)
    if sigma_lf + sigma_hf < 1e-50:
        return 0.0
    
    sigma_bs = sigma_lf * sigma_hf / (sigma_lf + sigma_hf)
    
    return sigma_bs


def calculate_target_strength(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
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
