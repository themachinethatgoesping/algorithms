#pragma once

/* generated doc strings */
#include ".docstrings/gridfunctions.doc.hpp"

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

#include <fmt/core.h>
#include <map>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <tuple>
#include <vector>

namespace themachinethatgoesping {
namespace algorithms {
namespace gridding {
namespace functions {

template<typename T_vector>
inline auto get_minmax(const T_vector& sv, const int mp_cores = 1)
{
    using t_val = typename T_vector::value_type;
    if (sv.size() == 0)
        throw std::runtime_error("Expected non-empty array.");

    t_val minv = std::numeric_limits<t_val>::max();
    t_val maxv = std::numeric_limits<t_val>::lowest();

#pragma omp parallel for num_threads(mp_cores) reduction(min : minv) reduction(max : maxv)
    for (const auto v : sv)
    {
        if (v < minv)
            minv = v;
        if (v > maxv)
            maxv = v;
    }

    if (minv == std::numeric_limits<t_val>::max())
        minv = std::numeric_limits<t_val>::quiet_NaN();
    if (maxv == std::numeric_limits<t_val>::min())
        maxv = std::numeric_limits<t_val>::quiet_NaN();

    return std::make_tuple(minv, maxv);
}

template<typename T_vector>
inline auto get_minmax(const T_vector& sx, const T_vector& sy, const int mp_cores = 1)
{
    using t_val = typename T_vector::value_type;
    if (sx.size() != sy.size())
        throw std::runtime_error(fmt::format("Expected equal array lengths. "
                                             "sx.size() = {}, sy.size() = {}",
                                             sx.size(),
                                             sy.size()));

    using t_val = typename T_vector::value_type;

    t_val minx = std::numeric_limits<t_val>::max();
    t_val maxx = std::numeric_limits<t_val>::lowest();
    t_val miny = std::numeric_limits<t_val>::max();
    t_val maxy = std::numeric_limits<t_val>::lowest();

#pragma omp parallel for num_threads(mp_cores) reduction(min : minx, miny)                         \
    reduction(max : maxx, maxy)
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i];
        if (x < minx)
            minx = x;
        if (x > maxx)
            maxx = x;
        if (y < miny)
            miny = y;
        if (y > maxy)
            maxy = y;
    }
    if (minx == std::numeric_limits<t_val>::max())
        minx = std::numeric_limits<t_val>::quiet_NaN();
    if (maxx == std::numeric_limits<t_val>::min())
        maxx = std::numeric_limits<t_val>::quiet_NaN();
    if (miny == std::numeric_limits<t_val>::max())
        miny = std::numeric_limits<t_val>::quiet_NaN();
    if (maxy == std::numeric_limits<t_val>::min())
        maxy = std::numeric_limits<t_val>::quiet_NaN();

    return std::make_tuple(minx, maxx, miny, maxy);
}

template<typename T_vector>
inline auto get_minmax(const T_vector& sx,
                       const T_vector& sy,
                       const T_vector& sz,
                       const int       mp_cores = 1)
{
    using t_val = typename T_vector::value_type;
    if (sx.size() != sy.size() || sy.size() != sz.size())
        throw std::runtime_error(fmt::format("Expected equal array lengths. "
                                             "sx.size() = {}, sy.size() = {}, sz.size() = {}",
                                             sx.size(),
                                             sy.size(),
                                             sz.size()));

    using t_val = typename T_vector::value_type;

    t_val minx = std::numeric_limits<t_val>::max();
    t_val maxx = std::numeric_limits<t_val>::lowest();
    t_val miny = std::numeric_limits<t_val>::max();
    t_val maxy = std::numeric_limits<t_val>::lowest();
    t_val minz = std::numeric_limits<t_val>::max();
    t_val maxz = std::numeric_limits<t_val>::lowest();

#pragma omp parallel for num_threads(mp_cores) reduction(min : minx, miny, minz)                   \
    reduction(max : maxx, maxy, maxz)
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], z = sz[i];
        if (x < minx)
            minx = x;
        if (x > maxx)
            maxx = x;
        if (y < miny)
            miny = y;
        if (y > maxy)
            maxy = y;
        if (z < minz)
            minz = z;
        if (z > maxz)
            maxz = z;
    }
    if (minx == std::numeric_limits<t_val>::max())
        minx = std::numeric_limits<t_val>::quiet_NaN();
    if (maxx == std::numeric_limits<t_val>::min())
        maxx = std::numeric_limits<t_val>::quiet_NaN();
    if (miny == std::numeric_limits<t_val>::max())
        miny = std::numeric_limits<t_val>::quiet_NaN();
    if (maxy == std::numeric_limits<t_val>::min())
        maxy = std::numeric_limits<t_val>::quiet_NaN();
    if (minz == std::numeric_limits<t_val>::max())
        minz = std::numeric_limits<t_val>::quiet_NaN();
    if (maxz == std::numeric_limits<t_val>::min())
        maxz = std::numeric_limits<t_val>::quiet_NaN();

