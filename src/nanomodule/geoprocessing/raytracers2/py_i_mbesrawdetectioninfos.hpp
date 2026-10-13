// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/mbesrawdetectiondata.hpp"
#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/i_mbesrawdetectioninfos.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

#define DOC_I_MBESRawDetectionInfos(ARG)                                                           \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, I_MBESRawDetectionInfos, ARG)

namespace py_pingtools = themachinethatgoesping::algorithms::pingtools;

/**
 * @brief Add the I_MBESRawDetectionInfos interface functions to a nanobind class.
 *        nanobind does not support binding multiple C++ bases, so classes that inherit the
 *        interface alongside another nanobind base (e.g. I_PingBottom) call this helper to expose
 *        the interface functions directly on their python class.
 */
template<typename T_BaseClass, typename T_PyClass>
void add_I_MBESRawDetectionInfos_interface(T_PyClass& cls)
{
    namespace nb = nanobind;

    // --- per transmission ---
    cls.def("get_ref_pingtime",
            &T_BaseClass::get_ref_pingtime,
            DOC_I_MBESRawDetectionInfos(get_ref_pingtime));
    cls.def("get_ref_pose", &T_BaseClass::get_ref_pose, DOC_I_MBESRawDetectionInfos(get_ref_pose));
    cls.def("get_surface_sound_speed",
            &T_BaseClass::get_surface_sound_speed,
            DOC_I_MBESRawDetectionInfos(get_surface_sound_speed));

    // --- per sector ---
    cls.def("get_sector_beam_numbers",
            &T_BaseClass::get_sector_beam_numbers,
            DOC_I_MBESRawDetectionInfos(get_sector_beam_numbers));
    cls.def("get_sector_tx_time_delays",
            &T_BaseClass::get_sector_tx_time_delays,
            DOC_I_MBESRawDetectionInfos(get_sector_tx_time_delays));
    cls.def("get_sector_tx_steering_angles",
            &T_BaseClass::get_sector_tx_steering_angles,
            DOC_I_MBESRawDetectionInfos(get_sector_tx_steering_angles));
    cls.def("get_sector_tx_offsets",
            &T_BaseClass::get_sector_tx_offsets,
            DOC_I_MBESRawDetectionInfos(get_sector_tx_offsets));
    cls.def("get_rx_offsets",
            &T_BaseClass::get_rx_offsets,
            DOC_I_MBESRawDetectionInfos(get_rx_offsets));
    cls.def("get_number_of_sectors",
            &T_BaseClass::get_number_of_sectors,
            DOC_I_MBESRawDetectionInfos(get_number_of_sectors));
    cls.def("get_sector_tx_times",
            &T_BaseClass::get_sector_tx_times,
            DOC_I_MBESRawDetectionInfos(get_sector_tx_times));

    // --- per sounding ---
    cls.def("get_number_of_soundings",
            &T_BaseClass::get_number_of_soundings,
            DOC_I_MBESRawDetectionInfos(get_number_of_soundings));

    cls.def("get_soundings_two_way_travel_times",
            nb::overload_cast<>(&T_BaseClass::get_soundings_two_way_travel_times),
            DOC_I_MBESRawDetectionInfos(get_soundings_two_way_travel_times));
    cls.def("get_soundings_two_way_travel_times",
            nb::overload_cast<const py_pingtools::BeamSelection&>(
                &T_BaseClass::get_soundings_two_way_travel_times),
            DOC_I_MBESRawDetectionInfos(get_soundings_two_way_travel_times_2),
            nb::arg("beam_selection"));

    cls.def("get_soundings_rx_steering_angles",
            nb::overload_cast<>(&T_BaseClass::get_soundings_rx_steering_angles),
            DOC_I_MBESRawDetectionInfos(get_soundings_rx_steering_angles));
    cls.def("get_soundings_rx_steering_angles",
            nb::overload_cast<const py_pingtools::BeamSelection&>(
                &T_BaseClass::get_soundings_rx_steering_angles),
            DOC_I_MBESRawDetectionInfos(get_soundings_rx_steering_angles_2),
            nb::arg("beam_selection"));

    cls.def("get_soundings_sector_numbers",
            nb::overload_cast<>(&T_BaseClass::get_soundings_sector_numbers),
            DOC_I_MBESRawDetectionInfos(get_soundings_sector_numbers));
    cls.def("get_soundings_sector_numbers",
            nb::overload_cast<const py_pingtools::BeamSelection&>(
                &T_BaseClass::get_soundings_sector_numbers),
            DOC_I_MBESRawDetectionInfos(get_soundings_sector_numbers_2),
            nb::arg("beam_selection"));

    cls.def("get_soundings_rx_times",
            nb::overload_cast<>(&T_BaseClass::get_soundings_rx_times),
            DOC_I_MBESRawDetectionInfos(get_soundings_rx_times));
    cls.def("get_soundings_rx_times",
            nb::overload_cast<const py_pingtools::BeamSelection&>(
                &T_BaseClass::get_soundings_rx_times),
            DOC_I_MBESRawDetectionInfos(get_soundings_rx_times_2),
            nb::arg("beam_selection"));

    cls.def("get_beam_numbers_per_tx_sector",
            nb::overload_cast<>(&T_BaseClass::get_beam_numbers_per_tx_sector),
            DOC_I_MBESRawDetectionInfos(get_beam_numbers_per_tx_sector));
    cls.def("get_beam_numbers_per_tx_sector",
            nb::overload_cast<const py_pingtools::BeamSelection&>(
                &T_BaseClass::get_beam_numbers_per_tx_sector),
            DOC_I_MBESRawDetectionInfos(get_beam_numbers_per_tx_sector_2),
            nb::arg("beam_selection"));

    // --- feature queries ---
    cls.def("has_ref_pingtime",
            &T_BaseClass::has_ref_pingtime,
            DOC_I_MBESRawDetectionInfos(has_ref_pingtime));
    cls.def("has_ref_pose", &T_BaseClass::has_ref_pose, DOC_I_MBESRawDetectionInfos(has_ref_pose));
    cls.def("has_surface_sound_speed",
            &T_BaseClass::has_surface_sound_speed,
            DOC_I_MBESRawDetectionInfos(has_surface_sound_speed));
    cls.def("has_sector_beam_numbers",
            &T_BaseClass::has_sector_beam_numbers,
            DOC_I_MBESRawDetectionInfos(has_sector_beam_numbers));
    cls.def("has_sector_tx_time_delays",
            &T_BaseClass::has_sector_tx_time_delays,
            DOC_I_MBESRawDetectionInfos(has_sector_tx_time_delays));
    cls.def("has_sector_tx_steering_angles",
            &T_BaseClass::has_sector_tx_steering_angles,
            DOC_I_MBESRawDetectionInfos(has_sector_tx_steering_angles));
    cls.def("has_sector_tx_offsets",
            &T_BaseClass::has_sector_tx_offsets,
            DOC_I_MBESRawDetectionInfos(has_sector_tx_offsets));
    cls.def("has_rx_offsets",
            &T_BaseClass::has_rx_offsets,
            DOC_I_MBESRawDetectionInfos(has_rx_offsets));
    cls.def("has_number_of_soundings",
            &T_BaseClass::has_number_of_soundings,
            DOC_I_MBESRawDetectionInfos(has_number_of_soundings));
    cls.def("has_soundings_two_way_travel_times",
            &T_BaseClass::has_soundings_two_way_travel_times,
            DOC_I_MBESRawDetectionInfos(has_soundings_two_way_travel_times));
    cls.def("has_soundings_rx_steering_angles",
            &T_BaseClass::has_soundings_rx_steering_angles,
            DOC_I_MBESRawDetectionInfos(has_soundings_rx_steering_angles));
    cls.def("has_soundings_sector_numbers",
            &T_BaseClass::has_soundings_sector_numbers,
            DOC_I_MBESRawDetectionInfos(has_soundings_sector_numbers));

    // --- downcast / snapshot ---
    cls.def("get_mbesrawdetectioninfos",
            &T_BaseClass::get_mbesrawdetectioninfos,
            nb::rv_policy::reference_internal,
            DOC_I_MBESRawDetectionInfos(get_mbesrawdetectioninfos));
    cls.def("get_mbesrawdetectiondata",
            &T_BaseClass::get_mbesrawdetectiondata,
            DOC_I_MBESRawDetectionInfos(get_mbesrawdetectiondata));
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
