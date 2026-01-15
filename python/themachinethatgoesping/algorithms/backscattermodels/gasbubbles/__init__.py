import numpy as np

from .parameters import *  # noqa: F403
from . import thuraisingham
from . import li
from . import bubble_dissolution

import matplotlib.pyplot as plt


def plot_bs_comparison(
    frequency_hz: float = 300e3,
    water_depth_m: float = 50.0,
    bubble_diameter_min_mm: float = 0.001,
    bubble_diameter_max_mm: float = 20.0,
    num_points: int = 50000,
    ax: plt.Axes | None = None,
) -> plt.Axes:
    """
    Plot backscattering strength from both Thuraisingham and Li models vs bubble diameter.

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
        Number of points for the plot (default: 500)
    ax : plt.Axes | None
        Matplotlib axes to plot on. If None, creates new figure.

    Returns
    -------
    plt.Axes
        The matplotlib axes with the plot
    """
    # Generate bubble radii on log scale (convert mm to m for models)
    radii_mm = np.linspace(bubble_diameter_min_mm, bubble_diameter_max_mm, num_points)/ 2
    radii_m = radii_mm * 1e-3   # Convert diameter mm to radius m

    # Calculate backscattering strength from both models
    bs_thuraisingham = np.array(
        [
            thuraisingham.calculate_sigma_bs(frequency_hz, r, water_depth_m)
            for r in radii_m
        ]
    )
    bs_li = np.array(
        [li.calculate_sigma_bs(frequency_hz, r, water_depth_m) for r in radii_m]
    )

    # Convert to dB
    bs_thuraisingham_db = 10 * np.log10(bs_thuraisingham)
    bs_li_db = 10 * np.log10(bs_li)

    # Create plot
    if ax is None:
        fig, ax = plt.subplots(figsize=(10, 6))

    ax.plot(radii_mm*2, bs_thuraisingham_db, label="Thuraisingham", linewidth=2)
    ax.plot(radii_mm*2, bs_li_db, label="Li", linewidth=2, linestyle="--")

    ax.set_xlabel("Bubble diameter (mm)")
    ax.set_ylabel("Backscattering strength (dB)")
    ax.set_title(
        f"Gas Bubble Backscattering: f = {frequency_hz/1e3:.0f} kHz, "
        f"depth = {water_depth_m:.0f} m"
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