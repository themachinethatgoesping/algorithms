#pragma once

/* generated doc strings */
#include ".docstrings/resamplingfunctions.doc.hpp"

/* generated doc strings */
#include ".docstrings/gridfunctions.doc.hpp"

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

#include <xtensor/misc/xsort.hpp>
#include <xtensor/views/xindex_view.hpp>

#include <fmt/format.h>
#include <map>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <tuple>
#include <vector>

namespace themachinethatgoesping {
namespace algorithms {
namespace gridding {
namespace functions {

/**
 * @brief Compute resampled coordinates for gridding operations using statistical heuristics
 *
 * This function generates a uniform grid of coordinates based on input data ranges and resolutions.
 * It uses statistical analysis (quantiles and IQR) to determine appropriate grid bounds and
 * resolution when explicit limits are not provided or when heuristic bounds are more suitable.
 *
 * @param values_min Array of minimum values for each data point (e.g., depth ranges)
 * @param values_max Array of maximum values for each data point
 * @param values_res Array of resolution values for each data point
 * @param grid_min Explicit minimum grid bound (optional)
 * @param grid_max Explicit maximum grid bound (optional)
 * @param max_steps Maximum number of grid points. If exceeded, switches to linear spacing
 *
 * @return coordinates: xtensor array of uniformly spaced grid coordinates
 **/
template<tools::helper::c_xtensor_1d t_xtensor_1d>
inline t_xtensor_1d compute_resampled_coordinates(
    const t_xtensor_1d&                     values_min,
    const t_xtensor_1d&                     values_max,
    const t_xtensor_1d&                     values_res,
    const typename t_xtensor_1d::value_type grid_min =
        std::numeric_limits<typename t_xtensor_1d::value_type>::quiet_NaN(),
    const typename t_xtensor_1d::value_type grid_max =
        std::numeric_limits<typename t_xtensor_1d::value_type>::quiet_NaN(),
    const size_t max_steps = 1024)
{
    using namespace xt;
    using t_value = typename t_xtensor_1d::value_type;

    // Filter to finite extent values. Min/max define the grid span.
    const auto values_min_filtered = xt::filter(values_min, xt::isfinite(values_min));
    const auto values_max_filtered = xt::filter(values_max, xt::isfinite(values_max));

    // Filter resolutions to strictly positive, finite values only. A per-element
    // resolution of zero (or negative) represents a degenerate input - e.g. a
    // ping whose min/max extent collapsed onto a single sample - and must not be
    // allowed to drive the global grid resolution to zero. Including such values
    // previously produced a zero step size and therefore an empty coordinate
    // array (and a downstream IndexError in the consumers).
    const auto res_valid_mask    = xt::eval(xt::isfinite(values_res) && (values_res > t_value(0)));
    const auto values_res_filtered = xt::filter(values_res, res_valid_mask);

    // Without any finite extent we cannot build a meaningful grid.
    if (values_min_filtered.size() == 0 || values_max_filtered.size() == 0)
        throw std::invalid_argument(
            "compute_resampled_coordinates: values_min/values_max contain no finite values");

    // Calculate grid bounds using 90%-10% quantile heuristics
    t_value heuristic_min, heuristic_max;

    if (!std::isfinite(grid_min))
    {
        const auto quantiles =
            xt::quantile(values_min_filtered, { t_value(0.10), t_value(0.50), t_value(0.90) });
        const auto iqr = quantiles.unchecked(2) - quantiles.unchecked(0);
        heuristic_min  = quantiles.unchecked(1) - iqr * t_value(1.5);
    }
    else
        heuristic_min = grid_min;

    if (!std::isfinite(grid_max))
    {
        const auto quantiles =
            xt::quantile(values_max_filtered, { t_value(0.10), t_value(0.50), t_value(0.90) });
        const auto iqr = quantiles.unchecked(2) - quantiles.unchecked(0);
        heuristic_max  = quantiles.unchecked(1) + iqr * t_value(1.5);
    }
    else
        heuristic_max = grid_max;

    // select real or heuristic min/max
    t_value y_min = std::max(t_value(xt::amin(values_min_filtered)()), heuristic_min);
    t_value y_max = std::min(t_value(xt::amax(values_max_filtered)()), heuristic_max);

    // Calculate the grid resolution from the (positive) per-element resolutions
    // using 75%-25% quantile heuristics. The heuristic clamps away tiny outlier
    // resolutions while the global minimum keeps the grid fine enough to not
    // lose data.
    t_value res = std::numeric_limits<t_value>::quiet_NaN();
    if (values_res_filtered.size() > 0)
    {
        const auto quantiles =
            xt::quantile(values_res_filtered, { t_value(0.25), t_value(0.50), t_value(0.75) });
        const auto    iqr_res = quantiles.unchecked(2) - quantiles.unchecked(0);
        const t_value heuristic_min_resolution = quantiles.unchecked(1) - iqr_res * t_value(1.5);
        res = std::max(t_value(xt::amin(values_res_filtered)()), heuristic_min_resolution);
    }

    // Guarantee a finite, strictly positive resolution. When no usable resolution
    // exists (all inputs degenerate) fall back to spanning the grid range with
    // max_steps cells.
    if (!std::isfinite(res) || res <= t_value(0))
    {
        const t_value span  = y_max - y_min;
        const size_t  steps = max_steps > 1 ? max_steps : size_t(2);
        res = (std::isfinite(span) && span > t_value(0)) ? span / t_value(steps) : t_value(1);
    }

    // Guarantee a non-degenerate span so at least one grid cell (two coordinates)
    // is produced even when all extents collapsed onto a single value.
    if (!(y_max > y_min))
        y_max = y_min + res;

    // Predict the regular-grid size (matching xt::arange's element count) so we
    // can switch to a bounded number of steps without first materializing an
    // oversized array.
    const size_t n_points = static_cast<size_t>(std::ceil(((y_max + res) - y_min) / res));

    if (n_points > max_steps)
        return xt::linspace(y_min, y_max, max_steps < 2 ? size_t(2) : max_steps);

    return xt::arange(y_min, y_max + res, res);
}

} // namespace functions
} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
