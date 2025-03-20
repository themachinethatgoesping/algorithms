// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/find_local_maxima2.doc.hpp"

#include <omp.h>
#include <vector>
#include <xtensor/xadapt.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xmanipulation.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace imageprocessing {
namespace functions {

template<typename T>
inline auto get_max_val2(const T& a, const bool& accept_nans)
{
    if constexpr (std::is_floating_point_v<T>)
    {
        if (accept_nans)
        {
            return xt::nanmax(a)();
        }
    }

    return xt::amax(a)();
}

template<tools::helper::c_xtensor_3d t_xtensor_3d>
/**
 * @brief Finds the local maxima in a 3D tensor.
 *
 * This function searches for local maxima in a 3D tensor `data`. A local maximum
 * is defined as a value that is greater than or equal to all of its 26 neighbors.
 * The function can optionally apply a threshold to ignore values below a certain
 * level.
 *
 * @tparam t_xtensor_3d The type of the 3D tensor.
 * @param data The 3D tensor in which to find local maxima.
 * @param threshold An optional threshold value. Only values greater than this
 *                  threshold will be considered as potential local maxima.
 * @param mp_cores The number of cores to use for parallel processing. Defaults to 1.
 * @return A tuple containing four vectors: X, Y, Z coordinates of the local maxima,
 *         and their corresponding values.
 *
 * @note The function uses OpenMP for parallel processing.
 * @note The template parameter must be a 3D tensor.
 */
auto find_local_maxima2(
    const t_xtensor_3d&                                    data,
    const std::optional<typename t_xtensor_3d::value_type> threshold   = std::nullopt,
    const bool                                             accept_nans = true,
    const int                                              mp_cores    = 1)
{
    using value_type = typename t_xtensor_3d::value_type;

    static_assert(tools::helper::c_xtensor_3d<t_xtensor_3d>,
                  "Template parameter must be a 3D tensor");

    const int64_t max_z = data.shape()[2] - 1;
    const int64_t max_y = data.shape()[1] - 1;
    const int64_t max_x = data.shape()[0] - 1;

    std::vector<xt::xtensor_fixed<int64_t, xt::xshape<3>>> XYZ;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for collapse(3) schedule(guided)
        for (int64_t x = 1; x < max_x; ++x)
        {
            for (int64_t y = 1; y < max_y; ++y)
            {
                for (int64_t z = 1; z < max_z; ++z)
                {
                    const value_type val = data.unchecked(x, y, z);

                    if (!(val > threshold_val))
                        continue;

                    auto neighborhood = xt::view(data,
                                                 xt::range(x - 1, x + 2),
                                                 xt::range(y - 1, y + 2),
                                                 xt::range(z - 1, z + 2));

                    if (val == get_max_val2(neighborhood, accept_nans))
                    {
#pragma omp critical
                        {
                            XYZ.push_back({ x, y, z });
                        }
                    }
                }
            }
        }
    }

    return XYZ;
}

template<tools::helper::c_xtensor_2d t_xtensor_2d>
/**
 * @brief Finds the local maxima in a 2D tensor.
 *
 * This function searches for local maxima in a 2D tensor `data`. A local maximum
 * is defined as a value that is greater than or equal to all of its 8 neighbors.
 * The function can optionally apply a threshold to ignore values below a certain
 * level.
 *
 * @tparam t_xtensor_2d The type of the 2D tensor.
 * @param data The 2D tensor in which to find local maxima.
 * @param threshold An optional threshold value. Only values greater than this
 *                  threshold will be considered as potential local maxima.
 * @param mp_cores The number of cores to use for parallel processing. Defaults to 1.
 * @return A tuple containing three vectors: X, Y coordinates of the local maxima,
 *         and their corresponding values.
 *
 * @note The function uses OpenMP for parallel processing.
 * @note The template parameter must be a 2D tensor.
 */
auto find_local_maxima2(
    const t_xtensor_2d&                                    data,
    const std::optional<typename t_xtensor_2d::value_type> threshold   = std::nullopt,
    const bool                                             accept_nans = true,
    const int                                              mp_cores    = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");

    const int64_t max_y = data.shape()[1] - 1;
    const int64_t max_x = data.shape()[0] - 1;

    std::vector<xt::xtensor_fixed<int64_t, xt::xshape<2>>> XY;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for collapse(2) schedule(guided)
        for (int64_t x = 1; x < max_x; ++x)
        {
            for (int64_t y = 1; y < max_y; ++y)
            {
                const value_type val = data.unchecked(x, y);

                if (!(val > threshold_val))
                    continue;

                auto neighborhood =
                    xt::view(data, xt::range(x - 1, x + 2), xt::range(y - 1, y + 2));

                if (val == get_max_val2(neighborhood, accept_nans))
                {
#pragma omp critical
                    {
                        XY.push_back({ x, y });
                    }
                }
            }
        }
    }

    return XY;
}

template<tools::helper::c_xtensor_1d t_xtensor_1d>
/**
 * @brief Finds the local maxima in a 1D tensor.
 *
 * This function searches for local maxima in a 1D tensor `data`. A local maximum
 * is defined as a value that is greater than or equal to its two neighbors.
 * The function can optionally apply a threshold to ignore values below a certain
 * level.
 *
 * @tparam t_xtensor_1d The type of the 1D tensor.
 * @param data The 1D tensor in which to find local maxima.
 * @param threshold An optional threshold value. Only values greater than this
 *                  threshold will be considered as potential local maxima.
 * @param mp_cores The number of cores to use for parallel processing. Defaults to 1.
 * @return A tuple containing two vectors: indices of the local maxima,
 *         and their corresponding values.
 *
 * @note The function uses OpenMP for parallel processing.
 * @note The template parameter must be a 1D tensor.
 */
auto find_local_maxima2(
    const t_xtensor_1d&                                    data,
    const std::optional<typename t_xtensor_1d::value_type> threshold   = std::nullopt,
    const bool                                             accept_nans = true,
    const int                                              mp_cores    = 1)
{
    using value_type = typename t_xtensor_1d::value_type;

    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    const int64_t max_x = data.shape()[0] - 1;

    std::vector<int64_t> X;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for schedule(guided)
        for (int64_t x = 1; x < max_x; ++x)
        {
            const value_type val = data.unchecked(x);

            if (!(val > threshold_val))
                continue;

            auto neighborhood = xt::view(data, xt::range(x - 1, x + 2));

            if (val == get_max_val2(neighborhood, accept_nans))
            {
#pragma omp critical
                {
                    X.push_back(x);
                }
            }
        }
    }

    return X;
}

}
}
}
}