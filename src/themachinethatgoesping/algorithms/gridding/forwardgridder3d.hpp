// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 Peter Urban, GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/forwardgridder3d.doc.hpp"


#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>

#include "functions/gridfunctions.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace gridding {

/**
 * @brief Simple class to generate 3D grids (images) and interpolate xyz data onto a grid
 * using simple forward mapping algorithms (e.g. block mean, weighted mean interpolation)
 */
template<std::floating_point t_float>
class ForwardGridder3D
{
  public:
    // -- factory methods --
    /**
     * @brief Create gridder with resolution "res"
     * xmin,xmax,ymin,ymax,zmin,zmax are determined to exactly contain the given data vectors
     * (sx,sy,sz)
     *
     * @param res x,y and z resolution of the grid
     * @param sx array with x data
     * @param sy array with y data
     * @param sz array with z data
     * @return ForwardGridder3D object
     */
    template<typename T_vector>
    static ForwardGridder3D from_data(t_float         res,
                                    const T_vector& sx,
                                    const T_vector& sy,
                                    const T_vector& sz)
    {
        auto [min_x, max_x, min_y, max_y, min_z, max_z] = functions::get_minmax(sx, sy, sz);
        return from_res(res, min_x, max_x, min_y, max_y, min_z, max_z);
    }

    /**
     * @brief Create gridder setting xres,yres and zres to "res"
     *
     * @param res x,y and z resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @param min_y smallest y value that must be contained within the grid
     * @param max_y largest y value that must be contained within the grid
     * @param min_z smallest z value that must be contained within the grid
     * @param max_z largest z value that must be contained within the grid
     * @return ForwardGridder3D object
     */
    static ForwardGridder3D from_res(t_float res,
                                   t_float min_x,
                                   t_float max_x,
                                   t_float min_y,
                                   t_float max_y,
                                   t_float min_z,
                                   t_float max_z)
    {
        return ForwardGridder3D(res, res, res, min_x, max_x, min_y, max_y, min_z, max_z);
    }

    /**
     * @brief Initialize forward gridder class using grid parameters.
     *
     * @param xres x resolution of the grid
     * @param yres y resolution of the grid
     * @param zres z resolution of the grid
     * @param min_x smallest x value that must be contained within the grid
     * @param max_x largest x value that must be contained within the grid
     * @param min_y smallest y value that must be contained within the grid
     * @param max_y largest y value that must be contained within the grid
     * @param min_z smallest z value that must be contained within the grid
     * @param max_z largest z value that must be contained within the grid
     * @param xbase x base position of the grid, by default 0.0
     * @param ybase y base position of the grid, by default 0.0
     * @param zbase z base position of the grid, by default 0.0
     */
    ForwardGridder3D(t_float xres,
                   t_float yres,
                   t_float zres,
                   t_float min_x,
                   t_float max_x,
                   t_float min_y,
                   t_float max_y,
                   t_float min_z,
                   t_float max_z,
                   t_float xbase = 0,
                   t_float ybase = 0,
                   t_float zbase = 0)
        : _xres(xres)
        , _yres(yres)
        , _zres(zres)
        , _xbase(xbase)
        , _ybase(ybase)
        , _zbase(zbase)
    {
        // Compute center values of the grid cells that contain min_x, _y, _z and max_x, _y, _z
        _xmin = functions::get_grd_value(min_x, _xbase, _xres);
        _xmax = functions::get_grd_value(max_x, _xbase, _xres);
        _ymin = functions::get_grd_value(min_y, _ybase, _yres);
        _ymax = functions::get_grd_value(max_y, _ybase, _yres);
        _zmin = functions::get_grd_value(min_z, _zbase, _zres);
        _zmax = functions::get_grd_value(max_z, _zbase, _zres);

        // Compute the number of elements from (including) min_x, _y, _z to max_x, _y, _z
        _nx = static_cast<int>((_xmax - _xmin) / _xres) + 1;
        _ny = static_cast<int>((_ymax - _ymin) / _yres) + 1;
        _nz = static_cast<int>((_zmax - _zmin) / _zres) + 1;

        // Compute x,y,z borders (extent of the outermost grid cells)
        _border_xmin = _xmin - _xres / 2;
        _border_xmax = _xmax + _xres / 2;
        _border_ymin = _ymin - _yres / 2;
        _border_ymax = _ymax + _yres / 2;
        _border_zmin = _zmin - _zres / 2;
        _border_zmax = _zmax + _zres / 2;
    }

