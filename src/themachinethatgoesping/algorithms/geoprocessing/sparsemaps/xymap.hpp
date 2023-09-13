// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
// SPDX-FileCopyrightText: 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/xymap.doc.hpp"

#include <iostream>
#include <unordered_map>


#include "map_tools.hpp"
#include <cmath>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace sparsemaps {

// see
// http://stackoverflow.com/questions/33597127/creating-an-stdunordered-map-with-an-stdpair-as-key
template<class T, typename U>
struct PairHash
{
    size_t operator()(const std::pair<T, U>& key) const
    {
        return std::hash<T>()(key.first) ^ std::hash<U>()(key.second);
    }
};

template<class T, typename U>
struct PairEqual
{
    bool operator()(const std::pair<T, U>& lhs, const std::pair<T, U>& rhs) const
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

template<typename gridValueType>
class xyMap
    : public std::unordered_map<std::pair<uint64_t, uint64_t>,
                                gridValueType,
                                PairHash<uint64_t, uint64_t>,
                                PairEqual<uint64_t, uint64_t>>
{
    typedef std::unordered_map<std::pair<uint64_t, uint64_t>,
                               gridValueType,
                               PairHash<uint64_t, uint64_t>,
                               PairEqual<uint64_t, uint64_t>>
        t_xymap;

    double _xbase = 0, _ybase = 0;

    double _xres, _yres;

  protected:
    gridValueType _initValue;

  public:
    /* -------------------------------- initializer functions ---------------------------------- */
    xyMap() {}
    xyMap(double               xres,
          double               yres,
          const gridValueType& initValue,
          double               xbase = 0,
          double               ybase = 0)
    {
        reset(xres, yres, initValue, xbase, ybase);
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

    double get_gridFraction_x(double x) const { return (x - _xbase) / _xres; }

    double get_gridFraction_y(double y) const { return (y - _ybase) / _yres; }

    std::pair<uint64_t, uint64_t> get_gridPos_xy(double x, double y) const
    {
        return std::make_pair<uint64_t, uint64_t>(get_gridPos_x(x), get_gridPos_y(y));
    }

    std::pair<double, double> get_gridFraction_xy(double x, double y) const
    {
        return std::make_pair<uint64_t, uint64_t>(get_gridFraction_x(x), get_gridFraction_y(y));
    }

    std::tuple<std::array<uint64_t, 4>, std::array<uint64_t, 4>, std::array<double, 4>>
    get_indexWeights(double x, double y) const
    {
        double fraction_index_x = get_gridFraction_x(x);
        double fraction_index_y = get_gridFraction_y(y);

        double ifraction_x = std::fabs(std::fmod(fraction_index_x, 1));
        double ifraction_y = std::fabs(std::fmod(fraction_index_y, 1));

        double fraction_x = 1 - ifraction_x;
        double fraction_y = 1 - ifraction_y;

        if (fraction_index_x < 0)
            std::swap(fraction_x, ifraction_x);
        if (fraction_index_y < 0)
            std::swap(fraction_y, ifraction_y);

        // fraction_z = 1 - ifraction_z

        uint64_t ix1 = std::floor(fraction_index_x);
        uint64_t ix2 = std::ceil(fraction_index_x);
        uint64_t iy1 = std::floor(fraction_index_y);
        uint64_t iy2 = std::ceil(fraction_index_y);
        // iz1 = math.floor(fraction_index_z)
        // iz2 = math.ceil(fraction_index_z)

        std::array<uint64_t, 4> X = { ix1, ix1, ix2, ix2 };
        std::array<uint64_t, 4> Y = { iy1, iy2, iy1, iy2 };
        // Z = np.array([iz1, iz2, iz1, iz2, iz1, iz2, iz1, iz2])

        std::array<double, 4> WEIGHT = { fraction_x * fraction_y,
                                         fraction_x * ifraction_y,
                                         ifraction_x * fraction_y,
                                         ifraction_x * ifraction_y };

        return std::make_tuple(X, Y, WEIGHT);
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

    std::pair<double, double> get_xy(std::pair<uint64_t, uint64_t> gridPos_xy) const
    {
        return std::make_pair<double, double>(get_x(gridPos_xy.first), get_y(gridPos_xy.second));
    }
    std::pair<double, double> get_xy(uint64_t gridPos_x, uint64_t gridPos_y) const
    {
        return std::make_pair<double, double>(get_x(gridPos_x), get_y(gridPos_y));
    }

    gridValueType& at_xy(double x, double y)
    {
        return get_ref_with_default((t_xymap&)*this, get_gridPos_xy(x, y), _initValue);
    }
    gridValueType& at_xy(double x, double y, const gridValueType& _defaultValue)
    {
        return get_ref_with_default((t_xymap&)*this, get_gridPos_xy(x, y), _defaultValue);
    }

    gridValueType& at_gridPos(uint64_t gridPos_x, uint64_t gridPos_y)
    {
        return get_ref_with_default(
            (t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _initValue);
    }
    gridValueType& at_gridPos(uint64_t            gridPos_x,
                              uint64_t            gridPos_y,
                              const gridValueType& _defaultValue)
    {
        return get_ref_with_default(
            (t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _defaultValue);
    }

    gridValueType& at_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy)
    {
        return get_ref_with_default((t_xymap&)*this, gridPos_xy, _initValue);
    }
    gridValueType& at_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy,
                              const gridValueType&            _defaultValue)
    {
        return get_ref_with_default((t_xymap&)*this, gridPos_xy, _defaultValue);
    }

    const gridValueType& at_xy(double x, double y, const gridValueType& _defaultValue) const
    {
        return get_ref_with_default((t_xymap&)*this, get_gridPos_xy(x, y), _defaultValue);
    }

    const gridValueType& at_gridPos(uint64_t gridPos_x, uint64_t gridPos_y) const
    {
        return get_ref_with_default(
            (t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _initValue);
    }
    const gridValueType& at_gridPos(uint64_t            gridPos_x,
                                    uint64_t            gridPos_y,
                                    const gridValueType& _defaultValue) const
    {
        return get_ref_with_default(
            (t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _defaultValue);
    }

    const gridValueType& at_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy) const
    {
        return get_ref_with_default((t_xymap&)*this, gridPos_xy, _initValue);
    }
    const gridValueType& at_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy,
                                    const gridValueType&            _defaultValue) const
    {
        return get_ref_with_default((t_xymap&)*this, gridPos_xy, _defaultValue);
    }

    gridValueType get_at_xy(double x, double y) const
    {
        return get_with_default((t_xymap&)*this, get_gridPos_xy(x, y), _initValue);
    }
    gridValueType get_at_xy(double x, double y, const gridValueType& _defaultValue) const
    {
        return get_with_default((t_xymap&)*this, get_gridPos_xy(x, y), _defaultValue);
    }
    gridValueType get_at_gridPos(uint64_t gridPos_x, uint64_t gridPos_y) const
    {
        return get_with_default((t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _initValue);
    }
    gridValueType get_at_gridPos(uint64_t            gridPos_x,
                                 uint64_t            gridPos_y,
                                 const gridValueType& _defaultValue) const
    {
        return get_with_default(
            (t_xymap&)*this, std::make_pair(gridPos_x, gridPos_y), _defaultValue);
    }

    gridValueType& get_at_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy) const
    {
        return get_with_default((t_xymap&)*this, gridPos_xy, _initValue);
    }
    gridValueType& get_gridPos(std::pair<uint64_t, uint64_t> gridPos_xy,
                               const gridValueType&            _defaultValue) const
    {
        return get_with_default((t_xymap&)*this, gridPos_xy, _defaultValue);
    }

    /* -------------------------------- reset functions ----------------------------------------*/
    void reset(const gridValueType& initValue)
    {
        _initValue = initValue;

        // reset vector
        this->clear();

        if (!(_xres > 0))
            throw(std::runtime_error("ERROR[xymap]: _xres is 0 or negative!"));

        if (!(_yres > 0))
            throw(std::runtime_error("ERROR[xymap]: _yres is 0 or negative!"));
    }

    void reset(double               xres,
               double               yres,
               const gridValueType& initValue,
               double               xbase = 0,
               double               ybase = 0)
    {
        // set values
        _yres  = yres;
        _xres  = xres;
        _ybase = ybase;
        _xbase = xbase;

        reset(initValue);
    }

    template<typename template_gridValueType>
    void reset(const xyMap<template_gridValueType>& template_map, gridValueType initValue)
    {
        _xres  = template_map.get_xres();
        _yres  = template_map.get_yres();
        _xbase = template_map.get_xbase();
        _ybase = template_map.get_ybase();

        reset(initValue);
    }
    void reset(const xyMap<gridValueType>& template_map)
    {
        _xres  = template_map.get_xres();
        _yres  = template_map.get_yres();
        _xbase = template_map.get_xbase();
        _ybase = template_map.get_ybase();

        reset(template_map.get_initValue());
    }

    /* -------------------------------- get setup functions ------------------------------------ */

    double        get_xres() const { return _xres; }
    double        get_yres() const { return _yres; }
    double        get_xbase() const { return _xbase; }
    double        get_ybase() const { return _ybase; }
    gridValueType get_initValue() const { return _initValue; }

    /* -------------------------------- print functions ---------------------------------------- */

    // print setup
    void print_setup(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xyGrid setup --"
           << "Don't know what to do. Please implement me" << std::endl;
    }

    // print setup
    void print_grid(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xyGrid classeses --" << std::endl;
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
