// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// backward.hpp — high-performance backward-mapping kernels for
// BeamSampleGeometry.
//
// Contains the full template definitions PLUS `extern template` declarations
// for the standard `xt::xtensor<float, 2>/<float, 1>` instantiation. Most
// TUs that include this header will therefore NOT instantiate the kernels
// locally — the symbols are resolved at link time from backward.cpp.
//
// TUs that need to instantiate a *new* type combination (e.g. nanobind
// pytensor in the Python binding TU) just include this header and call
// the kernels with the new types — the compiler will instantiate locally
// for any combination not listed in the `extern template` block.
//
// To pre-instantiate an additional type combination centrally (faster
// builds for repeated use): add a matching `extern template` line below
// AND a matching `template` line in backward.cpp.
//
// Note: This file was generated with assistance from Claude Opus (Anthropic).
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/backward.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "../datastructures/beamsamplegeometry.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace functions {

/**
 * @brief Backward-map WCI data into a (y, z) image via nearest-neighbor.
 *
 * For each output pixel, finds the nearest beam via depth-invariant tangent
 * matching and computes the sample number from the pixel's Euclidean range
 * to the sensor (matching BTConstantSVP behaviour).
 *
 * When supersampling > 1, each pixel probes S×S sub-pixel locations and
 * averages the results for anti-aliasing.
 *
 * @param geom            beam/sample geometry (must have y and z affines)
 * @param data            WCI data [n_beams x max_samples]
 * @param y_coordinates   target crosstrack coordinates [n_y], must be sorted
 * @param z_coordinates   target depth coordinates [n_z], must be sorted
 * @param supersampling   sub-pixel factor per axis (default 1)
 * @param mp_cores        OpenMP threads (default 1)
 * @return image [n_y x n_z], NaN where no valid data
 */
template<tools::helper::c_xtensor_2d t_xtensor_out,
         tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_1d_y,
         tools::helper::c_xtensor_1d t_xtensor_1d_z>
