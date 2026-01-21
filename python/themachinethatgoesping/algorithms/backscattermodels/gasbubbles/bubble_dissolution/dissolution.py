# SPDX-FileCopyrightText: 2026 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

"""
Bubble dissolution simulation.

This module implements bubble mass transfer and dissolution during rise
through the water column. The primary model follows Leifer & Patro (2002)
and McGinnis et al. (2006).

The bubble loses gas to dissolution as it rises, while also expanding
due to decreasing hydrostatic pressure. The net effect depends on:
- Initial bubble size
- Rise velocity 
- Mass transfer coefficient (Sherwood number correlation)
- Gas concentration gradient between bubble and water
- Pressure/temperature changes during rise

Key equations:
- Mass transfer: dm/dt = -k_L * A * (C_s - C_∞)  
- Radius change: dr/dt combines dissolution + expansion terms
- Integration performed from initial depth to surface
"""

from dataclasses import dataclass, field
from typing import Callable, Optional
import numpy as np
from scipy.integrate import solve_ivp

from .environment import BubbleEnvironment, GasProperties
from .rise_velocity import bubble_rise_velocity_clift, get_rise_velocity_model


@dataclass
class BubbleDissolutionResult:
    """Results from bubble dissolution simulation.
    
    Contains the bubble state at each depth step during rise.
    
    Attributes
    ----------
    depths : np.ndarray
        Depth values (m), decreasing from initial depth to surface
    radii : np.ndarray
        Bubble equivalent radius (m) at each depth
    rise_velocities : np.ndarray
        Rise velocity (m/s) at each depth
    gas_fractions : dict[str, np.ndarray]
        Mole fraction of each gas species at each depth
    times : np.ndarray
        Time since release (s) at each depth
    dissolved_mass : np.ndarray
        Cumulative mass dissolved (kg) at each depth
    initial_radius : float
        Initial bubble radius (m)
    initial_depth : float
        Initial (release) depth (m)
    final_radius : float
        Final radius at surface or dissolution (m)
    dissolution_depth : float
        Depth at which bubble fully dissolved (m), or 0 if reached surface
    reached_surface : bool
        True if bubble reached surface with remaining gas
    """
    depths: np.ndarray
    radii: np.ndarray
    rise_velocities: np.ndarray
    gas_fractions: dict
    times: np.ndarray
    dissolved_mass: np.ndarray
    initial_radius: float
    initial_depth: float
    final_radius: float = 0.0
    dissolution_depth: float = 0.0
    reached_surface: bool = True


def sherwood_number(
    radius: float,
    rise_velocity: float,
    depth: float,
    env: BubbleEnvironment,
    gas: GasProperties
) -> float:
    """Calculate Sherwood number for mass transfer.
    
    The Sherwood number relates convective to diffusive mass transport.
    Uses correlations from Clift et al. (1978) for bubbles.
    
    Parameters
    ----------
    radius : float
        Bubble radius (m)
    rise_velocity : float
        Rise velocity (m/s)
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
    gas : GasProperties
        Properties of the dissolving gas
        
    Returns
    -------
    float
        Sherwood number (dimensionless)
        
    Notes
    -----
    Sh = k_L * d / D, where:
    - k_L: mass transfer coefficient (m/s)
    - d: bubble diameter (m)
    - D: molecular diffusivity (m²/s)
    
    For clean bubbles (mobile interface):
        Sh = 1.13 * Pe^0.5 for Pe > 100 (Levich, 1962)
        
    For dirty bubbles (rigid interface):
        Sh = 2 + 0.6 * Re^0.5 * Sc^0.33 (Ranz-Marshall)
    """
    diameter = 2 * radius
    nu = env.kinematic_viscosity(depth)
    T = env.temperature_profile(depth)
    D = gas.diffusivity_at_temperature(T)
    
    # Dimensionless numbers
    Re = rise_velocity * diameter / nu  # Reynolds
    Sc = nu / D  # Schmidt
    Pe = Re * Sc  # Peclet
    
    if env.bubble_surface_condition == "clean":
        # Levich correlation for mobile interface
        if Pe > 100:
            Sh = 1.13 * np.sqrt(Pe)
        else:
            # Low Pe - diffusion dominated
            Sh = 2.0 + 0.6 * np.sqrt(Re) * Sc**(1/3)
    else:
        # Ranz-Marshall for rigid interface (dirty bubble)
        Sh = 2.0 + 0.6 * np.sqrt(max(Re, 0.1)) * Sc**(1/3)
    
    return max(Sh, 2.0)  # Minimum is pure diffusion


