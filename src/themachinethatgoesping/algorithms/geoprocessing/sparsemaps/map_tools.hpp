// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
// SPDX-FileCopyrightText: 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

/**************************************************************************************************
 *
 * simple helper functions maps
 *
 ***************************************************************************************************/

#pragma once

/* generated doc strings */
#include ".docstrings/map_tools.doc.hpp"

/* std includes */
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace sparsemaps {

//--------------------------------- simple map functions ------------------------------------------
// source: http://stackoverflow.com/questions/2333728/stdmap-default-value
template<template<class, class, class...> class C, typename K, typename V, typename... Args>
V get_from_map_with_default(const C<K, V, Args...>& m, K const& key, const V& defval)
{
    typename C<K, V, Args...>::const_iterator it = m.find(key);
    if (it == m.end())
        return defval;
    return it->second;
}

// return a reference (default value will be inserted on access!
template<template<class, class, class...> class C, typename K, typename V, typename... Args>
V& get_ref_with_default(C<K, V, Args...>& m, K const& key, const V& defval)
{
    typename C<K, V, Args...>::iterator it = m.find(key);
    if (it == m.end())
    {
        m[key] = defval;
        return m[key];
    }
    return it->second;
}

template<template<class, class, class...> class C, typename K, typename V, typename... Args>
void increment_with_default(C<K, V, Args...>& m,
                            K const&          key,
                            const V&          defval,
                            const int         increment = 1)
{
    typename C<K, V, Args...>::const_iterator it = m.find(key);
    if (it != m.end())
        m[key] += increment;
    else
        m[key] = defval + increment;
}

template<template<class, class, class...> class C, typename K, typename V, typename... Args>
void write_map(const C<K, V, Args...>& m, std::ostream& os)
{
    unsigned long num = m.size();
    os.write((char*)&num, sizeof(num));
    for (auto& kv : m)
    {
        os.write((char*)&(kv.first), sizeof(K));
        os.write((char*)&(kv.second), sizeof(V));
        // os.write((char*)&kv,sizeof(kv));
    }
}

template<template<class, class, class...> class C, typename K, typename V, typename... Args>
void read_map(C<K, V, Args...>& m, std::istream& is)
{
    unsigned long num;
    is.read((char*)&num, sizeof(num));
    for (unsigned int i = 0; i < num; ++i)
    {
        K key;
        V val;
        is.read((char*)&key, sizeof(K));
        is.read((char*)&val, sizeof(V));
        m[key] = val;
        // std::pair<char,unsigned int> kv;
        // is.read((char*)&kv,sizeof(kv));
        // m.insert(kv);
    }
}

template<template<class, class, class...> class C, typename K, typename V, typename... Args>
std::string write_map_txt(const C<K, V, Args...>& m,
                          std::string             header  = "",
                          std::string             prefix  = "",
                          std::string             midfix  = ":\t",
                          std::string             postfix = "\n")
{
    std::stringstream sstream;
    sstream << header;
    for (auto& kv : m)
        sstream << prefix << kv.first << midfix << kv.second << postfix;

    return sstream.str();
}

template<template<class, class, class...> class C, typename K, typename V, typename... Args>
std::string write_map_txt_keytostring(const C<K, V, Args...>& m,
                                      std::string             header  = "",
                                      std::string             prefix  = "",
                                      std::string             midfix  = ":\t",
                                      std::string             postfix = "\n")
{
    std::stringstream sstream;
    sstream << header;
    for (auto& kv : m)
        sstream << prefix << std::to_string(kv.first) << midfix << kv.second << postfix;

    return sstream.str();
}

}
}
}
} // namespace
