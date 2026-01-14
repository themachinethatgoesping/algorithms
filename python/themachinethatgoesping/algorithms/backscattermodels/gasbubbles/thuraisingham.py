import math
from math import pi

from .parameters import EnvironmentalParameters


def calculate_sigma_bs(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> float:
    """Calculate backscattering cross-section using Thuraisingham (1997) model.
    
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
    """
    if params is None:
        params = EnvironmentalParameters()
    
    r = bubble_radius
    freq = frequency
    depth = waterdepth
    
    # Calculate atmospheric gas density from ideal gas law: rho = P*M/(R*T)
    rhoG0 = params.P_atm * params.Mm / (params.R * params.T)
    
    # Static pressure at depth (Pa)
    Pst = params.P_atm + params.rho_liq * params.g * abs(depth)
    
    # Resonance radius
    r_fix = math.sqrt(3 * params.gamma * Pst / params.rho_liq) / (2 * pi * freq)
    
    # Wave number
    k = 2 * pi * freq / params.cw
    
    # Gas density at depth (kg/m³)
    rhoG = rhoG0 * (1 + 2 * params.tau / (Pst * r)) * (1 + 0.1 * abs(depth))
    
    # Thermal diffusivity of gas (m²/s)
    Da = params.K_gas / (rhoG * params.Cp)
    
    # Minnaert angular frequency (rad/s)
    wM = math.sqrt(3 * params.gamma * Pst / (params.rho_liq * r * r))
    
    # Dimensionless damping (Ainslie)
    Drad = wM * r / params.cw  # radiation damping term
    Dtherm = 3 * (params.gamma - 1) / r * math.sqrt(Da / (2 * wM))  # thermal damping term
    XX = (1/3 + 2.2) * params.eta_s
    Dvisc = 3 * XX / (params.rho_liq * wM * r * r)  # viscous damping term
    damp = Drad + Dtherm + Dvisc  # total dimensionless damping
    
    # Thuraisingham (1997) factor
    Thur_factor = (math.sin(k * r) / (k * r))**2 / (1 + (k * r)**2)
    
    # Backscattering cross-section
    sigma_bs = (r**2 / (((r_fix / r)**2 - 1)**2 + damp**2)) * Thur_factor
    
    return sigma_bs