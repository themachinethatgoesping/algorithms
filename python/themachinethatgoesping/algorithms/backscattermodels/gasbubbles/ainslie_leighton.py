"""
Ainslie & Leighton (2009, 2011) comprehensive bubble backscattering model.

Implementation of the rigorous bubble scattering model from:
- Ainslie, M.A. & Leighton, T.G. (2009). "Near resonant bubble acoustic 
  cross-section corrections, including examples from oceanography, volcanology, 
  and biomedical ultrasound." J. Acoust. Soc. Am., 126(5), 2163–2175.
- Ainslie, M.A. & Leighton, T.G. (2011). "Review of scattering and extinction 
  cross-sections, damping factors, and resonance frequencies of a spherical 
  gas bubble." J. Acoust. Soc. Am., 130(5), 3184–3208.

This model provides a comprehensive treatment including:
1. Complex polytropic exponent for thermal effects (Prosperetti 1977)
2. Surface tension corrections to resonance frequency
3. Thermal and viscous damping with proper frequency dependence
4. Near-resonant corrections for finite ka effects

This is the most accurate analytic model for single bubble scattering
across a wide range of frequencies and bubble sizes.
"""

import math
import cmath
from math import pi

from .parameters import EnvironmentalParameters


def calculate_gas_properties(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters,
) -> tuple[float, float, float]:
    """Calculate gas pressure, density, and thermal diffusivity.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters
        Environmental parameters
    
    Returns
    -------
    tuple[float, float, float]
        Gas pressure (Pa), gas density (kg/m³), thermal diffusivity (m²/s)
    """
    a = bubble_radius
    depth = abs(waterdepth)
    
    # Gas pressure inside bubble including surface tension (Young-Laplace)
    P_gas = params.P_atm + params.rho_liq * params.g * depth - params.pv + 2 * params.tau / a
    
    # Gas density from ideal gas law
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    
    # Thermal diffusivity of gas
    D_g = params.K_gas / (rho_gas * params.Cp)
    
    return P_gas, rho_gas, D_g


def calculate_complex_polytropic_index(
    frequency: float,
    bubble_radius: float,
    thermal_diffusivity: float,
    gamma: float,
) -> complex:
    """Calculate the complex polytropic index Γ.
    
    The complex polytropic index accounts for the transition between
    isothermal (Γ = 1) and adiabatic (Γ = γ) behavior depending on
    the thermal penetration depth relative to bubble size.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    thermal_diffusivity : float
        Thermal diffusivity of gas (m²/s)
    gamma : float
        Ratio of specific heats
    
    Returns
    -------
    complex
        Complex polytropic index Γ
    
    Notes
    -----
    Following Prosperetti (1977):
    
    Γ = γ / [1 - (1+i)X/2 / tanh((1+i)X/2) × 6i(γ-1)/X²]
    
    where X = a × sqrt(2ω/D_g) is the dimensionless thermal parameter.
    
    - For X >> 1 (high frequency or large bubble): Γ → γ (adiabatic)
    - For X << 1 (low frequency or small bubble): Γ → 1 (isothermal)
    """
    a = bubble_radius
    omega = 2 * pi * frequency
    D_g = thermal_diffusivity
    
    # Dimensionless thermal parameter
    X = a * math.sqrt(2 * omega / D_g)
    
    # Handle limiting cases to avoid numerical issues
    if X > 500:
        # Adiabatic limit
        return complex(gamma, 0)
    elif X < 1e-6:
        # Isothermal limit
        return complex(1.0, 0)
    
    try:
        X_complex = (1 + 1j) * X / 2
        tanh_X = cmath.tanh(X_complex)
        
        thermal_correction = (X_complex / tanh_X) * (6j * (gamma - 1) / (X * X))
        Gamma = gamma / (1 - thermal_correction)
        
    except (OverflowError, ZeroDivisionError):
        Gamma = complex(gamma, 0)
    
    return Gamma


