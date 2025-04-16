// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 Peter Urban, GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/forwardgridder2d.doc.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>



#include <xtensor/containers/xtensor.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "functions/gridfunctions.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace gridding {

/**
 * @brief Simple class to generate 2D grids (images) and interpolate xy data onto a grid
 * using simple forward mapping algorithms (e.g. block mean, weighted mean interpolation)
 */
template<std::floating_point t_float>
class ForwardGridder2D
{
  public:
    // -- factory methods --
    /**
     * @brief Create gridder with resolution "res"
     * xmin,xmax,ymin,ymax are determined to exactly contain the given data vectors
     * (sx,sy)
     *
     * @param res x and y resolution of the grid
     * @param sx array with x data
     * @param sy array with y data
     * @return ForwardGridder2D object
     */
    template<typename T_vector>
    static ForwardGridder2D from_data(t_float res, const T_vector& sx, const T_vector& sy)
    {
        auto [min_x, max_x, min_y, max_y] = functions::get_minmax(sx, sy);
        return from_res(res, min_x, max_x, min_y, max_y);
    }

    /**
     * @brief Create gridder setting xres and yres to "res"
     *
     * @param res x and y resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @param min_y smallest y value that must be contained within the grid
     * @param max_y largest y value that must be contained within the grid
     * @return ForwardGridder2D object
     */
    static ForwardGridder2D from_res(t_float res,
                                     t_float min_x,
                                     t_float max_x,
                                     t_float min_y,
                                     t_float max_y)
    {
        return ForwardGridder2D(res, res, min_x, max_x, min_y, max_y);
    }

    /**
     * @brief Initialize forward gridder class using grid parameters.
     *
     * @param xres x resolution of the grid
     * @param yres y resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @param min_y smallest y value that must be contained within the grid
     * @param max_y largest y value that must be contained within the grid
     * @param xbase x base position of the grid, by default 0.0
     * @param ybase y base position of the grid, by default 0.0
     */
    ForwardGridder2D(t_float xres,
                     t_float yres,
                     t_float min_x,
                     t_float max_x,
                     t_float min_y,
                     t_float max_y,
                     t_float xbase = 0,
                     t_float ybase = 0)
        : _xres(xres)
        , _yres(yres)
        , _xbase(xbase)
        , _ybase(ybase)
    {
        // Compute center values of the grid cells that contain min_x, _y and max_x, _y
        _xmin = functions::get_grd_value(min_x, _xbase, _xres);
        _xmax = functions::get_grd_value(max_x, _xbase, _xres);
        _ymin = functions::get_grd_value(min_y, _ybase, _yres);
        _ymax = functions::get_grd_value(max_y, _ybase, _yres);

        // Compute the number of elements from (including) min_x, _y to max_x, _y
        _nx = static_cast<int>((_xmax - _xmin) / _xres) + 1;
        _ny = static_cast<int>((_ymax - _ymin) / _yres) + 1;

        // Compute x,y borders (extent of the outermost grid cells)
        _border_xmin = _xmin - _xres / 2;
        _border_xmax = _xmax + _xres / 2;
        _border_ymin = _ymin - _yres / 2;
        _border_ymax = _ymax + _yres / 2;
    }

    bool operator==(const ForwardGridder2D& other) const = default;

