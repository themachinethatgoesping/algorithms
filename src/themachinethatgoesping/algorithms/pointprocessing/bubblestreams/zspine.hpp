// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
#pragma once

/* generated doc strings */
#include ".docstrings/zspine.doc.hpp"

#include <stdexcept>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/vectorinterpolators.hpp>

#include "../functions/segment_in_weighted_quantiles.hpp"
#include "../functions/weighted_median.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pointprocessing {
namespace bubblestreams {

class ZSpine
{
  public:
    using coord_type  = double;
    using t_extr_mode = tools::vectorinterpolators::t_extr_mode;

  private:
    std::vector<coord_type> _x, _y, _z;
    bool                    _is_altitude = false;

    // x,y,z,is_altitude
    std::optional<std::tuple<coord_type, coord_type, coord_type>> _origin;

    tools::vectorinterpolators::AkimaInterpolator<coord_type> _x_interpolator;
    tools::vectorinterpolators::AkimaInterpolator<coord_type> _y_interpolator;

    std::tuple<std::vector<coord_type>, std::vector<coord_type>, std::vector<coord_type>>
    get_sorted_by_z(std::optional<coord_type> cutoff_z = std::nullopt)
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

    void sort_by_z()
    {
        auto [sorted_x, sorted_y, sorted_z] = get_sorted_by_z();
        _x                                  = std::move(sorted_x);
        _y                                  = std::move(sorted_y);
        _z                                  = std::move(sorted_z);
    }

    void rebuild_interpolators()
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

  public:
    ZSpine() = default;

    ZSpine(bool is_altitude)
        : _is_altitude(is_altitude)
    {
    }

    ZSpine(std::vector<coord_type> x,
           std::vector<coord_type> y,
           std::vector<coord_type> z,
           bool                    is_altitude = false)
        : _x(std::move(x))
        , _y(std::move(y))
        , _z(std::move(z))
        , _is_altitude(is_altitude)
    {
        sort_by_z();
        rebuild_interpolators();
    }

    // operators
    bool operator==(const ZSpine& other) const = default;

    // --- factory functions ---
    template<tools::helper::c_xtensor_1d t_xtensor_coord,
             tools::helper::c_xtensor_1d t_xtensor_weight>
    static ZSpine from_point_cloud(const t_xtensor_coord&  x,
                                   const t_xtensor_coord&  y,
                                   const t_xtensor_coord&  z,
                                   const t_xtensor_weight& weights,
                                   const int64_t           n_quantiles,
                                   bool                    is_altitude = false)
    {
        // segment points into z quantiles
        auto segment_indices = functions::segment_in_weighted_quantiles(z, weights, n_quantiles);
        std::vector<coord_type> x_vals(segment_indices.size());
        std::vector<coord_type> y_vals(segment_indices.size());
        std::vector<coord_type> z_vals(segment_indices.size());

        for (size_t i = 0; i < segment_indices.size(); ++i)
        {
            t_xtensor_coord  segment_x = xt::index_view(x, segment_indices[i]);
            t_xtensor_coord  segment_y = xt::index_view(y, segment_indices[i]);
            t_xtensor_coord  segment_z = xt::index_view(z, segment_indices[i]);
            t_xtensor_weight segment_w = xt::index_view(weights, segment_indices[i]);

            auto [x_median, y_median, z_median] =
                functions::weighted_median(segment_x, segment_y, segment_z, segment_w);

            x_vals[i] = x_median;
            y_vals[i] = y_median;
            z_vals[i] = z_median;
        }

        return ZSpine(std::move(x_vals), std::move(y_vals), std::move(z_vals), is_altitude);
    }

    // --- origin functions ---
    void reset_origin()
    {
        _origin.reset();
        rebuild_interpolators();
    }
    void set_origin(coord_type x, coord_type y, coord_type z)
    {
        _origin = std::make_tuple(x, y, z);
        rebuild_interpolators();
    }
    void estimate_origin(coord_type bottom_z, coord_type slope_modifier = 0.95)
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

    // --- get functions ---
    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    auto get_xy_vec(const t_xtensor_coord& z) const
    {
        auto x = xt::empty_like(z);
        auto y = xt::empty_like(z);

        for (size_t i = 0; i < z.size(); ++i)
        {
            x[i] = _x_interpolator(z[i]);
            y[i] = _y_interpolator(z[i]);
        }

        return xt::eval(xt::stack(xt::xtuple(x, y), 1));
    }

