// SPDX-FileCopyrightText: 2024 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/wcicorrection.doc.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <fmt/core.h>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xexpression.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "rangecorrection.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_wci_correction(t_xtensor_2d        wci,
                                         const t_xtensor_1d& per_beam_offset,
                                         const t_xtensor_1d& per_sample_offset,
                                         int                 mp_cores = 1)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    // assert that beam_offset has the same shape as wci.shape[0]
    if (wci.shape(0) != per_beam_offset.size())
        throw std::invalid_argument(fmt::format("wci.shape(0) [{}] != per_beam_offset.size() [{}]",
                                                wci.shape(0),
                                                per_beam_offset.size()));
    if (wci.shape(1) != per_sample_offset.size())

        throw std::invalid_argument(
            fmt::format("wci.shape(1) [{}] != per_sample_offset.size() [{}]",
                        wci.shape(1),
                        per_sample_offset.size()));

    // TODO: speed up using graphics card?
    // Apply the range correction to each sample
    wci += xt::view(per_sample_offset, xt::newaxis(), xt::all());

// pulse factor (here the loop is faster than broadcasting)
#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
        xt::row(wci, bi) += per_beam_offset.unchecked(bi);
    return wci;
}

}
}
}
}
