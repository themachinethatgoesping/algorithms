// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 Peter Urban, GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/forwardgridder1d.doc.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "functions/gridfunctions.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace gridding {

/**
 * @brief Simple class to generate 1D grids and interpolate x data onto a grid
 * using simple forward mapping algorithms (e.g. block mean, weighted mean interpolation)
 */
template<std::floating_point t_float>
class ForwardGridder1D
{
  public:
    // -- factory methods --
    /**
     * @brief Create gridder with resolution "res"
     * xmin,xmax are determined to exactly contain the given data vector (sx)
     *
     * @param res x resolution of the grid
     * @param sx array with x data
     * @return ForwardGridder1D object
     */
    template<typename T_vector>
    static ForwardGridder1D from_data(t_float res, const T_vector& sx)
    {
        auto [min_x, max_x] = functions::get_minmax(sx);
        return from_res(res, min_x, max_x);
    }

    /**
     * @brief Create gridder setting xres to "res"
     *
     * @param res x resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @return ForwardGridder1D object
     */
    static ForwardGridder1D from_res(t_float res, t_float min_x, t_float max_x)
    {
        return ForwardGridder1D(res, min_x, max_x);
    }

    /**
     * @brief Initialize forward gridder class using grid parameters.
     *
     * @param xres x resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @param xbase x base position of the grid, by default 0.0
     */
    ForwardGridder1D(t_float xres, t_float min_x, t_float max_x, t_float xbase = 0)
        : _xres(xres)
        , _xbase(xbase)
    {
        // Compute center values of the grid cells that contain min_x and max_x
        _xmin = functions::get_grd_value(min_x, _xbase, _xres);
        _xmax = functions::get_grd_value(max_x, _xbase, _xres);

        // Compute the number of elements from (including) min_x to max_x
        _nx = static_cast<int>((_xmax - _xmin) / _xres) + 1;

        // Compute x borders (extent of the outermost grid cells)
        _border_xmin = _xmin - _xres / 2;
        _border_xmax = _xmax + _xres / 2;
    }

    bool operator==(const ForwardGridder1D& other) const = default;

    /**
     * @brief Create empty grid images
     *
     * @return std::tuple<xt::xtensor<t_float, 1>, xt::xtensor<t_float, 1>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d>
    std::tuple<t_xtensor_1d, t_xtensor_1d> get_empty_grd_images() const
    {
        t_xtensor_1d image_values  = xt::zeros<t_float>({ static_cast<size_t>(_nx) });
        t_xtensor_1d image_weights = xt::zeros<t_float>({ static_cast<size_t>(_nx) });

        return std::make_tuple(image_values, image_weights);
    }

    template<typename T_vector>
    auto group_blocks(const T_vector& sx, const T_vector& s_val) const
    {
        return functions::group_blocks(sx, s_val, _xmin, _xres, _nx);
    }

    /**
     * @brief Interpolate 1D points onto 1d images using block mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values. If empty a new image will be created.
     * @param image_weights Image with weights. If empty a new image will be created.
     * @return std::tuple<xt::xtensor<t_float, 1>, xt::xtensor<t_float, 1>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T_vector>
    std::tuple<t_xtensor_1d, t_xtensor_1d> interpolate_block_mean(const T_vector& sx,
                                                                  const T_vector& s_val) const
    {
        auto image_values_weights = get_empty_grd_images<t_xtensor_1d>();

        interpolate_block_mean_inplace(
            sx, s_val, std::get<0>(image_values_weights), std::get<1>(image_values_weights));

        return image_values_weights;
    }

    /**
     * @brief Interpolate 1D points onto 1d images using block mean interpolation (inplace version)
     *
     * @tparam T_vector
     * @param sx x values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values will be edited inplace
     * @param image_weights Image with weights will be edited inplace
     * @return std::tuple<xt::xtensor<t_float, 1>, xt::xtensor<t_float, 1>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T_vector>
    void interpolate_block_mean_inplace(const T_vector& sx,
                                        const T_vector& s_val,
                                        t_xtensor_1d&   image_values,
                                        t_xtensor_1d&   image_weights) const
    {
        if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx))
            throw std::runtime_error(
                "ERROR: image_values dimensions do not fit ForwardGridder1D dimensions!");
        if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx))
            throw std::runtime_error(
                "ERROR: image_weight dimensions do not fit ForwardGridder1D dimensions!");

        functions::grd_block_mean(sx, s_val, _xmin, _xres, _nx, image_values, image_weights);
    }

    /**
     * @brief Interpolate 1D points onto 1d images using weighted mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param s_val amplitudes / volume backscattering coefficients
     * @return std::tuple<xt::xtensor<t_float, 1>, xt::xtensor<t_float, 1>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T_vector>
    std::tuple<t_xtensor_1d, t_xtensor_1d> interpolate_weighted_mean(const T_vector& sx,
                                                                     const T_vector& s_val) const
    {
        auto image_values_weights = get_empty_grd_images<t_xtensor_1d>();

        interpolate_weighted_mean_inplace(
            sx, s_val, std::get<0>(image_values_weights), std::get<1>(image_values_weights));

        return image_values_weights;
    }

    /**
     * @brief Interpolate 1D points onto 1d images using weighted mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values. If empty a new image will be created.
     * @param image_weights Image with weights. If empty a new image will be created.
     * @return std::tuple<xt::xtensor<t_float, 1>, xt::xtensor<t_float, 1>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_1d t_xtensor_1d, typename T_vector>
    void interpolate_weighted_mean_inplace(const T_vector& sx,
                                           const T_vector& s_val,
                                           t_xtensor_1d&   image_values,
                                           t_xtensor_1d&   image_weights) const
    {
        if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx))
            throw std::runtime_error(
                "ERROR: image_values dimensions do not fit ForwardGridder1D dimensions!");
        if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx))
            throw std::runtime_error(
                "ERROR: image_weight dimensions do not fit ForwardGridder1D dimensions!");

        functions::grd_weighted_mean(sx, s_val, _xmin, _xres, _nx, image_values, image_weights);
    }

    /**
     * @brief Returns the min/max value of a list.
     *
     * @tparam T_vector
     * @param sx 1D array with x positions
     * @return std::tuple<t_float, t_float> (xmin,xmax)
     */
    template<typename T_vector>
    static std::tuple<t_float, t_float> get_minmax(const T_vector& sx)
    {
        return functions::get_minmax(sx);
    }