    std::pair<coord_type, coord_type> get_xy(coord_type z) const
    {
        return std::make_pair(_x_interpolator(z), _y_interpolator(z));
    }

    auto get_spine_points(const bool with_origin = true) const
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
        return std::make_tuple(std::move(_x), std::move(_y), std::move(_z));
    }

    auto get_spine(size_t n_values, bool with_origin = true) const
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

    const auto& get_origin() const { return _origin; }
    bool        get_is_altitude() const { return _is_altitude; }

    // --- get internal informations ---
    const auto& get_x_interpolator() const { return _x_interpolator; }
    const auto& get_y_interpolator() const { return _y_interpolator; }

    void add_point(coord_type x, coord_type y, coord_type z)
    {
        _x.push_back(x);
        _y.push_back(y);
        _z.push_back(z);

        sort_by_z();
        rebuild_interpolators();
    }

    void add_points(std::vector<coord_type> x, std::vector<coord_type> y, std::vector<coord_type> z)
    {
        _x.insert(_x.end(), x.begin(), x.end());
        _y.insert(_y.end(), y.begin(), y.end());
        _z.insert(_z.end(), z.begin(), z.end());

        sort_by_z();
        rebuild_interpolators();
    }

    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    void displace_points_inplace(t_xtensor_coord&          x,
                                 t_xtensor_coord&          y,
                                 const t_xtensor_coord&    z,
                                 std::optional<coord_type> bottom_z = std::nullopt,
                                 const bool                inverse  = false,
                                 const int                 mp_cores = 1) const
    {
        if (x.size() != y.size() || x.size() != z.size())
            throw std::runtime_error("x, y and z must have the same size");

        if (!(bottom_z.has_value() || _origin.has_value()))
            throw std::runtime_error("Either bottom_z or spine origin must be set!");

        // not using structured bindings here because not yet supported by openmp on clang
        //const auto [origin_x, origin_y] = get_xy(bottom_z.value_or(std::get<2>(*_origin)));
        const auto origin_values = get_xy(bottom_z.value_or(std::get<2>(*_origin)));
        const auto origin_x = origin_values.first;
        const auto origin_y = origin_values.second;

#pragma omp parallel for num_threads(mp_cores)
        for (size_t i = 0; i < z.size(); ++i)
        {
            if (inverse)
            {
                x[i] -= origin_x - _x_interpolator(z[i]);
                y[i] -= origin_y - _y_interpolator(z[i]);
            }
            else
            {
                x[i] += origin_x - _x_interpolator(z[i]);
                y[i] += origin_y - _y_interpolator(z[i]);
            }
        }
    }

    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    std::pair<t_xtensor_coord, t_xtensor_coord> displace_points(
        const t_xtensor_coord&    x,
        const t_xtensor_coord&    y,
        const t_xtensor_coord&    z,
        std::optional<coord_type> bottom_z = std::nullopt,
        const bool                inverse  = false,
        const int                 mp_cores = 1) const
    {
        std::pair<t_xtensor_coord, t_xtensor_coord> xy = { x, y };

        displace_points_inplace(xy.first, xy.second, z, bottom_z, inverse, mp_cores);
        return xy;
    }

    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    t_xtensor_coord displace_points_x(const t_xtensor_coord&    x,
                                      const t_xtensor_coord&    z,
                                      std::optional<coord_type> bottom_z = std::nullopt,
                                      const bool                inverse  = false,
                                      const int                 mp_cores = 1) const
    {
        if (x.size() != z.size())
            throw std::runtime_error("x, and z must have the same size");

        if (!(bottom_z.has_value() || _origin.has_value()))
            throw std::runtime_error("Either bottom_z or spine origin must be set!");

        const auto origin_x = _x_interpolator((bottom_z.value_or(std::get<2>(*_origin))));

        t_xtensor_coord x_r = x;

#pragma omp parallel for num_threads(mp_cores)
        for (size_t i = 0; i < z.size(); ++i)
        {
            if (inverse)
                x_r[i] -= origin_x - _x_interpolator(z[i]);
            else
                x_r[i] += origin_x - _x_interpolator(z[i]);
        }

        return x_r;
    }

    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    t_xtensor_coord displace_points_y(const t_xtensor_coord&    y,
                                      const t_xtensor_coord&    z,
                                      std::optional<coord_type> bottom_z = std::nullopt,
                                      const bool                inverse  = false,
                                      const int                 mp_cores = 1) const
    {
        if (y.size() != z.size())
            throw std::runtime_error("y, and z must have the same size");

        if (!(bottom_z.has_value() || _origin.has_value()))
            throw std::runtime_error("Either bottom_z or spine origin must be set!");

        const auto origin_y = _y_interpolator((bottom_z.value_or(std::get<2>(*_origin))));

        auto y_r = xt::empty_like(y);

#pragma omp parallel for num_threads(mp_cores)
        for (size_t i = 0; i < z.size(); ++i)
        {
            if (inverse)
                y_r[i] = y[i] - origin_y + _y_interpolator(z[i]);
            else
                y_r[i] = y[i] + origin_y - _y_interpolator(z[i]);
        }

        return y_r;
    }

