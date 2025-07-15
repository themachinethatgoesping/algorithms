// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/grow_regions.doc.hpp"

/* generated doc strings */
#include ".docstrings/find_local_maxima.doc.hpp"

#include <optional>
#include <vector>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <xtensor/containers/xadapt.hpp>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/views/xview.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace imageprocessing {
namespace functions {

template<typename t_region>
inline bool is_null_region(const t_region& region, const t_region& null_region)
{
    // if t_region is a floating point check if null_region is NaN
    if constexpr (std::is_floating_point_v<t_region>)
    {
        if (std::isnan(null_region))
            return std::isnan(region);
    }

    return region == null_region;
}

template<typename t_region_view>
inline auto get_non_null_mask(const t_region_view&                      region_view,
                              const typename t_region_view::value_type& null_region)
{
    if constexpr (std::is_floating_point_v<typename t_region_view::value_type>)
    {
        if (std::isnan(null_region))
            return xt::isfinite(region_view);
    }

    return xt::not_equal(region_view, null_region);
}

template<tools::helper::c_xtensor_3d t_xtensor_regions, tools::helper::c_xtensor_3d t_xtensor_data>
/**
 * @brief Grow regions in a 3D volume based on data values and neighboring regions.
 *
 * This function expands existing regions by assigning region labels to neighboring null cells
 * that meet certain criteria. A cell is assigned the region label of its neighbor with the
 * highest data value, provided the cell's own data value exceeds the specified threshold.
 *
 * @tparam t_xtensor_regions Type of the 3D tensor containing region labels
 * @tparam t_xtensor_data Type of the 3D tensor containing data values
 *
 * @param regions_volume 3D tensor containing region labels, will be modified by the function
 * @param data_volume 3D tensor containing data values, used to determine region growth
 * @param null_region Value representing unlabeled/null regions in the regions_volume
 * @param threshold Optional minimum data value for a cell to be considered for region assignment
 *                  If not provided, all cells are considered regardless of value
 * @param force_negative_gradient If true, only grow regions where the neighbor's data value
 *                                is less than the current cell's value (enforcing a negative
 * gradient)
 * @param mp_cores Number of cores to use for parallel processing
 *
 * @return true if at least one cell was assigned to a region, false otherwise
 *
 * @throws std::invalid_argument if regions_volume and data_volume have different shapes
 *
 * @note The function uses OpenMP for parallel processing when mp_cores > 1
 * @note Template parameters must be 3D tensors (checked via static_assert)
 */
bool grow_regions(
    [[maybe_unused]] t_xtensor_regions&
        regions_volume, // is definitely used, the [[]] is only added to work around clang bug
    const t_xtensor_data&                                    data_volume,
    const typename t_xtensor_regions::value_type             null_region,
    const std::optional<typename t_xtensor_data::value_type> threshold               = std::nullopt,
    const bool                                               force_negative_gradient = true,
    const int                                                mp_cores                = 1)
{
    using value_type  = typename t_xtensor_data::value_type;
    using region_type = typename t_xtensor_regions::value_type;

    static_assert(tools::helper::c_xtensor_3d<t_xtensor_regions>,
                  "Template parameter must be a 3D tensor");
    static_assert(tools::helper::c_xtensor_3d<t_xtensor_data>,
                  "Template parameter must be a 3D tensor");

    if (regions_volume.shape() != data_volume.shape())
        throw std::invalid_argument("regions_volume and data_volume must have the same shape.");

    const int64_t nz = data_volume.shape()[2];
    const int64_t ny = data_volume.shape()[1];
    const int64_t nx = data_volume.shape()[0];

    // create vectors to store region changes
    std::vector<xt::xtensor_fixed<int64_t, xt::xshape<3>>> grow_region_indices;
    std::vector<region_type>                               grow_region_regions;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for
        for (int64_t x = 0; x < nx; ++x)
        {
            const int64_t start_x = std::max(x - 1, int64_t(0));
            const int64_t end_x   = std::min(x + 2, nx);
            for (int64_t y = 0; y < ny; ++y)
            {
                const int64_t start_y = std::max(y - 1, int64_t(0));
                const int64_t end_y   = std::min(y + 2, ny);
                for (int64_t z = 0; z < nz; ++z)
                {
                    const int64_t start_z = std::max(z - 1, int64_t(0));
                    const int64_t end_z   = std::min(z + 2, nz);

                    // test if current neighbor is null
                    const region_type region = regions_volume.unchecked(x, y, z);

                    if (!is_null_region(region, null_region))
                        continue;

                    // test if current value exceeds threshold
                    const value_type val = data_volume.unchecked(x, y, z);

                    if (!(val > threshold_val))
                        continue;

                    // Get the neighboring regions
                    auto regions_neighborhood = xt::view(regions_volume,
                                                         xt::range(start_x, end_x),
                                                         xt::range(start_y, end_y),
                                                         xt::range(start_z, end_z));

                    // Create a boolean mask for non-null regions
                    auto non_null_mask = get_non_null_mask(regions_neighborhood, null_region);

                    // Filter regions values based on non-null mask
                    auto neighbor_regions = xt::filter(regions_neighborhood, non_null_mask);

                    // Check if there are any non-null regions
                    if (neighbor_regions.size() == 0)
                        continue;

                    // Filter data values based on non-null mask
                    auto neighborhood_data = xt::view(data_volume,
                                                      xt::range(start_x, end_x),
                                                      xt::range(start_y, end_y),
                                                      xt::range(start_z, end_z));

                    auto non_null_data = xt::filter(neighborhood_data, non_null_mask);

                    // get the index of the maximum value in the non-null data
                    auto max_index = xt::argmax(non_null_data)();

                    // test gradient
                    if (force_negative_gradient && non_null_data.unchecked(max_index) < val)
                        continue;

#pragma omp critical
                    {
                        grow_region_indices.push_back({ x, y, z });
                        grow_region_regions.push_back(neighbor_regions.unchecked(max_index));
                    }
                }
            }
        }
    }

