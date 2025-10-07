// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/backwardmapping.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <xtensor/containers/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace imageprocessing {
namespace functions {

struct UniformAxis
{
    double origin;
    double spacing;
    size_t size;

    constexpr UniformAxis() = default;

    constexpr UniformAxis(const double origin_, const double spacing_, const size_t size_)
        : origin(origin_)
        , spacing(spacing_)
        , size(size_)
    {}

    template<tools::helper::c_xtensor_1d t_xtensor_1d>
    explicit UniformAxis(const t_xtensor_1d& coordinates, const double tolerance = 1e-9)
        : origin(0.0)
        , spacing(0.0)
        , size(coordinates.size())
    {
        if (tolerance < 0.0 || !std::isfinite(tolerance))
            throw std::invalid_argument("UniformAxis tolerance must be a finite, non-negative value");

        if (size == 0)
            return;

        origin = static_cast<double>(coordinates(0));

        if (!std::isfinite(origin))
            throw std::invalid_argument("UniformAxis coordinates must be finite");

        if (size == 1)
            return;

        const double first = static_cast<double>(coordinates(1));

        if (!std::isfinite(first))
            throw std::invalid_argument("UniformAxis coordinates must be finite");

        spacing = first - origin;

        if (!std::isfinite(spacing))
            throw std::invalid_argument("UniformAxis computed spacing must be finite");

        if (spacing <= 0.0)
            throw std::invalid_argument("UniformAxis requires strictly positive spacing");

        double previous_value = first;
        const double base_scale = std::max(1.0, std::abs(spacing));

        for (size_t idx = 2; idx < size; ++idx)
        {
            const double current = static_cast<double>(coordinates(idx));

            if (!std::isfinite(current))
                throw std::invalid_argument("UniformAxis coordinates must be finite");

            const double diff = current - previous_value;

            if (!std::isfinite(diff))
                throw std::invalid_argument("UniformAxis computed spacing must be finite");

            const double scale = std::max({ 1.0, base_scale, std::abs(diff) });
            if (std::abs(diff - spacing) > tolerance * scale)
                throw std::invalid_argument("UniformAxis coordinates are not uniformly spaced within tolerance");

            previous_value = current;
        }
    }
};

namespace detail {

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline size_t lower_bound_index(const t_xtensor_1d& coords, const T value)
{
    size_t left  = 0;
    size_t right = coords.size();

    while (left < right)
    {
        const size_t mid = left + (right - left) / 2;
        if (coords(mid) < value)
            left = mid + 1;
        else
            right = mid;
    }

    return left;
}

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline size_t nearest_index(const t_xtensor_1d& coords, const T value)
{
    if (coords.size() == 0)
        throw std::invalid_argument("Coordinate array must not be empty");

    if (coords.size() == 1)
        return 0;

    const size_t upper = lower_bound_index(coords, value);

    if (upper == 0)
        return 0;

    if (upper >= coords.size())
        return coords.size() - 1;

    const size_t lower = upper - 1;

    const auto lower_diff = std::abs(static_cast<double>(value - coords(lower)));
    const auto upper_diff = std::abs(static_cast<double>(coords(upper) - value));

    return (upper_diff < lower_diff) ? upper : lower;
}

struct Bracket
{
    size_t lower;
    size_t upper;
    double weight;
};

template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T>
inline Bracket bracket_indices(const t_xtensor_1d& coords, const T value)
{
    if (coords.size() == 0)
        throw std::invalid_argument("Coordinate array must not be empty");

    if (coords.size() == 1)
        return Bracket{ 0, 0, 0.0 };

    const size_t upper = lower_bound_index(coords, value);

    if (upper == 0)
        return Bracket{ 0, 0, 0.0 };

    if (upper >= coords.size())
        return Bracket{ coords.size() - 1, coords.size() - 1, 0.0 };

    const size_t lower = upper - 1;
    const auto  x0     = static_cast<double>(coords(lower));
    const auto  x1     = static_cast<double>(coords(upper));

    const double denom = x1 - x0;
    const double w     = (denom == 0.0)
                             ? 0.0
                             : std::clamp((static_cast<double>(value) - x0) / denom, 0.0, 1.0);

    return Bracket{ lower, upper, w };
}

inline void validate_uniform_axis(const UniformAxis& axis, const char* name, const bool allow_empty = false)
{
    if (!std::isfinite(axis.origin))
        throw std::invalid_argument(std::string(name) + " origin must be finite");

    if (axis.size == 0)
    {
        if (!allow_empty)
            throw std::invalid_argument(std::string(name) + " size must be greater than zero");

        if (!std::isfinite(axis.spacing))
            throw std::invalid_argument(std::string(name) + " spacing must be finite");

        return;
    }

    if (axis.size > 1)
    {
        if (!std::isfinite(axis.spacing))
            throw std::invalid_argument(std::string(name) + " spacing must be finite");

        if (axis.spacing <= 0.0)
            throw std::invalid_argument(std::string(name) + " spacing must be strictly positive");
    }
}

inline size_t nearest_index_uniform(const UniformAxis& axis, const double inv_spacing, const double value)
{
    if (axis.size == 1)
        return 0;

    const double pos = (value - axis.origin) * inv_spacing;

    if (pos <= 0.0)
        return 0;

    const double max_index = static_cast<double>(axis.size - 1);
    if (pos >= max_index)
        return axis.size - 1;

    const double lower_pos = std::floor(pos);
    const double upper_pos = lower_pos + 1.0;

    const double lower_diff = pos - lower_pos;
    const double upper_diff = upper_pos - pos;

    if (upper_diff < lower_diff)
        return static_cast<size_t>(lower_pos) + 1;

    return static_cast<size_t>(lower_pos);
}

inline Bracket bracket_indices_uniform(const UniformAxis& axis, const double inv_spacing, const double value)
{
    if (axis.size == 1)
        return Bracket{ 0, 0, 0.0 };

    const double pos = (value - axis.origin) * inv_spacing;

    if (pos <= 0.0)
        return Bracket{ 0, 0, 0.0 };

    const double upper_limit = static_cast<double>(axis.size - 1);

    if (pos >= upper_limit)
        return Bracket{ axis.size - 1, axis.size - 1, 0.0 };

    const size_t lower = static_cast<size_t>(std::floor(pos));
    const size_t upper = lower + 1;
    const double frac  = pos - static_cast<double>(lower);

    return Bracket{ lower, upper, frac };
}

} // namespace detail

template<tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_1d t_xtensor_new_x,
         tools::helper::c_xtensor_1d t_xtensor_new_y>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_nearest(
    const t_xtensor_2d&    reference,
    const t_xtensor_ref_x& reference_x,
    const t_xtensor_ref_y& reference_y,
    const t_xtensor_new_x& new_x,
    const t_xtensor_new_y& new_y,
    const int              mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size(), new_y.size() });

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < new_y.size(); ++iy)
        {
            const auto ref_ix = detail::nearest_index(reference_x, new_x(ix));
            const auto ref_iy = detail::nearest_index(reference_y, new_y(iy));
            output(ix, iy)    = reference(ref_ix, ref_iy);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_2d>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_nearest(
    const t_xtensor_2d& reference,
    const UniformAxis&  reference_x,
    const UniformAxis&  reference_y,
    const UniformAxis&  new_x,
    const UniformAxis&  new_y,
    const int           mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    detail::validate_uniform_axis(reference_x, "reference_x");
    detail::validate_uniform_axis(reference_y, "reference_y");
    detail::validate_uniform_axis(new_x, "new_x", true);
    detail::validate_uniform_axis(new_y, "new_y", true);

    if (reference.shape()[0] != reference_x.size || reference.shape()[1] != reference_y.size)
        throw std::invalid_argument("Reference axes must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size, new_y.size });

    const int    threads            = std::max(1, mp_cores);
    const double inv_ref_x_spacing  = (reference_x.size > 1) ? 1.0 / reference_x.spacing : 0.0;
    const double inv_ref_y_spacing  = (reference_y.size > 1) ? 1.0 / reference_y.spacing : 0.0;
    const double new_x_origin       = new_x.origin;
    const double new_x_spacing      = new_x.spacing;
    const double new_y_origin       = new_y.origin;
    const double new_y_spacing      = new_y.spacing;
    const size_t new_x_size         = new_x.size;
    const size_t new_y_size         = new_y.size;

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x_size; ++ix)
    {
        for (size_t iy = 0; iy < new_y_size; ++iy)
        {
            const double coord_x = new_x_origin + new_x_spacing * static_cast<double>(ix);
            const double coord_y = new_y_origin + new_y_spacing * static_cast<double>(iy);

            const auto ref_ix = (reference_x.size == 1)
                                    ? size_t{ 0 }
                                    : detail::nearest_index_uniform(reference_x, inv_ref_x_spacing, coord_x);
            const auto ref_iy = (reference_y.size == 1)
                                    ? size_t{ 0 }
                                    : detail::nearest_index_uniform(reference_y, inv_ref_y_spacing, coord_y);

            output(ix, iy) = reference(ref_ix, ref_iy);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_2d,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_1d t_xtensor_new_x,
         tools::helper::c_xtensor_1d t_xtensor_new_y>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_bilinear(
    const t_xtensor_2d&    reference,
    const t_xtensor_ref_x& reference_x,
    const t_xtensor_ref_y& reference_y,
    const t_xtensor_new_x& new_x,
    const t_xtensor_new_y& new_y,
    const int              mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size(), new_y.size() });

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < new_y.size(); ++iy)
        {
            const auto bx = detail::bracket_indices(reference_x, new_x(ix));
            const auto by = detail::bracket_indices(reference_y, new_y(iy));

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) +
                                               wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) +
                                               wy1 * static_cast<double>(v11));

            output(ix, iy) = static_cast<value_type>(interpolated);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_2d>