    // Various utility methods
    auto get_xres() const { return _xres; }
    auto get_xmin() const { return _xmin; }
    auto get_xmax() const { return _xmax; }
    auto get_xbase() const { return _xbase; }
    auto get_nx() const { return _nx; }
    auto get_border_xmin() const { return _border_xmin; }
    auto get_border_xmax() const { return _border_xmax; }

    int get_x_index(t_float x) const { return functions::get_index(x, _xmin, _xres); }

    t_float get_x_index_fraction(t_float x) const
    {
        return functions::get_index_fraction(x, _xmin, _xres);
    }

    t_float get_x_value(int x_index) const
    {
        return functions::get_value(static_cast<t_float>(x_index), _xmin, _xres);
    }

    t_float get_x_grd_value(t_float x) const { return functions::get_grd_value(x, _xmin, _xres); }

    std::vector<t_float> get_extent_x() const { return { _border_xmin, _border_xmax }; }

    std::vector<t_float> get_extent(const std::string& axis = "x") const
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
                default:
                    throw std::invalid_argument(std::string("Invalid axis ") + ax +
                                                ". Only 'x' is allowed.");
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

    // ----- objectprinter -----
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "ForwardGridder1D", float_precision, superscript_exponents);

        printer.register_section("grid parameters");
        printer.register_value("xres", _xres);
        printer.register_value("xbase", _xbase);
        printer.register_value("xmin", _xmin);
        printer.register_value("xmax", _xmax);
        printer.register_value("nx", _nx);

        printer.register_section("grid borders");
        printer.register_value("border_xmin", _border_xmin);
        printer.register_value("border_xmax", _border_xmax);

        return printer;
    }

    // ----- serialization -----
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_xres), sizeof(t_float) * 6 + sizeof(int));
    }

    static ForwardGridder1D from_stream(std::istream& is)
    {
        ForwardGridder1D gridder;
        is.read(reinterpret_cast<char*>(&gridder._xres), sizeof(t_float) * 6 + sizeof(int));
        return gridder;
    }

    // ----- class helper macros -----
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(ForwardGridder1D)

  private:
    ForwardGridder1D() = default;

    // Grid parameters
    t_float _xres;
    t_float _xbase;
    t_float _xmin, _xmax;
    t_float _border_xmin, _border_xmax;
    int     _nx;

    // Helper method
    std::tuple<t_float, t_float, int> _get_min_and_offset() const
    {
        return std::make_tuple(_xmin, _xres, _nx);
    }
};

} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
