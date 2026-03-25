// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/beamsamplegeometry.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <limits>
#include <optional>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/math/simd.hpp>

#include "beamaffine1d.hpp"
#include "xyz.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief Stores per-ping beam/sample geometry as linear affines.
 *
 * Each optional affine maps sample_nr → a coordinate (x, y, or z) per beam:
 *   coord[beam] = affine.offsets[beam] + affine.slopes[beam] * sample_nr
 *
 * The user can set only the affines they need (e.g. z-only for depth images).
 * Transformation functions will throw if the required affine is not set.
 */
struct BeamSampleGeometry
{
  private:
    size_t _n_beams = 0;

    xt::xtensor<float, 1>        _first_sample_numbers; ///< [n_beams] first valid sample nr
    xt::xtensor<unsigned int, 1> _number_of_samples;     ///< [n_beams] number of samples

    std::optional<BeamAffine1D> _affine_x; ///< sample_nr → x per beam
    std::optional<BeamAffine1D> _affine_y; ///< sample_nr → y per beam
    std::optional<BeamAffine1D> _affine_z; ///< sample_nr → z per beam

  public:
    /**
     * @brief Construct an empty BeamSampleGeometry
     */
    BeamSampleGeometry() = default;

    /**
     * @brief Construct a BeamSampleGeometry with beam metadata only (no affines set)
     *
     * @param first_sample_numbers first valid sample number per beam [n_beams]
     * @param number_of_samples number of samples per beam [n_beams]
     */
    BeamSampleGeometry(xt::xtensor<float, 1>        first_sample_numbers,
                       xt::xtensor<unsigned int, 1> number_of_samples)
        : _n_beams(first_sample_numbers.size())
        , _first_sample_numbers(std::move(first_sample_numbers))
        , _number_of_samples(std::move(number_of_samples))
    {
        if (_first_sample_numbers.size() != _number_of_samples.size())
            throw std::runtime_error(fmt::format(
                "BeamSampleGeometry: first_sample_numbers ({}) and number_of_samples ({}) must "
                "have same size",
                _first_sample_numbers.size(),
                _number_of_samples.size()));
    }

    bool operator==(const BeamSampleGeometry& rhs) const = default;

    // --- accessors ---
    size_t                              get_n_beams() const { return _n_beams; }
    const xt::xtensor<float, 1>&        get_first_sample_numbers() const { return _first_sample_numbers; }
    const xt::xtensor<unsigned int, 1>& get_number_of_samples() const { return _number_of_samples; }

    // --- last sample numbers (computed) ---
    xt::xtensor<float, 1> get_last_sample_numbers() const
    {
        return _first_sample_numbers + xt::cast<float>(_number_of_samples) - 1.0f;
    }

    // --- flat-index helpers ---

    /**
     * @brief Compute per-beam cumulative offsets into a flat sample array.
     *
     * flat_offsets[b] = sum(number_of_samples[0..b-1])
     *
     * Use with: flat_index = flat_offsets[beam] + (sample_nr - first_sample_numbers[beam])
     *
     * @return xt::xtensor<unsigned int, 1> [n_beams]
     */
    xt::xtensor<unsigned int, 1> get_flat_offsets() const
    {
        auto offsets = xt::xtensor<unsigned int, 1>::from_shape({_n_beams});
        unsigned int cum = 0;
        for (size_t b = 0; b < _n_beams; ++b)
        {
            offsets.unchecked(b) = cum;
            cum += _number_of_samples.unchecked(b);
        }
        return offsets;
    }

    /**
     * @brief Total number of samples across all beams.
     */
    unsigned int get_total_samples() const
    {
        unsigned int total = 0;
        for (size_t b = 0; b < _n_beams; ++b)
            total += _number_of_samples.unchecked(b);
        return total;
    }

    // --- affine accessors ---
    bool has_x_affine() const { return _affine_x.has_value(); }
    bool has_y_affine() const { return _affine_y.has_value(); }
    bool has_z_affine() const { return _affine_z.has_value(); }

    const BeamAffine1D& get_x_affine() const
    {
        if (!_affine_x)
            throw std::runtime_error("BeamSampleGeometry: x affine not set");
        return *_affine_x;
    }

    const BeamAffine1D& get_y_affine() const
    {
        if (!_affine_y)
            throw std::runtime_error("BeamSampleGeometry: y affine not set");
        return *_affine_y;
    }

    const BeamAffine1D& get_z_affine() const
    {
        if (!_affine_z)
            throw std::runtime_error("BeamSampleGeometry: z affine not set");
        return *_affine_z;
    }