def mass_transfer_coefficient(
    radius: float,
    rise_velocity: float,
    depth: float,
    env: BubbleEnvironment,
    gas: GasProperties
) -> float:
    """Calculate mass transfer coefficient k_L.
    
    Parameters
    ----------
    radius : float
        Bubble radius (m)
    rise_velocity : float
        Rise velocity (m/s)
    depth : float
        Water depth (m)
    env : BubbleEnvironment
        Environmental conditions
    gas : GasProperties
        Gas properties
        
    Returns
    -------
    float
        Mass transfer coefficient k_L (m/s)
    """
    T = env.temperature_profile(depth)
    D = gas.diffusivity_at_temperature(T)
    diameter = 2 * radius
    
    Sh = sherwood_number(radius, rise_velocity, depth, env, gas)
    
    return Sh * D / diameter


def bubble_dissolution_rate(
    radius: float,
    depth: float,
    env: BubbleEnvironment,
    rise_velocity: float,
    gas_moles: dict[str, float] | None = None,
) -> tuple[float, dict[str, float], dict[str, float]]:
    """Calculate the rate of bubble radius change due to gas exchange.
    
    This handles BOTH dissolution OUT (gas leaving bubble) AND dissolution IN
    (gas entering bubble from surrounding water). For a pure methane bubble:
    - CH4 leaves (high partial pressure in bubble vs low dissolved in water)
    - N2, O2 enter (saturated in seawater, zero/low in bubble)
    
    Parameters
    ----------
    radius : float
        Current bubble radius (m)
    depth : float
        Current depth (m)
    env : BubbleEnvironment
        Environmental conditions
    rise_velocity : float
        Current rise velocity (m/s)
    gas_moles : dict[str, float] | None
        Current moles of each gas in bubble. If None, uses env.gas_composition
        with ideal gas law to estimate initial moles.
        
    Returns
    -------
    tuple[float, dict[str, float], dict[str, float]]
        - dr/dt: Rate of radius change (m/s)
        - dn/dt per species: Molar flux rates (mol/s), positive = leaving bubble
        - Updated gas mole fractions
        
    Notes
    -----
    From Leifer & Patro (2002) / McGinnis et al. (2006):
    
    For each gas i:
        dn_i/dt = -k_L,i * A * (C_sat,i - C_ambient,i)
    
    where:
        C_sat,i = H_i * p_i  (Henry's law at bubble surface)
        C_ambient,i = H_i * p_ambient,i  (dissolved in water)
    
    Negative dn/dt means gas ENTERS the bubble.
    """
    if radius <= 1e-7:  # Effectively dissolved
        return 0.0, {}, {}
    
    P = env.pressure_at_depth(depth)
    T = env.temperature_profile(depth)
    R = 8.314  # J/(mol·K)
    
    bubble_area = 4 * np.pi * radius**2
    bubble_volume = (4/3) * np.pi * radius**3
    
    # Calculate current moles if not provided
    if gas_moles is None:
        # Estimate from ideal gas law: PV = nRT
        total_moles = P * bubble_volume / (R * T)
        gas_moles = {
            gas: frac * total_moles 
            for gas, frac in env.gas_composition.items()
        }
    
    # Total moles for calculating partial pressures
    total_moles = sum(gas_moles.values())
    if total_moles <= 0:
        return 0.0, {}, {}
    
    # Calculate molar flux for each gas (positive = leaving bubble)
    molar_flux = {}  # mol/s
    total_mass_flux = 0.0  # kg/s
    
    # All gases that could exchange (in bubble OR dissolved in water)
    all_gases = set(gas_moles.keys()) | set(env.dissolved_gas_saturation.keys())
    
    for gas_name in all_gases:
        if gas_name not in env.gas_properties:
            continue
            
        gas = env.gas_properties[gas_name]
        
        # Current moles and mole fraction
        n_gas = gas_moles.get(gas_name, 0.0)
        x_gas = n_gas / total_moles if total_moles > 0 else 0.0
        
        # Partial pressure in bubble
        p_bubble = x_gas * P
        
        # Equilibrium concentration at bubble surface (Henry's law)
        H = gas.henry_constant_at_temperature(T)  # mol/(m³·Pa)
        C_sat = H * p_bubble  # mol/m³
        
        # Ambient dissolved concentration in seawater
        # Based on equilibrium with atmosphere at surface
        sat_level = env.dissolved_gas_saturation.get(gas_name, 0.0)
        if gas_name == "O2":
            p_atm_frac = 0.21  # 21% of atmosphere
        elif gas_name == "N2":
            p_atm_frac = 0.78  # 78% of atmosphere
        elif gas_name == "CO2":
            p_atm_frac = 0.0004  # ~400 ppm
        else:  # CH4 and others
            p_atm_frac = 1e-6  # Trace
        
        p_ambient = sat_level * p_atm_frac * env.surface_pressure
        C_ambient = H * p_ambient  # mol/m³
        
        # Mass transfer coefficient for this gas
        k_L = mass_transfer_coefficient(radius, rise_velocity, depth, env, gas)
        
        # Molar flux: positive = leaving bubble, negative = entering bubble
        dn_dt = k_L * bubble_area * (C_sat - C_ambient)
        
        molar_flux[gas_name] = dn_dt
        total_mass_flux += dn_dt * gas.molar_mass
    
    # Calculate radius change from net mass flux
    # Total moles changes: dn_total/dt = -sum(dn_i/dt) [negative = moles added]
    # Volume from ideal gas: V = n*R*T/P
    # dV/dt = (R*T/P) * dn/dt
    
    net_molar_flux = sum(molar_flux.values())  # mol/s leaving
    dV_dt_dissolution = -(R * T / P) * net_molar_flux
    
    # dr/dt from dV/dt = 4πr² dr/dt
    dr_dt_dissolution = dV_dt_dissolution / bubble_area if bubble_area > 0 else 0.0
    
    # Expansion due to pressure decrease during rise
    dP_dz = -env.water_density * env.gravity
    dz_dt = rise_velocity
    dP_dt = dP_dz * (-dz_dt)  # Rising = depth decreasing
    
    # At constant n, T: P*V = const, so V*dP + P*dV = 0
    # dV/dt = -V/P * dP/dt
    dV_dt_expansion = -bubble_volume / P * dP_dt
    dr_dt_expansion = dV_dt_expansion / bubble_area if bubble_area > 0 else 0.0
    
    total_dr_dt = dr_dt_dissolution + dr_dt_expansion
    
    # Calculate updated gas mole fractions
    # (for reporting, actual update happens in simulation loop)
    new_total = sum(n - molar_flux.get(gas, 0.0) * 0.1 for gas, n in gas_moles.items())
    if new_total > 0:
        current_fractions = {
            gas: n / total_moles for gas, n in gas_moles.items() if n > 0
        }
    else:
        current_fractions = {}
    
    return total_dr_dt, molar_flux, current_fractions