    // update regions
    xt::index_view(regions_volume, grow_region_indices) = xt::adapt(grow_region_regions);

    return grow_region_indices.size() > 0;
}

template<tools::helper::c_xtensor_2d t_xtensor_regions, tools::helper::c_xtensor_2d t_xtensor_data>
/**
 * @brief Grow regions in a 2D image based on data values and neighboring regions.
 *
 * This function expands existing regions by assigning region labels to neighboring null cells
 * that meet certain criteria. A cell is assigned the region label of its neighbor with the
 * highest data value, provided the cell's own data value exceeds the specified threshold.
 *
 * @tparam t_xtensor_regions Type of the 2D tensor containing region labels
 * @tparam t_xtensor_data Type of the 2D tensor containing data values
 *
 * @param regions_image 2D tensor containing region labels, will be modified by the function
 * @param data_image 2D tensor containing data values, used to determine region growth
 * @param null_region Value representing unlabeled/null regions in the regions_image
 * @param threshold Optional minimum data value for a cell to be considered for region assignment
 *                  If not provided, all cells are considered regardless of value
 * @param force_negative_gradient If true, only grow regions where the neighbor's data value
 *                                is less than the current cell's value (enforcing a negative
 * gradient)
 * @param mp_cores Number of cores to use for parallel processing
 *
 * @return true if at least one cell was assigned to a region, false otherwise
 *
 * @throws std::invalid_argument if regions_image and data_image have different shapes
 *
 * @note The function uses OpenMP for parallel processing when mp_cores > 1
 * @note Template parameters must be 2D tensors (checked via static_assert)
 */
bool grow_regions([[maybe_unused]] t_xtensor_regions&                      regions_image,
                  const t_xtensor_data&                                    data_image,
                  const typename t_xtensor_regions::value_type             null_region,
                  const std::optional<typename t_xtensor_data::value_type> threshold = std::nullopt,
                  const bool force_negative_gradient                                 = true,
                  const int  mp_cores                                                = 1)
{
    using value_type  = typename t_xtensor_data::value_type;
    using region_type = typename t_xtensor_regions::value_type;

    static_assert(tools::helper::c_xtensor_2d<t_xtensor_regions>,
                  "Template parameter must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_data>,
                  "Template parameter must be a 2D tensor");

    if (regions_image.shape() != data_image.shape())
        throw std::invalid_argument("regions_image and data_image must have the same shape.");

    const int64_t ny = data_image.shape()[1];
    const int64_t nx = data_image.shape()[0];

    // create vectors to store region changes
    std::vector<xt::xtensor_fixed<int64_t, xt::xshape<2>>> grow_region_indices;
    std::vector<region_type>                               grow_region_regions;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for
        for (int64_t x = 0; x < nx; ++x)
        {
            const int64_t start_x = std::max(x - 1, int64_t(0));
            const int64_t end_x   = std::min(x + 2, nx);
            for (int64_t y = 0; y < ny; ++y)
            {
                const int64_t start_y = std::max(y - 1, int64_t(0));
                const int64_t end_y   = std::min(y + 2, ny);

                // test if current neighbor is null
                const region_type region = regions_image.unchecked(x, y);

                if (!is_null_region(region, null_region))
                    continue;

                // test if current value exceeds threshold
                const value_type val = data_image.unchecked(x, y);

                if (!(val > threshold_val))
                    continue;

                // Get the neighboring regions
                auto regions_neighborhood =
                    xt::view(regions_image, xt::range(start_x, end_x), xt::range(start_y, end_y));

                // Create a boolean mask for non-null regions
                auto non_null_mask = get_non_null_mask(regions_neighborhood, null_region);

                // Filter regions values based on non-null mask
                auto neighbor_regions = xt::filter(regions_neighborhood, non_null_mask);

                // Check if there are any non-null regions
                if (neighbor_regions.size() == 0)
                    continue;

                // Filter data values based on non-null mask
                auto neighborhood_data =
                    xt::view(data_image, xt::range(start_x, end_x), xt::range(start_y, end_y));

                auto non_null_data = xt::filter(neighborhood_data, non_null_mask);

                // get the index of the maximum value in the non-null data
                auto max_index = xt::argmax(non_null_data)();

                // test gradient
                if (force_negative_gradient && non_null_data.unchecked(max_index) < val)
                    continue;

#pragma omp critical
                {
                    grow_region_indices.push_back({ x, y });
                    grow_region_regions.push_back(neighbor_regions.unchecked(max_index));
                }
            }
        }
    }

    // update regions
    xt::index_view(regions_image, grow_region_indices) = xt::adapt(grow_region_regions);

    return grow_region_indices.size() > 0;
}