xt::xtensor<typename t_xtensor_2d::value_type, 2> backward_map_bilinear(
    const t_xtensor_2d& reference,
    const UniformAxis&  reference_x,
    const UniformAxis&  reference_y,
    const UniformAxis&  new_x,
    const UniformAxis&  new_y,
    const int           mp_cores = 1)
{
    using value_type = typename t_xtensor_2d::value_type;

    detail::validate_uniform_axis(reference_x, "reference_x");
    detail::validate_uniform_axis(reference_y, "reference_y");
    detail::validate_uniform_axis(new_x, "new_x", true);
    detail::validate_uniform_axis(new_y, "new_y", true);

    if (reference.shape()[0] != reference_x.size || reference.shape()[1] != reference_y.size)
        throw std::invalid_argument("Reference axes must match reference image shape");

    auto output = xt::xtensor<value_type, 2>::from_shape({ new_x.size, new_y.size });

    const int    threads            = std::max(1, mp_cores);
    const double inv_ref_x_spacing  = (reference_x.size > 1) ? 1.0 / reference_x.spacing : 0.0;
    const double inv_ref_y_spacing  = (reference_y.size > 1) ? 1.0 / reference_y.spacing : 0.0;
    const double new_x_origin       = new_x.origin;
    const double new_x_spacing      = new_x.spacing;
    const double new_y_origin       = new_y.origin;
    const double new_y_spacing      = new_y.spacing;
    const size_t new_x_size         = new_x.size;
    const size_t new_y_size         = new_y.size;

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < new_x_size; ++ix)
    {
        for (size_t iy = 0; iy < new_y_size; ++iy)
        {
            const double coord_x = new_x_origin + new_x_spacing * static_cast<double>(ix);
            const double coord_y = new_y_origin + new_y_spacing * static_cast<double>(iy);

            const auto bx = (reference_x.size == 1)
                                ? detail::Bracket{ 0, 0, 0.0 }
                                : detail::bracket_indices_uniform(reference_x, inv_ref_x_spacing, coord_x);
            const auto by = (reference_y.size == 1)
                                ? detail::Bracket{ 0, 0, 0.0 }
                                : detail::bracket_indices_uniform(reference_y, inv_ref_y_spacing, coord_y);

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) + wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) + wy1 * static_cast<double>(v11));

            output(ix, iy) = static_cast<value_type>(interpolated);
        }
    }

    return output;
}