    // --- affine setters ---
    void set_x_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_x_affine");
        _affine_x = std::move(affine);
    }

    void set_y_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_y_affine");
        _affine_y = std::move(affine);
    }

    void set_z_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_z_affine");
        _affine_z = std::move(affine);
    }

    void set_xyz_affines(BeamAffine1D affine_x, BeamAffine1D affine_y, BeamAffine1D affine_z)
    {
        check_affine_size(affine_x, "set_xyz_affines (x)");
        check_affine_size(affine_y, "set_xyz_affines (y)");
        check_affine_size(affine_z, "set_xyz_affines (z)");
        _affine_x = std::move(affine_x);
        _affine_y = std::move(affine_y);
        _affine_z = std::move(affine_z);
    }

    // --- factory functions ---

    /**
     * @brief Create a BeamSampleGeometry from a base XYZ location and per-beam bottom XYZ
     * locations with their corresponding sample numbers.
     *
     * Computes affines such that:
     *   coord(sample_nr) = base_coord + (bottom_coord - base_coord) / bottom_sample_nr *
     * sample_nr
     *
     * @param base_xyz base location (single point, e.g. transducer position)
     * @param bottom_xyz per-beam bottom locations [n_beams]
     * @param bottom_sample_numbers per-beam sample number at the bottom [n_beams]
     * @param first_sample_numbers per-beam first valid sample number [n_beams]
     * @param number_of_samples per-beam number of samples [n_beams]
     * @return BeamSampleGeometry with x, y, z affines set
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_snr,
             tools::helper::c_xtensor_1d t_xtensor_1d_fsn,
             tools::helper::c_xtensor_1d t_xtensor_1d_nos>
    static BeamSampleGeometry from_bottom_xyz(
        const XYZ<1>&            bottom_xyz,
        float                    base_x,
        float                    base_y,
        float                    base_z,
        const t_xtensor_1d_snr&  bottom_sample_numbers,
        t_xtensor_1d_fsn         first_sample_numbers,
        t_xtensor_1d_nos         number_of_samples)
    {
        BeamSampleGeometry geom{xt::xtensor<float, 1>(first_sample_numbers),
                                xt::xtensor<unsigned int, 1>(number_of_samples)};

        geom.set_x_affine(
            BeamAffine1D::from_base_and_endpoints(base_x, bottom_xyz.x, bottom_sample_numbers));
        geom.set_y_affine(
            BeamAffine1D::from_base_and_endpoints(base_y, bottom_xyz.y, bottom_sample_numbers));
        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(base_z, bottom_xyz.z, bottom_sample_numbers));

        return geom;
    }

    /**
     * @brief Create a BeamSampleGeometry with z affine only, from a base depth and per-beam
     * bottom depths at known sample numbers.
     *
     * @param base_z base depth (e.g. transducer depth)
     * @param bottom_depths per-beam bottom depths [n_beams]
     * @param bottom_sample_numbers per-beam sample number at the bottom [n_beams]
     * @param first_sample_numbers per-beam first valid sample number [n_beams]
     * @param number_of_samples per-beam number of samples [n_beams]
     * @return BeamSampleGeometry with z affine set
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_vals,
             tools::helper::c_xtensor_1d t_xtensor_1d_snr,
             tools::helper::c_xtensor_1d t_xtensor_1d_fsn,
             tools::helper::c_xtensor_1d t_xtensor_1d_nos>
    static BeamSampleGeometry from_bottom_z(
        float                    base_z,
        const t_xtensor_1d_vals& bottom_depths,
        const t_xtensor_1d_snr&  bottom_sample_numbers,
        t_xtensor_1d_fsn         first_sample_numbers,
        t_xtensor_1d_nos         number_of_samples)
    {
        BeamSampleGeometry geom{xt::xtensor<float, 1>(first_sample_numbers),
                                xt::xtensor<unsigned int, 1>(number_of_samples)};

        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(base_z, bottom_depths, bottom_sample_numbers));

        return geom;
    }

    /**
     * @brief Create a BeamSampleGeometry from crosstrack angles and ranges.
     *
     * Computes y and z affines from the crosstrack angle and range at a known
     * sample number per beam (no x affine set since no alongtrack info):
     *   y(sample_nr) = (-range * sin(crosstrack_angle)) / range_sample_nr * sample_nr
     *   z(sample_nr) = ( range * cos(crosstrack_angle)) / range_sample_nr * sample_nr
     *
     * @param crosstrack_angles in °, positive portside up, 0 == downwards [n_beams]
     * @param ranges in m, per beam [n_beams]
     * @param range_sample_numbers sample number at which the range was measured [n_beams]
     * @param first_sample_numbers first valid sample number per beam [n_beams]
     * @param number_of_samples number of samples per beam [n_beams]
     * @return BeamSampleGeometry with y and z affines set
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_angles,
             tools::helper::c_xtensor_1d t_xtensor_1d_ranges,
             tools::helper::c_xtensor_1d t_xtensor_1d_snr,
             tools::helper::c_xtensor_1d t_xtensor_1d_fsn,
             tools::helper::c_xtensor_1d t_xtensor_1d_nos>
    static BeamSampleGeometry from_angle_and_range(
        const t_xtensor_1d_angles& crosstrack_angles,
        const t_xtensor_1d_ranges& ranges,
        const t_xtensor_1d_snr&    range_sample_numbers,
        t_xtensor_1d_fsn           first_sample_numbers,
        t_xtensor_1d_nos           number_of_samples)
    {
        constexpr float deg2rad = static_cast<float>(M_PI) / 180.0f;
        auto ct_rad = crosstrack_angles * deg2rad;

        xt::xtensor<float, 1> y_endpoints = -ranges * xt::sin(ct_rad);
        xt::xtensor<float, 1> z_endpoints =  ranges * xt::cos(ct_rad);

        BeamSampleGeometry geom{xt::xtensor<float, 1>(first_sample_numbers),
                                xt::xtensor<unsigned int, 1>(number_of_samples)};
        geom.set_y_affine(
            BeamAffine1D::from_base_and_endpoints(0.0f, y_endpoints, range_sample_numbers));
        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(0.0f, z_endpoints, range_sample_numbers));
        return geom;
    }

    /**
     * @brief Create a BeamSampleGeometry from alongtrack angles, crosstrack angles, and ranges.
     *
     * Uses the independent / Mills Cross formulation: the TX and RX arrays measure
     * angles independently in perpendicular planes, so each angle contributes one
     * horizontal displacement:
     *   x(sample_nr) = ( range * sin(at_angle))  / range_sample_nr * sample_nr
     *   y(sample_nr) = (-range * sin(ct_angle))  / range_sample_nr * sample_nr
     *   z(sample_nr) = sqrt(range² - x² - y²)    / range_sample_nr * sample_nr
     *
     * @param alongtrack_angles in °, positive bow up, 0 == downwards [n_beams]
     * @param crosstrack_angles in °, positive portside up, 0 == downwards [n_beams]
     * @param ranges in m, per beam [n_beams]
     * @param range_sample_numbers sample number at which the range was measured [n_beams]
     * @param first_sample_numbers first valid sample number per beam [n_beams]
     * @param number_of_samples number of samples per beam [n_beams]
     * @return BeamSampleGeometry with x, y, z affines set
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_angles,
             tools::helper::c_xtensor_1d t_xtensor_1d_ranges,
             tools::helper::c_xtensor_1d t_xtensor_1d_snr,
             tools::helper::c_xtensor_1d t_xtensor_1d_fsn,
             tools::helper::c_xtensor_1d t_xtensor_1d_nos>
    static BeamSampleGeometry from_angles_and_range(
        const t_xtensor_1d_angles& alongtrack_angles,
        const t_xtensor_1d_angles& crosstrack_angles,
        const t_xtensor_1d_ranges& ranges,
        const t_xtensor_1d_snr&    range_sample_numbers,
        t_xtensor_1d_fsn           first_sample_numbers,
        t_xtensor_1d_nos           number_of_samples)
    {
        constexpr float deg2rad = static_cast<float>(M_PI) / 180.0f;
        auto at_rad = alongtrack_angles * deg2rad;
        auto ct_rad = crosstrack_angles * deg2rad;

        xt::xtensor<float, 1> x_endpoints =  ranges * xt::sin(at_rad);
        xt::xtensor<float, 1> y_endpoints = -ranges * xt::sin(ct_rad);
        xt::xtensor<float, 1> z_endpoints =  xt::sqrt(ranges * ranges
                                                       - x_endpoints * x_endpoints
                                                       - y_endpoints * y_endpoints);

        BeamSampleGeometry geom{xt::xtensor<float, 1>(first_sample_numbers),
                                xt::xtensor<unsigned int, 1>(number_of_samples)};
        geom.set_x_affine(
            BeamAffine1D::from_base_and_endpoints(0.0f, x_endpoints, range_sample_numbers));
        geom.set_y_affine(
            BeamAffine1D::from_base_and_endpoints(0.0f, y_endpoints, range_sample_numbers));
        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(0.0f, z_endpoints, range_sample_numbers));
        return geom;
    }

  private:
    // --- SIMD-optimized forward helpers ---

    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_sn>
    static xt::xtensor<float, 1> forward_coord_flat_(
        const BeamAffine1D& affine,
        const t_xtensor_1d_bi& beam_indices,
        const t_xtensor_1d_sn& sample_numbers)
    {
        static_assert(
            std::is_same_v<typename std::decay_t<t_xtensor_1d_bi>::value_type, uint32_t>,
            "beam_indices must have uint32_t element type");
        static_assert(
            std::is_same_v<typename std::decay_t<t_xtensor_1d_sn>::value_type, float>,
            "sample_numbers must have float element type");

        const size_t n = beam_indices.size();
        auto result = xt::xtensor<float, 1>::from_shape({n});
        if (n == 0) return result;

        const auto* bi = beam_indices.data();
        const auto* sn = sample_numbers.data();

        size_t i = 0;
        while (i < n)
        {
            uint32_t beam      = bi[i];
            size_t   run_start = i;
            while (i < n && bi[i] == beam)
                ++i;

            // out[j] = sample_numbers[j] * slope + offset
            tools::math::fma_dispatch(result.data() + run_start,
                                       sn + run_start,
                                       affine.slopes.unchecked(beam),
                                       affine.offsets.unchecked(beam),
                                       i - run_start);
        }
        return result;
    }

    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_fs,
             tools::helper::c_xtensor_1d t_xtensor_1d_ls>
    static xt::xtensor<float, 2> forward_coord_range_(
        const BeamAffine1D& affine,
        const t_xtensor_1d_bi& beam_indices,
        const t_xtensor_1d_fs& first_sample_numbers,
        const t_xtensor_1d_ls& last_sample_numbers,
        uint32_t sample_step)
    {
        static_assert(
            std::is_same_v<typename std::decay_t<t_xtensor_1d_bi>::value_type, uint32_t>,
            "beam_indices must have uint32_t element type");
        static_assert(
            std::is_same_v<typename std::decay_t<t_xtensor_1d_fs>::value_type, uint32_t>,
            "first_sample_numbers must have uint32_t element type");
        static_assert(
            std::is_same_v<typename std::decay_t<t_xtensor_1d_ls>::value_type, uint32_t>,
            "last_sample_numbers must have uint32_t element type");

        const size_t n_sel = beam_indices.size();
        const auto*  bi    = beam_indices.data();
        const auto*  fs    = first_sample_numbers.data();
        const auto*  ls    = last_sample_numbers.data();

        // Compute max sample count across selected beams
        size_t max_samples = 0;
        for (size_t i = 0; i < n_sel; ++i)
        {
            if (ls[i] >= fs[i])
            {
                size_t count = size_t(ls[i] - fs[i]) / sample_step + 1;
                max_samples  = std::max(max_samples, count);
            }
        }

        if (max_samples == 0 || n_sel == 0)
            return xt::xtensor<float, 2>::from_shape({n_sel, size_t(0)});

        // Pre-allocate index ramp [0, 1, 2, ..., max_samples-1]
        auto ramp = xt::xtensor<float, 1>::from_shape({max_samples});
        for (size_t j = 0; j < max_samples; ++j)
            ramp.unchecked(j) = static_cast<float>(j);

        // Allocate NaN-filled result
        auto result = xt::xtensor<float, 2>::from_shape({n_sel, max_samples});
        std::fill(result.data(), result.data() + result.size(),
                  std::numeric_limits<float>::quiet_NaN());

        for (size_t i = 0; i < n_sel; ++i)
        {
            uint32_t beam  = bi[i];
            uint32_t first = fs[i];
            uint32_t last  = ls[i];
            if (last < first) continue;
            size_t count = size_t(last - first) / sample_step + 1;

            // value(j) = offset + slope * (first + j * step)
            //          = (offset + slope * first) + (slope * step) * j
            float slope_val   = affine.slopes.unchecked(beam);
            float base_prime  = affine.offsets.unchecked(beam) +
                                slope_val * static_cast<float>(first);
            float slope_prime = slope_val * static_cast<float>(sample_step);

            tools::math::fma_dispatch(result.data() + i * max_samples,
                                       ramp.data(),
                                       slope_prime,
                                       base_prime,
                                       count);
        }

        return result;
    }

    /**
     * @brief Compute the full flat coordinate array for all beams and samples.
     *
     * For each beam b, fills positions [flat_offsets[b] .. +number_of_samples[b]-1]
     * with: affine.offset[b] + affine.slope[b] * (first_sample_numbers[b] + j)
     * One SIMD FMA call per beam.
     */
    xt::xtensor<float, 1> forward_coord_all_(const BeamAffine1D& affine) const
    {
        unsigned int total = get_total_samples();
        auto result = xt::xtensor<float, 1>::from_shape({size_t(total)});
        if (total == 0) return result;

        // Build ramp [0, 1, 2, ...] large enough for the widest beam
        size_t max_ns = *std::max_element(
            _number_of_samples.data(),
            _number_of_samples.data() + _n_beams);
        auto ramp = xt::xtensor<float, 1>::from_shape({max_ns});
        for (size_t j = 0; j < max_ns; ++j)
            ramp.unchecked(j) = static_cast<float>(j);

        size_t pos = 0;
        for (size_t b = 0; b < _n_beams; ++b)
        {
            unsigned int ns = _number_of_samples.unchecked(b);
            if (ns == 0) continue;

            // value(j) = offset[b] + slope[b] * (first_sample[b] + j)
            //          = (offset[b] + slope[b] * first_sample[b]) + slope[b] * j
            float slope = affine.slopes.unchecked(b);
            float base  = affine.offsets.unchecked(b) +
                          slope * _first_sample_numbers.unchecked(b);

            tools::math::fma_dispatch(result.data() + pos,
                                       ramp.data(),
                                       slope,
                                       base,
                                       ns);
            pos += ns;
        }
        return result;
    }

  public:
    // --- forward transformations using SIMD-optimized FMA ---

    /**
     * @brief Compute x coordinate for (beam_index, sample_number) pairs.
     *
     * When beam_indices are sorted (consecutive same-beam entries),
     * each run is dispatched as a single SIMD FMA call for maximum throughput.
     *
     * @param beam_indices beam index per entry [N], values in [0, n_beams)
     * @param sample_numbers float sample number per entry [N]
     * @return xt::xtensor<float, 1> x coordinate per entry [N]
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_sn>
    xt::xtensor<float, 1> forward_x(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_sn& sample_numbers) const
    {
        return forward_coord_flat_(get_x_affine(), beam_indices, sample_numbers);
    }

    /// @copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_sn&) const
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_sn>
    xt::xtensor<float, 1> forward_y(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_sn& sample_numbers) const
    {
        return forward_coord_flat_(get_y_affine(), beam_indices, sample_numbers);
    }

    /// @copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_sn&) const
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_sn>
    xt::xtensor<float, 1> forward_z(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_sn& sample_numbers) const
    {
        return forward_coord_flat_(get_z_affine(), beam_indices, sample_numbers);
    }

    /**
     * @brief Compute x coordinate for sample ranges per beam.
     *
     * For each selected beam, generates sample numbers
     * [first, first+step, ..., <= last] and computes
     * x = offset + slope * sample_nr via SIMD FMA.
     * Result is 2D [n_selected_beams x max_samples]; trailing
     * unused cells are NaN.
     *
     * @param beam_indices per selected beam [B], values in [0, n_beams)
     * @param first_sample_numbers first sample number per beam [B]
     * @param last_sample_numbers last sample number per beam [B]
     * @param sample_step step between consecutive samples (default 1)
     * @return xt::xtensor<float, 2> [B x max_samples]
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_fs,
             tools::helper::c_xtensor_1d t_xtensor_1d_ls>
    xt::xtensor<float, 2> forward_x(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_fs& first_sample_numbers,
                                      const t_xtensor_1d_ls& last_sample_numbers,
                                      uint32_t sample_step = 1) const
    {
        return forward_coord_range_(get_x_affine(), beam_indices, first_sample_numbers,
                                     last_sample_numbers, sample_step);
    }

    /// @copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_fs&, const t_xtensor_1d_ls&, uint32_t) const
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_fs,
             tools::helper::c_xtensor_1d t_xtensor_1d_ls>
    xt::xtensor<float, 2> forward_y(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_fs& first_sample_numbers,
                                      const t_xtensor_1d_ls& last_sample_numbers,
                                      uint32_t sample_step = 1) const
    {
        return forward_coord_range_(get_y_affine(), beam_indices, first_sample_numbers,
                                     last_sample_numbers, sample_step);
    }

    /// @copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_fs&, const t_xtensor_1d_ls&, uint32_t) const
    template<tools::helper::c_xtensor_1d t_xtensor_1d_bi,
             tools::helper::c_xtensor_1d t_xtensor_1d_fs,
             tools::helper::c_xtensor_1d t_xtensor_1d_ls>
    xt::xtensor<float, 2> forward_z(const t_xtensor_1d_bi& beam_indices,
                                      const t_xtensor_1d_fs& first_sample_numbers,
                                      const t_xtensor_1d_ls& last_sample_numbers,
                                      uint32_t sample_step = 1) const
    {
        return forward_coord_range_(get_z_affine(), beam_indices, first_sample_numbers,
                                     last_sample_numbers, sample_step);
    }

    // --- full flat coordinate arrays ---

    /**
     * @brief Compute the full flat x coordinate array for all beams and samples.
     *
     * The result has get_total_samples() elements laid out contiguously
     * per beam. Index with:
     *   flat_index = get_flat_offsets()[beam] + (sample_nr - first_sample_numbers[beam])
     */
    xt::xtensor<float, 1> forward_x_flat() const { return forward_coord_all_(get_x_affine()); }

    /// @copydoc forward_x_flat
    xt::xtensor<float, 1> forward_y_flat() const { return forward_coord_all_(get_y_affine()); }

    /// @copydoc forward_x_flat
    xt::xtensor<float, 1> forward_z_flat() const { return forward_coord_all_(get_z_affine()); }

  public:
    // ----- file I/O -----
    static BeamSampleGeometry from_stream(std::istream& is)
    {
        BeamSampleGeometry data;

        is.read(reinterpret_cast<char*>(&data._n_beams), sizeof(size_t));

        data._first_sample_numbers = xt::xtensor<float, 1>::from_shape({ data._n_beams });
        data._number_of_samples    = xt::xtensor<unsigned int, 1>::from_shape({ data._n_beams });

        is.read(reinterpret_cast<char*>(data._first_sample_numbers.data()),
                sizeof(float) * data._n_beams);
        is.read(reinterpret_cast<char*>(data._number_of_samples.data()),
                sizeof(unsigned int) * data._n_beams);

        // read optional affines
        bool has_x, has_y, has_z;
        is.read(reinterpret_cast<char*>(&has_x), sizeof(bool));
        is.read(reinterpret_cast<char*>(&has_y), sizeof(bool));
        is.read(reinterpret_cast<char*>(&has_z), sizeof(bool));

        if (has_x)
            data._affine_x = BeamAffine1D::from_stream(is);
        if (has_y)
            data._affine_y = BeamAffine1D::from_stream(is);
        if (has_z)
            data._affine_z = BeamAffine1D::from_stream(is);

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_n_beams), sizeof(size_t));

        os.write(reinterpret_cast<const char*>(_first_sample_numbers.data()),
                 sizeof(float) * _n_beams);
        os.write(reinterpret_cast<const char*>(_number_of_samples.data()),
                 sizeof(unsigned int) * _n_beams);

        // write optional affines
        bool has_x = _affine_x.has_value();
        bool has_y = _affine_y.has_value();
        bool has_z = _affine_z.has_value();
        os.write(reinterpret_cast<const char*>(&has_x), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&has_y), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&has_z), sizeof(bool));

        if (has_x)
            _affine_x->to_stream(os);
        if (has_y)
            _affine_y->to_stream(os);
        if (has_z)
            _affine_z->to_stream(os);
    }

  private:
    void check_affine_size(const BeamAffine1D& affine, const std::string& name) const
    {
        if (affine.size() != _n_beams)
            throw std::runtime_error(
                fmt::format("BeamSampleGeometry::{}: affine size ({}) does not match n_beams ({})",
                            name,
                            affine.size(),
                            _n_beams));
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamSampleGeometry", float_precision, superscript_exponents);

        printer.register_value("n_beams", _n_beams);
        printer.register_container("first_sample_numbers", _first_sample_numbers);
        printer.register_container("number_of_samples", _number_of_samples);
        printer.register_value("has_x_affine", has_x_affine());
        printer.register_value("has_y_affine", has_y_affine());
        printer.register_value("has_z_affine", has_z_affine());

        if (_affine_x)
            printer.register_section("x_affine");
        if (_affine_y)
            printer.register_section("y_affine");
        if (_affine_z)
            printer.register_section("z_affine");

        return printer;
    }

  public:
    // -- class helper function macros --
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSampleGeometry)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