    bool operator==(const ForwardGridder3D& other) const = default;

    /**
     * @brief Create empty grid images
     *
     * @return std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_3d t_xtensor_3d>
    std::tuple<t_xtensor_3d, t_xtensor_3d> get_empty_grd_images() const
    {
        xt::xtensor<t_float, 3> image_values = xt::zeros<t_float>(
            { static_cast<size_t>(_nx), static_cast<size_t>(_ny), static_cast<size_t>(_nz) });
        xt::xtensor<t_float, 3> image_weights = xt::zeros<t_float>(
            { static_cast<size_t>(_nx), static_cast<size_t>(_ny), static_cast<size_t>(_nz) });

        return std::make_tuple(image_values, image_weights);
    }

    /**
     * @brief Interpolate 3D points onto 3d images using block mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param sz z values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values. If empty a new image will be created.
     * @param image_weights Image with weights. If empty a new image will be created.
     * @return std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_3d t_xtensor_3d, typename T_vector>
    std::tuple<t_xtensor_3d, t_xtensor_3d> interpolate_block_mean(
        const T_vector& sx,
        const T_vector& sy,
        const T_vector& sz,
        const T_vector& s_val,
        t_xtensor_3d    image_values  = t_xtensor_3d(),
        t_xtensor_3d    image_weights = t_xtensor_3d()) const
    {
        if (image_values.size() == 0 || image_weights.size() == 0)
        {
            auto [val, weight] = get_empty_grd_images<t_xtensor_3d>();
            image_values       = val;
            image_weights      = weight;
        }
        else
        {
            if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx) ||
                static_cast<size_t>(image_values.shape()[1]) != static_cast<size_t>(_ny) ||
                static_cast<size_t>(image_values.shape()[2]) != static_cast<size_t>(_nz))
                throw std::runtime_error(
                    "ERROR: image_values dimensions do not fit ForwardGridder3D dimensions!");
            if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx) ||
                static_cast<size_t>(image_weights.shape()[1]) != static_cast<size_t>(_ny) ||
                static_cast<size_t>(image_weights.shape()[2]) != static_cast<size_t>(_nz))
                throw std::runtime_error(
                    "ERROR: image_weight dimensions do not fit ForwardGridder3D dimensions!");
        }

        functions::grd_block_mean(sx,
                                  sy,
                                  sz,
                                  s_val,
                                  _xmin,
                                  _xres,
                                  _nx,
                                  _ymin,
                                  _yres,
                                  _ny,
                                  _zmin,
                                  _zres,
                                  _nz,
                                  image_values,
                                  image_weights);

        return std::make_tuple(image_values, image_weights);
    }

    /**
     * @brief Interpolate 3D points onto 3d images using weighted mean interpolation
     *
     * @tparam T_vector
     * @param sx x values
     * @param sy y values
     * @param sz z values
     * @param s_val amplitudes / volume backscattering coefficients
     * @param image_values Image with values. If empty a new image will be created.
     * @param image_weights Image with weights. If empty a new image will be created.
     * @return std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>> image_values,
     * image_weights
     */
    template<tools::helper::c_xtensor_3d t_xtensor_3d, typename T_vector>
    std::tuple<t_xtensor_3d, t_xtensor_3d> interpolate_weighted_mean(
        const T_vector& sx,
        const T_vector& sy,
        const T_vector& sz,
        const T_vector& s_val,
        t_xtensor_3d    image_values  = t_xtensor_3d(),
        t_xtensor_3d    image_weights = t_xtensor_3d()) const
    {
        if (image_values.size() == 0 || image_weights.size() == 0)
        {
            auto [val, weight] = get_empty_grd_images<t_xtensor_3d>();
            image_values       = val;
            image_weights      = weight;
        }
        else
        {
            if (static_cast<size_t>(image_values.shape()[0]) != static_cast<size_t>(_nx) ||
                static_cast<size_t>(image_values.shape()[1]) != static_cast<size_t>(_ny) ||
                static_cast<size_t>(image_values.shape()[2]) != static_cast<size_t>(_nz))
                throw std::runtime_error(
                    "ERROR: image_values dimensions do not fit ForwardGridder3D dimensions!");
            if (static_cast<size_t>(image_weights.shape()[0]) != static_cast<size_t>(_nx) ||
                static_cast<size_t>(image_weights.shape()[1]) != static_cast<size_t>(_ny) ||
                static_cast<size_t>(image_weights.shape()[2]) != static_cast<size_t>(_nz))
                throw std::runtime_error(
                    "ERROR: image_weight dimensions do not fit ForwardGridder3D dimensions!");
        }

        functions::grd_weighted_mean(sx,
                                     sy,
                                     sz,
                                     s_val,
                                     _xmin,
                                     _xres,
                                     _nx,
                                     _ymin,
                                     _yres,
                                     _ny,
                                     _zmin,
                                     _zres,
                                     _nz,
                                     image_values,
                                     image_weights);

        return std::make_tuple(image_values, image_weights);
    }