  public:
    // ----- file I/O -----
    static ZSpine from_stream(std::istream& is)
    {
        ZSpine data;
        size_t n;
        is.read(reinterpret_cast<char*>(&n), sizeof(n));
        data._x.resize(n);
        data._y.resize(n);
        data._z.resize(n);
        is.read(reinterpret_cast<char*>(data._x.data()), n * sizeof(coord_type));
        is.read(reinterpret_cast<char*>(data._y.data()), n * sizeof(coord_type));
        is.read(reinterpret_cast<char*>(data._z.data()), n * sizeof(coord_type));

        is.read(reinterpret_cast<char*>(&data._is_altitude), sizeof(data._is_altitude));
        bool has_origin;
        is.read(reinterpret_cast<char*>(&has_origin), sizeof(has_origin));
        if (has_origin)
        {
            coord_type x, y, z;
            is.read(reinterpret_cast<char*>(&x), sizeof(x));
            is.read(reinterpret_cast<char*>(&y), sizeof(y));
            is.read(reinterpret_cast<char*>(&z), sizeof(z));
            data._origin = std::make_tuple(x, y, z);
        }

        data.rebuild_interpolators();

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        using tools::classhelper::stream::container_to_stream;
        size_t n = _x.size();
        os.write(reinterpret_cast<const char*>(&n), sizeof(n));
        os.write(reinterpret_cast<const char*>(_x.data()), n * sizeof(coord_type));
        os.write(reinterpret_cast<const char*>(_y.data()), n * sizeof(coord_type));
        os.write(reinterpret_cast<const char*>(_z.data()), n * sizeof(coord_type));

        os.write(reinterpret_cast<const char*>(&_is_altitude), sizeof(_is_altitude));
        bool has_origin = _origin.has_value();
        os.write(reinterpret_cast<const char*>(&has_origin), sizeof(has_origin));
        if (has_origin)
        {
            auto [x, y, z] = _origin.value();
            os.write(reinterpret_cast<const char*>(&x), sizeof(x));
            os.write(reinterpret_cast<const char*>(&y), sizeof(y));
            os.write(reinterpret_cast<const char*>(&z), sizeof(z));
        }
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("ZSpine", float_precision, superscript_exponents);

        if (_is_altitude)
            printer.register_string("is_altitude", "true", "z=altitude");
        else
            printer.register_string("is_altitude", "false", "z=depth");

        if (_origin.has_value())
        {
            auto [x, y, z] = _origin.value();
            printer.register_section("Origin");
            printer.register_value("x", x, "m");
            printer.register_value("y", y, "m");
            printer.register_value("z", z, "m");
        }
        else
        {
            printer.register_string("Origin", "not set");
        }

        printer.register_section("Spine");
        printer.register_value("n_segments", _z.size());
        auto [x, y, z] = get_spine_points();
        printer.register_container("x", x, "m");
        printer.register_container("y", y, "m");
        printer.register_container("z", z, "m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(ZSpine)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace bubblestreams
} // namespace pointprocessing
} // namespace algorithms
} // namespace themachinethatgoesping