from dataclasses import dataclass

@dataclass
class EnvironmentalParameters:
    """Parameters for gas bubble backscatter models.
    
    Default values are typical for methane (CH4) bubbles in seawater.
    """
    
    T: float = 281.29
    """Temperature (K), default 8.14°C = 281.29 K"""
    
    # Physical properties
    tau: float = 0.0745
    """Surface tension (N/m)"""
    
    pv: float = 872.0
    """Vapor pressure (Pa) at 10°C"""
    
    eta_s: float = 1.5e-3
    """Shear viscosity (Pa·s)"""
    
    K_gas: float = 8e-2
    """Thermal conductivity of the gas CH4 (W/(m·K))"""
    
    Cp: float = 2191.0
    """Specific heat capacity at constant pressure (J/(kg·K))"""
    
    g: float = 9.81
    """Gravity (m/s²)"""
    
    rho_liq: float = 1025.0
    """Seawater density (kg/m³)"""
    
    P_atm: float = 101e3
    """Atmospheric pressure (Pa)"""
    
    cw: float = 1485.0
    """Sound speed in water (m/s)"""
    
    gamma: float = 1.299
    """Specific heat ratio of the gas CH4"""
    
    Mm: float = 0.016
    """Molar mass of the gas CH4 (kg/mol)"""
    
    R: float = 8.31
    """Gas constant (m²·kg·s⁻²·K⁻¹·mol⁻¹)"""