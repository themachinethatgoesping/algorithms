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
#include <xsimd/xsimd.hpp>
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

// --- assertations ---

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void assert_wci_axis_shape(const t_xtensor_2d& wci,
                                  const t_xtensor_1d& per_beam_offset,
                                  size_t              axis,
                                  std::string_view    axis_name)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    // assert that beam_offset has the same shape as wci.shape(axis)
    if (wci.shape(axis) != per_beam_offset.size())
        throw std::invalid_argument(fmt::format("ERROR[{}]: wci.shape({}) [{}] != {}.size() [{}]",
                                                __func__,
                                                axis,
                                                wci.shape(axis),
                                                axis_name,
                                                per_beam_offset.size()));
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void assert_wci_beam_sample_shape(const t_xtensor_2d& wci,
                                         const t_xtensor_1d& per_beam_offset,
                                         const t_xtensor_1d& per_sample_offset)
{
    assert_wci_axis_shape(wci, per_beam_offset, 0, "per_beam_offset");
    assert_wci_axis_shape(wci, per_sample_offset, 1, "per_sample_offset");
}

// --- apply corrections ---

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_sample_correction(const t_xtensor_2d& wci,
                                                 const t_xtensor_1d& per_beam_offset,
                                                 const t_xtensor_1d& per_sample_offset,
                                                 int                 mp_cores = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    // Apply the range correction to each sample

    t_xtensor_2d result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + per_beam_offset.unchecked(bi) + per_sample_offset;

    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_correction(const t_xtensor_2d& wci,
                                          const t_xtensor_1d& per_beam_offset,
                                          int                 mp_cores = 1)
{
    assert_wci_axis_shape(wci, per_beam_offset, 0, "per_beam_offset");

    t_xtensor_2d result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + per_beam_offset.unchecked(bi);

    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_sample_correction(const t_xtensor_2d&  wci,
                                            const t_xtensor_1d&  per_sample_offset,
                                            [[maybe_unused]] int mp_cores = 1)
{
    assert_wci_axis_shape(wci, per_sample_offset, 1, "per_sample_offset");

    return wci + xt::view(per_sample_offset, xt::newaxis(), xt::all());
}

//--- these functions are for benchmarking purposes ---

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_sample_correction_loop(const t_xtensor_2d& wci,
                                                      const t_xtensor_1d& per_beam_offset,
                                                      const t_xtensor_1d& per_sample_offset,
                                                      int                 mp_cores = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    t_xtensor_2d result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
    {
        auto beam_offset = per_beam_offset.unchecked(bi);
#pragma GCC ivdep
        for (unsigned int si = 0; si < per_sample_offset.size(); ++si)
            result.unchecked(bi, si) =
                wci.unchecked(bi, si) + beam_offset + per_sample_offset.unchecked(si);
    }
    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_sample_correction_xtensor2(const t_xtensor_2d& wci,
                                                          const t_xtensor_1d& per_beam_offset,
                                                          const t_xtensor_1d& per_sample_offset,
                                                          int                 mp_cores = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    // Apply the range correction to each sample
    t_xtensor_2d result = wci + xt::view(per_sample_offset, xt::newaxis(), xt::all());
    // pulse factor (here the loop is faster than broadcasting)

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
        xt::row(result, bi) += per_beam_offset.unchecked(bi);

    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_sample_correction_xtensor3(const t_xtensor_2d& wci,
                                                          const t_xtensor_1d& per_beam_offset,
                                                          const t_xtensor_1d& per_sample_offset,
                                                          int                 mp_cores = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    // Apply the range correction to each sample
    return wci + xt::view(per_sample_offset, xt::newaxis(), xt::all()) +
           xt::view(per_beam_offset, xt::all(), xt::newaxis());
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_sample_correction_xsimd(const t_xtensor_2d& wci,
                                                       const t_xtensor_1d& per_beam_offset,
                                                       const t_xtensor_1d& per_sample_offset,
                                                       int                 mp_cores = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    // Get the size of the vectors
    int64_t nbeams   = per_beam_offset.size();
    int64_t nsamples = per_sample_offset.size();

    // Determine the SIMD batch size
    int64_t simd_size          = xsimd::batch<float>::size;
    int64_t max_simd_sample_nr = nsamples - simd_size;

    t_xtensor_2d result = xt::empty_like(wci);

    // Parallelize the outer loop using OpenMP
#pragma omp parallel for num_threads(mp_cores)
    for (int64_t bi = 0; bi < nbeams; ++bi)
    {
        // Load the beam offset for the current row
        float beam_offset = per_beam_offset.unchecked(bi);

        // Process the inner loop in chunks of SIMD batch size
        int64_t si = 0;
        for (; si < max_simd_sample_nr; si += simd_size)
        {
            // Load the sample offsets into a SIMD batch
            xsimd::batch<float> sample_offset =
                xsimd::load_aligned(&(per_sample_offset.unchecked(si)));

            // Load the current values from the wci matrix
            xsimd::batch<float> wci_values = xsimd::load_aligned(&wci.unchecked(bi, si));

            // Perform the SIMD addition
            wci_values += beam_offset + sample_offset;

            // Store the results back to the wci matrix
            xsimd::store_aligned(&result.unchecked(bi, si), wci_values);
        }

        // Process the remaining samples in the inner loop
        for (; si < nsamples; ++si)
        {
            result.unchecked(bi, si) =
                wci.unchecked(bi, si) + beam_offset + per_sample_offset.unchecked(si);
        }
    }

    return result;
}

}
}
}
}
