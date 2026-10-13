// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>
#include <xtensor-python/nanobind/pytensor.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/mbesrawdetectiondata.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

#define DOC_MBESRawDetectionData(ARG)                                                              \
    DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, MBESRawDetectionData, ARG)

void init_c_mbesrawdetectiondata(nanobind::module_& m)
{
    namespace nb = nanobind;
    using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

    nb::class_<MBESRawDetectionData, I_MBESRawDetectionInfos>(
        m,
        "MBESRawDetectionData",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, MBESRawDetectionData))

        .def(nb::init<>(), DOC_MBESRawDetectionData(MBESRawDetectionData))
        .def("__eq__", &MBESRawDetectionData::operator==, nb::arg("other"))

        // --- setters ---
        .def("set_ref_pingtime",
             &MBESRawDetectionData::set_ref_pingtime,
             DOC_MBESRawDetectionData(set_ref_pingtime),
             nb::arg("value"))
        .def("set_ref_pose",
             &MBESRawDetectionData::set_ref_pose,
             DOC_MBESRawDetectionData(set_ref_pose),
             nb::arg("value"))
        .def("set_surface_sound_speed",
             &MBESRawDetectionData::set_surface_sound_speed,
             DOC_MBESRawDetectionData(set_surface_sound_speed),
             nb::arg("value"))
        .def("set_sector_beam_numbers",
             &MBESRawDetectionData::set_sector_beam_numbers,
             DOC_MBESRawDetectionData(set_sector_beam_numbers),
             nb::arg("value"))
        .def("set_sector_tx_time_delays",
             &MBESRawDetectionData::set_sector_tx_time_delays,
             DOC_MBESRawDetectionData(set_sector_tx_time_delays),
             nb::arg("value"))
        .def("set_sector_tx_steering_angles",
             &MBESRawDetectionData::set_sector_tx_steering_angles,
             DOC_MBESRawDetectionData(set_sector_tx_steering_angles),
             nb::arg("value"))
        .def("set_sector_tx_offsets",
             &MBESRawDetectionData::set_sector_tx_offsets,
             DOC_MBESRawDetectionData(set_sector_tx_offsets),
             nb::arg("value"))
        .def("set_rx_offsets",
             &MBESRawDetectionData::set_rx_offsets,
             DOC_MBESRawDetectionData(set_rx_offsets),
             nb::arg("value"))
        .def("set_number_of_soundings",
             &MBESRawDetectionData::set_number_of_soundings,
             DOC_MBESRawDetectionData(set_number_of_soundings),
             nb::arg("value"))
        .def("set_soundings_two_way_travel_times",
             &MBESRawDetectionData::set_soundings_two_way_travel_times,
             DOC_MBESRawDetectionData(set_soundings_two_way_travel_times),
             nb::arg("value"))
        .def("set_soundings_rx_steering_angles",
             &MBESRawDetectionData::set_soundings_rx_steering_angles,
             DOC_MBESRawDetectionData(set_soundings_rx_steering_angles),
             nb::arg("value"))
        .def("set_soundings_sector_numbers",
             &MBESRawDetectionData::set_soundings_sector_numbers,
             DOC_MBESRawDetectionData(set_soundings_sector_numbers),
             nb::arg("value"))

        // ----- nanobind macros -----
        __PYCLASS_DEFAULT_COPY__(MBESRawDetectionData)
            __PYCLASS_DEFAULT_BINARY__(MBESRawDetectionData)
                __PYCLASS_DEFAULT_PRINTING__(MBESRawDetectionData);
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
