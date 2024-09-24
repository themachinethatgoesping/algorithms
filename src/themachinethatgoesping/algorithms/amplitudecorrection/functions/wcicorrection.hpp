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
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include "rangecorrection.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

template<template<typename, size_t> typename t_xtensor, typename t_float>
inline t_xtensor<t_float, 2> apply_wci_correction(t_xtensor<t_float, 2>        wci,
                                                  const t_xtensor<t_float, 1>& per_beam_offset,
                                                  const t_xtensor<t_float, 1>& per_sample_offset,
                                                  int                          mp_cores = 1)
{
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