    return std::make_tuple(minx, maxx, miny, maxy, minz, maxz);
}

template<std::floating_point T>
inline int get_index(const T val, const T grd_val_min, const T grd_res)
{
    return static_cast<int>(std::lround((val - grd_val_min) / grd_res));
}

template<std::floating_point T>
inline T get_index_fraction(const T val, const T grd_val_min, const T grd_res)
{
    return (val - grd_val_min) / grd_res;
}

template<std::floating_point T>
inline T get_value(const T index, const T grd_val_min, const T grd_res)
{
    return grd_val_min + grd_res * static_cast<T>(index);
}

template<std::floating_point T>
inline T get_grd_value(const T value, const T grd_val_min, const T grd_res)
{
    return get_value(static_cast<T>(get_index(value, grd_val_min, grd_res)), grd_val_min, grd_res);
}

template<typename t_vector, std::floating_point t_float, std::integral t_int>
inline auto group_blocks(const t_vector& sx,
                         const t_vector& sy,
                         const t_vector& sz,
                         const t_vector& sv,
                         const t_float   xmin,
                         const t_float   xres,
                         const t_int     nx,
                         const t_float   ymin,
                         const t_float   yres,
                         const t_int     ny,
                         const t_float   zmin,
                         const t_float   zres,
                         const t_int     nz)
{
    std::map<size_t, std::vector<typename t_vector::value_type>> blocks;

    const auto x_stride = ny;
    const auto y_stride = nx * ny;

    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], z = sz[i], v = sv[i];

        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);
            const int iy = get_index(y, ymin, yres);
            const int iz = get_index(z, zmin, zres);

            if (ix < 0 || iy < 0 || iz < 0)
                continue;
            if (ix >= nx || iy >= ny || iz >= nz)
                continue;

            blocks[iz + iy * y_stride + ix * x_stride].push_back(v);
        }
    }

    return blocks;
}

template<typename t_vector, std::floating_point t_float, std::integral t_int>
inline auto group_blocks(const t_vector& sx,
                         const t_vector& sy,
                         const t_vector& sv,
                         const t_float   xmin,
                         const t_float   xres,
                         const t_int     nx,
                         const t_float   ymin,
                         const t_float   yres,
                         const t_int     ny)
{
    std::map<size_t, std::vector<typename t_vector::value_type>> blocks;

    const auto x_stride = ny;

    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], v = sv[i];

        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);
            const int iy = get_index(y, ymin, yres);

            if (ix < 0 || iy < 0)
                continue;
            if (ix >= nx || iy >= ny)
                continue;

            blocks[iy + ix * x_stride].push_back(v);
        }
    }

    return blocks;
}

template<typename t_vector, std::floating_point t_float, std::integral t_int>
inline auto group_blocks(const t_vector& sx,
                         const t_vector& sv,
                         const t_float   xmin,
                         const t_float   xres,
                         const t_int     nx)
{
    std::map<size_t, std::vector<typename t_vector::value_type>> blocks;

    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], v = sv[i];

        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);

            if (ix < 0)
                continue;
            if (ix >= nx)
                continue;

            blocks[ix].push_back(v);
        }
    }

    return blocks;
}

