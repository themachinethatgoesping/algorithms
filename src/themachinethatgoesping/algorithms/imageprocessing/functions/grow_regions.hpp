// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/grow_regions.doc.hpp"

/* generated doc strings */
#include ".docstrings/find_local_maxima.doc.hpp"

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
auto grow_regions(
    const t_xtensor_3d&                                    data,
    const std::optional<typename t_xtensor_3d::value_type> threshold   = std::nullopt,
    const bool                                             accept_nans = true,
    const int                                              mp_cores    = 1)
{
    using value_type = typename t_xtensor_3d::value_type;

    static_assert(tools::helper::c_xtensor_3d<t_xtensor_3d>,
                  "Template parameter must be a 3D tensor");

    int64_t max_z = data.shape()[2] - 1;
    int64_t max_y = data.shape()[1] - 1;
    int64_t max_x = data.shape()[0] - 1;

    std::vector<int64_t>    X;
    std::vector<int64_t>    Y;
    std::vector<int64_t>    Z;
    std::vector<value_type> V;

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
                    const value_type val = data(x, y, z);

                    if (!(val > threshold_val))
                        continue;

                    auto neighborhood = xt::view(data,
                                                 xt::range(x - 1, x + 2),
                                                 xt::range(y - 1, y + 2),
                                                 xt::range(z - 1, z + 2));

                    value_type max_val;
                    if (accept_nans)
                    {
                        max_val = xt::nanmax(neighborhood)();
                    }
                    else
                    {
                        max_val = xt::amax(neighborhood)();
                    }

                    if (val == max_val)
                    {
#pragma omp critical
                        {
                            X.push_back(x);
                            Y.push_back(y);
                            Z.push_back(z);
                            V.push_back(val);
                        }
                    }
                }
            }
        }
    }

    return std::tuple(X, Y, Z, V);
}


}
}
}
}