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
    using coord_type = double;

  private:
    tools::vectorinterpolators::AkimaInterpolator<coord_type> _x_interpolator;
    tools::vectorinterpolators::AkimaInterpolator<coord_type> _y_interpolator;

  public:
    ZSpine() = default;
    ZSpine(std::vector<coord_type> x, std::vector<coord_type> y, std::vector<coord_type> z)
        : _x_interpolator(z, std::move(x))
        , _y_interpolator(std::move(z), std::move(y))
    {
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
                                   const int64_t           n_quantiles)
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

        return ZSpine(x_vals, y_vals, z_vals);
    }

    // --- get functions ---
    template<tools::helper::c_xtensor_1d t_xtensor_coord>
    auto get_xy(const t_xtensor_coord& z) const
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

    auto get_spine_points() const
    {
        const auto& x = _x_interpolator.get_data_Y();
        const auto& y = _y_interpolator.get_data_Y();
        const auto& z = _x_interpolator.get_data_X();

        return std::make_tuple(x, y, z);
    }

    auto get_spine(size_t n_values) const
    {
        auto z = xt::linspace(
            _x_interpolator.get_data_X().front(), _x_interpolator.get_data_X().back(), n_values);

        auto x = xt::empty_like(z);
        auto y = xt::empty_like(z);

        for (size_t i = 0; i < z.size(); ++i)
        {
            x[i] = _x_interpolator(z[i]);
            y[i] = _y_interpolator(z[i]);
        }

        return xt::eval(xt::stack(xt::xtuple(x, y, z), 0));
    }

    // --- get internal informations ---
    const auto& get_x_interpolator() const { return _x_interpolator; }
    const auto& get_y_interpolator() const { return _y_interpolator; }

  public:
    // ----- file I/O -----
    static ZSpine from_stream(std::istream& is)
    {
        ZSpine data;

        data._x_interpolator =
            tools::vectorinterpolators::AkimaInterpolator<coord_type>::from_stream(is);
        data._y_interpolator =
            tools::vectorinterpolators::AkimaInterpolator<coord_type>::from_stream(is);

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        _x_interpolator.to_stream(os);
        _y_interpolator.to_stream(os);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("ZSpine", float_precision, superscript_exponents);

        printer.register_section("Spine");
        printer.register_value("n_segments", _x_interpolator.get_data_X().size());
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