// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "i_mbesrawdetectioninfos.hpp"

#include <stdexcept>

#include <fmt/format.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

std::string I_MBESRawDetectionInfos::class_name() const
{
    return "I_MBESRawDetectionInfos";
}

void I_MBESRawDetectionInfos::throw_not_implemented(std::string_view method_name) const
{
    throw std::runtime_error(fmt::format(
        "method '{}' is not implemented for class '{}'", method_name, class_name()));
}

I_MBESRawDetectionInfos::I_MBESRawDetectionInfos()  = default;
I_MBESRawDetectionInfos::~I_MBESRawDetectionInfos() = default;

// --------------------------- information per transmission ----------------------------
double I_MBESRawDetectionInfos::get_ref_pingtime()
{
    throw_not_implemented(__func__);
}

navigation::datastructures::SensorPose I_MBESRawDetectionInfos::get_ref_pose()
{
    throw_not_implemented(__func__);
}

float I_MBESRawDetectionInfos::get_surface_sound_speed()
{
    throw_not_implemented(__func__);
}

// --------------------------- information per sector ----------------------------
xt::xtensor<size_t, 1> I_MBESRawDetectionInfos::get_sector_beam_numbers()
{
    throw_not_implemented(__func__);
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_sector_tx_time_delays()
{
    throw_not_implemented(__func__);
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_sector_tx_steering_angles()
{
    throw_not_implemented(__func__);
}

std::vector<navigation::datastructures::SensorPose> I_MBESRawDetectionInfos::get_sector_tx_offsets()
{
    throw_not_implemented(__func__);
}

navigation::datastructures::SensorPose I_MBESRawDetectionInfos::get_rx_offsets()
{
    throw_not_implemented(__func__);
}

size_t I_MBESRawDetectionInfos::get_number_of_sectors()
{
    return get_sector_beam_numbers().size();
}

xt::xtensor<double, 1> I_MBESRawDetectionInfos::get_sector_tx_times()
{
    const auto   tx_time_delays = get_sector_tx_time_delays();
    const double ref_pingtime   = get_ref_pingtime();

    auto tx_times = xt::xtensor<double, 1>::from_shape({ tx_time_delays.size() });
    for (size_t i = 0; i < tx_times.size(); ++i)
        tx_times.unchecked(i) = double(tx_time_delays.unchecked(i)) + ref_pingtime;

    return tx_times;
}

// --------------------------- information per sounding -------------------------
size_t I_MBESRawDetectionInfos::get_number_of_soundings()
{
    throw_not_implemented(__func__);
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_soundings_two_way_travel_times()
{
    return get_soundings_two_way_travel_times(pingtools::BeamSelection(get_number_of_soundings()));
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_soundings_two_way_travel_times(
    [[maybe_unused]] const pingtools::BeamSelection& bs)
{
    throw_not_implemented(__func__);
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_soundings_rx_steering_angles()
{
    return get_soundings_rx_steering_angles(pingtools::BeamSelection(get_number_of_soundings()));
}

xt::xtensor<float, 1> I_MBESRawDetectionInfos::get_soundings_rx_steering_angles(
    [[maybe_unused]] const pingtools::BeamSelection& bs)
{
    throw_not_implemented(__func__);
}

xt::xtensor<size_t, 1> I_MBESRawDetectionInfos::get_soundings_sector_numbers()
{
    return get_soundings_sector_numbers(pingtools::BeamSelection(get_number_of_soundings()));
}

xt::xtensor<size_t, 1> I_MBESRawDetectionInfos::get_soundings_sector_numbers(
    [[maybe_unused]] const pingtools::BeamSelection& bs)
{
    throw_not_implemented(__func__);
}

xt::xtensor<double, 1> I_MBESRawDetectionInfos::get_soundings_rx_times()
{
    return get_soundings_rx_times(pingtools::BeamSelection(get_number_of_soundings()));
}

xt::xtensor<double, 1> I_MBESRawDetectionInfos::get_soundings_rx_times(
    const pingtools::BeamSelection& bs)
{
    const auto two_way_travel_times = get_soundings_two_way_travel_times(bs);
    const auto sector_numbers       = get_soundings_sector_numbers(bs);
    const auto sector_tx_times      = get_sector_tx_times();

    auto rx_times = xt::xtensor<double, 1>::from_shape({ two_way_travel_times.size() });
    for (size_t i = 0; i < rx_times.size(); ++i)
        rx_times.unchecked(i) = double(two_way_travel_times.unchecked(i)) +
                                sector_tx_times.unchecked(sector_numbers.unchecked(i));

    return rx_times;
}

std::vector<std::vector<size_t>> I_MBESRawDetectionInfos::get_beam_numbers_per_tx_sector()
{
    return get_beam_numbers_per_tx_sector(pingtools::BeamSelection(get_number_of_soundings()));
}

std::vector<std::vector<size_t>> I_MBESRawDetectionInfos::get_beam_numbers_per_tx_sector(
    const pingtools::BeamSelection& bs)
{
    const auto sector_numbers = get_soundings_sector_numbers(bs);

    std::vector<std::vector<size_t>> beam_numbers_per_tx_sector(get_number_of_sectors());
    for (size_t i = 0; i < sector_numbers.size(); ++i)
    {
        const size_t sector_number = sector_numbers.unchecked(i);
        if (sector_number >= beam_numbers_per_tx_sector.size())
            throw std::runtime_error(fmt::format(
                "I_MBESRawDetectionInfos: invalid tx sector number: {}", sector_number));

        beam_numbers_per_tx_sector[sector_number].push_back(i);
    }

    return beam_numbers_per_tx_sector;
}

// --------------------------- feature availability -----------------------------
bool I_MBESRawDetectionInfos::has_ref_pingtime() const { return false; }
bool I_MBESRawDetectionInfos::has_ref_pose() const { return false; }
bool I_MBESRawDetectionInfos::has_surface_sound_speed() const { return false; }
bool I_MBESRawDetectionInfos::has_sector_beam_numbers() const { return false; }
bool I_MBESRawDetectionInfos::has_sector_tx_time_delays() const { return false; }
bool I_MBESRawDetectionInfos::has_sector_tx_steering_angles() const { return false; }
bool I_MBESRawDetectionInfos::has_sector_tx_offsets() const { return false; }
bool I_MBESRawDetectionInfos::has_rx_offsets() const { return false; }
bool I_MBESRawDetectionInfos::has_number_of_soundings() const { return false; }
bool I_MBESRawDetectionInfos::has_soundings_two_way_travel_times() const { return false; }
bool I_MBESRawDetectionInfos::has_soundings_rx_steering_angles() const { return false; }
bool I_MBESRawDetectionInfos::has_soundings_sector_numbers() const { return false; }

// --------------------------- object printer -----------------------------------
tools::classhelper::ObjectPrinter I_MBESRawDetectionInfos::__printer__(
    unsigned int float_precision,
    bool         superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer(
        this->class_name(), float_precision, superscript_exponents);

    printer.register_section("MBES raw detection infos");

    // only calls const has_ functions (const-safe); the raw getters are non-const
    const std::vector<std::pair<std::string, bool (I_MBESRawDetectionInfos::*)() const>> features = {
        { "ref_pingtime", &I_MBESRawDetectionInfos::has_ref_pingtime },
        { "ref_pose", &I_MBESRawDetectionInfos::has_ref_pose },
        { "surface_sound_speed", &I_MBESRawDetectionInfos::has_surface_sound_speed },
        { "sector_beam_numbers", &I_MBESRawDetectionInfos::has_sector_beam_numbers },
        { "sector_tx_time_delays", &I_MBESRawDetectionInfos::has_sector_tx_time_delays },
        { "sector_tx_steering_angles", &I_MBESRawDetectionInfos::has_sector_tx_steering_angles },
        { "sector_tx_offsets", &I_MBESRawDetectionInfos::has_sector_tx_offsets },
        { "rx_offsets", &I_MBESRawDetectionInfos::has_rx_offsets },
        { "number_of_soundings", &I_MBESRawDetectionInfos::has_number_of_soundings },
        { "soundings_two_way_travel_times",
          &I_MBESRawDetectionInfos::has_soundings_two_way_travel_times },
        { "soundings_rx_steering_angles",
          &I_MBESRawDetectionInfos::has_soundings_rx_steering_angles },
        { "soundings_sector_numbers", &I_MBESRawDetectionInfos::has_soundings_sector_numbers }
    };

    std::string available;
    std::string missing;
    for (const auto& [name, func] : features)
    {
        if ((this->*func)())
            available += (available.empty() ? "" : ", ") + name;
        else
            missing += (missing.empty() ? "" : ", ") + name;
    }

    printer.register_string("available", available.empty() ? "(none)" : available);
    printer.register_string("not available", missing.empty() ? "(none)" : missing);

    return printer;
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
