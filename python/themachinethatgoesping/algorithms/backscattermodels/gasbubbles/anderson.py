"""
Anderson (1950) fluid sphere scattering model.

Implementation of the classic exact solution for scattering from a fluid sphere:
Anderson, V.C. (1950). "Sound scattering from a fluid sphere." 
J. Acoust. Soc. Am., 22(4), 426–431.

This model provides the full modal (partial wave) solution using spherical
Bessel functions. It is valid for all values of ka (not just ka << 1) but
does not include viscous or thermal damping effects.

For gas bubbles where the density contrast is large, this model captures
the monopole-dominated scattering behavior but will show infinite scattering
at exact resonance due to the absence of damping.

Reference implementation validated against:
https://github.com/SvenGastauer/scatmod/blob/master/python/fluid_sphere.py
"""

import math
from math import pi
from typing import Tuple

try:
    import scipy.special as ss
    HAS_SCIPY = True
except ImportError:
    HAS_SCIPY = False

from .parameters import EnvironmentalParameters


def _spherical_jn(n: int, x: float, derivative: bool = False) -> float:
    """Spherical Bessel function of the first kind j_n(x).
    
    Uses scipy if available, otherwise falls back to manual calculation.
    """
    if HAS_SCIPY:
        return ss.spherical_jn(n, x, derivative=derivative)
    
    # Manual implementation for when scipy is not available
    if abs(x) < 1e-15:
        if derivative:
            return 1.0 / 3.0 if n == 1 else 0.0
        return 1.0 if n == 0 else 0.0
    
    if not derivative:
        if n == 0:
            return math.sin(x) / x
        elif n == 1:
            return math.sin(x) / (x * x) - math.cos(x) / x
        else:
            # Upward recurrence
            j_nm1 = math.sin(x) / x
            j_n = math.sin(x) / (x * x) - math.cos(x) / x
            for m in range(1, n):
                j_np1 = ((2 * m + 1) / x) * j_n - j_nm1
                j_nm1 = j_n
                j_n = j_np1
            return j_n
    else:
        # Derivative: j_n'(x) = j_{n-1}(x) - (n+1)/x * j_n(x)
        # For n=0: j_0'(x) = -j_1(x)
        if n == 0:
            return -_spherical_jn(1, x)
        else:
            return _spherical_jn(n - 1, x) - (n + 1) / x * _spherical_jn(n, x)


def _spherical_yn(n: int, x: float, derivative: bool = False) -> float:
    """Spherical Bessel function of the second kind y_n(x) (Neumann function).
    
    Uses scipy if available, otherwise falls back to manual calculation.
    """
    if HAS_SCIPY:
        return ss.spherical_yn(n, x, derivative=derivative)
    
    # Manual implementation
    if abs(x) < 1e-15:
        return -float('inf')
    
    if not derivative:
        if n == 0:
            return -math.cos(x) / x
        elif n == 1:
            return -math.cos(x) / (x * x) - math.sin(x) / x
        else:
            # Upward recurrence
            y_nm1 = -math.cos(x) / x
            y_n = -math.cos(x) / (x * x) - math.sin(x) / x
            for m in range(1, n):
                y_np1 = ((2 * m + 1) / x) * y_n - y_nm1
                y_nm1 = y_n
                y_n = y_np1
            return y_n
    else:
        # Derivative: y_n'(x) = y_{n-1}(x) - (n+1)/x * y_n(x)
        if n == 0:
            return -_spherical_yn(1, x)
        else:
            return _spherical_yn(n - 1, x) - (n + 1) / x * _spherical_yn(n, x)


def _spherical_hankel_1(n: int, x: float) -> complex:
    """Spherical Hankel function of the first kind h_n^(1)(x) = j_n(x) + i*y_n(x)."""
    return complex(_spherical_jn(n, x), _spherical_yn(n, x))


def _spherical_hankel_1_derivative(n: int, x: float) -> complex:
    """Derivative of spherical Hankel function of the first kind."""
    return complex(_spherical_jn(n, x, derivative=True), 
                   _spherical_yn(n, x, derivative=True))


