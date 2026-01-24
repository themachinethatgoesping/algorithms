"""
echoSMs wrapper for gas bubble backscattering models.

Wrapper around the echoSMs library (https://github.com/ices-tools-dev/echoSMs)
providing a consistent interface for computing acoustic backscatter from gas bubbles.

Available models:
- **MSSModel**: Modal Series Solution for spheres (exact solution)
- **PSMSModel**: Prolate Spheroidal Modal Series for prolate spheroids with 
  incidence angle support (exact solution)

The echoSMs library implements benchmark models from:
Jech, J.M. et al. (2015). Comparisons among ten models of acoustic backscattering 
used in aquatic ecosystem research. JASA 138, 3742-3764.
https://doi.org/10.1121/1.4937607

Installation:
    pip install echosms

Usage:
    The main functions `calculate_sigma_bs_mss` and `calculate_sigma_bs_psms` 
    automatically handle both scalar and array inputs for frequency and radius.
"""

import math
from math import pi
from typing import Union

import numpy as np

from .parameters import EnvironmentalParameters

# Type alias for scalar or array inputs
ArrayLike = Union[float, int, list, np.ndarray]

# Try to import echosms, provide helpful error if not installed
try:
    import echosms
    from echosms import MSSModel, PSMSModel
    from echosms.utils import boundary_type as bt
    ECHOSMS_AVAILABLE = True
except ImportError:
    ECHOSMS_AVAILABLE = False
    MSSModel = None
    PSMSModel = None
    bt = None


def _check_echosms_available():
    """Raise ImportError if echosms is not available."""
    if not ECHOSMS_AVAILABLE:
        raise ImportError(
            "echosms is not installed. Install it with: pip install echosms"
        )


def _is_array_like(x) -> bool:
    """Check if x is array-like (list, tuple, or ndarray with size > 1)."""
    if isinstance(x, (list, tuple)):
        return len(x) > 1
    if isinstance(x, np.ndarray):
        return x.size > 1
    return False


def _to_scalar(x):
    """Convert single-element array to scalar."""
    if isinstance(x, np.ndarray) and x.size == 1:
        return float(x.flat[0])
    return x


