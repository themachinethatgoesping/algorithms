// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// I_MBESRawDetectionInfos — common interface for raw MBES detection information
// -----------------------------------------------------------------------------
// Provides a format independent view on the raw (uncorrected) multibeam
// echosounder detection information that a raytracer needs to turn detections
// into georeferenced soundings:
//   * per transmission: reference ping time, reference pose, surface sound speed
//   * per transmit sector: reference beam number, tx time delay, tx steering
//     angle, tx transducer offset, rx transducer offset
//   * per sounding (beam): two way travel time, rx steering angle, tx sector
//     number
//
// All quantities use the geoprocessing conventions (meters, seconds, degrees;
// z positive downwards; angles measured from straight down).
//
// Access pattern (same as I_PingBottom / KMALLPingBottom):
//   * the version that takes a pingtools::BeamSelection is the virtual one that
//     concrete classes override (it reads/returns exactly the selected beams).
//   * the version without a BeamSelection is implemented here and simply calls
//     the virtual version with a selection of all beams.
// The raw per-sector / per-transmission getters (which have no beam-selection
// variant) are virtual and throw by default so concrete classes can implement
// them incrementally. Derived quantities (number of sectors/soundings' absolute
// tx/rx times, beam-numbers-per-sector) are implemented here in terms of the
// raw getters.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/i_mbesrawdetectioninfos.doc.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include <xtensor/containers/xtensor.hpp>

#include "../../pingtools/beamselection.hpp"

#include <themachinethatgoesping/navigation/datastructures/sensorpose.hpp>
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

// IGNORE_DOC:mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_MBESRawDetectionData
class MBESRawDetectionData; // defined in mbesrawdetectiondata.hpp

/**
 * @brief Common interface that provides the raw MBES detection information
 *        (per transmission / per sector / per sounding) required by raytracers.
 *
 * The getters that take a pingtools::BeamSelection are virtual and throw
 * not_implemented by default. Concrete classes (e.g. the ping bottom/watercolumn
 * interfaces or MBESRawDetectionData) override the subset they can provide. The
 * getters without a BeamSelection are implemented in terms of the virtual ones.
 */
class I_MBESRawDetectionInfos
{
  protected:
    /**
     * @brief Name of the (derived) class, used by the object printer.
     * @return std::string
     */
    virtual std::string class_name() const;

    /// @brief Throw a descriptive exception for a raw getter that is not implemented by a derived
    /// class. Uniquely named to avoid clashing with I_PingCommon::not_implemented under multiple
    /// inheritance (I_PingBottom / I_PingWatercolumn).
    [[noreturn]] void throw_not_implemented(std::string_view method_name) const;

  public:
    I_MBESRawDetectionInfos();
    virtual ~I_MBESRawDetectionInfos();

    // --------------------------- information per transmission ----------------------------
    /**
     * @brief Reference ping time of the transmission (unix timestamp in seconds).
     *        Equal to the tx time for a single sector transmission.
     * @return double
     */
    virtual double get_ref_pingtime();

    /**
     * @brief Reference pose that soundings generated from this transmission are referenced to.
     *        Rotating/translating the soundings by this pose converts them into the world
     *        coordinate system.
     * @return navigation::datastructures::SensorPose
     */
    virtual navigation::datastructures::SensorPose get_ref_pose();

    /**
     * @brief Sound speed at the transducer face (m/s), used to convert steering angles to
     *        directions.
     * @return float
     */
    virtual float get_surface_sound_speed();

    // --------------------------- information per sector ----------------------------
    /**
     * @brief Reference beam number for each transmit sector (length: number of sectors).
     * @return xt::xtensor<size_t, 1>
     */
    virtual xt::xtensor<size_t, 1> get_sector_beam_numbers();

    /**
     * @brief Transmit time delay of each sector relative to the reference ping time (s).
     * @return xt::xtensor<float, 1>
     */
    virtual xt::xtensor<float, 1> get_sector_tx_time_delays();

    /**
     * @brief Transmit steering angle of each sector (degrees).
     * @return xt::xtensor<float, 1>
     */
    virtual xt::xtensor<float, 1> get_sector_tx_steering_angles();

    /**
     * @brief Transmit transducer offset/pose for each sector.
     * @return std::vector<navigation::datastructures::SensorPose>
     */
    virtual std::vector<navigation::datastructures::SensorPose> get_sector_tx_offsets();

    /**
     * @brief Receive transducer offset/pose (common to all soundings of this transmission).
     * @return navigation::datastructures::SensorPose
     */
    virtual navigation::datastructures::SensorPose get_rx_offsets();

