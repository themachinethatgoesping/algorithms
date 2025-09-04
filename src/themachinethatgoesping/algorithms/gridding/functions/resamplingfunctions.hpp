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

#include <fmt/core.h>
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

    // Filter finite values
    const auto values_min_filtered = xt::filter(values_min, xt::isfinite(values_min));
    const auto values_max_filtered = xt::filter(values_max, xt::isfinite(values_max));
    const auto values_res_filtered = xt::filter(values_res, xt::isfinite(values_res));

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

    // Calculate min grid resoltuion using 75%-25% quantile heuristics
    const auto quantiles =
        xt::quantile(values_res_filtered, { t_value(0.25), t_value(0.50), t_value(0.75) });
    const auto    iqr_res                  = quantiles.unchecked(2) - quantiles.unchecked(0);
    const t_value heuristic_min_resolution = quantiles.unchecked(1) - iqr_res * t_value(1.5);

    // select real or heuristic min/max/res
    const t_value res   = std::max(t_value(xt::amin(values_res_filtered)()), heuristic_min_resolution);
    const t_value y_min = std::max(t_value(xt::amin(values_min_filtered)()), heuristic_min);
    const t_value y_max = std::min(t_value(xt::amax(values_max_filtered)()), heuristic_max);

    // Generate coordinates
    auto coordinates = xt::arange(y_min, y_max + res, res);

    if (coordinates.size() > max_steps){
        return xt::linspace(y_min, y_max, max_steps);
    }

    return coordinates;
}

} // namespace functions
} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
