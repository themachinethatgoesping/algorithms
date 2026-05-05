// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// backward.cpp — explicit template instantiations.
//
// Add a new pair of `template ...` instantiations below whenever you add a
// matching `extern template` declaration to backward.hpp. The linker will
// otherwise fail with "undefined reference" for any TU that includes
// backward.hpp but tries to call a non-instantiated specialization.
//
// Note: This file was generated with assistance from Claude Opus (Anthropic).
// -----------------------------------------------------------------------------

// Including backward.hpp brings in the full template definitions PLUS the
// `extern template` declarations. The explicit `template ...` lines below
// override the externs and emit the symbols into this TU.
#include "backward.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace functions {

template xt::xtensor<float, 2> backward_nearest<xt::xtensor<float, 2>,
                                                xt::xtensor<float, 2>,
                                                xt::xtensor<float, 1>,
                                                xt::xtensor<float, 1>>(
    const datastructures::BeamSampleGeometry&,
    const xt::xtensor<float, 2>&,
    const xt::xtensor<float, 1>&,
    const xt::xtensor<float, 1>&,
    unsigned int,
    int);

template xt::xtensor<float, 2> backward_bilinear<xt::xtensor<float, 2>,
                                                 xt::xtensor<float, 2>,
                                                 xt::xtensor<float, 1>,
                                                 xt::xtensor<float, 1>>(
    const datastructures::BeamSampleGeometry&,
    const xt::xtensor<float, 2>&,
    const xt::xtensor<float, 1>&,
    const xt::xtensor<float, 1>&,
    unsigned int,
    int);

} // namespace functions
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
