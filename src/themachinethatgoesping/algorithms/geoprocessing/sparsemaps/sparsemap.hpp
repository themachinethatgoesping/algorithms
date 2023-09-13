// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
// SPDX-FileCopyrightText: 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sparsemap.doc.hpp"

#include <iostream>
#include <unordered_map>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace sparsemaps {

template<typename t_GridValue, typename t_Dim>
struct ArrayHash
{
    size_t operator()(std::array<t_GridValue, t_Dim>& key) const
    {
        return std::hash<T>()(std::get<0>(key)) ^ std::hash<U>()(std::get<1>(key)) ^
               std::hash<U>()(std::get<2>(key));
    }
};

template<typename t_GridValue, typename t_Dim>
struct ArrayEqual
{
    bool operator()(const std::array<t_GridValue, t_Dim>& lhs, const std::array<t_GridValue, t_Dim>& rhs) const
    {
        return std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) == std::get<1>(rhs) &&
               std::get<2>(lhs) == std::get<2>(rhs);
    }
};

template<typename t_GridValue, typename t_Dim>
class SparseMap
    : public std::unordered_map<std::array<t_GridValue, t_Dim>,
                                t_GridValue,
                                ArrayHash<uint64_t, uint64_t, uint64_t>,
                                ArrayEqual<uint64_t, uint64_t, uint64_t>>
{
    using t_sparsemap = std::unordered_map<std::tuple<uint64_t, uint64_t, uint64_t>,
                                           t_GridValue,
                                           ArrayHash<uint64_t, uint64_t, uint64_t>,
                                           ArrayEqual<uint64_t, uint64_t, uint64_t>>;

    double _xbase = 0, _ybase = 0, _zbase = 0;
    double _xres, _yres, _zres;

    t_GridValue _initValue;

  public:
    /* -------------------------------- initializer functions ---------------------------------- */
    SparseMap() {}
    SparseMap(double             xres,
              double             yres,
              double             zres,
              const t_GridValue& initValue,
              double             xbase = 0,
              double             ybase = 0,
              double             zbase = 0)
    {
        reset(xres, yres, zres, initValue, xbase, ybase, zbase);
    }

    /* -------------------------------- access functions --------------------------------------- */
    uint64_t get_gridPos_x(double x) const
    {
        uint64_t pos;

        pos = std::llround(((x - _xbase) / _xres));

        return pos;
    }

    uint64_t get_gridPos_y(double y) const
    {
        uint64_t pos;

        pos = std::llround(((y - _ybase) / _yres));

        return pos;
    }

    uint64_t get_gridPos_z(double z) const
    {
        uint64_t pos;

        pos = std::llround(((z - _zbase) / _zres));

        return pos;
    }

    // -- weighted interpolation --
    // fractional indizes
    double get_gridFraction_x(double x) const { return (x - _xbase) / _xres; }

    double get_gridFraction_y(double y) const { return (y - _ybase) / _yres; }

    double get_gridFraction_z(double z) const { return (z - _zbase) / _zres; }

    std::tuple<std::array<uint64_t, 8>,
               std::array<uint64_t, 8>,
               std::array<uint64_t, 8>,
               std::array<double, 8>>
    get_indexWeights(double x, double y, double z) const
    {
        double fraction_index_x = get_gridFraction_x(x);
        double fraction_index_y = get_gridFraction_y(y);
        double fraction_index_z = get_gridFraction_z(z);

        double ifraction_x = std::fabs(std::fmod(fraction_index_x, 1));
        double ifraction_y = std::fabs(std::fmod(fraction_index_y, 1));
        double ifraction_z = std::fabs(std::fmod(fraction_index_z, 1));

        double fraction_x = 1 - ifraction_x;
        double fraction_y = 1 - ifraction_y;
        double fraction_z = 1 - ifraction_z;

        if (fraction_index_x < 0)
            std::swap(fraction_x, ifraction_x);
        if (fraction_index_y < 0)
            std::swap(fraction_y, ifraction_y);
        if (fraction_index_z < 0)
            std::swap(fraction_z, ifraction_z);

        uint64_t ix1 = std::floor(fraction_index_x);
        uint64_t ix2 = std::ceil(fraction_index_x);
        uint64_t iy1 = std::floor(fraction_index_y);
        uint64_t iy2 = std::ceil(fraction_index_y);
        uint64_t iz1 = std::floor(fraction_index_z);
        uint64_t iz2 = std::ceil(fraction_index_z);

        std::array<uint64_t, 8> X = { ix1, ix1, ix1, ix1, ix2, ix2, ix2, ix2 };
        std::array<uint64_t, 8> Y = { iy1, iy1, iy2, iy2, iy1, iy1, iy2, iy2 };
        std::array<uint64_t, 8> Z = { iz1, iz2, iz1, iz2, iz1, iz2, iz1, iz2 };

        double vx   = 1 * fraction_x;
        double vxy  = vx * fraction_y;
        double vxiy = vx * ifraction_y;

        double vix   = 1 * ifraction_x;
        double vixy  = vix * fraction_y;
        double vixiy = vix * ifraction_y;

        std::array<double, 8> WEIGHT = { vxy * fraction_z,   vxy * ifraction_z,  vxiy * fraction_z,
                                         vxiy * ifraction_z, vixy * fraction_z,  vixy * ifraction_z,
                                         vixiy * fraction_z, vixiy * ifraction_z };

        return std::make_tuple(X, Y, Z, WEIGHT);
    }

    std::tuple<uint64_t, uint64_t, uint64_t> get_gridPos_xyz(double x, double y, double z) const
    {
        return std::make_tuple<uint64_t, uint64_t, uint64_t>(
            get_gridPos_x(x), get_gridPos_y(y), get_gridPos_z(z));
    }

    double get_x(uint64_t gridPos_x) const
    {
        double x = (_xbase + (_xres) * (double)gridPos_x);

        return x;
    }

    double get_y(uint64_t gridPos_y) const
    {
        double y = (_ybase + (_yres) * (double)gridPos_y);

        return y;
    }

    double get_z(uint64_t gridPos_z) const
    {
        double z = (_zbase + (_zres) * (double)gridPos_z);

        return z;
    }

    std::tuple<double, double, double> get_xyz(
        std::tuple<uint64_t, uint64_t, uint64_t> gridPos_xyz) const
    {
        return std::make_tuple<double, double, double>(get_x(std::get<0>(gridPos_xyz)),
                                                       get_y(std::get<1>(gridPos_xyz)),
                                                       get_z(std::get<2>(gridPos_xyz)));
    }
    std::tuple<double, double, double> get_xyz(uint64_t gridPos_x,
                                               uint64_t gridPos_y,
                                               uint64_t gridPos_z) const
    {
        return std::make_tuple<double, double, double>(
            get_x(gridPos_x), get_y(gridPos_y), get_z(gridPos_z));
    }

    t_GridValue& at_xyz(double x, double y, double z)
    {
        return get_ref_with_default((t_sparsemap&)*this, get_gridPos_xyz(x, y, z), _initValue);
    }
    t_GridValue& at_xyz(double x, double y, double z, const t_GridValue& _defaultValue)
    {
        return get_ref_with_default((t_sparsemap&)*this, get_gridPos_xyz(x, y, z), _defaultValue);
    }

    t_GridValue& at_gridPos(uint64_t gridPos_x, uint64_t gridPos_y, uint64_t gridPos_z)
    {
        return get_ref_with_default(
            (t_sparsemap&)*this, std::make_tuple(gridPos_x, gridPos_y, gridPos_z), _initValue);
    }
    t_GridValue& at_gridPos(uint64_t           gridPos_x,
                            uint64_t           gridPos_y,
                            uint64_t           gridPos_z,
                            const t_GridValue& _defaultValue)
    {
        return get_ref_with_default(
            (t_sparsemap&)*this, std::make_tuple(gridPos_x, gridPos_y, gridPos_z), _defaultValue);
    }

    t_GridValue& at_gridPos(std::tuple<uint64_t, uint64_t, uint64_t> gridPos_xyz)
    {
        return get_ref_with_default((t_sparsemap&)*this, gridPos_xyz, _initValue);
    }
    t_GridValue& at_gridPos(std::tuple<uint64_t, uint64_t, uint64_t> gridPos_xyz,
                            const t_GridValue&                       _defaultValue)
    {
        return get_ref_with_default((t_sparsemap&)*this, gridPos_xyz, _defaultValue);
    }

    t_GridValue get_at_xyz(double x, double y, double z) const
    {
        return get_with_default((t_sparsemap&)*this, get_gridPos_xyz(x, y, z), _initValue);
    }
    t_GridValue get_at_xyz(double x, double y, double z, const t_GridValue& _defaultValue) const
    {
        return get_with_default((t_sparsemap&)*this, get_gridPos_xyz(x, y, z), _defaultValue);
    }
    t_GridValue get_at_gridPos(uint64_t gridPos_x, uint64_t gridPos_y, uint64_t gridPos_z) const
    {
        return get_ref_with_default(
            (t_sparsemap&)*this, std::make_tuple(gridPos_x, gridPos_y, gridPos_z), _initValue);
    }
    t_GridValue get_at_gridPos(uint64_t           gridPos_x,
                               uint64_t           gridPos_y,
                               uint64_t           gridPos_z,
                               const t_GridValue& _defaultValue) const
    {
        return get_ref_with_default(
            (t_sparsemap&)*this, std::make_tuple(gridPos_x, gridPos_y, gridPos_z), _defaultValue);
    }

    /* -------------------------------- reset functions ----------------------------------------*/
    void reset(const t_GridValue& initValue)
    {
        _initValue = initValue;

        // reset vector
        this->clear();

        if (!(_xres > 0))
            throw(std::runtime_error("ERROR[SparseMap]: _xres is 0 or negative!"));

        if (!(_yres > 0))
            throw(std::runtime_error("ERROR[SparseMap]: _yres is 0 or negative!"));

        if (!(_zres > 0))
            throw(std::runtime_error("ERROR[SparseMap]: _zres is 0 or negative!"));
    }

    void reset(double             xres,
               double             yres,
               double             zres,
               const t_GridValue& initValue,
               double             xbase = 0,
               double             ybase = 0,
               double             zbase = 0)
    {
        // set values
        _yres  = yres;
        _xres  = xres;
        _zres  = zres;
        _ybase = ybase;
        _xbase = xbase;
        _zbase = zbase;

        reset(initValue);
    }

    template<typename template_gridValueType>
    void reset(const SparseMap<template_gridValueType>& template_map, t_GridValue initValue)
    {
        _xres  = template_map.get_xres();
        _yres  = template_map.get_yres();
        _zres  = template_map.get_zres();
        _xbase = template_map.get_xbase();
        _ybase = template_map.get_ybase();
        _zbase = template_map.get_zybase();

        reset(initValue);
    }
    void reset(const SparseMap<t_GridValue>& template_map)
    {
        _xres  = template_map.get_xres();
        _yres  = template_map.get_yres();
        _zres  = template_map.get_zres();
        _xbase = template_map.get_xbase();
        _ybase = template_map.get_ybase();
        _zbase = template_map.get_zbase();

        reset(template_map.get_initValue());
    }

    /* -------------------------------- get setup functions ------------------------------------ */

    double      get_xres() const { return _xres; }
    double      get_yres() const { return _yres; }
    double      get_zres() const { return _zres; }
    double      get_xbase() const { return _xbase; }
    double      get_ybase() const { return _ybase; }
    double      get_zbase() const { return _zbase; }
    t_GridValue get_initValue() const { return _initValue; }

    /* -------------------------------- print functions ---------------------------------------- */

    // print setup
    void print_setup(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xyzGrid setup --"
           << "Don't know what to do. Please implement me" << std::endl;
    }

    // print setup
    void print_grid(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xyzGrid classeses --" << std::endl;
        os << "Don't know what to do. Please implement me" << std::endl;
        //        for(unsigned int x = 0; x < _numOfClasses_x; x++)
        //            for(unsigned int y = 0; y < _numOfClasses_y; y++)
        //            {
        //                os << "\tx y: " << x << " " << y << "\t" << get_x(x) << "\t" << get_y(y)
        //                << "\t" << at_gridPos(x,y) << std::endl;
        //            }
    }
};

}
}
}
}
