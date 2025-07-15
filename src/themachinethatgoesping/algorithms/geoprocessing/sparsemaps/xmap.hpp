// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
// SPDX-FileCopyrightText: 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/xmap.doc.hpp"

#include <iostream>
#include <math.h>
#include <unordered_map>


#include "map_tools.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace sparsemaps {

template<typename gridValueType>
class xMap : public std::unordered_map<uint64_t, gridValueType>
{
    double _xbase = 0;

    double _xres; // steps per 1

    gridValueType _initValue;

  public:
    /* -------------------------------- initializer functions ---------------------------------- */
    xMap() {}
    xMap(double xres, const gridValueType& initValue, double xbase = 0.0)
    {
        reset(xres, initValue, xbase);
    }

    /* -------------------------------- access functions --------------------------------------- */
    uint64_t get_gridPos(double x) const
    {
        uint64_t pos;

        pos = std::llround((x - _xbase) / _xres);

        return pos;
    }

    double get_x(uint64_t gridPos) const
    {
        double x = (_xbase + (_xres) * (double)gridPos);

        return x;
    }

    double get_lower_border_x(uint64_t gridPos) const
    {
        double x = (_xbase + (_xres) * (double)gridPos);

        return x - 0.5 * _xres;
    }
    double get_upper_border_x(uint64_t gridPos) const
    {
        double x = (_xbase + (_xres) * (double)gridPos);

        return x + 0.5 * _xres;
    }

    gridValueType&       operator[](uint64_t x) { return at_gridPos(x); }
    const gridValueType& operator[](uint64_t x) const { return at_gridPos(x); }

    gridValueType& operator()(double x) { return at_x(x); }
    gridValueType& operator()(double x, const gridValueType& defaultValue)
    {
        return at_x(x, defaultValue);
    }

    const gridValueType& operator()(double x) const { return at_x(x); }
    const gridValueType& operator()(double x, const gridValueType& defaultValue) const
    {
        return at_x(x, defaultValue);
    }

    gridValueType& at_x(double x)
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), _initValue);
    }
    gridValueType& at_x(double x, const gridValueType& defaultValue)
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), defaultValue);
    }
    gridValueType& at_gridPos(uint64_t gridPos)
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, _initValue);
    }

    gridValueType& at_gridPos(uint64_t gridPos, const gridValueType& defaultValue)
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, defaultValue);
    }

    const gridValueType& at_x(double x) const
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), _initValue);
    }
    const gridValueType& at_x(double x, const gridValueType& defaultValue) const
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), defaultValue);
    }
    const gridValueType& at_gridPos(uint64_t gridPos) const
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, _initValue);
    }

    const gridValueType& at_gridPos(uint64_t gridPos, const gridValueType& defaultValue) const
    {
        return get_ref_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, defaultValue);
    }

    //    const gridValueType& c_at_x(double x) const
    //    {
    //        return get_ref_with_default(
    //                    (std::unordered_map<int64_t4_t
    //                    int64_t,gridValueType>&)*this,get_gridPos(x),_initValue);
    //    }
    //    const  gridValueType& c_at_x(double x, const gridValueType& defaultValue) const
    //    {
    //        return get_ref_with_default(
    //                    (std::unordered_map<int64_t
    //                    int64_t,gridValueType>&)*this,get_gridPos(x),defaultValue);
    //    }
    //    const gridValueType& c_at_gridPos(uint64_t gridPos) const
    //    {
    //        return get_ref_with_default(
    //                    (std::unordered_map<uint64_t,gridValueType>&)*this,gridPos,_initValue);
    //    }

    //    const gridValueType& c_at_gridPos(uint64_t gridPos, const gridValueType& defaultValue)
    //    const
    //    {
    //        return get_ref_with_default(
    //                    (std::unordered_map<uint64_t,gridValueType>&)*this,gridPos,defaultValue);
    //    }

    gridValueType get_at_x(double x) const
    {
        return get_from_map_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), _initValue);
    }
    gridValueType get_at_x(double x, const gridValueType& defaultValue) const
    {
        return get_from_map_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, get_gridPos(x), defaultValue);
    }
    gridValueType get_at_gridPos(uint64_t gridPos) const
    {
        return get_from_map_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, _initValue);
    }
    gridValueType get_at_gridPos(uint64_t gridPos, const gridValueType& defaultValue) const
    {
        return get_from_map_with_default(
            (std::unordered_map<uint64_t, gridValueType>&)*this, gridPos, defaultValue);
    }

    /* -------------------------------- reset functions ----------------------------------------*/
    void reset(const gridValueType& initValue)
    {
        // reset vector
        this->clear();
        _initValue = initValue;

        if (!(_xres > 0))
            throw(std::runtime_error("ERROR[xmap:reset]: _xres is negative!"));
    }

    void reset(double xres, const gridValueType& initValue, double xbase = 0.0)
    {

        _xres  = xres;
        _xbase = xbase;

        reset(initValue);
    }

    template<typename template_gridValueType>
    void reset(const xMap<template_gridValueType>& template_map, gridValueType initValue)
    {
        _xres  = template_map.get_xres();
        _xbase = template_map.get_xbase();

        reset(initValue);
    }
    void reset(const xMap<gridValueType>& template_map)
    {
        _xres  = template_map.get_xres();
        _xbase = template_map.get_xbase();

        reset(template_map.get_initValue());
    }

    /* -------------------------------- get setup functions ------------------------------------ */

    double               get_xres() const { return _xres; }
    double               get_xbase() const { return _xbase; }
    const gridValueType& get_initValue() const { return _initValue; }

    /* -------------------------------- print functions ---------------------------------------- */

    // print setup
    void print_setup(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xGrid setup --"
           << "\n\t- xres: " << _xres << std::endl;
    }

    // print setup
    void print_grid(std::ostream& os = std::cerr, unsigned int precision = 10) const
    {
        os.precision(precision);

        os << "-- xGrid xes --" << std::endl;
        os << "HELP: Implement me!" << std::endl;
        //        for(unsigned int i = 0; i < _numOfClasses; i++)
        //        {
        //            os << "x " << i << "\t" << get_x(i) << "\t" << at_gridPos(i) << std::endl;
        //        }
    }
};

}}}}
