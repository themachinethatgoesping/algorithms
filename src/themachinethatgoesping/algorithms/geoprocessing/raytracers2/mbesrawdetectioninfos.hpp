// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// BeamTrace — polyline of a single ray through a layered sound velocity profile
// -----------------------------------------------------------------------------
// Stores the path-defining points of one traced beam in the athwartships y-z
// plane (x/forward = 0). Point 0 is always the launch point; the remaining
// points are the vertices where the analytic ray path changes segment: every
// sound-velocity-profile layer crossing and every interior turning point, plus
// the final point at the requested two-way travel time.
//
// Frame / sign conventions (matching the geoprocessing xyz convention):
//   * z is depth, positive downwards.
//   * horizontal_offset is the signed athwartships coordinate y, positive
//     starboard.
//   * the incident angle is measured from straight down (+z): 0 deg points
//     down, +-90 deg is horizontal, +-180 deg points up. Following the
//     right-handed rotation about +x (forward), a direction with positive y
//     and positive z has a negative angle (i.e. starboard-down is negative,
//     port-down is positive).
//
// Only the quantities that the trace produces directly are stored (depth,
// horizontal offset, two-way travel time and the cosine of the ray angle).
// The incident angle in degrees and the cumulative along-ray range are derived
// on access, so no work is spent on them unless they are requested. Because
// every stored segment lies inside a single layer it is exactly one circular
// arc, which lets the range be reconstructed exactly from the stored points.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/MBESRawDetectionInfos.doc.hpp"

/* generated doc strings */
#include ".docstrings/beamtrace.doc.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/timeconv.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

#include <themachinethatgoesping/navigation/datastructures/sensorpose.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Provide information on MBES detections per beam per sector
 *        Used e.g. by raytracers
 */
class I_MBESRawDetectionInfos
{
  protected:
    std::string class_name() const override;

  public:
    /// @brief Construct an empty BeamTrace (no points stored).
    I_MBESRawDetectionInfos();
    virtual ~I_MBESRawDetectionInfos() = default;

    /**
     * @brief Ref ping time of the transmission. Equal to tx time for single sector transmission.
     *
     * @return double
     */
    virtual double get_ref_pingtime() const = 0;

    /**
     * @brief Get the reference pose that xyz soundings generated from this transmission should be
     * referenced to. Rotating/translating the xyz soundings by this pose will convert the into the
     * world coordinate system.
     *
     * @return navigation::datastructures::SensorPose
     */
    virtual navigation::datastructures::SensorPose get_ref_pose() const = 0;

    // --------------------------- information per sector ----------------------------
    virtual const xt::xtensor<size_t, 1>& get_sector_beam_numbers() const       = 0;
    virtual const xt::xtensor<float, 1>&  get_sector_tx_time_delays() const     = 0;
    virtual const xt::xtensor<float, 1>&  get_sector_tx_steering_angles() const = 0;
    /**
     * @brief Get number of transmit sectors in this transmission
     *
     * @return size_t
     */
    virtual size_t get_number_of_sectors() const { return get_sector_beam_numbers.size(); }

    virtual const xt::xtensor<double, 1>& get_sector_tx_times() const
    {
        return xt::cast<double>(get_sector_tx_time_delays()) + get_ref_pingtime();
    }

    // --------------------------- transducer offsets -------------------------------
    virtual const std::vector<navigation::datastructures::SensorPose>& get_sector_tx_offsets()
        const                                                                    = 0;
    virtual const navigation::datastructures::SensorPose& get_rx_offsets() const = 0;

    // --------------------------- information per sounding -------------------------
    virtual const xt::xtensor<float, 1>&  get_sounding_two_way_travel_times() const = 0;
    virtual const xt::xtensor<float, 1>&  get_soundings_rx_steering_angles() const  = 0;
    virtual const xt::xtensor<size_t, 1>& get_soundings_sector_numbers() const      = 0;

    virtual const xt::xtensor<double, 1>& get_soundings_rx_times() const
    {
        auto                   sector_tx_times = get_sector_tx_times();
        xt::xtensor<double, 1> soundings_rx_times =
            xt::cast<double>(get_sounding_two_way_travel_times());

        const auto  nsectors            = get_number_of_sectors;
        const auto& sector_beam_numbers = get_sector_beam_numbers;
        for (size_t sn; sn < nsectors; ++sn)
        {
            xt::view(soundings_rx_times, soundings_rx_times.unchecked(sn)) +=
                sector_tx_times.unchecked(sn);
        }

        return soundings_rx_times;
    }

    virtual float get_surface_sound_speed() const;

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            this->class_name(), float_precision, superscript_exponents);

        printer.register_section("Transmission reference") double timestamp = get_ref_pingtime();

        std::string time_str =
            tools::timeconv::unixtime_to_datestring(timestamp, 2, "%d/%m/%Y %H:%M:%S");
        printer.register_value("Time info", time_str, std::to_string(timestamp));

        printer.register_section("Sector information")
            printer.register_value('nsectors', get_number_of_sectors());
        printer.register_container('sector_tx_times', get_sector_tx_times(), 's');
        printer.register_container('sector_tx_time_delays', get_sector_tx_time_delays(), 's');
        printer.register_container(
            'sector_tx_steering_angles', get_sector_tx_steering_angles(), '°');
        printer.register_container('sector_beam_numbers', get_sector_beam_numbers());

        return printer;
    }

  public:
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