template<std::floating_point t_float>
inline auto get_index_weights(const t_float frac_x, const t_float frac_y, const t_float frac_z)
    -> std::
        tuple<std::array<int, 8>, std::array<int, 8>, std::array<int, 8>, std::array<t_float, 8>>
{
    t_float ifraction_x = frac_x - std::floor(frac_x);
    t_float ifraction_y = frac_y - std::floor(frac_y);
    t_float ifraction_z = frac_z - std::floor(frac_z);

    t_float fraction_x = 1.0 - ifraction_x;
    t_float fraction_y = 1.0 - ifraction_y;
    t_float fraction_z = 1.0 - ifraction_z;

    int ix1 = static_cast<int>(std::floor(frac_x));
    int ix2 = static_cast<int>(std::ceil(frac_x));
    int iy1 = static_cast<int>(std::floor(frac_y));
    int iy2 = static_cast<int>(std::ceil(frac_y));
    int iz1 = static_cast<int>(std::floor(frac_z));
    int iz2 = static_cast<int>(std::ceil(frac_z));

    std::array<int, 8> X = { ix1, ix1, ix1, ix1, ix2, ix2, ix2, ix2 };
    std::array<int, 8> Y = { iy1, iy1, iy2, iy2, iy1, iy1, iy2, iy2 };
    std::array<int, 8> Z = { iz1, iz2, iz1, iz2, iz1, iz2, iz1, iz2 };

    t_float vx    = fraction_x;
    t_float vxy   = vx * fraction_y;
    t_float vxiy  = vx * ifraction_y;
    t_float vix   = ifraction_x;
    t_float vixy  = vix * fraction_y;
    t_float vixiy = vix * ifraction_y;

    std::array<t_float, 8> WEIGHT = { vxy * fraction_z,   vxy * ifraction_z,  vxiy * fraction_z,
                                      vxiy * ifraction_z, vixy * fraction_z,  vixy * ifraction_z,
                                      vixiy * fraction_z, vixiy * ifraction_z };

    return { X, Y, Z, WEIGHT };
}

template<typename t_vector,
         tools::helper::c_xtensor_3d t_xtensor_3d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_weighted_mean(const t_vector& sx,
                              const t_vector& sy,
                              const t_vector& sz,
                              const t_vector& sv,
                              const t_float   xmin,
                              const t_float   xres,
                              const t_int     nx,
                              const t_float   ymin,
                              const t_float   yres,
                              const t_int     ny,
                              const t_float   zmin,
                              const t_float   zres,
                              const t_int     nz,
                              t_xtensor_3d&   image_values,
                              t_xtensor_3d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], z = sz[i], v = sv[i];
        if (std::isfinite(v))
        {
            const auto [X, Y, Z, WEIGHT] = get_index_weights(get_index_fraction(x, xmin, xres),
                                                             get_index_fraction(y, ymin, yres),
                                                             get_index_fraction(z, zmin, zres));

            for (size_t idx = 0; idx < 8; ++idx)
            {
                const auto ix = X[idx];
                const auto iy = Y[idx];
                const auto iz = Z[idx];
                const auto w  = WEIGHT[idx];
                if (w == t_float(0.0))
                    continue;

                if (ix < 0 || iy < 0 || iz < 0)
                    continue;
                if (ix >= nx || iy >= ny || iz >= nz)
                    continue;

                image_values.unchecked(ix, iy, iz) += v * w;
                image_weights.unchecked(ix, iy, iz) += w;
            }
        }
    }
}

template<typename t_vector,
         tools::helper::c_xtensor_3d t_xtensor_3d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_block_mean(const t_vector& sx,
                           const t_vector& sy,
                           const t_vector& sz,
                           const t_vector& sv,
                           const t_float   xmin,
                           const t_float   xres,
                           const t_int     nx,
                           const t_float   ymin,
                           const t_float   yres,
                           const t_int     ny,
                           const t_float   zmin,
                           const t_float   zres,
                           const t_int     nz,
                           t_xtensor_3d&   image_values,
                           t_xtensor_3d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], z = sz[i], v = sv[i];

        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);
            const int iy = get_index(y, ymin, yres);
            const int iz = get_index(z, zmin, zres);

            if (ix < 0 || iy < 0 || iz < 0)
                continue;
            if (ix >= nx || iy >= ny || iz >= nz)
                continue;

            image_values.unchecked(ix, iy, iz) += v;
            image_weights.unchecked(ix, iy, iz) += 1.0;
        }
    }
}

/* 2D overloads */
template<std::floating_point t_float>
inline auto get_index_weights(const t_float frac_x, const t_float frac_y)
    -> std::tuple<std::array<int, 4>, std::array<int, 4>, std::array<t_float, 4>>
{
    const t_float ifraction_x = frac_x - std::floor(frac_x);
    const t_float fraction_x  = 1.0 - ifraction_x;
    const t_float ifraction_y = frac_y - std::floor(frac_y);
    const t_float fraction_y  = 1.0 - ifraction_y;

    const int ix1 = static_cast<int>(std::floor(frac_x));
    const int ix2 = static_cast<int>(std::ceil(frac_x));
    const int iy1 = static_cast<int>(std::floor(frac_y));
    const int iy2 = static_cast<int>(std::ceil(frac_y));

    std::array<int, 4> X = { ix1, ix1, ix2, ix2 };
    std::array<int, 4> Y = { iy1, iy2, iy1, iy2 };

    std::array<t_float, 4> W = { fraction_x * fraction_y,
                                 fraction_x * ifraction_y,
                                 ifraction_x * fraction_y,
                                 ifraction_x * ifraction_y };

    return { X, Y, W };
}

