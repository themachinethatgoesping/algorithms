// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// MBESRawDetectionData — concrete, owning snapshot of I_MBESRawDetectionInfos
// -----------------------------------------------------------------------------
// Stores a copy of the raw MBES detection information (per transmission / per
// sector / per sounding) and implements the I_MBESRawDetectionInfos getters from
// the stored data. Provides set_ functions, feature (has_) queries, object
// printing and to/from stream + binary (de)serialization following the ping
// class conventions.
//
// A getter throws if the corresponding data was not set (clear exceptions). This
// is used by I_PingBottom/I_PingWatercolumn::get_mbesrawdetectiondata(), which
// fills an MBESRawDetectionData from a ping and fails if information is missing.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/mbesrawdetectiondata.doc.hpp"

#include <optional>
#include <string>
#include <vector>

#include <themachinethatgoesping/tools/classhelper/stream.hpp>

#include "i_mbesrawdetectioninfos.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Concrete, owning implementation of I_MBESRawDetectionInfos.
 *
 * Stores the raw detection information and returns it from the interface getters.
 * Getters throw if the corresponding data has not been set.
 */
class MBESRawDetectionData : public I_MBESRawDetectionInfos
{
    // --- per transmission ---
    std::optional<double>                                 _ref_pingtime;
    std::optional<navigation::datastructures::SensorPose> _ref_pose;
    std::optional<float>                                  _surface_sound_speed;

    // --- per sector ---
    std::optional<xt::xtensor<size_t, 1>>                              _sector_beam_numbers;
    std::optional<xt::xtensor<float, 1>>                               _sector_tx_time_delays;
    std::optional<xt::xtensor<float, 1>>                               _sector_tx_steering_angles;
    std::optional<std::vector<navigation::datastructures::SensorPose>> _sector_tx_offsets;
    std::optional<navigation::datastructures::SensorPose>              _rx_offsets;

    // --- per sounding ---
    std::optional<size_t>                 _number_of_soundings;
    std::optional<xt::xtensor<float, 1>>  _soundings_two_way_travel_times;
    std::optional<xt::xtensor<float, 1>>  _soundings_rx_steering_angles;
    std::optional<xt::xtensor<size_t, 1>> _soundings_sector_numbers;

  protected:
    std::string class_name() const override { return "MBESRawDetectionData"; }

  public:
    MBESRawDetectionData()           = default;
    ~MBESRawDetectionData() override = default;

    bool operator==(const MBESRawDetectionData& other) const;

    // --------------------------- setters ------------------------------------------
    void set_ref_pingtime(double value) { _ref_pingtime = value; }
    void set_ref_pose(navigation::datastructures::SensorPose value) { _ref_pose = value; }
    void set_surface_sound_speed(float value) { _surface_sound_speed = value; }

    void set_sector_beam_numbers(xt::xtensor<size_t, 1> value)
    {
        _sector_beam_numbers = std::move(value);
    }
    void set_sector_tx_time_delays(xt::xtensor<float, 1> value)
    {
        _sector_tx_time_delays = std::move(value);
    }
    void set_sector_tx_steering_angles(xt::xtensor<float, 1> value)
    {
        _sector_tx_steering_angles = std::move(value);
    }
    void set_sector_tx_offsets(std::vector<navigation::datastructures::SensorPose> value)
    {
        _sector_tx_offsets = std::move(value);
    }
    void set_rx_offsets(navigation::datastructures::SensorPose value) { _rx_offsets = value; }

    void set_number_of_soundings(size_t value) { _number_of_soundings = value; }
    void set_soundings_two_way_travel_times(xt::xtensor<float, 1> value)
    {
        _soundings_two_way_travel_times = std::move(value);
    }
    void set_soundings_rx_steering_angles(xt::xtensor<float, 1> value)
    {
        _soundings_rx_steering_angles = std::move(value);
    }
    void set_soundings_sector_numbers(xt::xtensor<size_t, 1> value)
    {
        _soundings_sector_numbers = std::move(value);
    }

    // --------------------------- interface getters --------------------------------
    // un-hide the no-arg base overloads that delegate to the (overridden) bs-versions
    using I_MBESRawDetectionInfos::get_soundings_rx_steering_angles;
    using I_MBESRawDetectionInfos::get_soundings_sector_numbers;
    using I_MBESRawDetectionInfos::get_soundings_two_way_travel_times;

    double                                 get_ref_pingtime() override;
    navigation::datastructures::SensorPose get_ref_pose() override;
    float                                  get_surface_sound_speed() override;

    xt::xtensor<size_t, 1> get_sector_beam_numbers() override;
    xt::xtensor<float, 1>  get_sector_tx_time_delays() override;
    xt::xtensor<float, 1>  get_sector_tx_steering_angles() override;
    std::vector<navigation::datastructures::SensorPose> get_sector_tx_offsets() override;
    navigation::datastructures::SensorPose              get_rx_offsets() override;

    size_t                get_number_of_soundings() override;
    xt::xtensor<float, 1> get_soundings_two_way_travel_times(
        const pingtools::BeamSelection& bs) override;
    xt::xtensor<float, 1> get_soundings_rx_steering_angles(
        const pingtools::BeamSelection& bs) override;
    xt::xtensor<size_t, 1> get_soundings_sector_numbers(const pingtools::BeamSelection& bs) override;

    // --------------------------- feature queries ----------------------------------
    bool has_ref_pingtime() const override { return _ref_pingtime.has_value(); }
    bool has_ref_pose() const override { return _ref_pose.has_value(); }
    bool has_surface_sound_speed() const override { return _surface_sound_speed.has_value(); }
    bool has_sector_beam_numbers() const override { return _sector_beam_numbers.has_value(); }
    bool has_sector_tx_time_delays() const override { return _sector_tx_time_delays.has_value(); }
    bool has_sector_tx_steering_angles() const override
    {
        return _sector_tx_steering_angles.has_value();
    }
    bool has_sector_tx_offsets() const override { return _sector_tx_offsets.has_value(); }
    bool has_rx_offsets() const override { return _rx_offsets.has_value(); }
    bool has_number_of_soundings() const override { return _number_of_soundings.has_value(); }
    bool has_soundings_two_way_travel_times() const override
    {
        return _soundings_two_way_travel_times.has_value();
    }
    bool has_soundings_rx_steering_angles() const override
    {
        return _soundings_rx_steering_angles.has_value();
    }
    bool has_soundings_sector_numbers() const override
    {
        return _soundings_sector_numbers.has_value();
    }

    // --------------------------- from/to stream -----------------------------------
    static MBESRawDetectionData from_stream(std::istream& is);
    void                        to_stream(std::ostream& os) const;

    // --------------------------- object printer -----------------------------------
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs to_stream and from_stream)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(MBESRawDetectionData)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