template<tools::helper::c_xtensor_2d t_xtensor_reference,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_2d t_xtensor_target,
         tools::helper::c_xtensor_1d t_xtensor_target_x,
         tools::helper::c_xtensor_1d t_xtensor_target_y>
void backward_map_nearest_add(
    const t_xtensor_reference&    reference,
    const t_xtensor_ref_x&        reference_x,
    const t_xtensor_ref_y&        reference_y,
    t_xtensor_target&             target,
    const t_xtensor_target_x&     target_x,
    const t_xtensor_target_y&     target_y,
    const int                     mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    if (target.shape()[0] != target_x.size() || target.shape()[1] != target_y.size())
        throw std::invalid_argument("Target coordinate arrays must match target image shape");

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < target_y.size(); ++iy)
        {
            const auto ref_ix = detail::nearest_index(reference_x, target_x(ix));
            const auto ref_iy = detail::nearest_index(reference_y, target_y(iy));
            target(ix, iy) += static_cast<target_value_type>(reference(ref_ix, ref_iy));
        }
    }
}

template<tools::helper::c_xtensor_2d t_xtensor_reference, tools::helper::c_xtensor_2d t_xtensor_target>
void backward_map_nearest_add(
    const t_xtensor_reference& reference,
    const UniformAxis&         reference_x,
    const UniformAxis&         reference_y,
    t_xtensor_target&          target,
    const UniformAxis&         target_x,
    const UniformAxis&         target_y,
    const int                  mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    detail::validate_uniform_axis(reference_x, "reference_x");
    detail::validate_uniform_axis(reference_y, "reference_y");
    detail::validate_uniform_axis(target_x, "target_x", true);
    detail::validate_uniform_axis(target_y, "target_y", true);

    if (reference.shape()[0] != reference_x.size || reference.shape()[1] != reference_y.size)
        throw std::invalid_argument("Reference axes must match reference image shape");

    if (target.shape()[0] != target_x.size || target.shape()[1] != target_y.size)
        throw std::invalid_argument("Target axes must match target image shape");

    const int    threads            = std::max(1, mp_cores);
    const double inv_ref_x_spacing  = (reference_x.size > 1) ? 1.0 / reference_x.spacing : 0.0;
    const double inv_ref_y_spacing  = (reference_y.size > 1) ? 1.0 / reference_y.spacing : 0.0;
    const double target_x_origin    = target_x.origin;
    const double target_x_spacing   = target_x.spacing;
    const double target_y_origin    = target_y.origin;
    const double target_y_spacing   = target_y.spacing;
    const size_t target_x_size      = target_x.size;
    const size_t target_y_size      = target_y.size;

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x_size; ++ix)
    {
        for (size_t iy = 0; iy < target_y_size; ++iy)
        {
            const double coord_x = target_x_origin + target_x_spacing * static_cast<double>(ix);
            const double coord_y = target_y_origin + target_y_spacing * static_cast<double>(iy);

            const auto ref_ix = (reference_x.size == 1)
                                    ? size_t{ 0 }
                                    : detail::nearest_index_uniform(reference_x, inv_ref_x_spacing, coord_x);
            const auto ref_iy = (reference_y.size == 1)
                                    ? size_t{ 0 }
                                    : detail::nearest_index_uniform(reference_y, inv_ref_y_spacing, coord_y);

            target(ix, iy) += static_cast<target_value_type>(reference(ref_ix, ref_iy));
        }
    }
}