    /**
     * @brief Number of transmit sectors in this transmission (derived from get_sector_beam_numbers).
     * @return size_t
     */
    size_t get_number_of_sectors();

    /**
     * @brief Absolute transmit time of each sector (derived: ref ping time + tx time delay).
     * @return xt::xtensor<double, 1>
     */
    xt::xtensor<double, 1> get_sector_tx_times();

    // --------------------------- information per sounding -------------------------
    /**
     * @brief Number of soundings (beams) in this transmission.
     * @return size_t
     */
    virtual size_t get_number_of_soundings();

    /**
     * @brief Two way travel time of all soundings (s).
     * @return xt::xtensor<float, 1>
     */
    xt::xtensor<float, 1> get_soundings_two_way_travel_times();
    /**
     * @brief Two way travel time of the selected soundings (s).
     * @param bs beam selection
     * @return xt::xtensor<float, 1>
     */
    virtual xt::xtensor<float, 1> get_soundings_two_way_travel_times(
        const pingtools::BeamSelection& bs);

    /**
     * @brief Receive steering angle of all soundings (degrees).
     * @return xt::xtensor<float, 1>
     */
    xt::xtensor<float, 1> get_soundings_rx_steering_angles();
    /**
     * @brief Receive steering angle of the selected soundings (degrees).
     * @param bs beam selection
     * @return xt::xtensor<float, 1>
     */
    virtual xt::xtensor<float, 1> get_soundings_rx_steering_angles(
        const pingtools::BeamSelection& bs);

    /**
     * @brief Transmit sector number of all soundings (index into the per-sector arrays).
     * @return xt::xtensor<size_t, 1>
     */
    xt::xtensor<size_t, 1> get_soundings_sector_numbers();
    /**
     * @brief Transmit sector number of the selected soundings (index into the per-sector arrays).
     * @param bs beam selection
     * @return xt::xtensor<size_t, 1>
     */
    virtual xt::xtensor<size_t, 1> get_soundings_sector_numbers(const pingtools::BeamSelection& bs);

    /**
     * @brief Absolute receive time of all soundings (derived: sector tx time + two way travel
     *        time).
     * @return xt::xtensor<double, 1>
     */
    xt::xtensor<double, 1> get_soundings_rx_times();
    /**
     * @brief Absolute receive time of the selected soundings (derived).
     * @param bs beam selection
     * @return xt::xtensor<double, 1>
     */
    xt::xtensor<double, 1> get_soundings_rx_times(const pingtools::BeamSelection& bs);

    /**
     * @brief Beam (sounding) indices grouped by their transmit sector number.
     * @return std::vector<std::vector<size_t>> (outer index: sector number)
     */
    std::vector<std::vector<size_t>> get_beam_numbers_per_tx_sector();
    /**
     * @brief Beam (sounding) indices of the selection grouped by their transmit sector number.
     * @param bs beam selection
     * @return std::vector<std::vector<size_t>> (outer index: sector number)
     */
    virtual std::vector<std::vector<size_t>> get_beam_numbers_per_tx_sector(
        const pingtools::BeamSelection& bs);

    // --------------------------- feature availability -----------------------------
    // Return true if the corresponding getter can be called without throwing. Default is false;
    // concrete classes (e.g. MBESRawDetectionData or a ping class that implements the interface)
    // override the ones they provide.
    virtual bool has_ref_pingtime() const;
    virtual bool has_ref_pose() const;
    virtual bool has_surface_sound_speed() const;
    virtual bool has_sector_beam_numbers() const;
    virtual bool has_sector_tx_time_delays() const;
    virtual bool has_sector_tx_steering_angles() const;
    virtual bool has_sector_tx_offsets() const;
    virtual bool has_rx_offsets() const;
    virtual bool has_number_of_soundings() const;
    virtual bool has_soundings_two_way_travel_times() const;
    virtual bool has_soundings_rx_steering_angles() const;
    virtual bool has_soundings_sector_numbers() const;

    // --------------------------- downcast / snapshot ------------------------------
    /**
     * @brief Return a reference to this object as the abstract I_MBESRawDetectionInfos interface.
     *        No data is copied; the (virtual) getters stay connected to the concrete class.
     * @return I_MBESRawDetectionInfos&
     */
    I_MBESRawDetectionInfos& get_mbesrawdetectioninfos() { return *this; }

    /**
     * @brief Copy the raw detection information into an owning MBESRawDetectionData object.
     *        Throws (clear exception) if any required getter is not implemented.
     * @return MBESRawDetectionData
     */
    MBESRawDetectionData get_mbesrawdetectiondata();

    // --------------------------- object printer -----------------------------------
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

  public:
    // -- class helper function macros --
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
