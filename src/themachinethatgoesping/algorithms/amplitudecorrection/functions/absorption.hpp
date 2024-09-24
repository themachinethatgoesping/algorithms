// SPDX-FileCopyrightText: 2024 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/absorption.doc.hpp"

#include <gswteos-10.h>
#include <themachinethatgoesping/tools/helper.hpp>

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
inline double its90_to_its68(double T90)
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
inline double calc_sound_velocity(double depth_m,
                                  double temperature_c,
                                  double salinity_psu,
                                  double latitude  = 0.0,
                                  double longitude = 0.0)
{
    double sea_pressure = gsw_p_from_z(-depth_m, latitude, 0., 0.);
    double sa           = gsw_sa_from_sp(salinity_psu, sea_pressure, longitude, latitude);
    return gsw_sound_speed_t_exact(sa, temperature_c, sea_pressure);
}

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
inline double calc_absorption_coefficient_db_m(double frequency_hz,
                                               double depth_m,
                                               double sound_velocity_m_s,
                                               double temperature_c,
                                               double salinity_psu,
                                               double pH = 8)
{
    // Convert frequency_hz to kHz
    frequency_hz = frequency_hz * 0.001;

    // Convert temperature_c from ITS-90 to ITS-68
    temperature_c = its90_to_its68(temperature_c);

    //
    double temperature2    = temperature_c * temperature_c;
    double temperature3    = temperature2 * temperature_c;
    double temperature_c_k = 1 / (temperature_c + 273.);
    double depth2          = depth_m * depth_m;
    double frequency2      = frequency_hz * frequency_hz;

    double A1 = 8.86 * std::pow(10., 0.78 * pH - 5.0) / sound_velocity_m_s;
    double A2 = 21.44 * salinity_psu * (1 + 0.025 * temperature_c) / sound_velocity_m_s;
    // Only for temperature_c
    double A3;
    if (temperature_c <= 20)
        A3 = 0.0004937 - 0.0000259 * temperature_c + 0.000000911 * temperature2 -
             0.000000015 * temperature3;
    else
        A3 = 0.0003964 - 0.00001146 * temperature_c + 0.000000145 * temperature2 -
             0.00000000065 * temperature3;

    double f1 = 2.8 * std::sqrt(salinity_psu / 35.) * std::pow(10., 4. - (1245. * temperature_c_k));

    double f2 = 8.17 * std::pow(10., 8 - (1990. * temperature_c_k));
    f2 /= (1. + 0.0018 * (salinity_psu - 35.));

    double P2 = 1 - 0.000137 * depth_m + 0.0000000062 * depth2;
    double P3 = 1 - 0.0000383 * depth_m + 0.00000000049 * depth2;

    double absorption = 0;

    absorption += A1 * f1 / (f1 * f1 + frequency2);
    absorption += A2 * P2 * f2 / (f2 * f2 + frequency2);
    absorption += A3 * P3;
    absorption *= frequency2;

    // Convert dB/km to dB/m
    return absorption * 0.001;
}

}
}
}
}