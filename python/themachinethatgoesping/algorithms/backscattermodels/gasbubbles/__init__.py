"""
Gas bubble backscattering models.

This module provides implementations of various bubble acoustic backscatter models:

- **medwin**: Medwin (1977) / Clay & Medwin - Classic damped resonant bubble model
- **anderson**: Anderson (1950) - Fluid sphere scattering (modal/partial wave solution)
- **ainslie_leighton**: Ainslie & Leighton (2009, 2011) - Comprehensive model with
  complex polytropic index and near-resonant corrections
- **li**: Li et al. (2020) - Based on Ainslie-Leighton with Thuraisingham correction
- **thuraisingham**: Thuraisingham (1997) - High-frequency correction model
- **stanton**: Stanton (1989) - High-pass heuristic model
- **spheroidal**: Non-spherical bubble corrections for oblate spheroid bubbles
- **echosms_wrapper**: Wrapper for echoSMs library (exact modal solutions):
  - MSSModel: Modal Series Solution for spheres
  - PSMSModel: Prolate Spheroidal Modal Series with incidence angle support

Shape regimes (based on Eötvös number):
- Eo < 0.1: Spherical (r < ~0.35mm in seawater) - use any spherical model
- 0.1 < Eo < 10: Ellipsoidal/Oblate (0.35mm < r < ~3.5mm) - use spheroidal module
- Eo > 10: Spherical cap (r > ~3.5mm) - complex regime, models less reliable

References
----------
- Anderson, V.C. (1950). JASA 22(4), 426-431.
- Medwin, H. (1977). Ultrasonics 15, 7-13.
- Clay, C.S. & Medwin, H. (1998). Fundamentals of Acoustical Oceanography.
- Stanton, T.K. (1989). JASA 86(4), 1499-1510.
- Feuillade, C. & Werby, M.F. (1994). JASA 96(6), 3684-3692.
- Thuraisingham, R.A. (1997). Ultrasonics 35, 357-366.
- Ainslie, M.A. & Leighton, T.G. (2009). JASA 126(5), 2163-2175.
- Ainslie, M.A. & Leighton, T.G. (2011). JASA 130(5), 3184-3208.
- Li, J. et al. (2020). JGR Oceans 125(9), e2020JC016360.
- Clift, R., Grace, J.R. & Weber, M.E. (1978). Bubbles, Drops, and Particles.
- Jech, J.M. et al. (2015). JASA 138, 3742-3764. (echoSMs benchmark models)
"""

import numpy as np

from .parameters import *  # noqa: F403
from . import thuraisingham
from . import li
from . import medwin
from . import anderson
from . import ainslie_leighton
from . import stanton
from . import spheroidal
from . import bubble_dissolution
from . import echosms_wrapper

import matplotlib.pyplot as plt


def plot_bs_comparison(
    frequency_hz: float = 300e3,
    water_depth_m: float = 50.0,
    bubble_diameter_min_mm: float = 0.001,
    bubble_diameter_max_mm: float = 20.0,
    num_points: int = 50000,
    ax: plt.Axes | None = None,
    models: list[str] | None = None,
) -> plt.Axes:
    """
    Plot backscattering strength from multiple models vs bubble diameter.

    Parameters
    ----------
    frequency_hz : float
        Acoustic frequency in Hz (default: 300 kHz)
    water_depth_m : float
        Water depth in meters (default: 50 m)
    bubble_diameter_min_mm : float
        Minimum bubble diameter in mm (default: 0.001 mm)
    bubble_diameter_max_mm : float
        Maximum bubble diameter in mm (default: 20 mm)
    num_points : int
        Number of points for the plot (default: 50000)
    ax : plt.Axes | None
        Matplotlib axes to plot on. If None, creates new figure.
    models : list[str] | None
        List of models to plot. Options: 'thuraisingham', 'li', 'medwin', 
        'anderson', 'ainslie_leighton'. If None, plots all models.

    Returns
    -------
    plt.Axes
        The matplotlib axes with the plot
    """
    if models is None:
        models = ['thuraisingham', 'li', 'medwin', 'ainslie_leighton', 'stanton']
    
    # Generate bubble radii on log scale (convert mm to m for models)
    radii_mm = np.linspace(bubble_diameter_min_mm, bubble_diameter_max_mm, num_points) / 2
    radii_m = radii_mm * 1e-3   # Convert diameter mm to radius m

    # Model configurations
    model_config = {
        'thuraisingham': {
            'module': thuraisingham,
            'label': 'Thuraisingham (1997)',
            'linestyle': '-',
            'linewidth': 2,
        },
        'li': {
            'module': li,
            'label': 'Li et al. (2020)',
            'linestyle': '--',
            'linewidth': 2,
        },
        'medwin': {
            'module': medwin,
            'label': 'Medwin (1977)',
            'linestyle': '-.',
            'linewidth': 2,
        },
        'anderson': {
            'module': anderson,
            'label': 'Anderson (1950)',
            'linestyle': ':',
            'linewidth': 2,
        },
        'ainslie_leighton': {
            'module': ainslie_leighton,
            'label': 'Ainslie & Leighton (2011)',
            'linestyle': '-',
            'linewidth': 1.5,
        },
        'stanton': {
            'module': stanton,
            'label': 'Stanton (1989)',
            'linestyle': '--',
            'linewidth': 1.5,
        },
        'spheroidal': {
            'module': spheroidal,
            'label': 'Spheroidal (oblate)',
            'linestyle': '-.',
            'linewidth': 1.5,
        },
    }

    # Create plot
    if ax is None:
        fig, ax = plt.subplots(figsize=(12, 7))

    # Calculate and plot each model
    for model_name in models:
        if model_name not in model_config:
            print(f"Warning: Unknown model '{model_name}', skipping.")
            continue
        
        config = model_config[model_name]
        module = config['module']
        
        bs_values = np.array([
            module.calculate_sigma_bs(frequency_hz, r, water_depth_m)
            for r in radii_m
        ])
        
        # Convert to dB
        bs_db = 10 * np.log10(np.clip(bs_values, 1e-50, None))
        
        ax.plot(
            radii_mm * 2, 
            bs_db, 
            label=config['label'],
            linestyle=config['linestyle'],
            linewidth=config['linewidth'],
        )

    ax.set_xlabel("Bubble diameter (mm)")
    ax.set_ylabel("Backscattering strength (dB re 1 m²)")
    ax.set_title(
        f"Gas Bubble Backscattering Model Comparison\n"
        f"f = {frequency_hz/1e3:.0f} kHz, depth = {water_depth_m:.0f} m"
    )
    ax.legend()
    ax.grid(True, which="both", linestyle="-", alpha=0.3)

    return ax


def main():
    """Main function to demonstrate bubble backscattering model comparison."""
    plot_bs_comparison()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()