def simulate_bubble_rise(
    initial_radius: float,
    initial_depth: float,
    env: BubbleEnvironment,
    depth_resolution: float = 1.0,
    rise_velocity_model: str = "clift",
    min_radius: float = 1e-6,
    max_time: float = 7200.0,
) -> BubbleDissolutionResult:
    """Simulate bubble rise and dissolution through the water column.
    
    Integrates the coupled rise and dissolution equations from the initial
    depth to the surface (or until bubble completely dissolves). Tracks
    multi-gas exchange: CH4 dissolves out while N2/O2 dissolve into the bubble.
    
    Parameters
    ----------
    initial_radius : float
        Initial bubble equivalent spherical radius (m)
    initial_depth : float
        Initial (release) depth (m)
    env : BubbleEnvironment
        Environmental conditions including initial gas composition
    depth_resolution : float
        Approximate depth interval for output (m). Default: 1.0 m
    rise_velocity_model : str
        Rise velocity model name: "clift", "leifer", or "woolf"
    min_radius : float
        Minimum radius before bubble considered dissolved (m). Default: 1 μm
    max_time : float
        Maximum simulation time (s). Default: 7200 s (2 hours)
        
    Returns
    -------
    BubbleDissolutionResult
        Complete simulation results including:
        - depths, radii, rise_velocities arrays
        - gas_fractions: dict mapping gas name to array of mole fractions vs depth
        - This shows how e.g. a 100% CH4 bubble gains N2/O2 during rise
        
    Examples
    --------
    >>> env = BubbleEnvironment()  # Default: 100% CH4, 8°C, dirty bubbles
    >>> result = simulate_bubble_rise(
    ...     initial_radius=0.002,  # 2 mm
    ...     initial_depth=100.0,   # 100 m
    ...     env=env,
    ...     depth_resolution=5.0
    ... )
    >>> print(f"CH4 fraction at surface: {result.gas_fractions['CH4'][-1]:.1%}")
    >>> print(f"N2 fraction at surface: {result.gas_fractions['N2'][-1]:.1%}")
    """
    # Get rise velocity model
    rise_model = get_rise_velocity_model(rise_velocity_model)
    
    # Initialize gas moles from ideal gas law: PV = nRT
    P_init = env.pressure_at_depth(initial_depth)
    T_init = env.temperature_profile(initial_depth)
    R = 8.314  # J/(mol·K)
    V_init = (4/3) * np.pi * initial_radius**3
    total_moles_init = P_init * V_init / (R * T_init)
    
    # Initial moles of each gas
    gas_moles = {}
    for gas_name, frac in env.gas_composition.items():
        gas_moles[gas_name] = frac * total_moles_init
    
    # Also initialize moles for gases not in bubble but dissolved in water
    for gas_name in env.dissolved_gas_saturation.keys():
        if gas_name not in gas_moles:
            gas_moles[gas_name] = 0.0
    
    # Initialize storage
    depths_list = [initial_depth]
    radii_list = [initial_radius]
    times_list = [0.0]
    velocities_list = [rise_model.compute_velocity(initial_radius, initial_depth, env)]
    dissolved_mass_list = [0.0]
    
    # Store initial gas fractions
    total_moles = sum(gas_moles.values())
    initial_fractions = {gas: n/total_moles for gas, n in gas_moles.items() if total_moles > 0}
    gas_fractions_list = [initial_fractions]
    
    # Current state
    current_depth = initial_depth
    current_radius = initial_radius
    current_time = 0.0
    total_dissolved = 0.0
    
    # Simulation loop (simple Euler - could upgrade to RK45)
    dt = 0.1  # Time step (s)
    
    while current_depth > 0 and current_radius > min_radius and current_time < max_time:
        # Calculate rise velocity
        u = rise_model.compute_velocity(current_radius, current_depth, env)
        
        # Calculate dissolution rates with current gas composition
        dr_dt, molar_flux, _ = bubble_dissolution_rate(
            current_radius, current_depth, env, u, gas_moles
        )
        
        # Update gas moles (negative flux = gas entering bubble)
        for gas_name, dn_dt in molar_flux.items():
            gas_moles[gas_name] = max(0.0, gas_moles.get(gas_name, 0.0) - dn_dt * dt)
            
            # Track mass dissolved (only count positive flux = leaving bubble)
            if dn_dt > 0:
                gas = env.gas_properties.get(gas_name)
                if gas:
                    total_dissolved += dn_dt * gas.molar_mass * dt
        
        # Update depth (rising = depth decreasing)
        dz = -u * dt
        new_depth = current_depth + dz
        
        # Update radius
        new_radius = current_radius + dr_dt * dt
        
        # Check for complete dissolution
        if new_radius <= min_radius:
            new_radius = 0.0
            break
            
        # Check for reaching surface
        if new_depth <= 0:
            new_depth = 0.0
            
        # Update current state
        current_depth = max(new_depth, 0)
        current_radius = max(new_radius, 0)
        current_time += dt
        
        # Store results at depth_resolution intervals
        if len(depths_list) == 0 or \
           abs(current_depth - depths_list[-1]) >= depth_resolution or \
           current_depth <= 0:
            depths_list.append(current_depth)
            radii_list.append(current_radius)
            times_list.append(current_time)
            velocities_list.append(u)
            dissolved_mass_list.append(total_dissolved)
            
            # Store current gas fractions
            total_moles = sum(gas_moles.values())
            if total_moles > 0:
                fracs = {gas: n/total_moles for gas, n in gas_moles.items()}
            else:
                fracs = {gas: 0.0 for gas in gas_moles.keys()}
            gas_fractions_list.append(fracs)
    
    # Ensure we have final state
    if depths_list[-1] != current_depth:
        depths_list.append(current_depth)
        radii_list.append(current_radius)
        times_list.append(current_time)
        velocities_list.append(
            rise_model.compute_velocity(current_radius, current_depth, env)
            if current_radius > min_radius else 0.0
        )
        dissolved_mass_list.append(total_dissolved)
        
        total_moles = sum(gas_moles.values())
        if total_moles > 0:
            fracs = {gas: n/total_moles for gas, n in gas_moles.items()}
        else:
            fracs = {gas: 0.0 for gas in gas_moles.keys()}
        gas_fractions_list.append(fracs)
    
    # Convert to arrays
    depths = np.array(depths_list)
    radii = np.array(radii_list)
    times = np.array(times_list)
    velocities = np.array(velocities_list)
    dissolved_mass = np.array(dissolved_mass_list)
    
    # Organize gas fractions - include ALL gases that were tracked
    all_tracked_gases = set()
    for gf in gas_fractions_list:
        all_tracked_gases.update(gf.keys())
    
    gas_fractions = {}
    for gas_name in all_tracked_gases:
        gas_fractions[gas_name] = np.array([
            gf.get(gas_name, 0.0) for gf in gas_fractions_list
        ])
    
    reached_surface = current_depth <= 0 and current_radius > min_radius
    dissolution_depth = current_depth if current_radius <= min_radius else 0.0
    
    return BubbleDissolutionResult(
        depths=depths,
        radii=radii,
        rise_velocities=velocities,
        gas_fractions=gas_fractions,
        times=times,
        dissolved_mass=dissolved_mass,
        initial_radius=initial_radius,
        initial_depth=initial_depth,
        final_radius=current_radius,
        dissolution_depth=dissolution_depth,
        reached_surface=reached_surface,
    )