def calculate_complex_stiffness(
    bubble_radius: float,
    gas_pressure: float,
    polytropic_index: complex,
    params: EnvironmentalParameters,
) -> complex:
    """Calculate the complex stiffness parameter Ω².
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    gas_pressure : float
        Gas pressure inside bubble (Pa)
    polytropic_index : complex
        Complex polytropic index Γ
    params : EnvironmentalParameters
        Environmental parameters
    
    Returns
    -------
    complex
        Complex stiffness parameter Ω² (rad²/s²)
    
    Notes
    -----
    Ω² = (1 / ρa²) × (3ΓP_g - 2τ/a)
    
    The real part gives the resonance frequency squared.
    The imaginary part contributes to thermal damping.
    """
    a = bubble_radius
    
    Omega2 = (1 / (params.rho_liq * a * a)) * (
        3 * polytropic_index * gas_pressure - 2 * params.tau / a
    )
    
    return Omega2


def calculate_resonance_frequency(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate the resonance frequency including thermal and surface tension effects.
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters
    
    Returns
    -------
    float
        Resonance frequency (Hz)
    
    Notes
    -----
    ω_0 = sqrt(Re{Ω²})
    f_0 = ω_0 / (2π)
    
    This differs from the simple Minnaert frequency by including:
    1. Surface tension correction (significant for small bubbles)
    2. Thermal effects via the polytropic index
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    
    # Gas properties
    P_gas, rho_gas, D_g = calculate_gas_properties(a, waterdepth, params)
    
    # For resonance frequency calculation, use a representative frequency
    # to get the polytropic index (iterate if needed for precision)
    # Start with Minnaert estimate
    f_estimate = (1 / (2 * pi * a)) * math.sqrt(3 * params.gamma * P_gas / params.rho_liq)
    
    # Calculate polytropic index at estimated frequency
    Gamma = calculate_complex_polytropic_index(f_estimate, a, D_g, params.gamma)
    
    # Complex stiffness
    Omega2 = calculate_complex_stiffness(a, P_gas, Gamma, params)
    
    # Resonance angular frequency
    omega_0_sq = Omega2.real
    if omega_0_sq <= 0:
        omega_0_sq = 1e-20
    
    omega_0 = math.sqrt(omega_0_sq)
    f_0 = omega_0 / (2 * pi)
    
    return f_0


def calculate_damping_coefficients(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters,
) -> tuple[float, float, float]:
    """Calculate thermal, viscous, and combined damping coefficients.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters
        Environmental parameters
    
    Returns
    -------
    tuple[float, float, float]
        Thermal damping β_th, viscous damping β_vis, total damping β_0 (all in rad/s)
    
    Notes
    -----
    β_th = Im{Ω²} / (2ω)    (thermal damping)
    β_vis = 2η_s / (ρa²)    (viscous damping)
    β_0 = β_th + β_vis       (total damping)
    """
    a = bubble_radius
    omega = 2 * pi * frequency
    
    # Gas properties
    P_gas, rho_gas, D_g = calculate_gas_properties(a, waterdepth, params)
    
    # Complex polytropic index
    Gamma = calculate_complex_polytropic_index(frequency, a, D_g, params.gamma)
    
    # Complex stiffness
    Omega2 = calculate_complex_stiffness(a, P_gas, Gamma, params)
    
    # Thermal damping coefficient
    beta_th = Omega2.imag / (2 * omega)
    
    # Viscous damping coefficient
    beta_vis = 2 * params.eta_s / (params.rho_liq * a * a)
    
    # Combined damping
    beta_0 = beta_th + beta_vis
    
    return beta_th, beta_vis, beta_0


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Ainslie & Leighton model.
    
    This comprehensive model includes:
    - Complex polytropic exponent for thermal effects
    - Surface tension corrections
    - Thermal and viscous damping
    - Near-resonant corrections for finite ka
    
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
    The backscattering cross-section (Eq. 14 from Ainslie & Leighton 2011):
    
    σ_bs = a² / [((ω_0/ω)² - 1 - (2β_0/ω)ka)² + ((2β_0/ω) + (ω_0/ω)²ka)²]
    
    where:
    - a = bubble radius
    - ω = angular frequency
    - ω_0 = resonance angular frequency (from Re{Ω²})
    - β_0 = combined thermal and viscous damping coefficient
    - k = wavenumber in water
    
    The terms involving ka provide near-resonant corrections that account
    for radiation damping and finite-wavelength effects.
    
    References
    ----------
    Ainslie, M.A. & Leighton, T.G. (2011). JASA 130(5), 3184–3208.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    omega = 2 * pi * frequency
    
    # Wavenumber and ka
    k = omega / params.cw
    ka = k * a
    
    # Gas properties
    P_gas, rho_gas, D_g = calculate_gas_properties(a, waterdepth, params)
    
    # Complex polytropic index
    Gamma = calculate_complex_polytropic_index(frequency, a, D_g, params.gamma)
    
    # Complex stiffness
    Omega2 = calculate_complex_stiffness(a, P_gas, Gamma, params)
    
    # Resonance angular frequency
    omega_0_sq = Omega2.real
    if omega_0_sq <= 0:
        omega_0_sq = 1e-20
    omega_0 = math.sqrt(omega_0_sq)
    
    # Damping coefficients
    beta_th = Omega2.imag / (2 * omega)
    beta_vis = 2 * params.eta_s / (params.rho_liq * a * a)
    beta_0 = beta_th + beta_vis
    
    # Frequency ratio
    omega_ratio_sq = (omega_0 / omega) ** 2
    
    # Damping ratio
    damping_ratio = 2 * beta_0 / omega
    
    # Backscattering cross-section with near-resonant corrections
    term_1 = omega_ratio_sq - 1 - damping_ratio * ka
    term_2 = damping_ratio + omega_ratio_sq * ka
    
    denominator = term_1 ** 2 + term_2 ** 2
    
    if denominator < 1e-30:
        denominator = 1e-30
    
    sigma_bs = a ** 2 / denominator
    
    return sigma_bs


def calculate_sigma_bs_with_thuraisingham(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section with Thuraisingham correction.
    
    This adds the Thuraisingham (1997) correction factor for cases where
    ka is not much smaller than 1, which is common for high-frequency
    echosounders imaging larger bubbles.
    
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
    The Thuraisingham correction factor is:
    
    T(ka) = [sin(ka)/(ka)]² / [1 + (ka)²]
    
    This accounts for the breakdown of the monopole approximation
    when the bubble is not small compared to the wavelength.
    
    References
    ----------
    Thuraisingham, R.A. (1997). Ultrasonics 35, 357–366.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Base cross-section
    sigma_bs_base = calculate_sigma_bs(frequency, bubble_radius, waterdepth, params)
    
    # Thuraisingham correction
    omega = 2 * pi * frequency
    k = omega / params.cw
    ka = k * bubble_radius
    
    if ka < 1e-10:
        thur_factor = 1.0
    else:
        sinc_ka = math.sin(ka) / ka
        thur_factor = (sinc_ka ** 2) / (1 + ka ** 2)
    
    return sigma_bs_base * thur_factor


def calculate_extinction_cross_section(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate extinction cross-section σ_e.
    
    The extinction cross-section represents the total power removed
    from the incident wave (scattering + absorption).
    
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
        Extinction cross-section (m²)
    
    Notes
    -----
    σ_e = 4π Im{f_s} / k
    
    where f_s is the forward scattering amplitude.
    
    For a monopole scatterer:
    σ_e = (4πa² / [(ω_0/ω)² - 1]² + δ²) × δ
    
    where δ is the total damping.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    omega = 2 * pi * frequency
    k = omega / params.cw
    
    # Get damping
    _, _, beta_0 = calculate_damping_coefficients(frequency, a, waterdepth, params)
    
    # Resonance frequency
    f_0 = calculate_resonance_frequency(a, waterdepth, params)
    omega_0 = 2 * pi * f_0
    
    # Dimensionless damping
    delta = 2 * beta_0 / omega
    
    # Frequency ratio
    freq_term = (omega_0 / omega) ** 2 - 1
    
    # Extinction cross-section
    denominator = freq_term ** 2 + delta ** 2
    if denominator < 1e-30:
        denominator = 1e-30
    
    sigma_e = (4 * pi * a ** 2 / denominator) * delta / k
    
    return sigma_e


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
