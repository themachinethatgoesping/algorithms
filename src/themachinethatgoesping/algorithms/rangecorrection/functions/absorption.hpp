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
namespace rangecorrection {
namespace functions {

// --- helper functions ---
/**
 * @brief Convert temperature from ITS-68 to ITS-90
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
 * @brief Compute the logarithmic absorption coefficient in dB/m based on Francois and Garrison
 * (1982) [taken from Fisheries Acoustics Theory and Practice, 2nd Edition, Simmonds and MacLennan,
 * 2005]
 *
 * @param frequency // Frequency in Hz
 * @param depth     // Depth in m
 * @param speedOfSound // Speed of sound in m/s
 * @param temperature  // ITS-90 temperature in degrees Celsius
 * @param salinity_psu  // Salinity in PSU
 * @param pH // pH
 * @return double
 */
double calc_absorption_coefficient_db_m(double frequency,
                                        double depth,
                                        double speedOfSound,
                                        double temperature,
                                        double salinity_psu,
                                        double pH = 8)
{
    // Convert frequency to kHz
    frequency = frequency / 1000.0;

    // Convert temperature from ITS-90 to ITS-68
    temperature = its90_to_its68(temperature);

    //
    double temperature2 = temperature * temperature;
    double temperature3 = temperature2 * temperature;
    double depth2       = depth * depth;
    double frequency2   = frequency2 * frequency2;

    double A1 = 8.86 * std::pow(10., 0.78 * pH - 5.0) / speedOfSound;
    double A2 = 21.44 * salinity_psu * (1 + 0.025 * temperature) / speedOfSound;
    // Only for temperature
    double A3;
    if (temperature <= 20)
        A3 = 0.0004937 - 0.0000259 * temperature + 0.000000911 * temperature2 -
             0.000000015 * temperature3;
    else
        A3 = 0.0003964 - 0.00001146 * temperature + 0.000000145 * temperature2 -
             0.00000000065 * temperature3;

    double f1 =
        2.8 * std::sqrt(salinity_psu / 35.) * std::pow(10., 4. - (1245. / (temperature + 273.)));

    double f2 = 8.17 * std::pow(10., 8 - (1990. / (temperature + 273.)));
    f2 /= (1. + 0.0018 * (salinity_psu - 35.));

    double P2 = 1 - 0.000137 * depth + 0.0000000062 * depth2;
    double P3 = 1 - 0.0000383 * depth + 0.00000000049 * depth2;

    double absorption = 0;

    absorption += A1 * f1 / (f1 * f1 + frequency2);
    absorption += A2 * P2 * f2 / (f2 * f2 + frequency2);
    absorption += A3 * P3;
    absorption *= frequency2;

    // Convert dB/km to dB/m
    return absorption / 1000.0;
}

}
}
}
}