def simulate_bubble_population(
    radii: np.ndarray,
    initial_depth: float,
    env: BubbleEnvironment,
    depth_resolution: float = 5.0,
    rise_velocity_model: str = "clift",
) -> dict:
    """Simulate dissolution for a population of bubbles with different sizes.
    
    Parameters
    ----------
    radii : np.ndarray
        Array of initial bubble radii (m)
    initial_depth : float
        Release depth (m)
    env : BubbleEnvironment
        Environmental conditions
    depth_resolution : float
        Output depth resolution (m)
    rise_velocity_model : str
        Rise velocity model name
        
    Returns
    -------
    dict
        Dictionary containing:
        - "depth_grid": common depth array
        - "radius_distribution": 2D array [n_bubbles, n_depths]
        - "velocity_distribution": 2D array
        - "fraction_surviving": fraction reaching each depth
        - "individual_results": list of BubbleDissolutionResult
        
    Examples
    --------
    >>> radii = np.linspace(0.0005, 0.005, 10)  # 0.5-5 mm
    >>> env = BubbleEnvironment()
    >>> pop_result = simulate_bubble_population(radii, 200.0, env)
    """
    results = []
    
    for r in radii:
        result = simulate_bubble_rise(
            initial_radius=r,
            initial_depth=initial_depth,
            env=env,
            depth_resolution=depth_resolution,
            rise_velocity_model=rise_velocity_model,
        )
        results.append(result)
    
    # Create common depth grid
    min_depth = 0.0
    n_depths = int(initial_depth / depth_resolution) + 1
    depth_grid = np.linspace(initial_depth, min_depth, n_depths)
    
    # Interpolate each bubble's trajectory onto common grid
    n_bubbles = len(radii)
    radius_dist = np.zeros((n_bubbles, n_depths))
    velocity_dist = np.zeros((n_bubbles, n_depths))
    
    for i, result in enumerate(results):
        # Interpolate radius vs depth
        if len(result.depths) > 1:
            # Need to handle that depths are decreasing
            depths_ascending = result.depths[::-1]
            radii_ascending = result.radii[::-1]
            velocities_ascending = result.rise_velocities[::-1]
            
            radius_dist[i, :] = np.interp(
                depth_grid[::-1], 
                depths_ascending,
                radii_ascending,
                left=0.0,  # Dissolved before reaching surface
                right=result.initial_radius
            )[::-1]
            
            velocity_dist[i, :] = np.interp(
                depth_grid[::-1],
                depths_ascending,
                velocities_ascending,
                left=0.0,
                right=result.rise_velocities[0] if len(result.rise_velocities) > 0 else 0.0
            )[::-1]
        else:
            radius_dist[i, :] = result.radii[0] if len(result.radii) > 0 else 0.0
            velocity_dist[i, :] = result.rise_velocities[0] if len(result.rise_velocities) > 0 else 0.0
    
    # Calculate fraction surviving at each depth
    fraction_surviving = np.sum(radius_dist > 1e-7, axis=0) / n_bubbles
    
    return {
        "depth_grid": depth_grid,
        "radius_distribution": radius_dist,
        "velocity_distribution": velocity_dist,
        "fraction_surviving": fraction_surviving,
        "individual_results": results,
    }