    /**
     * @brief Returns the min/max value of three lists (same size).
     *
     * @tparam T_vector
     * @param sx 1D array with x positions
     * @param sy 1D array with y positions
     * @param sz 1D array with z positions
     * @return std::tuple<t_float, t_float, t_float, t_float, t_float, t_float>
     * (xmin,xmax,ymin,ymax,zmin,zmax)
     */
    template<typename T_vector>
    static std::tuple<t_float, t_float, t_float, t_float, t_float, t_float>
    get_minmax(const T_vector& sx, const T_vector& sy, const T_vector& sz)
    {
        return functions::get_minmax(sx, sy, sz);
    }

    // Various utility methods
    auto get_xres() const { return _xres; }
    auto get_yres() const { return _yres; }
    auto get_zres() const { return _zres; }
    auto get_xmin() const { return _xmin; }
    auto get_xmax() const { return _xmax; }
    auto get_ymin() const { return _ymin; }
    auto get_ymax() const { return _ymax; }
    auto get_zmin() const { return _zmin; }
    auto get_zmax() const { return _zmax; }
    auto get_xbase() const { return _xbase; }
    auto get_ybase() const { return _ybase; }
    auto get_zbase() const { return _zbase; }
    auto get_nx() const { return _nx; }
    auto get_ny() const { return _ny; }
    auto get_nz() const { return _nz; }
    auto get_border_xmin() const { return _border_xmin; }
    auto get_border_xmax() const { return _border_xmax; }
    auto get_border_ymin() const { return _border_ymin; }
    auto get_border_ymax() const { return _border_ymax; }
    auto get_border_zmin() const { return _border_zmin; }
    auto get_border_zmax() const { return _border_zmax; }

    int get_x_index(t_float x) const { return functions::get_index(x, _xmin, _xres); }

    int get_y_index(t_float y) const { return functions::get_index(y, _ymin, _yres); }

    int get_z_index(t_float z) const { return functions::get_index(z, _zmin, _zres); }

    t_float get_x_index_fraction(t_float x) const
    {
        return functions::get_index_fraction(x, _xmin, _xres);
    }

    t_float get_y_index_fraction(t_float y) const
    {
        return functions::get_index_fraction(y, _ymin, _yres);
    }

    t_float get_z_index_fraction(t_float z) const
    {
        return functions::get_index_fraction(z, _zmin, _zres);
    }

    t_float get_x_value(int x_index) const
    {
        return functions::get_value(static_cast<t_float>(x_index), _xmin, _xres);
    }