template<tools::helper::c_xtensor_1d t_xtensor_regions, tools::helper::c_xtensor_1d t_xtensor_data>
/**
 * @brief Grow regions in a 1D array based on data values and neighboring regions.
 *
 * This function expands existing regions by assigning region labels to neighboring null cells
 * that meet certain criteria. A cell is assigned the region label of its neighbor with the
 * highest data value, provided the cell's own data value exceeds the specified threshold.
 *
 * @tparam t_xtensor_regions Type of the 1D tensor containing region labels
 * @tparam t_xtensor_data Type of the 1D tensor containing data values
 *
 * @param regions_array 1D tensor containing region labels, will be modified by the function
 * @param data_array 1D tensor containing data values, used to determine region growth
 * @param null_region Value representing unlabeled/null regions in the regions_array
 * @param threshold Optional minimum data value for a cell to be considered for region assignment
 *                  If not provided, all cells are considered regardless of value
 * @param force_negative_gradient If true, only grow regions where the neighbor's data value
 *                                is less than the current cell's value (enforcing a negative
 * gradient)
 * @param mp_cores Number of cores to use for parallel processing
 *
 * @return true if at least one cell was assigned to a region, false otherwise
 *
 * @throws std::invalid_argument if regions_array and data_array have different shapes
 *
 * @note The function uses OpenMP for parallel processing when mp_cores > 1
 * @note Template parameters must be 1D tensors (checked via static_assert)
 */
bool grow_regions([[maybe_unused]] t_xtensor_regions&                      regions_array,
                  const t_xtensor_data&                                    data_array,
                  const typename t_xtensor_regions::value_type             null_region,
                  const std::optional<typename t_xtensor_data::value_type> threshold = std::nullopt,
                  const bool force_negative_gradient                                 = true,
                  const int  mp_cores                                                = 1)
{
    using value_type  = typename t_xtensor_data::value_type;
    using region_type = typename t_xtensor_regions::value_type;

    static_assert(tools::helper::c_xtensor_1d<t_xtensor_regions>,
                  "Template parameter must be a 1D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_data>,
                  "Template parameter must be a 1D tensor");

    if (regions_array.shape() != data_array.shape())
        throw std::invalid_argument("regions_array and data_array must have the same shape.");

    const int64_t nx = data_array.shape()[0];

    // create vectors to store region changes
    std::vector<xt::xtensor_fixed<int64_t, xt::xshape<1>>> grow_region_indices;
    std::vector<region_type>                               grow_region_regions;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for
        for (int64_t x = 0; x < nx; ++x)
        {
            const int64_t start_x = std::max(x - 1, int64_t(0));
            const int64_t end_x   = std::min(x + 2, nx);

            // test if current neighbor is null
            const region_type region = regions_array.unchecked(x);

            if (!is_null_region(region, null_region))
                continue;

            // test if current value exceeds threshold
            const value_type val = data_array.unchecked(x);

            if (!(val > threshold_val))
                continue;

            // Get the neighboring regions
            auto regions_neighborhood = xt::view(regions_array, xt::range(start_x, end_x));

            // Create a boolean mask for non-null regions
            auto non_null_mask = get_non_null_mask(regions_neighborhood, null_region);

            // Filter regions values based on non-null mask
            auto neighbor_regions = xt::filter(regions_neighborhood, non_null_mask);

            // Check if there are any non-null regions
            if (neighbor_regions.size() == 0)
                continue;

            // Filter data values based on non-null mask
            auto neighborhood_data = xt::view(data_array, xt::range(start_x, end_x));

            auto non_null_data = xt::filter(neighborhood_data, non_null_mask);

            // get the index of the maximum value in the non-null data
            auto max_index = xt::argmax(non_null_data)();

            // test gradient
            if (force_negative_gradient && non_null_data.unchecked(max_index) < val)
                continue;

#pragma omp critical
            {
                grow_region_indices.push_back({ x });
                grow_region_regions.push_back(neighbor_regions.unchecked(max_index));
            }
        }
    }

    // update regions
    xt::index_view(regions_array, grow_region_indices) = xt::adapt(grow_region_regions);

    return grow_region_indices.size() > 0;
}

}
}
}
}