def calculate_Cn_coefficient(
    n: int,
    ka: float,
    k1a: float,
    g: float,
    h: float,
) -> complex:
    """Calculate the C_n coefficient following Anderson (1950).
    
    This is the key coefficient that determines the scattering amplitude.
    
    Parameters
    ----------
    n : int
        Mode number (0 = monopole, 1 = dipole, etc.)
    ka : float
        k × a in external medium (water)
    k1a : float  
        k × a in internal medium (gas/fluid sphere)
    g : float
        Density ratio (internal/external) = ρ_b / ρ_w
    h : float
        Sound speed ratio (internal/external) = c_b / c_w
    
    Returns
    -------
    complex
        C_n coefficient. Returns float('inf') if calculation would underflow,
        which will cause A_n = 0.
    
    Notes
    -----
    Following Anderson (1950) and scatmod implementation:
    
    C_n = [j_n'(k1a)/j_n'(ka) × y_n(ka)/j_n(k1a) - y_n'(ka)/j_n'(ka) × g×h] /
          [j_n'(k1a)/j_n'(ka) × j_n(ka)/j_n(k1a) - g×h]
    
    The scattering coefficient is then: A_n = -1 / (1 + i×C_n)
    """
    # Bessel functions
    jn_ka = _spherical_jn(n, ka)
    jn_k1a = _spherical_jn(n, k1a)
    yn_ka = _spherical_yn(n, ka)
    
    # Derivatives
    jn_ka_deriv = _spherical_jn(n, ka, derivative=True)
    jn_k1a_deriv = _spherical_jn(n, k1a, derivative=True)
    yn_ka_deriv = _spherical_yn(n, ka, derivative=True)
    
    # Check for underflow conditions - if Bessel functions are too small,
    # the contribution from this mode is negligible and C_n -> infinity
    # which gives A_n -> 0
    if abs(jn_ka_deriv) < 1e-100 or abs(jn_k1a) < 1e-100:
        return complex(float('inf'), 0)  # A_n will be ~0
    
    # C_n coefficient (Anderson 1950, also in scatmod)
    # Numerator: (j_n'(k1a)/j_n'(ka)) × (y_n(ka)/j_n(k1a)) - (y_n'(ka)/j_n'(ka)) × g×h
    # Denominator: (j_n'(k1a)/j_n'(ka)) × (j_n(ka)/j_n(k1a)) - g×h
    
    ratio_deriv = jn_k1a_deriv / jn_ka_deriv
    
    numerator = ratio_deriv * (yn_ka / jn_k1a) - (yn_ka_deriv / jn_ka_deriv) * g * h
    denominator = ratio_deriv * (jn_ka / jn_k1a) - g * h
    
    if abs(denominator) < 1e-100:
        return complex(float('inf'), 0)  # A_n will be ~0
    
    Cn = numerator / denominator
    
    return Cn


def calculate_scattering_coefficient(
    n: int,
    ka: float,
    k1a: float,
    g: float,
    h: float,
) -> complex:
    """Calculate the n-th partial wave scattering coefficient A_n.
    
    Parameters
    ----------
    n : int
        Mode number (0 = monopole, 1 = dipole, etc.)
    ka : float
        k × a in external medium (water)
    k1a : float
        k × a in internal medium (gas)
    g : float
        Density ratio (internal/external)
    h : float
        Sound speed ratio (internal/external)
    
    Returns
    -------
    complex
        Scattering coefficient A_n
    
    Notes
    -----
    A_n = -1 / (1 + i×C_n)
    
    For very high modes where Bessel functions underflow, C_n -> infinity
    and A_n -> 0.
    """
    Cn = calculate_Cn_coefficient(n, ka, k1a, g, h)
    
    # Handle inf properly: A_n = -1/(1+i*inf) -> 0
    if math.isinf(Cn.real) or math.isinf(Cn.imag):
        return complex(0, 0)
    
    # A_n = -1 / (1 + i*Cn)
    An = -1.0 / (1.0 + 1j * Cn)
    
    return An


