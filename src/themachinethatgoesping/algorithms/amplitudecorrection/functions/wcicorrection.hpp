// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/wcicorrection.doc.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <fmt/core.h>
#include <optional>
#include <xsimd/xsimd.hpp>

#include <xtensor/containers/xadapt.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "_shape_assertations.hpp"
#include "rangecorrection.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// --- apply corrections ---

/**
 * @brief Applies beam and sample corrections to the given 2D tensor.
 *
 * Apply beam and sample corrections to the input 2D tensor. per_beam correction is applied to each
 * sample in a beam, per_sample correction to each sample nr for each beam
 *
 * @tparam t_xtensor_2d Type of the 2D tensor.
 * @tparam t_xtensor_1d Type of the 1D tensor.
 * @param wci The input 2D tensor to which corrections will be applied.
 * @param per_beam_offset A 1D tensor containing the per-beam offsets.
 * @param per_sample_offset A 1D tensor containing the per-sample offsets.
 * @param mp_cores The number of cores to use for parallel processing (default is 1).
 * @return A 2D tensor with the applied beam and sample corrections.
 */
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
inline void inplace_beam_sample_correction([[maybe_unused]] t_xtensor_2d& wci,
                                           const t_xtensor_1d&            per_beam_offset,
                                           const t_xtensor_1d&            per_sample_offset,
                                           std::optional<size_t> min_beam_index = std::nullopt,
                                           std::optional<size_t> max_beam_index = std::nullopt,
                                           int                   mp_cores       = 1)
{
    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

        // Apply the range correction to each sample
#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = min_beam_index.value_or(0); bi <= max_beam_nr; ++bi)
        xt::row(wci, bi) += (per_beam_offset.unchecked(bi) + per_sample_offset);

    // wci = xt::eval(wci);
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_beam_correction(const t_xtensor_2d& wci,
                                          const t_xtensor_1d& per_beam_offset,
                                          int                 mp_cores = 1)
{
    assert_wci_axis_shape<0>(wci, per_beam_offset, "per_beam_offset");

    t_xtensor_2d result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < per_beam_offset.size(); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + per_beam_offset.unchecked(bi);

    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void inplace_beam_correction([[maybe_unused]] t_xtensor_2d& wci,
                                    const t_xtensor_1d&            per_beam_offset,
                                    std::optional<size_t>          min_beam_index = std::nullopt,
                                    std::optional<size_t>          max_beam_index = std::nullopt,
                                    int                            mp_cores       = 1)
{
    assert_wci_axis_shape<0>(wci, per_beam_offset, "per_beam_offset");

    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = min_beam_index.value_or(0); bi <= max_beam_nr; ++bi)
        xt::row(wci, bi) += per_beam_offset.unchecked(bi);

    // wci = xt::eval(wci);
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline t_xtensor_2d apply_sample_correction(const t_xtensor_2d& wci,
                                            const t_xtensor_1d& per_sample_offset,
                                            int                 mp_cores = 1)
{
    assert_wci_axis_shape<1>(wci, per_sample_offset, "per_sample_offset");

    if (mp_cores == 1)
        return wci + xt::view(per_sample_offset, xt::newaxis(), xt::all());

    auto result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < wci.shape(0); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + per_sample_offset;
    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void inplace_sample_correction([[maybe_unused]] t_xtensor_2d& wci,
                                      const t_xtensor_1d&            per_sample_offset,
                                      std::optional<size_t>          min_beam_index = std::nullopt,
                                      std::optional<size_t>          max_beam_index = std::nullopt,
                                      [[maybe_unused]] int           mp_cores       = 1)
{
    assert_wci_axis_shape<1>(wci, per_sample_offset, "per_sample_offset");

    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

    if (mp_cores == 1)
    {
        if (min_beam_index.has_value() || max_beam_index.has_value())
        {
            xt::view(wci, xt::range(min_beam_index.value_or(0), max_beam_nr + 1), xt::all()) +=
                xt::view(per_sample_offset, xt::newaxis(), xt::all());
        }
        else
            wci += xt::view(per_sample_offset, xt::newaxis(), xt::all());

        return;
    }

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = min_beam_index.value_or(0); bi <= max_beam_nr; ++bi)
        xt::row(wci, bi) += per_sample_offset;

    // wci = xt::eval(wci);
}

template<tools::helper::c_xtensor t_xtensor_2d>
inline t_xtensor_2d apply_system_offset(
    const t_xtensor_2d&                                          wci,
    typename tools::helper::xtensor_datatype<t_xtensor_2d>::type system_offset,
    int                                                          mp_cores = 1)
{
    if (mp_cores == 1)
        return wci + system_offset;

    auto result = xt::empty_like(wci);

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = 0; bi < wci.shape(0); ++bi)
        xt::row(result, bi) = xt::row(wci, bi) + system_offset;
    return result;
}

template<tools::helper::c_xtensor t_xtensor_2d>
inline void inplace_system_offset(
    [[maybe_unused]] t_xtensor_2d&                               wci,
    typename tools::helper::xtensor_datatype<t_xtensor_2d>::type system_offset,
    std::optional<size_t>                                        min_beam_index = std::nullopt,
    std::optional<size_t>                                        max_beam_index = std::nullopt,
    int                                                          mp_cores       = 1)
{
    size_t max_beam_nr = max_beam_index.value_or(wci.shape(0) - 1);
    if (max_beam_nr >= wci.shape(0))
        max_beam_nr = wci.shape(0) - 1;

    if (mp_cores == 1)
    {
        if (min_beam_index.has_value() || max_beam_index.has_value())
        {
            xt::view(wci, xt::range(min_beam_index.value_or(0), max_beam_nr + 1), xt::all()) +=
                system_offset;
            return;
        }
        else
        {
            wci += system_offset;
            return;
        }
    }

#pragma omp parallel for num_threads(mp_cores)
    for (unsigned int bi = min_beam_index.value_or(0); bi <= max_beam_nr; ++bi)
        xt::row(wci, bi) += system_offset;

    // wci = xt::eval(wci);
    return;
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
inline t_xtensor_2d apply_beam_sample_correction_xtensor3(const t_xtensor_2d&  wci,
                                                          const t_xtensor_1d&  per_beam_offset,
                                                          const t_xtensor_1d&  per_sample_offset,
                                                          [[maybe_unused]] int mp_cores = 1)
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
    using t_float  = tools::helper::xtensor_datatype<t_xtensor_2d>::type;
    using t_float1 = tools::helper::xtensor_datatype<t_xtensor_1d>::type;

    static_assert(std::is_same<t_float, t_float1>::value,
                  "float type for all tensors must be the same type");

    assert_wci_beam_sample_shape(wci, per_beam_offset, per_sample_offset);

    // Get the size of the vectors
    int64_t nbeams   = per_beam_offset.size();
    int64_t nsamples = per_sample_offset.size();

    // Determine the SIMD batch size
    int64_t simd_size          = xsimd::batch<t_float>::size;
    int64_t max_simd_sample_nr = nsamples - simd_size;

    t_xtensor_2d result = xt::empty_like(wci);

    // Parallelize the outer loop using OpenMP
#pragma omp parallel for num_threads(mp_cores)
    for (int64_t bi = 0; bi < nbeams; ++bi)
    {
        // Load the beam offset for the current row
        t_float beam_offset = per_beam_offset.unchecked(bi);

        // Process the inner loop in chunks of SIMD batch size
        int64_t si = 0;
        for (; si < max_simd_sample_nr; si += simd_size)
        {
            // Load the sample offsets into a SIMD batch
            xsimd::batch<t_float> sample_offset =
                xsimd::load_aligned(&(per_sample_offset.unchecked(si)));

            // Load the current values from the wci matrix
            xsimd::batch<t_float> wci_values = xsimd::load_aligned(&wci.unchecked(bi, si));

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
