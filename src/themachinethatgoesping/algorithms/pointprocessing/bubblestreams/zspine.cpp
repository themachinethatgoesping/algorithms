// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "zspine.hpp"

#include <stdexcept>
#include <algorithm>

namespace themachinethatgoesping {
namespace algorithms {
namespace pointprocessing {
namespace bubblestreams {

std::tuple<std::vector<ZSpine::coord_type>, std::vector<ZSpine::coord_type>, std::vector<ZSpine::coord_type>>
ZSpine::get_sorted_by_z(std::optional<coord_type> cutoff_z)
{
    auto                    indices = xt::argsort(xt::adapt(_z));
    std::vector<coord_type> sorted_x;
    std::vector<coord_type> sorted_y;
    std::vector<coord_type> sorted_z;

    const bool       is_altitude = _is_altitude;
    const bool       cut_z       = cutoff_z.has_value();
    const coord_type cutoff      = cutoff_z.value_or(0.0);

    for (const auto& index : indices)
    {
        // do not include points below the origin
        if (cut_z)
        {
            if (!is_altitude)
            {
                if (_z[index] >= cutoff)
                    break;
            }
            else
            {
                if (_z[index] <= cutoff)
                    continue;
            }
        }

        sorted_x.push_back(_x[indices[index]]);
        sorted_y.push_back(_y[indices[index]]);
        sorted_z.push_back(_z[indices[index]]);
    }

    return make_tuple(std::move(sorted_x), std::move(sorted_y), std::move(sorted_z));
}

void ZSpine::sort_by_z()
{
    auto [sorted_x, sorted_y, sorted_z] = get_sorted_by_z();
    _x                                  = std::move(sorted_x);
    _y                                  = std::move(sorted_y);
    _z                                  = std::move(sorted_z);
}

void ZSpine::rebuild_interpolators()
{
    // get_data_sorted_by_z and cut by origin z if origin was set
    auto [x, y, z] = get_sorted_by_z(_origin ? std::optional<coord_type>(std::get<2>(*_origin))
                                             : std::nullopt);

    // add origin point and 2 points at 1m intervals below it
    if (_origin.has_value())
    {
        auto [origin_x, origin_y, origin_z] = _origin.value();

        if (!_is_altitude)
        {
            x.insert(x.end(), 3, origin_x);
            y.insert(y.end(), 3, origin_y);

            z.push_back(origin_z);
            z.push_back(origin_z + 1.0);
            z.push_back(origin_z + 2.0);
        }
        else
        {
            x.insert(x.begin(), { origin_x, origin_x, origin_x });
            y.insert(y.begin(), { origin_y, origin_y, origin_y });
            z.insert(z.begin(), { origin_z - 2.0, origin_z - 1.0, origin_z });
        }
    }

    _x_interpolator.set_data_XY(z, std::move(x));
    _y_interpolator.set_data_XY(std::move(z), std::move(y));
}

// Constructor implementation
ZSpine::ZSpine(std::vector<coord_type> x,
               std::vector<coord_type> y,
               std::vector<coord_type> z,
               bool                    is_altitude)
    : _x(std::move(x))
    , _y(std::move(y))
    , _z(std::move(z))
    , _is_altitude(is_altitude)
{
    sort_by_z();
    rebuild_interpolators();
}

void ZSpine::set_origin(coord_type x, coord_type y, coord_type z)
{
    _origin = std::make_tuple(x, y, z);
    rebuild_interpolators();
}

void ZSpine::reset_origin()
{
    _origin.reset();
    rebuild_interpolators();
}

void ZSpine::estimate_origin(coord_type bottom_z, coord_type slope_modifier)
{
    if (_z.empty())
        throw std::runtime_error("Cannot estimate origin from empty spine");
    if (_z.size() == 1)
    {
        set_origin(_x.front(), _y.front(), bottom_z);
        return;
    }

    if (!_is_altitude)
    {
        if (bottom_z <= _z.back())
        {
            // reinstates interpolators to state without origin
            reset_origin();
            auto origin = get_xy(bottom_z);
            set_origin(origin.first, origin.second, bottom_z);
            return;
        }

        // make linear interpolation between last two points to get x and y at bottom_z
        coord_type dz = (bottom_z - _z.back()) / (_z.back() - _z[_z.size() - 2]);
        coord_type x  = _x.back() + (_x.back() - _x[_x.size() - 2]) * dz * slope_modifier;
        coord_type y  = _y.back() + (_y.back() - _y[_y.size() - 2]) * dz * slope_modifier;
        set_origin(x, y, bottom_z);
    }
    else
    {
        if (bottom_z >= _z.front())
        {
            // reinstates interpolators to state without origin
            reset_origin();
            auto origin = get_xy(bottom_z);
            set_origin(origin.first, origin.second, bottom_z);
            return;
        }

        // make linear interpolation from second to first point to get x and y at bottom_z
        coord_type dz = (bottom_z - _z[1]) / (_z[1] - _z[0]);
        coord_type x  = _x[1] + (_x[1] - _x[0]) * dz * slope_modifier;
        coord_type y  = _y[1] + (_y[1] - _y[0]) * dz * slope_modifier;
        set_origin(x, y, bottom_z);
    }
}

std::pair<ZSpine::coord_type, ZSpine::coord_type> ZSpine::get_xy(coord_type z) const
{
    return std::make_pair(_x_interpolator(z), _y_interpolator(z));
}

std::tuple<std::vector<ZSpine::coord_type>, std::vector<ZSpine::coord_type>, std::vector<ZSpine::coord_type>>
ZSpine::get_spine_points(const bool with_origin) const
{
    if (with_origin && _origin.has_value())
    {
        auto [x, y, z] = _origin.value();
        auto X         = _x;
        auto Y         = _y;
        auto Z         = _z;

        if (!_is_altitude)
        {
            X.insert(X.begin(), x);
            Y.insert(Y.begin(), y);
            Z.insert(Z.begin(), z);
        }
        else
        {
            X.push_back(x);
            Y.push_back(y);
            Z.push_back(z);
        }
        return std::make_tuple(std::move(X), std::move(Y), std::move(Z));
    }
    return std::make_tuple(_x, _y, _z);
}

auto ZSpine::get_spine(size_t n_values, bool with_origin) const
{
    auto min_z = _z.front();
    auto max_z = _z.back();

    if (with_origin && _origin.has_value())
    {
        auto [ox, oy, oz] = _origin.value();
        if (!_is_altitude)
            max_z = oz;
        else
            min_z = oz;
    }

    auto z = xt::linspace(min_z, max_z, n_values);

    auto x = xt::empty_like(z);
    auto y = xt::empty_like(z);

    for (size_t i = 0; i < z.size(); ++i)
    {
        x[i] = _x_interpolator(z[i]);
        y[i] = _y_interpolator(z[i]);
    }

    return xt::eval(xt::stack(xt::xtuple(x, y, z), 0));
}

void ZSpine::add_point(coord_type x, coord_type y, coord_type z)
{
    _x.push_back(x);
    _y.push_back(y);
    _z.push_back(z);

    sort_by_z();
    rebuild_interpolators();
}

}
}
}
}