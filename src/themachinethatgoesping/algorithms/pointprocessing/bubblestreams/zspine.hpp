// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
#pragma once

/* generated doc strings */
#include ".docstrings/zspine.doc.hpp"

#include <stdexcept>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/vectorinterpolators/akimainterpolator.hpp>

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
    get_sorted_by_z(std::optional<coord_type> cutoff_z = std::nullopt);

    void sort_by_z();

    void rebuild_interpolators();

  public:
    ZSpine() = default;

    ZSpine(bool is_altitude)
        : _is_altitude(is_altitude)
    {
    }

    ZSpine(std::vector<coord_type> x,
           std::vector<coord_type> y,
           std::vector<coord_type> z,
           bool                    is_altitude = false);

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
    void reset_origin();
    void set_origin(coord_type x, coord_type y, coord_type z);
    void estimate_origin(coord_type bottom_z, coord_type slope_modifier = 0.95);

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

    std::pair<coord_type, coord_type> get_xy(coord_type z) const;

    std::tuple<std::vector<coord_type>, std::vector<coord_type>, std::vector<coord_type>> get_spine_points(const bool with_origin = true) const;

    auto get_spine(size_t n_values, bool with_origin = true) const;

    const auto& get_origin() const { return _origin; }
    bool        get_is_altitude() const { return _is_altitude; }

    // --- get internal informations ---
    const auto& get_x_interpolator() const { return _x_interpolator; }
    const auto& get_y_interpolator() const { return _y_interpolator; }

    void add_point(coord_type x, coord_type y, coord_type z);

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