template<tools::helper::c_xtensor_2d t_xtensor_reference,
         tools::helper::c_xtensor_1d t_xtensor_ref_x,
         tools::helper::c_xtensor_1d t_xtensor_ref_y,
         tools::helper::c_xtensor_2d t_xtensor_target,
         tools::helper::c_xtensor_1d t_xtensor_target_x,
         tools::helper::c_xtensor_1d t_xtensor_target_y>
void backward_map_bilinear_add(
    const t_xtensor_reference&    reference,
    const t_xtensor_ref_x&        reference_x,
    const t_xtensor_ref_y&        reference_y,
    t_xtensor_target&             target,
    const t_xtensor_target_x&     target_x,
    const t_xtensor_target_y&     target_y,
    const int                     mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    if (reference.shape()[0] != reference_x.size() || reference.shape()[1] != reference_y.size())
        throw std::invalid_argument("Reference coordinate arrays must match reference image shape");

    if (target.shape()[0] != target_x.size() || target.shape()[1] != target_y.size())
        throw std::invalid_argument("Target coordinate arrays must match target image shape");

    const int threads = std::max(1, mp_cores);

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x.size(); ++ix)
    {
        for (size_t iy = 0; iy < target_y.size(); ++iy)
        {
            const auto bx = detail::bracket_indices(reference_x, target_x(ix));
            const auto by = detail::bracket_indices(reference_y, target_y(iy));

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) +
                                               wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) +
                                               wy1 * static_cast<double>(v11));

            target(ix, iy) += static_cast<target_value_type>(interpolated);
        }
    }
}