    t_float get_y_value(int y_index) const
    {
        return functions::get_value(static_cast<t_float>(y_index), _ymin, _yres);
    }

    t_float get_z_value(int z_index) const
    {
        return functions::get_value(static_cast<t_float>(z_index), _zmin, _zres);
    }

    t_float get_x_grd_value(t_float x) const { return functions::get_grd_value(x, _xmin, _xres); }

    t_float get_y_grd_value(t_float y) const { return functions::get_grd_value(y, _ymin, _yres); }

    t_float get_z_grd_value(t_float z) const { return functions::get_grd_value(z, _zmin, _zres); }

    std::vector<t_float> get_extent_x() const { return { _border_xmin, _border_xmax }; }

    std::vector<t_float> get_extent_y() const { return { _border_ymin, _border_ymax }; }

    std::vector<t_float> get_extent_z() const { return { _border_zmin, _border_zmax }; }

    std::vector<t_float> get_extent(const std::string& axis = "xyz") const
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
                case 'z': {
                    auto z_extent = get_extent_z();
                    extent.insert(extent.end(), z_extent.begin(), z_extent.end());
                    break;
                }
                default:
                    throw std::invalid_argument(std::string("Invalid axis ") + ax +
                                                ". Only 'x', 'y' and 'z' are allowed.");
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

    std::vector<t_float> get_z_coordinates() const
    {
        std::vector<t_float> coordinates;
        coordinates.reserve(_nz);

        for (int i = 0; i < _nz; ++i)
        {
            coordinates.push_back(get_z_value(i));
        }

        return coordinates;
    }

    // ----- objectprinter -----
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "ForwardGridder3D", float_precision, superscript_exponents);

        printer.register_section("grid parameters");
        printer.register_value("xres", _xres);
        printer.register_value("yres", _yres);
        printer.register_value("zres", _zres);
        printer.register_value("xbase", _xbase);
        printer.register_value("ybase", _ybase);
        printer.register_value("zbase", _zbase);
        printer.register_value("xmin", _xmin);
        printer.register_value("xmax", _xmax);
        printer.register_value("ymin", _ymin);
        printer.register_value("ymax", _ymax);
        printer.register_value("zmin", _zmin);
        printer.register_value("zmax", _zmax);
        printer.register_value("nx", _nx);
        printer.register_value("ny", _ny);
        printer.register_value("nz", _nz);

        printer.register_section("grid borders");
        printer.register_value("border_xmin", _border_xmin);
        printer.register_value("border_xmax", _border_xmax);
        printer.register_value("border_ymin", _border_ymin);
        printer.register_value("border_ymax", _border_ymax);
        printer.register_value("border_zmin", _border_zmin);
        printer.register_value("border_zmax", _border_zmax);

        return printer;
    }

    // ----- serialization -----
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_xres), sizeof(t_float) * 18 + sizeof(int) * 3);
    }

    static ForwardGridder3D from_stream(std::istream& is)
    {
        ForwardGridder3D gridder;
        is.read(reinterpret_cast<char*>(&gridder._xres), sizeof(t_float) * 18 + sizeof(int) * 3);
        return gridder;
    }

    // ----- class helper macros -----
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(ForwardGridder3D)

  private:
    ForwardGridder3D() = default;

    // Grid parameters
    t_float _xres, _yres, _zres;
    t_float _xbase, _ybase, _zbase;
    t_float _xmin, _xmax, _ymin, _ymax, _zmin, _zmax;
    t_float _border_xmin, _border_xmax, _border_ymin, _border_ymax, _border_zmin, _border_zmax;
    int     _nx, _ny, _nz;

    // Helper method
    std::tuple<t_float, t_float, int, t_float, t_float, int, t_float, t_float, int>
    _get_min_and_offset() const
    {
        return std::make_tuple(_xmin, _xres, _nx, _ymin, _yres, _ny, _zmin, _zres, _nz);
    }
};

} // namespace gridding
} // namespace algorithms
} // namespace themachinethatgoesping