template<typename t_vector,
         tools::helper::c_xtensor_2d t_xtensor_2d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_weighted_mean(const t_vector& sx,
                              const t_vector& sy,
                              const t_vector& sv,
                              const t_float   xmin,
                              const t_float   xres,
                              const t_int     nx,
                              const t_float   ymin,
                              const t_float   yres,
                              const t_int     ny,
                              t_xtensor_2d&   image_values,
                              t_xtensor_2d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], v = sv[i];
        if (std::isfinite(v))
        {
            const auto [X, Y, W] = get_index_weights(get_index_fraction(x, xmin, xres),
                                                     get_index_fraction(y, ymin, yres));
            for (size_t idx = 0; idx < 4; ++idx)
            {
                const auto ix = X[idx];
                const auto iy = Y[idx];
                const auto w  = W[idx];
                if (w == t_float(0.0))
                    continue;
                if (ix < 0 || iy < 0 || ix >= nx || iy >= ny)
                    continue;

                image_values.unchecked(ix, iy) += v * w;
                image_weights.unchecked(ix, iy) += w;
            }
        }
    }
}

template<typename t_vector,
         tools::helper::c_xtensor_2d t_xtensor_2d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_block_mean(const t_vector& sx,
                           const t_vector& sy,
                           const t_vector& sv,
                           const t_float   xmin,
                           const t_float   xres,
                           const t_int     nx,
                           const t_float   ymin,
                           const t_float   yres,
                           const t_int     ny,
                           t_xtensor_2d&   image_values,
                           t_xtensor_2d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], y = sy[i], v = sv[i];
        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);
            const int iy = get_index(y, ymin, yres);
            if (ix < 0 || iy < 0 || ix >= nx || iy >= ny)
                continue;

            image_values.unchecked(ix, iy) += v;
            image_weights.unchecked(ix, iy) += 1.0;
        }
    }
}

/* 1D overloads */
template<std::floating_point t_float>
inline auto get_index_weights(const t_float frac_x)
    -> std::tuple<std::array<int, 2>, std::array<t_float, 2>>
{
    const t_float ifraction_x = frac_x - std::floor(frac_x);
    const t_float fraction_x  = 1.0 - ifraction_x;

    const int ix1 = static_cast<int>(std::floor(frac_x));
    const int ix2 = static_cast<int>(std::ceil(frac_x));

    std::array<int, 2>     X = { ix1, ix2 };
    std::array<t_float, 2> W = { fraction_x, ifraction_x };

    return { X, W };
}

template<typename t_vector,
         tools::helper::c_xtensor_1d t_xtensor_1d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_weighted_mean(const t_vector& sx,
                              const t_vector& sv,
                              const t_float   xmin,
                              const t_float   xres,
                              const t_int     nx,
                              t_xtensor_1d&   image_values,
                              t_xtensor_1d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], v = sv[i];
        if (std::isfinite(v))
        {
            const auto [X, W] = get_index_weights(get_index_fraction(x, xmin, xres));
            for (size_t idx = 0; idx < 2; ++idx)
            {
                const auto ix = X[idx];
                const auto w  = W[idx];
                if (w == t_float(0.0))
                    continue;
                if (ix < 0 || ix >= nx)
                    continue;

                image_values.unchecked(ix) += v * w;
                image_weights.unchecked(ix) += w;
            }
        }
    }
}

template<typename t_vector,
         tools::helper::c_xtensor_1d t_xtensor_1d,
         std::floating_point         t_float,
         std::integral               t_int>
inline void grd_block_mean(const t_vector& sx,
                           const t_vector& sv,
                           const t_float   xmin,
                           const t_float   xres,
                           const t_int     nx,
                           t_xtensor_1d&   image_values,
                           t_xtensor_1d&   image_weights)
{
    for (size_t i = 0; i < sx.size(); ++i)
    {
        const auto x = sx[i], v = sv[i];
        if (std::isfinite(v))
        {
            const int ix = get_index(x, xmin, xres);
            if (ix < 0 || ix >= nx)
                continue;

            image_values.unchecked(ix) += v;
            image_weights.unchecked(ix) += 1.0;
        }
    }
}

} // namespace functions
} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