def get_gas_properties(
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> tuple[float, float]:
    """Calculate gas sound speed and density for use with echoSMs.
    
    Computes gas properties inside a bubble accounting for hydrostatic pressure
    and surface tension (Young-Laplace equation).
    
    Parameters
    ----------
    bubble_radius : float
        Bubble radius (m)
    waterdepth : float
        Water depth (m), positive downward or absolute value used
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    tuple[float, float]
        Gas sound speed (m/s), gas density (kg/m³)
    
    Notes
    -----
    Uses adiabatic sound speed: c = sqrt(gamma * P / rho)
    where P is the gas pressure including surface tension correction.
    """
    if params is None:
        params = EnvironmentalParameters()
    
    r = bubble_radius
    depth = abs(waterdepth)
    
    # Gas pressure inside bubble including surface tension (Young-Laplace)
    P_gas = params.P_atm + params.rho_liq * params.g * depth - params.pv + 2 * params.tau / r
    
    # Gas density from ideal gas law: rho = P * M / (R * T)
    rho_gas = P_gas * params.Mm / (params.R * params.T)
    
    # Adiabatic sound speed: c = sqrt(gamma * P / rho)
    c_gas = math.sqrt(params.gamma * P_gas / rho_gas)
    
    return c_gas, rho_gas


def calculate_sigma_bs_mss(
    frequency: ArrayLike,
    bubble_radius: ArrayLike,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> Union[float, np.ndarray]:
    """Calculate backscattering cross-section using echoSMs MSSModel.
    
    Uses the Modal Series Solution (MSS) for fluid-filled spheres from echoSMs.
    This is an exact analytical solution for spherical targets.
    
    Automatically handles scalar or array inputs for frequency and/or radius.
    
    Parameters
    ----------
    frequency : float or array-like
        Acoustic frequency (Hz). Can be scalar or array.
    bubble_radius : float or array-like
        Bubble radius (m). Can be scalar or array.
    waterdepth : float
        Water depth (m), positive downward or absolute value used
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    float or np.ndarray
        Backscattering cross-section (m²). Returns scalar if both inputs
        are scalar, otherwise returns array.
    
    Notes
    -----
    The MSS model assumes:
    - Spherical geometry (no incidence angle dependence)
    - Adiabatic gas behavior (appropriate for high frequencies away from resonance)
    - Homogeneous fluid interior
    
    For thermal/viscous damping effects near resonance, consider using
    the Ainslie-Leighton or Li models which include these corrections.
    
    Examples
    --------
    >>> # Single frequency and radius
    >>> sigma = calculate_sigma_bs_mss(38000, 0.001, 100)
    
    >>> # Multiple radii at single frequency
    >>> radii = np.array([0.0005, 0.001, 0.002])
    >>> sigma = calculate_sigma_bs_mss(38000, radii, 100)
    
    >>> # Multiple frequencies at single radius  
    >>> freqs = np.array([18000, 38000, 120000])
    >>> sigma = calculate_sigma_bs_mss(freqs, 0.001, 100)
    """
    _check_echosms_available()
    
    if params is None:
        params = EnvironmentalParameters()
    
    freq_is_array = _is_array_like(frequency)
    radius_is_array = _is_array_like(bubble_radius)
    
    # Case 1: Both scalar - use direct calculation
    if not freq_is_array and not radius_is_array:
        return _calculate_sigma_bs_mss_scalar(
            _to_scalar(frequency), _to_scalar(bubble_radius), waterdepth, params
        )
    
    # Case 2: Radius is array - must loop (gas properties depend on radius)
    if radius_is_array:
        radii = np.asarray(bubble_radius)
        freqs = np.asarray(frequency) if freq_is_array else frequency
        
        if freq_is_array:
            # Both are arrays - create 2D output (radii x freqs)
            result = np.empty((radii.size, np.asarray(freqs).size))
            for i, r in enumerate(radii.flat):
                result[i, :] = _calculate_sigma_bs_mss_freqs(freqs, r, waterdepth, params)
            return result
        else:
            # Only radius is array
            result = np.empty(radii.shape)
            for i, r in enumerate(radii.flat):
                result.flat[i] = _calculate_sigma_bs_mss_scalar(freqs, r, waterdepth, params)
            return result
    
    # Case 3: Only frequency is array - use echoSMs vectorization
    return _calculate_sigma_bs_mss_freqs(frequency, _to_scalar(bubble_radius), waterdepth, params)


def _calculate_sigma_bs_mss_scalar(
    frequency: float,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters,
) -> float:
    """Internal: Calculate sigma_bs for scalar frequency and radius."""
    # Convert numpy scalars to Python floats for echoSMs compatibility
    frequency = float(frequency)
    bubble_radius = float(bubble_radius)
    
    c_gas, rho_gas = get_gas_properties(bubble_radius, waterdepth, params)
    
    model = MSSModel()
    p = {
        'medium_c': float(params.cw),
        'medium_rho': float(params.rho_liq),
        'a': bubble_radius,
        'f': frequency,
        'boundary_type': bt.fluid_filled,
        'target_c': float(c_gas),
        'target_rho': float(rho_gas),
    }
    
    ts = model.calculate_ts(p)
    if isinstance(ts, (list, np.ndarray)):
        ts = ts[0]
    
    return np.power(10, 0.1 * ts)


def _calculate_sigma_bs_mss_freqs(
    frequencies: np.ndarray,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters,
) -> np.ndarray:
    """Internal: Calculate sigma_bs for array of frequencies at single radius."""
    c_gas, rho_gas = get_gas_properties(bubble_radius, waterdepth, params)
    
    model = MSSModel()
    p = {
        'medium_c': params.cw,
        'medium_rho': params.rho_liq,
        'a': bubble_radius,
        'f': np.asarray(frequencies),
        'boundary_type': bt.fluid_filled,
        'target_c': c_gas,
        'target_rho': rho_gas,
    }
    
    ts = model.calculate_ts(p)
    return np.power(10, 0.1 * np.asarray(ts))


def calculate_target_strength_mss(
    frequency: ArrayLike,
    bubble_radius: ArrayLike,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> Union[float, np.ndarray]:
    """Calculate target strength using echoSMs MSSModel.
    
    Automatically handles scalar or array inputs for frequency and/or radius.
    
    Parameters
    ----------
    frequency : float or array-like
        Acoustic frequency (Hz). Can be scalar or array.
    bubble_radius : float or array-like
        Bubble radius (m). Can be scalar or array.
    waterdepth : float
        Water depth (m)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float or np.ndarray
        Target strength (dB re 1 m²)
    """
    sigma_bs = calculate_sigma_bs_mss(frequency, bubble_radius, waterdepth, params)
    return 10 * np.log10(sigma_bs)


def calculate_sigma_bs_psms(
    frequency: ArrayLike,
    major_radius: ArrayLike,
    minor_radius: ArrayLike,
    waterdepth: float,
    theta: float = 90.0,
    params: EnvironmentalParameters = None,
) -> Union[float, np.ndarray]:
    """Calculate backscattering cross-section using echoSMs PSMSModel.
    
    Uses the Prolate Spheroidal Modal Series (PSMS) model from echoSMs.
    This is an exact analytical solution for prolate spheroid targets
    with support for arbitrary incidence angles.
    
    Automatically handles scalar or array inputs for frequency and radii.
    
    Parameters
    ----------
    frequency : float or array-like
        Acoustic frequency (Hz). Can be scalar or array.
    major_radius : float or array-like
        Prolate spheroid major axis radius (m). Must be >= minor_radius.
        Can be scalar or array.
    minor_radius : float or array-like
        Prolate spheroid minor axis radius (m). Must be <= major_radius.
        Can be scalar or array.
    waterdepth : float
        Water depth (m), positive downward or absolute value used
    theta : float, optional
        Incidence/pitch angle in degrees (default: 90.0)
        - 0° = end-on (acoustic wave along major axis)
        - 90° = broadside (acoustic wave perpendicular to major axis)
        - 180° = end-on from opposite direction
    params : EnvironmentalParameters, optional
        Environmental parameters. If None, uses default values.
    
    Returns
    -------
    float or np.ndarray
        Backscattering cross-section (m²)
    
    Notes
    -----
    The PSMS model uses the echoSMs coordinate convention:
    - theta is the pitch angle
    - 90° corresponds to broadside incidence (dorsal view)
    - 0° and 180° correspond to end-on incidence
    
    **Important**: This is a PROLATE spheroid model where major_radius >= minor_radius.
    For oblate spheroids (minor > major), the model is not applicable.
    
    For spherical bubbles (major_radius == minor_radius), the MSS model
    is used automatically since PSMS has a singularity for spheres.
    
    Examples
    --------
    >>> # Single values - prolate spheroid (cigar-shaped, a > b)
    >>> sigma = calculate_sigma_bs_psms(38000, 0.002, 0.001, 100)
    
    >>> # Multiple radii (prolate 2:1 aspect ratio)
    >>> radii = np.array([0.0005, 0.001, 0.002])
    >>> sigma = calculate_sigma_bs_psms(38000, radii*2, radii, 100, theta=90)
    """
    _check_echosms_available()
    
    if params is None:
        params = EnvironmentalParameters()
    
    freq_is_array = _is_array_like(frequency)
    major_is_array = _is_array_like(major_radius)
    minor_is_array = _is_array_like(minor_radius)
    
    # Case 1: All scalars
    if not freq_is_array and not major_is_array and not minor_is_array:
        return _calculate_sigma_bs_psms_scalar(
            _to_scalar(frequency), _to_scalar(major_radius), 
            _to_scalar(minor_radius), waterdepth, theta, params
        )
    
    # Case 2: Radii are arrays - must loop (gas properties depend on radius)
    if major_is_array or minor_is_array:
        major = np.asarray(major_radius)
        minor = np.asarray(minor_radius)
        
        # Broadcast if one is scalar
        if major.ndim == 0:
            major = np.full_like(minor, major)
        if minor.ndim == 0:
            minor = np.full_like(major, minor)
        
        result = np.empty(major.shape)
        for i, (a, b) in enumerate(zip(major.flat, minor.flat)):
            if freq_is_array:
                # This case would need 2D output - for now just use first freq
                result.flat[i] = _calculate_sigma_bs_psms_scalar(
                    np.asarray(frequency).flat[0], a, b, waterdepth, theta, params
                )
            else:
                result.flat[i] = _calculate_sigma_bs_psms_scalar(
                    _to_scalar(frequency), a, b, waterdepth, theta, params
                )
        return result
    
    # Case 3: Only frequency is array - use echoSMs vectorization
    return _calculate_sigma_bs_psms_freqs(
        frequency, _to_scalar(major_radius), _to_scalar(minor_radius), 
        waterdepth, theta, params
    )


def _calculate_sigma_bs_psms_scalar(
    frequency: float,
    major_radius: float,
    minor_radius: float,
    waterdepth: float,
    theta: float,
    params: EnvironmentalParameters,
) -> float:
    """Internal: Calculate sigma_bs for scalar inputs."""
    # Convert numpy scalars to Python floats for echoSMs compatibility
    frequency = float(frequency)
    major_radius = float(major_radius)
    minor_radius = float(minor_radius)
    
    # PSMS has singularity for spheres (a == b), use MSS instead
    if np.isclose(major_radius, minor_radius, rtol=1e-6):
        return _calculate_sigma_bs_mss_scalar(frequency, minor_radius, waterdepth, params)
    
    # PSMS is for PROLATE spheroids only (a > b)
    if minor_radius > major_radius:
        raise ValueError(
            f"PSMS model requires major_radius >= minor_radius (prolate spheroid). "
            f"Got major_radius={major_radius}, minor_radius={minor_radius}. "
            f"For oblate spheroids, swap the axes or use a different model."
        )
    
    c_gas, rho_gas = get_gas_properties(minor_radius, waterdepth, params)
    
    model = PSMSModel()
    p = {
        'medium_c': float(params.cw),
        'medium_rho': float(params.rho_liq),
        'a': major_radius,
        'b': minor_radius,
        'f': frequency,
        'theta': float(theta),
        'boundary_type': bt.fluid_filled,
        'target_c': float(c_gas),
        'target_rho': float(rho_gas),
    }
    
    ts = model.calculate_ts(p)
    if isinstance(ts, (list, np.ndarray)):
        ts = ts[0]
    
    return np.power(10, 0.1 * ts)


def _calculate_sigma_bs_psms_freqs(
    frequencies: np.ndarray,
    major_radius: float,
    minor_radius: float,
    waterdepth: float,
    theta: float,
    params: EnvironmentalParameters,
) -> np.ndarray:
    """Internal: Calculate sigma_bs for array of frequencies."""
    # PSMS has singularity for spheres
    if np.isclose(major_radius, minor_radius, rtol=1e-6):
        return _calculate_sigma_bs_mss_freqs(frequencies, minor_radius, waterdepth, params)
    
    c_gas, rho_gas = get_gas_properties(minor_radius, waterdepth, params)
    
    model = PSMSModel()
    p = {
        'medium_c': params.cw,
        'medium_rho': params.rho_liq,
        'a': major_radius,
        'b': minor_radius,
        'f': np.asarray(frequencies),
        'theta': theta,
        'boundary_type': bt.fluid_filled,
        'target_c': c_gas,
        'target_rho': rho_gas,
    }
    
    ts = model.calculate_ts(p)
    return np.power(10, 0.1 * np.asarray(ts))


def calculate_target_strength_psms(
    frequency: ArrayLike,
    major_radius: ArrayLike,
    minor_radius: ArrayLike,
    waterdepth: float,
    theta: float = 90.0,
    params: EnvironmentalParameters = None,
) -> Union[float, np.ndarray]:
    """Calculate target strength using echoSMs PSMSModel.
    
    Automatically handles scalar or array inputs.
    
    Parameters
    ----------
    frequency : float or array-like
        Acoustic frequency (Hz)
    major_radius : float or array-like
        Prolate spheroid major axis radius (m)
    minor_radius : float or array-like
        Prolate spheroid minor axis radius (m)
    waterdepth : float
        Water depth (m)
    theta : float, optional
        Incidence/pitch angle in degrees (default: 90.0)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    
    Returns
    -------
    float or np.ndarray
        Target strength (dB re 1 m²)
    
    Notes
    -----
    For spherical bubbles (major_radius == minor_radius), the MSS model
    is used automatically since PSMS has a singularity for spheres.
    """
    sigma_bs = calculate_sigma_bs_psms(frequency, major_radius, minor_radius, waterdepth, theta, params)
    return 10 * np.log10(sigma_bs)


# Legacy aliases for backward compatibility (deprecated)
def calculate_sigma_bs_mss_array(
    frequencies: np.ndarray,
    bubble_radius: float,
    waterdepth: float,
    params: EnvironmentalParameters = None,
    multiprocess: bool = False,
    progress: bool = False,
) -> np.ndarray:
    """Deprecated: Use calculate_sigma_bs_mss() which auto-detects arrays."""
    import warnings
    warnings.warn(
        "calculate_sigma_bs_mss_array is deprecated. "
        "Use calculate_sigma_bs_mss() which auto-detects arrays.",
        DeprecationWarning, stacklevel=2
    )
    return calculate_sigma_bs_mss(frequencies, bubble_radius, waterdepth, params)


def calculate_sigma_bs_mss_radii(
    frequency: float,
    bubble_radii: np.ndarray,
    waterdepth: float,
    params: EnvironmentalParameters = None,
) -> np.ndarray:
    """Deprecated: Use calculate_sigma_bs_mss() which auto-detects arrays."""
    import warnings
    warnings.warn(
        "calculate_sigma_bs_mss_radii is deprecated. "
        "Use calculate_sigma_bs_mss() which auto-detects arrays.",
        DeprecationWarning, stacklevel=2
    )
    return calculate_sigma_bs_mss(frequency, bubble_radii, waterdepth, params)


def calculate_sigma_bs_psms_radii(
    frequency: float,
    major_radii: np.ndarray,
    minor_radii: np.ndarray,
    waterdepth: float,
    theta: float = 90.0,
    params: EnvironmentalParameters = None,
) -> np.ndarray:
    """Deprecated: Use calculate_sigma_bs_psms() which auto-detects arrays."""
    import warnings
    warnings.warn(
        "calculate_sigma_bs_psms_radii is deprecated. "
        "Use calculate_sigma_bs_psms() which auto-detects arrays.",
        DeprecationWarning, stacklevel=2
    )
    return calculate_sigma_bs_psms(frequency, major_radii, minor_radii, waterdepth, theta, params)


def calculate_target_strength_psms_vs_angle(
    frequency: float,
    major_radius: float,
    minor_radius: float,
    waterdepth: float,
    theta_min: float = 0.0,
    theta_max: float = 180.0,
    theta_step: float = 1.0,
    params: EnvironmentalParameters = None,
    multiprocess: bool = False,
    progress: bool = False,
) -> tuple[np.ndarray, np.ndarray]:
    """Calculate target strength vs incidence angle.
    
    Convenience function for computing TS as a function of incidence angle
    for a prolate spheroid bubble.
    
    Parameters
    ----------
    frequency : float
        Acoustic frequency (Hz)
    major_radius : float
        Prolate spheroid major axis radius (m)
    minor_radius : float
        Prolate spheroid minor axis radius (m)
    waterdepth : float
        Water depth (m)
    theta_min : float, optional
        Minimum incidence angle in degrees (default: 0.0)
    theta_max : float, optional
        Maximum incidence angle in degrees (default: 180.0)
    theta_step : float, optional
        Angle step in degrees (default: 1.0)
    params : EnvironmentalParameters, optional
        Environmental parameters.
    multiprocess : bool, optional
        Use multiprocessing (default: False)
    progress : bool, optional
        Show progress bar (default: False)
    
    Returns
    -------
    tuple[np.ndarray, np.ndarray]
        (theta_array, ts_array) - Incidence angles (°) and target strengths (dB)
    """
    _check_echosms_available()
    
    if params is None:
        params = EnvironmentalParameters()
    
    # Get gas properties
    c_gas, rho_gas = get_gas_properties(minor_radius, waterdepth, params)
    
    # Create angle array
    theta_array = np.arange(theta_min, theta_max + theta_step, theta_step)
    
    # Create model and parameters
    model = PSMSModel()
    p = {
        'medium_c': params.cw,
        'medium_rho': params.rho_liq,
        'a': major_radius,
        'b': minor_radius,
        'f': frequency,
        'theta': theta_array,
        'boundary_type': bt.fluid_filled,
        'target_c': c_gas,
        'target_rho': rho_gas,
    }
    
    # Calculate TS
    ts = model.calculate_ts(p, multiprocess=multiprocess, progress=progress)
    ts_array = np.asarray(ts)
    
    return theta_array, ts_array


def get_reference_parameters(model_name: str) -> dict:
    """Get reference model parameters from echoSMs.
    
    Retrieves predefined parameters for benchmark models from Jech et al. (2015).
    
    Parameters
    ----------
    model_name : str
        Name of the reference model. Available gas-filled models:
        - 'gas filled sphere'
        - 'gas filled prolate spheroid'
    
    Returns
    -------
    dict
        Model parameters dictionary
    
    Examples
    --------
    >>> params = get_reference_parameters('gas filled sphere')
    >>> print(params)
    {'boundary_type': 'fluid-filled', 'a': 0.01, 'medium_rho': 1026.8, ...}
    """
    _check_echosms_available()
    
    rm = echosms.ReferenceModels()
    return rm.parameters(model_name)


def list_reference_models() -> list[str]:
    """List all available reference model names in echoSMs.
    
    Returns
    -------
    list[str]
        List of model names
    """
    _check_echosms_available()
    
    rm = echosms.ReferenceModels()
    return rm.names()


def get_benchmark_data_frequency(model_name: str) -> tuple[np.ndarray, np.ndarray]:
    """Get benchmark TS vs frequency data from echoSMs.
    
    Parameters
    ----------
    model_name : str
        Name of the benchmark model (e.g., 'gas filled sphere')
    
    Returns
    -------
    tuple[np.ndarray, np.ndarray]
        (frequencies, ts) - Frequency array (Hz) and TS array (dB)
    """
    _check_echosms_available()
    
    bm = echosms.BenchmarkData()
    return bm.freq_data(model_name)


def get_benchmark_data_angle(model_name: str) -> tuple[np.ndarray, np.ndarray]:
    """Get benchmark TS vs angle data from echoSMs.
    
    Parameters
    ----------
    model_name : str
        Name of the benchmark model (e.g., 'gas filled prolate spheroid')
    
    Returns
    -------
    tuple[np.ndarray, np.ndarray]
        (theta, ts) - Angle array (°) and TS array (dB)
    """
    _check_echosms_available()
    
    bm = echosms.BenchmarkData()
    return bm.angle_data(model_name)