t_xtensor_out backward_nearest(const datastructures::BeamSampleGeometry& geom,
                               const t_xtensor_2d&                       data,
                               const t_xtensor_1d_y&                     y_coordinates,
                               const t_xtensor_1d_z&                     z_coordinates,
                               unsigned int                              supersampling = 1,
                               int                                       mp_cores      = 1)
{
    if (!geom.has_y_affine() || !geom.has_z_affine())
        throw std::runtime_error("backward_nearest requires y and z affines");

    const size_t n_beams = geom.get_n_beams();
    if (static_cast<size_t>(data.shape()[0]) != n_beams)
        throw std::invalid_argument(fmt::format(
            "backward_nearest: data has {} beams, expected {}", data.shape()[0], n_beams));

    const size_t       max_si = data.shape()[1];
    const size_t       n_y    = y_coordinates.size();
    const size_t       n_z    = z_coordinates.size();
    const unsigned int S      = std::max(1u, supersampling);

    auto output = t_xtensor_out::from_shape({ n_y, n_z });
    std::fill(
        output.data(), output.data() + output.size(), std::numeric_limits<float>::quiet_NaN());

    if (n_beams == 0 || n_y == 0 || n_z == 0)
        return output;

    const auto& y_affine = geom.get_y_affine();
    const auto& z_affine = geom.get_z_affine();
    const auto& y_off    = y_affine.offsets;
    const auto& y_slp    = y_affine.slopes;
    const auto& z_off    = z_affine.offsets;
    const auto& z_slp    = z_affine.slopes;

    const auto& first_sample_numbers = geom.get_first_sample_numbers();
    const auto& number_of_samples    = geom.get_number_of_samples();

    // Sensor origin (assumed same for all beams)
    const float y_sensor = y_off.unchecked(0);
    const float z_sensor = z_off.unchecked(0);

    // Per-beam: tangent (for beam selection) and 1/range_per_sample (for sample#)
    std::vector<float> beam_tan(n_beams);
    std::vector<float> inv_rps(n_beams); // 1 / sqrt(y_slp² + z_slp²)

    for (size_t b = 0; b < n_beams; ++b)
    {
        float ys   = y_slp.unchecked(b);
        float zs   = z_slp.unchecked(b);
        float rps  = std::sqrt(ys * ys + zs * zs);
        inv_rps[b] = (rps > 1e-30f) ? 1.0f / rps : 0.0f;

        float zs_safe = zs;
        if (std::abs(zs_safe) < 1e-30f)
            zs_safe = std::copysign(1e-30f, zs_safe >= 0.0f ? 1.0f : -1.0f);
        beam_tan[b] = ys / zs_safe;
    }

    // Sort beams by tangent for monotonic walk
    std::vector<size_t> beam_order(n_beams);
    std::iota(beam_order.begin(), beam_order.end(), size_t(0));
    std::stable_sort(beam_order.begin(), beam_order.end(), [&](size_t a, size_t b) {
        return beam_tan[a] < beam_tan[b];
    });

    std::vector<float> sorted_tan(n_beams);
    for (size_t i = 0; i < n_beams; ++i)
        sorted_tan[i] = beam_tan[beam_order[i]];

    // Bounds: half tangent-spacing beyond edges
    float tan_bound_lo = sorted_tan[0];
    float tan_bound_hi = sorted_tan[n_beams - 1];
    if (n_beams >= 2)
    {
        tan_bound_lo -= 0.5f * (sorted_tan[1] - sorted_tan[0]);
        tan_bound_hi += 0.5f * (sorted_tan[n_beams - 1] - sorted_tan[n_beams - 2]);
    }

    // Sub-pixel spacing (only used when S > 1)
    const float y_spacing =
        (n_y > 1) ? static_cast<float>(y_coordinates.unchecked(1) - y_coordinates.unchecked(0))
                  : 1.0f;
    const float z_spacing =
        (n_z > 1) ? static_cast<float>(z_coordinates.unchecked(1) - z_coordinates.unchecked(0))
                  : 1.0f;

    const int threads = std::max(1, mp_cores);

#pragma omp parallel if (threads > 1) num_threads(threads)
    {
        std::vector<float>        accum(S > 1 ? n_y : 0, 0.0f);
        std::vector<unsigned int> valid(S > 1 ? n_y : 0, 0u);

#pragma omp for schedule(static)
        for (size_t iz = 0; iz < n_z; ++iz)
        {
            if (S > 1)
            {
                std::fill(accum.begin(), accum.end(), 0.0f);
                std::fill(valid.begin(), valid.end(), 0u);
            }

            for (unsigned int sz = 0; sz < S; ++sz)
            {
                float z_f = static_cast<float>(z_coordinates.unchecked(iz)) +
                            z_spacing * ((sz + 0.5f) / S - 0.5f);
                float dz     = z_f - z_sensor;
                float dz2    = dz * dz;
                float inv_dz = (std::abs(dz) > 1e-30f) ? 1.0f / dz : std::copysign(1e30f, dz);

                size_t tp = 0;
                for (size_t iy = 0; iy < n_y; ++iy)
                {
                    for (unsigned int sy = 0; sy < S; ++sy)
                    {
                        float y_f = static_cast<float>(y_coordinates.unchecked(iy)) +
                                    y_spacing * ((sy + 0.5f) / S - 0.5f);
                        float dy        = y_f - y_sensor;
                        float pixel_tan = dy * inv_dz;

                        if (pixel_tan < tan_bound_lo || pixel_tan > tan_bound_hi)
                            continue;

                        // Advance to nearest beam in tangent space
                        while (tp + 1 < n_beams && std::abs(sorted_tan[tp + 1] - pixel_tan) <
                                                       std::abs(sorted_tan[tp] - pixel_tan))
                            ++tp;

                        size_t bp = beam_order[tp];

                        // Range-based sample number
                        float range = std::sqrt(dy * dy + dz2);
                        float sn    = range * inv_rps[bp];
                        float si_f  = sn - first_sample_numbers.unchecked(bp);
                        int   si    = static_cast<int>(si_f);

                        if (si_f < -0.5f ||
                            si_f > static_cast<float>(number_of_samples.unchecked(bp)) - 0.5f)
                            continue;

                        if (si >= 0 &&
                            static_cast<unsigned int>(si) < number_of_samples.unchecked(bp) &&
                            static_cast<size_t>(si) < max_si)
                        {
                            float val =
                                static_cast<float>(data(bp, static_cast<size_t>(si)));
                            if (S == 1)
                                output(iy, iz) = val;
                            else
                            {
                                accum[iy] += val;
                                valid[iy]++;
                            }
                        }
                    }
                }
            }

            if (S > 1)
            {
                for (size_t iy = 0; iy < n_y; ++iy)
                    if (valid[iy] > 0)
                        output(iy, iz) = accum[iy] / static_cast<float>(valid[iy]);
            }
        }
    } // omp parallel

    return output;
}

/**
 * @brief Backward-map WCI data into a (y, z) image via bilinear interpolation.
 *
 * For each output pixel, brackets the nearest two beams and the fractional
 * sample number, then bilinearly interpolates. Sample numbers come from the
 * Euclidean range to the sensor.
 *
 * @copydetails backward_nearest
 */
template<tools::helper::c_xtensor_2d t_xtensor_out,
         tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_1d_y,
         tools::helper::c_xtensor_1d t_xtensor_1d_z>
