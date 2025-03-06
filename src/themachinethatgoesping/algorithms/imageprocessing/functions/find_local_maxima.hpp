// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

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
auto find_local_maxima(
    const t_xtensor_3d&                                    data,
    const std::optional<typename t_xtensor_3d::value_type> threshold = std::nullopt,
    int                                                    mp_cores  = 1)
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

                    if (val == xt::nanmax(neighborhood)())
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

// Similar implementation for 2D version...
// ...existing code...

// 2D version
template<tools::helper::c_xtensor_2d t_xtensor_2d>
auto find_local_maxima(
    const t_xtensor_2d&                                    data,
    const std::optional<typename t_xtensor_2d::value_type> threshold = std::nullopt,
    int                                                    mp_cores  = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");

    int64_t max_y = data.shape()[1] - 1;
    int64_t max_x = data.shape()[0] - 1;

    std::vector<int64_t>    X;
    std::vector<int64_t>    Y;
    std::vector<value_type> V;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for collapse(2) schedule(guided)
        for (int64_t x = 1; x < max_x; ++x)
        {
            for (int64_t y = 1; y < max_y; ++y)
            {
                const value_type val = data(x, y);

                if (!(val > threshold_val))
                    continue;

                auto neighborhood =
                    xt::view(data, xt::range(x - 1, x + 2), xt::range(y - 1, y + 2));

                if (val == xt::nanmax(neighborhood)())
                {
#pragma omp critical
                    {
                        X.push_back(x);
                        Y.push_back(y);
                        V.push_back(val);
                    }
                }
            }
        }
    }

    return std::tuple(X, Y, V);
}

// 1D version
template<tools::helper::c_xtensor_1d t_xtensor_1d>
auto find_local_maxima(
    const t_xtensor_1d&                                    data,
    const std::optional<typename t_xtensor_1d::value_type> threshold = std::nullopt,
    int                                                    mp_cores  = 1)
{
    using value_type = typename t_xtensor_1d::value_type;

    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    int64_t max_x = data.shape()[0] - 1;

    std::vector<int64_t>    X;
    std::vector<value_type> V;

    // preprocess threshold
    const auto threshold_val = threshold.value_or(std::numeric_limits<value_type>::lowest());

#pragma omp parallel num_threads(mp_cores)
    {
#pragma omp for schedule(guided)
        for (int64_t x = 1; x < max_x; ++x)
        {
            const value_type val = data(x);

            if (!(val > threshold_val))
                continue;

            auto neighborhood = xt::view(data, xt::range(x - 1, x + 2));

            if (val == xt::nanmax(neighborhood)())
            {
#pragma omp critical
                {
                    X.push_back(x);
                    V.push_back(val);
                }
            }
        }
    }

    return std::tuple(X, V);
}

}
}
}
}