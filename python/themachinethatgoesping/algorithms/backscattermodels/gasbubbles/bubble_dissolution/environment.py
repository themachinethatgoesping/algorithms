# SPDX-FileCopyrightText: 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""
Environmental parameters for bubble dissolution modeling.

This module defines the environmental conditions that affect bubble behavior
including water temperature, salinity, dissolved gas concentrations, and 
hydrostatic pressure profiles.
"""

from dataclasses import dataclass, field
from typing import Callable
import numpy as np


@dataclass
class GasProperties:
    """Properties of a gas species for dissolution modeling.
    
    Parameters
    ----------
    name : str
        Gas name (e.g., "CH4", "CO2", "N2", "O2")
    molar_mass : float
        Molar mass in kg/mol
    diffusivity_water : float
        Molecular diffusion coefficient in water at 20°C (m²/s)
    henry_constant_25C : float
        Henry's law constant at 25°C (mol/(m³·Pa)) - dimensionless form
    solubility_temperature_coeff : float
        Temperature coefficient for solubility adjustment (K⁻¹)
        
    Notes
    -----
    Default values are for methane (CH4) in seawater.
    """
    name: str = "CH4"
    molar_mass: float = 0.01604  # kg/mol for CH4
    diffusivity_water: float = 1.49e-9  # m²/s at 20°C for CH4
    henry_constant_25C: float = 1.4e-5  # mol/(m³·Pa) for CH4 (very low solubility)
    solubility_temperature_coeff: float = -0.0295  # K⁻¹ (decreases with temp)
    
    def diffusivity_at_temperature(self, T: float) -> float:
        """Calculate diffusivity adjusted for temperature.
        
        Uses Stokes-Einstein relation: D ∝ T/μ where μ is viscosity.
        
        Parameters
        ----------
        T : float
            Temperature in Kelvin
            
        Returns
        -------
        float
            Diffusivity in m²/s
        """
        T_ref = 293.15  # 20°C reference
        # Simplified: D scales with T (more accurate would use viscosity)
        return self.diffusivity_water * (T / T_ref)
    
    def henry_constant_at_temperature(self, T: float) -> float:
        """Calculate Henry's constant at given temperature.
        
        Uses van't Hoff equation for temperature dependence.
        
        Parameters
        ----------
        T : float
            Temperature in Kelvin
            
        Returns
        -------
        float
            Henry's constant in mol/(m³·Pa)
        """
        T_ref = 298.15  # 25°C reference
        return self.henry_constant_25C * np.exp(
            self.solubility_temperature_coeff * (1/T - 1/T_ref)
        )


# Pre-defined gas properties
METHANE = GasProperties(
    name="CH4",
    molar_mass=0.01604,
    diffusivity_water=1.49e-9,
    henry_constant_25C=1.4e-5,
    solubility_temperature_coeff=-0.0295,
)

CO2 = GasProperties(
    name="CO2", 
    molar_mass=0.04401,
    diffusivity_water=1.91e-9,
    henry_constant_25C=3.4e-4,  # Much more soluble than CH4
    solubility_temperature_coeff=-0.024,
)

NITROGEN = GasProperties(
    name="N2",
    molar_mass=0.02801,
    diffusivity_water=2.0e-9,
    henry_constant_25C=6.4e-6,  # Similar to CH4
    solubility_temperature_coeff=-0.015,
)

OXYGEN = GasProperties(
    name="O2",
    molar_mass=0.032,
    diffusivity_water=2.1e-9,
    henry_constant_25C=1.3e-5,
    solubility_temperature_coeff=-0.015,
)


@dataclass
class BubbleEnvironment:
    """Environmental conditions for bubble dissolution simulation.
    
    This class defines the water column properties that affect bubble behavior:
    temperature profile, salinity, dissolved gas concentrations, and current.
    
    Parameters
    ----------
    temperature_profile : Callable[[float], float]
        Function returning temperature (K) at given depth (m).
        Default: constant 281.29 K (8.14°C, typical deep ocean)
    salinity : float
        Salinity in PSU (default: 35.0)
    surface_pressure : float
        Atmospheric pressure at surface in Pa (default: 101325 Pa)
    water_density : float  
        Seawater density in kg/m³ (default: 1027)
    gravity : float
        Gravitational acceleration in m/s² (default: 9.81)
    gas_composition : dict[str, float]
        Gas mole fractions in bubble (should sum to 1.0)
        Default: 100% methane {"CH4": 1.0}
    dissolved_gas_saturation : dict[str, float]
        Saturation level of each gas in surrounding water (0-1 range)
        Default: {"CH4": 0.0} (no methane dissolved)
    gas_properties : dict[str, GasProperties]
        Properties for each gas species.
    bubble_surface_condition : str
        "clean" or "dirty" - affects mass transfer coefficient.
        Clean bubbles have mobile surface, dirty have immobile (surfactants).
        
    Examples
    --------
    >>> env = BubbleEnvironment()  # Default: 100% CH4, constant temperature
    >>> env = BubbleEnvironment(
    ...     temperature_profile=lambda z: 280 + 0.02*z,  # Thermocline
    ...     gas_composition={"CH4": 0.95, "N2": 0.05}
    ... )
    """
    temperature_profile: Callable[[float], float] = field(
        default_factory=lambda: lambda z: 281.29  # Constant ~8°C
    )
    salinity: float = 35.0
    surface_pressure: float = 101325.0  # Pa
    water_density: float = 1027.0  # kg/m³
    gravity: float = 9.81
    
    # Gas composition (mole fractions, should sum to 1.0)
    gas_composition: dict[str, float] = field(
        default_factory=lambda: {"CH4": 1.0}
    )
    
    # Dissolved gas saturation in water (fraction of saturation, 0-1)
    dissolved_gas_saturation: dict[str, float] = field(
        default_factory=lambda: {"CH4": 0.0, "N2": 0.95, "O2": 0.90}
    )
    
    # Gas properties database
    gas_properties: dict[str, GasProperties] = field(
        default_factory=lambda: {
            "CH4": METHANE,
            "CO2": CO2,
            "N2": NITROGEN,
            "O2": OXYGEN,
        }
    )
    
    bubble_surface_condition: str = "dirty"  # "clean" or "dirty"
    
    def pressure_at_depth(self, depth: float) -> float:
        """Calculate total pressure at given depth.
        
        Parameters
        ----------
        depth : float
            Water depth in meters (positive downward)
            
        Returns
        -------
        float
            Total pressure in Pa
        """
        return self.surface_pressure + self.water_density * self.gravity * abs(depth)
    
    def gas_density_at_depth(self, depth: float) -> float:
        """Calculate gas density inside bubble at depth using ideal gas law.
        
        Parameters
        ----------
        depth : float
            Water depth in meters
            
        Returns
        -------
        float
            Gas density in kg/m³
        """
        P = self.pressure_at_depth(depth)
        T = self.temperature_profile(depth)
        
        # Weighted average molar mass
        M_avg = sum(
            frac * self.gas_properties.get(gas, METHANE).molar_mass
            for gas, frac in self.gas_composition.items()
        )
        
        R = 8.314  # J/(mol·K)
        return P * M_avg / (R * T)
    
    def kinematic_viscosity(self, depth: float) -> float:
        """Calculate seawater kinematic viscosity.
        
        Simplified Sharqawy et al. (2010) correlation.
        
        Parameters
        ----------
        depth : float
            Water depth in meters
            
        Returns
        -------
        float
            Kinematic viscosity in m²/s
        """
        T = self.temperature_profile(depth)
        T_C = T - 273.15  # Convert to Celsius
        
        # Simplified correlation for seawater at S=35
        # More accurate: use Sharqawy et al. (2010) correlation
        mu = 1.79e-3 * np.exp(-0.0254 * T_C)  # Dynamic viscosity Pa·s
        return mu / self.water_density
    
    def surface_tension(self, depth: float) -> float:
        """Calculate water-gas surface tension.
        
        Parameters
        ----------
        depth : float
            Water depth in meters
            
        Returns
        -------
        float
            Surface tension in N/m
        """
        T = self.temperature_profile(depth)
        T_C = T - 273.15
        
        # Vargaftik et al. (1983) correlation for water-air
        # Adjusted slightly for seawater
        sigma = 0.0756 - 0.00014 * T_C  # N/m
        return max(sigma, 0.050)  # Prevent negative values