def calculate_backscatter_form_function(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    n_modes: int = None,
    params: EnvironmentalParameters = None,
) -> complex:
    """Calculate the backscattering form function f_bs.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m)
    n_modes : int, optional
        Number of partial wave modes to include. If None, uses ka + 20.
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    complex
        Backscattering form function f_bs
    
    Notes
    -----
    f_bs = (-i/k) × Σ_n (-1)^n × (2n+1) × A_n
    
    For backscattering (θ = π), all Legendre polynomials P_n(cos π) = (-1)^n
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    depth = abs(waterdepth)
    
    # Wavenumber in water
    k_water = 2 * pi * frequency / params.cw
    ka = k_water * a
    
    # Gas pressure and properties
    P_gas = params.P_atm + params.rho_liq * params.g * depth + 2 * params.tau / a
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    
    # Sound speed in gas (adiabatic)
    c_gas = math.sqrt(params.gamma * P_gas / rho_gas)
    
    # Wavenumber in gas
    k_gas = 2 * pi * frequency / c_gas
    k1a = k_gas * a
    
    # Density and sound speed ratios
    g = rho_gas / params.rho_liq  # density ratio
    h = c_gas / params.cw  # sound speed ratio
    
    # Number of modes (adaptive based on ka, following scatmod)
    if n_modes is None:
        n_modes = int(ka + 20)
    n_modes = max(n_modes, 10)  # At least 10 modes
    
    # Sum partial waves for backscattering
    # f_bs = (-i/k) × Σ_n (-1)^n × (2n+1) × A_n
    sum_term = complex(0, 0)
    for n in range(n_modes):
        An = calculate_scattering_coefficient(n, ka, k1a, g, h)
        sum_term += ((-1) ** n) * (2 * n + 1) * An
    
    f_bs = (-1j / k_water) * sum_term
    
    return f_bs


def calculate_form_function(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    n_modes: int = None,
    params: EnvironmentalParameters = None,
) -> complex:
    """Alias for calculate_backscatter_form_function for backward compatibility."""
    return calculate_backscatter_form_function(frequency, bubble_radius, waterdepth, n_modes, params)


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    n_modes: int = None,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Anderson (1950) model.
    
    This is the full partial wave (modal) solution for a fluid sphere.
    No damping is included, so the model shows infinite scattering at
    exact resonance frequencies.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m), positive downward or absolute value used
    n_modes : int, optional
        Number of partial wave modes to include. If None, uses ka + 20.
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    float
        Backscattering cross-section (m²)
    
    Notes
    -----
    The backscattering cross-section is:
    
    σ_bs = |f_bs|²
    
    where f_bs is the backscattering form function with units of length.
    
    f_bs = (-i/k) × Σ_n (-1)^n × (2n+1) × A_n
    
    For gas bubbles (large density contrast), the monopole term (n=0)
    dominates near resonance.
    
    References
    ----------
    Anderson, V.C. (1950). "Sound scattering from a fluid sphere."
    J. Acoust. Soc. Am., 22(4), 426–431.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    # Form function (has units of length due to -i/k factor)
    f_bs = calculate_backscatter_form_function(frequency, bubble_radius, waterdepth, n_modes, params)
    
    # Backscattering cross-section: σ_bs = |f_bs|²
    sigma_bs = abs(f_bs) ** 2
    
    return sigma_bs


def calculate_sigma_bs_monopole(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using monopole approximation.
    
    This simplified version uses only the monopole (n=0) term, valid when
    ka << 1 (long wavelength approximation).
    
    This is the undamped resonant scatterer model:
    
    σ_bs = a² / (f_0²/f² - 1)²
    
    Note: This diverges at resonance (f = f_0) because no damping is included.
    Use the Medwin model or Ainslie-Leighton model for realistic results.
    
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
    """
    if params is None:
        params = EnvironmentalParameters()
    
    a = bubble_radius
    depth = abs(waterdepth)
    
    # Static pressure
    P_0 = params.P_atm + params.rho_liq * params.g * depth
    
    # Minnaert resonance frequency
    f_0 = (1 / (2 * pi * a)) * math.sqrt(3 * params.gamma * P_0 / params.rho_liq)
    
    # Frequency ratio term
    freq_term = (f_0 / frequency) ** 2 - 1
    
    # Avoid division by zero at resonance
    if abs(freq_term) < 1e-10:
        return a ** 2 / 1e-10  # Large but finite value
    
    sigma_bs = a ** 2 / (freq_term ** 2)
    
    return sigma_bs


def calculate_target_strength(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    n_modes: int = None,
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
    n_modes : int, optional
        Number of partial wave modes. If None, uses ka + 20.
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float
        Target strength (dB re 1 m²)
    """
    sigma_bs = calculate_sigma_bs(frequency, bubble_radius, waterdepth, n_modes, params)
    
    if sigma_bs <= 0:
        return -float('inf')
    
    return 10 * math.log10(sigma_bs)
