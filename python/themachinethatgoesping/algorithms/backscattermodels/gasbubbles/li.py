"""
Li et al. (2020) bubble backscattering model.

Implementation of the acoustic backscattering cross-section model from:
Li, J., Roche, B., Bull, J.M., White, P.R., Leighton, T.G., Provenzano, G., 
Dewar, M., & Henstock, T.J. (2020). Broadband Acoustic Inversion for Gas Flux 
Quantification—Application to a Methane Plume at Scanner Pockmark, Central North Sea. 
Journal of Geophysical Research: Oceans, 125(9), e2020JC016360.
https://doi.org/10.1029/2020JC016360

This model combines the Ainslie & Leighton (2009, 2011) formulation with
Thuraisingham's (1997) correction factor for finite kr effects.
"""

import math
import cmath
from math import pi

from .parameters import EnvironmentalParameters
import numpy as np


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Li et al. (2020) model.
    
    This model is adapted from Ainslie & Leighton (2009, 2011) and includes
    Thuraisingham's (1997) correction factor for finite kr effects. It properly
    handles the case when kr is not much smaller than 1, which is common when
    using high-frequency echo sounders to image large bubbles.
    
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
        The model uses equation (5) from Li et al. (2020):
    
        σ_bs = r² / [((ω₀²/ω²) - 1 - (2β₀/ω)kr)² + ((2β₀/ω) + (ω₀²/ω²)kr)²]
            × [sin(kr)/(kr)]² / [1 + (kr)²]
    
    where:
    - r is the bubble radius
    - ω is the angular frequency of the incident sound
    - ω₀ is the resonance angular frequency (from Eq. 6)
    - β₀ is the combined thermal and viscous damping coefficient
    - c_w is the sound speed in water
    - k is the wave number
    
    The final factor [sin(kr)/(kr)]² / [1 + (kr)²] is Thuraisingham's correction
    which accounts for finite kr effects when the long-wavelength assumption breaks down.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    r = bubble_radius
    freq = frequency
    depth = abs(waterdepth)
    
    # Angular frequency
    omega = 2 * pi * freq
    
    # Wave number
    k = omega / params.cw
    
    # Gas pressure inside bubble (Eq. 8)
    # P_gas = P_atm + rho_liq * g * d - p_v + 2*tau/r
    P_gas = params.P_atm + params.rho_liq * params.g * depth - params.pv + 2 * params.tau / r
    
    # Gas density (Eq. 12)
    # rho_gas = P_gas * M_m / (R * T)
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    
    # Thermal diffusivity of gas (Eq. 11)
    # D_p = K_gas / (rho_gas * C_p)
    D_p = params.K_gas / (rho_gas * params.Cp)
    
    # Dimensionless thermal parameter X (Eq. 10)
    # X = r * sqrt(2 * omega / D_p)
    X = r * math.sqrt(2 * omega / D_p)
    
    # Complex polytropic index Gamma (Eq. 9)
    # Gamma = gamma / [1 - ((1+i)X/2)/tanh((1+i)X/2) * (6i(γ-1)/X²)]
    X_complex = (1 + 1j) * X / 2
    
    # Handle large X to avoid overflow in cosh/sinh
    # For large |X|, coth(X) → 1, and the thermal correction term → 0
    # meaning Gamma → gamma (adiabatic limit)
    if X > 500:
        # Adiabatic limit: large X means thermal effects are negligible
        Gamma = complex(params.gamma, 0)
    elif X < 1e-6:
        # Isothermal limit: very small X
        Gamma = complex(1.0, 0)
    else:
        try:
            tanh_X = cmath.tanh(X_complex)
            Gamma = params.gamma / (1 - (X_complex / tanh_X) * (6j * (params.gamma - 1) / (X * X)))
        except (OverflowError, ZeroDivisionError):
            # Fallback to adiabatic limit
            Gamma = complex(params.gamma, 0)
    
    # Complex stiffness parameter Omega^2 (Eq. 7)
    # Omega^2 = (1 / (rho_liq * r²)) * (3 * Gamma * P_gas - 2 * tau / r)
    Omega2 = (1 / (params.rho_liq * r * r)) * (3 * Gamma * P_gas - 2 * params.tau / r)
    
    # Resonance angular frequency omega_0 (from Eq. 6)
    # omega_0² = Re{Omega^2}
    omega_0_sq = Omega2.real
    if omega_0_sq <= 0:
        omega_0_sq = 1e-20  # Avoid negative or zero values
    omega_0 = math.sqrt(omega_0_sq)
    
    # Thermal damping coefficient (Eq. 14a)
    # beta_th = Im{Omega^2} / (2 * omega)
    beta_th = Omega2.imag / (2 * omega)
    
    # Viscous damping coefficient (Eq. 14b)
    # beta_vis = 2 * eta_S / (rho_liq * r²)
    beta_vis = 2 * params.eta_s / (params.rho_liq * r * r)
    
    # Combined damping coefficient beta_0 (Eq. 13)
    beta_0 = beta_th + beta_vis
    
    # Thuraisingham (1997) correction factor
    # [sin(kr)/(kr)]² / [1 + (kr)²]
    kr = k * r
    if kr < 1e-10:
        # Small kr limit: sin(kr)/kr -> 1, factor -> 1
        thur_factor = 1.0
    else:
        sinc_kr = math.sin(kr) / kr
        thur_factor = (sinc_kr ** 2) / (1 + kr ** 2)
    
    # Backscattering cross-section (Eq. 5)
    # sigma_bs = r² / [((ω₀²/ω²) - 1 - (2β₀/ω)kr)² + ((2β₀/ω) + (ω₀²/ω²)kr)²] × thur_factor
    
    omega_ratio_sq = (omega_0 / omega) ** 2
    
    term_1 = omega_ratio_sq - 1 - (2 * beta_0 / omega) * kr
    term_2 = (2 * beta_0 / omega) + omega_ratio_sq * kr
    
    denominator = term_1 ** 2 + term_2 ** 2
    
    if denominator < 1e-30:
        denominator = 1e-30  # Avoid division by zero at resonance
    
    sigma_bs = (r ** 2 / denominator) * thur_factor
    
    return sigma_bs


def calculate_target_strength(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate target strength in dB from the backscattering cross-section.
    
    TS = 10 * log10(sigma_bs / (4 * pi))
    
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
    
    # Target strength: TS = 10 * log10(sigma_bs / (4*pi))
    # Using the convention that sigma_bs = 4*pi * r_eq² where r_eq is equivalent sphere radius
    if sigma_bs <= 0:
        return -float('inf')
    
    return 10 * math.log10(sigma_bs / (4 * pi))


def calculate_resonance_frequency(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate the resonance frequency of a bubble.
    
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
    
    r = bubble_radius
    depth = abs(waterdepth)
    
    # Gas pressure inside bubble
    P_gas = params.P_atm + params.rho_liq * params.g * depth - params.pv + 2 * params.tau / r
    
    # Simplified Minnaert resonance frequency (isothermal case)
    # f_0 = (1 / (2*pi*r)) * sqrt(3 * gamma * P_gas / rho_liq)
    f_0 = (1 / (2 * pi * r)) * math.sqrt(3 * params.gamma * P_gas / params.rho_liq)
    
    return f_0