t_xtensor_out backward_bilinear(const datastructures::BeamSampleGeometry& geom,
                                const t_xtensor_2d&                       data,
                                const t_xtensor_1d_y&                     y_coordinates,
                                const t_xtensor_1d_z&                     z_coordinates,
                                unsigned int                              supersampling = 1,
                                int                                       mp_cores      = 1)
{
    if (!geom.has_y_affine() || !geom.has_z_affine())
        throw std::runtime_error("backward_bilinear requires y and z affines");

    const size_t n_beams = geom.get_n_beams();
    if (static_cast<size_t>(data.shape()[0]) != n_beams)
        throw std::invalid_argument(fmt::format(
            "backward_bilinear: data has {} beams, expected {}", data.shape()[0], n_beams));

    const size_t       max_si = data.shape()[1];
    const size_t       n_y    = y_coordinates.size();
    const size_t       n_z    = z_coordinates.size();
    const unsigned int S      = std::max(1u, supersampling);

    auto output = t_xtensor_out::from_shape({ n_y, n_z });
    std::fill(
        output.data(), output.data() + output.size(), std::numeric_limits<float>::quiet_NaN());

    if (n_beams == 0 || n_y == 0 || n_z == 0)
        return output;

    const auto& y_affine = geom.get_y_affine();
    const auto& z_affine = geom.get_z_affine();
    const auto& y_off    = y_affine.offsets;
    const auto& y_slp    = y_affine.slopes;
    const auto& z_off    = z_affine.offsets;
    const auto& z_slp    = z_affine.slopes;

    const auto& first_sample_numbers = geom.get_first_sample_numbers();
    const auto& number_of_samples    = geom.get_number_of_samples();

    const float y_sensor = y_off.unchecked(0);
    const float z_sensor = z_off.unchecked(0);

    std::vector<float> beam_tan(n_beams);
    std::vector<float> inv_rps(n_beams);
    std::vector<float> yz_slope(n_beams);
    std::vector<float> yz_intercept(n_beams);

    for (size_t b = 0; b < n_beams; ++b)
    {
        float ys   = y_slp.unchecked(b);
        float zs   = z_slp.unchecked(b);
        float rps  = std::sqrt(ys * ys + zs * zs);
        inv_rps[b] = (rps > 1e-30f) ? 1.0f / rps : 0.0f;

        float zs_safe = zs;
        if (std::abs(zs_safe) < 1e-30f)
            zs_safe = std::copysign(1e-30f, zs_safe >= 0.0f ? 1.0f : -1.0f);
        float inv_zs    = 1.0f / zs_safe;
        beam_tan[b]     = ys * inv_zs;
        yz_slope[b]     = beam_tan[b];
        yz_intercept[b] = y_off.unchecked(b) - yz_slope[b] * z_off.unchecked(b);
    }

    // Sort beams by tangent
    std::vector<size_t> beam_order(n_beams);
    std::iota(beam_order.begin(), beam_order.end(), size_t(0));
    std::stable_sort(beam_order.begin(), beam_order.end(), [&](size_t a, size_t b) {
        return beam_tan[a] < beam_tan[b];
    });

    std::vector<float> sorted_tan(n_beams);
    for (size_t i = 0; i < n_beams; ++i)
        sorted_tan[i] = beam_tan[beam_order[i]];

    float tan_bound_lo = sorted_tan[0];
    float tan_bound_hi = sorted_tan[n_beams - 1];
    if (n_beams >= 2)
    {
        tan_bound_lo -= 0.5f * (sorted_tan[1] - sorted_tan[0]);
        tan_bound_hi += 0.5f * (sorted_tan[n_beams - 1] - sorted_tan[n_beams - 2]);
    }

    const float y_spacing =
        (n_y > 1) ? static_cast<float>(y_coordinates.unchecked(1) - y_coordinates.unchecked(0))
                  : 1.0f;
    const float z_spacing =
        (n_z > 1) ? static_cast<float>(z_coordinates.unchecked(1) - z_coordinates.unchecked(0))
                  : 1.0f;

    const int threads = std::max(1, mp_cores);

#pragma omp parallel if (threads > 1) num_threads(threads)
    {
        std::vector<float>        accum(S > 1 ? n_y : 0, 0.0f);
        std::vector<unsigned int> valid(S > 1 ? n_y : 0, 0u);

        auto fetch = [&](size_t b, int si) -> float {
            if (si >= 0 && static_cast<unsigned int>(si) < number_of_samples.unchecked(b) &&
                static_cast<size_t>(si) < max_si)
                return static_cast<float>(data(b, static_cast<size_t>(si)));
            return std::numeric_limits<float>::quiet_NaN();
        };

        auto interp_sample = [&](size_t b, float si_f) -> float {
            if (si_f < -0.5f ||
                si_f > static_cast<float>(number_of_samples.unchecked(b)) - 0.5f)
                return std::numeric_limits<float>::quiet_NaN();
            int   si_lo = static_cast<int>(std::floor(si_f));
            int   si_hi = si_lo + 1;
            float ws    = si_f - static_cast<float>(si_lo);
            float v0    = fetch(b, si_lo);
            float v1    = fetch(b, si_hi);
            if (std::isnan(v0))
                return v1;
            if (std::isnan(v1))
                return v0;
            return v0 + ws * (v1 - v0);
        };

#pragma omp for schedule(static)
        for (size_t iz = 0; iz < n_z; ++iz)
        {
            if (S > 1)
            {
                std::fill(accum.begin(), accum.end(), 0.0f);
                std::fill(valid.begin(), valid.end(), 0u);
            }

            for (unsigned int sz = 0; sz < S; ++sz)
            {
                float z_f = static_cast<float>(z_coordinates.unchecked(iz)) +
                            z_spacing * ((sz + 0.5f) / S - 0.5f);
                float dz     = z_f - z_sensor;
                float dz2    = dz * dz;
                float inv_dz = (std::abs(dz) > 1e-30f) ? 1.0f / dz : std::copysign(1e30f, dz);

                size_t tp = 0;
                for (size_t iy = 0; iy < n_y; ++iy)
                {
                    for (unsigned int sy = 0; sy < S; ++sy)
                    {
                        float y_f = static_cast<float>(y_coordinates.unchecked(iy)) +
                                    y_spacing * ((sy + 0.5f) / S - 0.5f);
                        float dy        = y_f - y_sensor;
                        float pixel_tan = dy * inv_dz;

                        if (pixel_tan < tan_bound_lo || pixel_tan > tan_bound_hi)
                            continue;

                        // Advance tp so sorted_tan[tp] <= pixel_tan
                        while (tp + 1 < n_beams && sorted_tan[tp + 1] <= pixel_tan)
                            ++tp;

                        size_t b_lo = beam_order[tp];
                        size_t b_hi = (tp + 1 < n_beams) ? beam_order[tp + 1] : b_lo;

                        // Range-based sample numbers
                        float range = std::sqrt(dy * dy + dz2);

                        float si_lo_f =
                            range * inv_rps[b_lo] - first_sample_numbers.unchecked(b_lo);
                        float v_lo = interp_sample(b_lo, si_lo_f);

                        float val;
                        if (b_lo == b_hi)
                        {
                            val = v_lo;
                        }
                        else
                        {
                            // Bracket weight from beam y-positions at this depth
                            float by_lo = yz_intercept[b_lo] + yz_slope[b_lo] * z_f;
                            float by_hi = yz_intercept[b_hi] + yz_slope[b_hi] * z_f;
                            float d_by  = by_hi - by_lo;
                            float wy    = (d_by != 0.0f)
                                              ? std::clamp((y_f - by_lo) / d_by, 0.0f, 1.0f)
                                              : 0.0f;

                            float si_hi_f =
                                range * inv_rps[b_hi] - first_sample_numbers.unchecked(b_hi);
                            float v_hi = interp_sample(b_hi, si_hi_f);

                            if (std::isnan(v_lo))
                                val = v_hi;
                            else if (std::isnan(v_hi))
                                val = v_lo;
                            else
                                val = v_lo + wy * (v_hi - v_lo);
                        }

                        if (!std::isnan(val))
                        {
                            if (S == 1)
                                output(iy, iz) = val;
                            else
                            {
                                accum[iy] += val;
                                valid[iy]++;
                            }
                        }
                    }
                }
            }

            if (S > 1)
            {
                for (size_t iy = 0; iy < n_y; ++iy)
                    if (valid[iy] > 0)
                        output(iy, iz) = accum[iy] / static_cast<float>(valid[iy]);
            }
        }
    } // omp parallel

    return output;
}

// --- extern template suppressions ---
//
// Tell each TU including this header NOT to instantiate the standard
// xtensor specializations locally — they are linked from backward.cpp.

extern template xt::xtensor<float, 2> backward_nearest<xt::xtensor<float, 2>,
                                                       xt::xtensor<float, 2>,
                                                       xt::xtensor<float, 1>,
                                                       xt::xtensor<float, 1>>(
    const datastructures::BeamSampleGeometry&,
    const xt::xtensor<float, 2>&,
    const xt::xtensor<float, 1>&,
    const xt::xtensor<float, 1>&,
    unsigned int,
    int);

extern template xt::xtensor<float, 2> backward_bilinear<xt::xtensor<float, 2>,
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