    /**
     * @brief Create empty grid images
     *
     * @return std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_2d t_xtensor_2d>
    std::tuple<t_xtensor_2d, t_xtensor_2d> get_empty_grd_images() const
    {
        t_xtensor_2d image_values =
            xt::zeros<t_float>({ static_cast<size_t>(_nx), static_cast<size_t>(_ny) });
        t_xtensor_2d image_weights =
            xt::zeros<t_float>({ static_cast<size_t>(_nx), static_cast<size_t>(_ny) });

        return std::make_tuple(image_values, image_weights);
    }

    /**
     * @brief Interpolate 2D points onto 2d images using block mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param s_val amplitudes / volume backscattering coefficients
     * @return std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_2d t_xtensor_2d, typename T_vector>
    std::tuple<t_xtensor_2d, t_xtensor_2d> interpolate_block_mean(
        const T_vector& sx,
        const T_vector& sy,
        const T_vector& s_val) const
    {
        auto image_values_weights = get_empty_grd_images<t_xtensor_2d>();

        interpolate_block_mean_inplace(
            sx, sy, s_val, std::get<0>(image_values_weights), std::get<1>(image_values_weights));

        return image_values_weights;
    }

    /**
     * @brief Interpolate 2D points onto 2d images using block mean interpolation (inplace version)
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values will be edited inplace
     * @param image_weights Image with weights will be edited inplace
     */
    template<tools::helper::c_xtensor_2d t_xtensor_2d, typename T_vector>
    void interpolate_block_mean_inplace(const T_vector& sx,
                                        const T_vector& sy,
                                        const T_vector& s_val,
                                        t_xtensor_2d&   image_values,
                                        t_xtensor_2d&   image_weights) const
    {
        if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx) ||
            static_cast<size_t>(image_values.shape()[1]) != static_cast<size_t>(_ny))
            throw std::runtime_error(
                "ERROR: image_values dimensions do not fit ForwardGridder2D dimensions!");
        if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx) ||
            static_cast<size_t>(image_weights.shape()[1]) != static_cast<size_t>(_ny))
            throw std::runtime_error(
                "ERROR: image_weight dimensions do not fit ForwardGridder2D dimensions!");

        functions::grd_block_mean(
            sx, sy, s_val, _xmin, _xres, _nx, _ymin, _yres, _ny, image_values, image_weights);
    }

    /**
     * @brief Interpolate 2D points onto 2d images using weighted mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param s_val amplitudes / volume backscattering coefficients
     * @return std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_2d t_xtensor_2d, typename T_vector>
    std::tuple<t_xtensor_2d, t_xtensor_2d> interpolate_weighted_mean(
        const T_vector& sx,
        const T_vector& sy,
        const T_vector& s_val) const
    {
        auto image_values_weights = get_empty_grd_images<t_xtensor_2d>();

        interpolate_weighted_mean_inplace(
            sx, sy, s_val, std::get<0>(image_values_weights), std::get<1>(image_values_weights));

        return image_values_weights;
    }

    /**
     * @brief Interpolate 2D points onto 2d images using weighted mean interpolation (inplace version)
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values will be edited inplace
     * @param image_weights Image with weights will be edited inplace
     */
    template<tools::helper::c_xtensor_2d t_xtensor_2d, typename T_vector>
    void interpolate_weighted_mean_inplace(const T_vector& sx,
                                           const T_vector& sy,
                                           const T_vector& s_val,
                                           t_xtensor_2d&   image_values,
                                           t_xtensor_2d&   image_weights) const
    {
        if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx) ||
            static_cast<size_t>(image_values.shape()[1]) != static_cast<size_t>(_ny))
            throw std::runtime_error(
                "ERROR: image_values dimensions do not fit ForwardGridder2D dimensions!");
        if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx) ||
            static_cast<size_t>(image_weights.shape()[1]) != static_cast<size_t>(_ny))
            throw std::runtime_error(
                "ERROR: image_weight dimensions do not fit ForwardGridder2D dimensions!");

        functions::grd_weighted_mean(
            sx, sy, s_val, _xmin, _xres, _nx, _ymin, _yres, _ny, image_values, image_weights);
    }

    /**
     * @brief Returns the min/max value of two lists (same size).
     *
     * @tparam T_vector
     * @param sx 1D array with x positions
     * @param sy 1D array with y positions
     * @return std::tuple<t_float, t_float, t_float, t_float> (xmin,xmax,ymin,ymax)
     */
    template<typename T_vector>
    static std::tuple<t_float, t_float, t_float, t_float> get_minmax(const T_vector& sx,
                                                                     const T_vector& sy)
    {
        return functions::get_minmax(sx, sy);
    }

    // Various utility methods
    auto get_xres() const { return _xres; }
    auto get_yres() const { return _yres; }
    auto get_xmin() const { return _xmin; }
    auto get_xmax() const { return _xmax; }
    auto get_ymin() const { return _ymin; }
    auto get_ymax() const { return _ymax; }
    auto get_xbase() const { return _xbase; }
    auto get_ybase() const { return _ybase; }
    auto get_nx() const { return _nx; }
    auto get_ny() const { return _ny; }
    auto get_border_xmin() const { return _border_xmin; }
    auto get_border_xmax() const { return _border_xmax; }
    auto get_border_ymin() const { return _border_ymin; }
    auto get_border_ymax() const { return _border_ymax; }

    int get_x_index(t_float x) const { return functions::get_index(x, _xmin, _xres); }

    int get_y_index(t_float y) const { return functions::get_index(y, _ymin, _yres); }

    t_float get_x_index_fraction(t_float x) const
    {
        return functions::get_index_fraction(x, _xmin, _xres);
    }

    t_float get_y_index_fraction(t_float y) const
    {
        return functions::get_index_fraction(y, _ymin, _yres);
    }

    t_float get_x_value(int x_index) const
    {
        return functions::get_value(static_cast<t_float>(x_index), _xmin, _xres);
    }

    t_float get_y_value(int y_index) const
    {
        return functions::get_value(static_cast<t_float>(y_index), _ymin, _yres);
    }

    t_float get_x_grd_value(t_float x) const { return functions::get_grd_value(x, _xmin, _xres); }

    t_float get_y_grd_value(t_float y) const { return functions::get_grd_value(y, _ymin, _yres); }

    std::vector<t_float> get_extent_x() const { return { _border_xmin, _border_xmax }; }

    std::vector<t_float> get_extent_y() const { return { _border_ymin, _border_ymax }; }

    std::vector<t_float> get_extent(const std::string& axis = "xy") const
    {
        std::vector<t_float> extent;

        for (char ax : axis)
        {
            switch (ax)
            {
                case 'x': {
                    auto x_extent = get_extent_x();
                    extent.insert(extent.end(), x_extent.begin(), x_extent.end());
                    break;
                }
                case 'y': {
                    auto y_extent = get_extent_y();
                    extent.insert(extent.end(), y_extent.begin(), y_extent.end());
                    break;
                }
                default:
                    throw std::invalid_argument(std::string("Invalid axis ") + ax +
                                                ". Only 'x' and 'y' are allowed.");
            }
        }

        return extent;
    }

    std::vector<t_float> get_x_coordinates() const
    {
        std::vector<t_float> coordinates;
        coordinates.reserve(_nx);

        for (int i = 0; i < _nx; ++i)
        {
            coordinates.push_back(get_x_value(i));
        }

        return coordinates;
    }

    std::vector<t_float> get_y_coordinates() const
    {
        std::vector<t_float> coordinates;
        coordinates.reserve(_ny);

        for (int i = 0; i < _ny; ++i)
        {
            coordinates.push_back(get_y_value(i));
        }

        return coordinates;
    }

    // ----- objectprinter -----
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "ForwardGridder2D", float_precision, superscript_exponents);

        printer.register_section("grid parameters");
        printer.register_value("xres", _xres);
        printer.register_value("yres", _yres);
        printer.register_value("xbase", _xbase);
        printer.register_value("ybase", _ybase);
        printer.register_value("xmin", _xmin);
        printer.register_value("xmax", _xmax);
        printer.register_value("ymin", _ymin);
        printer.register_value("ymax", _ymax);
        printer.register_value("nx", _nx);
        printer.register_value("ny", _ny);

        printer.register_section("grid borders");
        printer.register_value("border_xmin", _border_xmin);
        printer.register_value("border_xmax", _border_xmax);
        printer.register_value("border_ymin", _border_ymin);
        printer.register_value("border_ymax", _border_ymax);

        return printer;
    }

    // ----- serialization -----
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_xres), sizeof(t_float) * 12 + sizeof(int) * 2);
    }

    static ForwardGridder2D from_stream(std::istream& is)
    {
        ForwardGridder2D gridder;
        is.read(reinterpret_cast<char*>(&gridder._xres), sizeof(t_float) * 12 + sizeof(int) * 2);
        return gridder;
    }

    // ----- class helper macros -----
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(ForwardGridder2D)

  private:
    ForwardGridder2D() = default;

    // Grid parameters
    t_float _xres, _yres;
    t_float _xbase, _ybase;
    t_float _xmin, _xmax, _ymin, _ymax;
    t_float _border_xmin, _border_xmax, _border_ymin, _border_ymax;
    int     _nx, _ny;

    // Helper method
    std::tuple<t_float, t_float, int, t_float, t_float, int> _get_min_and_offset() const
    {
        return std::make_tuple(_xmin, _xres, _nx, _ymin, _yres, _ny);
    }
};

} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