template<tools::helper::c_xtensor_2d t_xtensor_reference, tools::helper::c_xtensor_2d t_xtensor_target>
void backward_map_bilinear_add(
    const t_xtensor_reference& reference,
    const UniformAxis&         reference_x,
    const UniformAxis&         reference_y,
    t_xtensor_target&          target,
    const UniformAxis&         target_x,
    const UniformAxis&         target_y,
    const int                  mp_cores = 1)
{
    using target_value_type = typename t_xtensor_target::value_type;

    detail::validate_uniform_axis(reference_x, "reference_x");
    detail::validate_uniform_axis(reference_y, "reference_y");
    detail::validate_uniform_axis(target_x, "target_x", true);
    detail::validate_uniform_axis(target_y, "target_y", true);

    if (reference.shape()[0] != reference_x.size || reference.shape()[1] != reference_y.size)
        throw std::invalid_argument("Reference axes must match reference image shape");

    if (target.shape()[0] != target_x.size || target.shape()[1] != target_y.size)
        throw std::invalid_argument("Target axes must match target image shape");

    const int    threads            = std::max(1, mp_cores);
    const double inv_ref_x_spacing  = (reference_x.size > 1) ? 1.0 / reference_x.spacing : 0.0;
    const double inv_ref_y_spacing  = (reference_y.size > 1) ? 1.0 / reference_y.spacing : 0.0;
    const double target_x_origin    = target_x.origin;
    const double target_x_spacing   = target_x.spacing;
    const double target_y_origin    = target_y.origin;
    const double target_y_spacing   = target_y.spacing;
    const size_t target_x_size      = target_x.size;
    const size_t target_y_size      = target_y.size;

#pragma omp parallel for collapse(2) if (threads > 1) num_threads(threads)
    for (size_t ix = 0; ix < target_x_size; ++ix)
    {
        for (size_t iy = 0; iy < target_y_size; ++iy)
        {
            const double coord_x = target_x_origin + target_x_spacing * static_cast<double>(ix);
            const double coord_y = target_y_origin + target_y_spacing * static_cast<double>(iy);

            const auto bx = (reference_x.size == 1)
                                ? detail::Bracket{ 0, 0, 0.0 }
                                : detail::bracket_indices_uniform(reference_x, inv_ref_x_spacing, coord_x);
            const auto by = (reference_y.size == 1)
                                ? detail::Bracket{ 0, 0, 0.0 }
                                : detail::bracket_indices_uniform(reference_y, inv_ref_y_spacing, coord_y);

            const auto& v00 = reference(bx.lower, by.lower);
            const auto& v01 = reference(bx.lower, by.upper);
            const auto& v10 = reference(bx.upper, by.lower);
            const auto& v11 = reference(bx.upper, by.upper);

            const double wx0 = 1.0 - bx.weight;
            const double wx1 = bx.weight;
            const double wy0 = 1.0 - by.weight;
            const double wy1 = by.weight;

            const double interpolated = wx0 * (wy0 * static_cast<double>(v00) + wy1 * static_cast<double>(v01)) +
                                         wx1 * (wy0 * static_cast<double>(v10) + wy1 * static_cast<double>(v11));

            target(ix, iy) += static_cast<target_value_type>(interpolated);
        }
    }
}

} // namespace functions
} // namespace imageprocessing
} // namespace algorithms
} // namespace themachinethatgoesping