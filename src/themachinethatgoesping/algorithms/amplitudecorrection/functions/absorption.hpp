// SPDX-FileCopyrightText: 2025 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/absorption.doc.hpp"


namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// --- helper functions ---
/**
 * @brief Convert temperature_c from ITS-68 to ITS-90
 *
 * @param T90
 * @return double
 */
template<typename t_float>
inline t_float its90_to_its68(t_float T90)
{
    // return (T90*T90)/gsw_t90_from_t68(T90);
    return T90 * 1.00024;
}

/**
 * @brief Compute the speed of sound in seawater using the TEOS-10 library
 *
 * @param depth_m
 * @param temperature_c
 * @param salinity_psu
 * @param latitude
 * @param longitude
 * @return double
 */
double calc_sound_velocity(double depth_m,
                          double temperature_c,
                          double salinity_psu,
                          double latitude  = 0.0,
                          double longitude = 0.0);

/**
 * @brief Compute the logarithmic absorption coefficient in dB/m based on Francois and Garrison
 * (1982) [taken from Fisheries Acoustics Theory and Practice, 2nd Edition, Simmonds and MacLennan,
 * 2005]
 *
 * @param frequency_hz // Frequency_hz in Hz
 * @param depth_m     // Depth_m in m
 * @param sound_velocity_m_s // Speed of sound in m/s
 * @param temperature_c  // ITS-90 temperature_c in degrees Celsius
 * @param salinity_psu  // Salinity in PSU
 * @param pH // pH
 * @return double
 */
double calc_absorption_coefficient_db_m(double frequency_hz,
                                       double depth_m,
                                       double sound_velocity_m_s,
                                       double temperature_c,
                                       double salinity_psu,
                                       double pH = 8);

}
}
}
}