// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "mbesrawdetectiondata.hpp"

#include <stdexcept>

#include <fmt/format.h>

#include <xtensor/core/xmath.hpp>
#include <xtensor/reducers/xnorm.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

namespace {

// ---- optional (de)serialization helpers ----
template<typename t_value>
void write_opt_scalar(std::ostream& os, const std::optional<t_value>& value)
{
    const bool has = value.has_value();
    os.write(reinterpret_cast<const char*>(&has), sizeof(bool));
    if (has)
        os.write(reinterpret_cast<const char*>(&value.value()), sizeof(t_value));
}

template<typename t_value>
std::optional<t_value> read_opt_scalar(std::istream& is)
{
    bool has = false;
    is.read(reinterpret_cast<char*>(&has), sizeof(bool));
    if (!has)
        return std::nullopt;
    t_value value;
    is.read(reinterpret_cast<char*>(&value), sizeof(t_value));
    return value;
}

template<typename t_value>
void write_opt_xtensor(std::ostream& os, const std::optional<xt::xtensor<t_value, 1>>& value)
{
    const bool has = value.has_value();
    os.write(reinterpret_cast<const char*>(&has), sizeof(bool));
    if (has)
    {
        const size_t size = value->size();
        os.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(value->data()), size * sizeof(t_value));
    }
}

template<typename t_value>
std::optional<xt::xtensor<t_value, 1>> read_opt_xtensor(std::istream& is)
{
    bool has = false;
    is.read(reinterpret_cast<char*>(&has), sizeof(bool));
    if (!has)
        return std::nullopt;
    size_t size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    auto value = xt::xtensor<t_value, 1>::from_shape({ size });
    is.read(reinterpret_cast<char*>(value.data()), size * sizeof(t_value));
    return value;
}

void write_opt_pose(std::ostream&                                                os,
                    const std::optional<navigation::datastructures::SensorPose>& value)
{
    const bool has = value.has_value();
    os.write(reinterpret_cast<const char*>(&has), sizeof(bool));
    if (has)
        value->to_stream(os);
}

std::optional<navigation::datastructures::SensorPose> read_opt_pose(std::istream& is)
{
    bool has = false;
    is.read(reinterpret_cast<char*>(&has), sizeof(bool));
    if (!has)
        return std::nullopt;
    return navigation::datastructures::SensorPose::from_stream(is);
}

void write_opt_pose_vector(
    std::ostream&                                                             os,
    const std::optional<std::vector<navigation::datastructures::SensorPose>>& value)
{
    const bool has = value.has_value();
    os.write(reinterpret_cast<const char*>(&has), sizeof(bool));
    if (has)
    {
        const size_t size = value->size();
        os.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        for (const auto& pose : *value)
            pose.to_stream(os);
    }
}

std::optional<std::vector<navigation::datastructures::SensorPose>> read_opt_pose_vector(
    std::istream& is)
{
    bool has = false;
    is.read(reinterpret_cast<char*>(&has), sizeof(bool));
    if (!has)
        return std::nullopt;
    size_t size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    std::vector<navigation::datastructures::SensorPose> poses;
    poses.reserve(size);
    for (size_t i = 0; i < size; ++i)
        poses.push_back(navigation::datastructures::SensorPose::from_stream(is));
    return poses;
}

// ---- equality helpers ----
template<typename t_value>
bool opt_xtensor_equal(const std::optional<xt::xtensor<t_value, 1>>& a,
                       const std::optional<xt::xtensor<t_value, 1>>& b)
{
    if (a.has_value() != b.has_value())
        return false;
    if (!a.has_value())
        return true;
    if (a->size() != b->size())
        return false;
    return xt::all(xt::equal(*a, *b));
}

// ---- beam selection ----
template<typename t_value>
xt::xtensor<t_value, 1> select_soundings(const xt::xtensor<t_value, 1>&  values,
                                         const pingtools::BeamSelection& bs)
{
    const auto& beam_numbers = bs.get_beam_numbers();
    auto        result = xt::xtensor<t_value, 1>::from_shape({ beam_numbers.size() });
    for (size_t i = 0; i < beam_numbers.size(); ++i)
        result.unchecked(i) = values.unchecked(beam_numbers[i]);
    return result;
}

} // namespace

// --------------------------- interface getters --------------------------------
double MBESRawDetectionData::get_ref_pingtime()
{
    if (!_ref_pingtime)
        throw_not_implemented(__func__);
    return *_ref_pingtime;
}

navigation::datastructures::SensorPose MBESRawDetectionData::get_ref_pose()
{
    if (!_ref_pose)
        throw_not_implemented(__func__);
    return *_ref_pose;
}

float MBESRawDetectionData::get_surface_sound_speed()
{
    if (!_surface_sound_speed)
        throw_not_implemented(__func__);
    return *_surface_sound_speed;
}

xt::xtensor<size_t, 1> MBESRawDetectionData::get_sector_beam_numbers()
{
    if (!_sector_beam_numbers)
        throw_not_implemented(__func__);
    return *_sector_beam_numbers;
}

xt::xtensor<float, 1> MBESRawDetectionData::get_sector_tx_time_delays()
{
    if (!_sector_tx_time_delays)
        throw_not_implemented(__func__);
    return *_sector_tx_time_delays;
}

xt::xtensor<float, 1> MBESRawDetectionData::get_sector_tx_steering_angles()
{
    if (!_sector_tx_steering_angles)
        throw_not_implemented(__func__);
    return *_sector_tx_steering_angles;
}

std::vector<navigation::datastructures::SensorPose> MBESRawDetectionData::get_sector_tx_offsets()
{
    if (!_sector_tx_offsets)
        throw_not_implemented(__func__);
    return *_sector_tx_offsets;
}

navigation::datastructures::SensorPose MBESRawDetectionData::get_rx_offsets()
{
    if (!_rx_offsets)
        throw_not_implemented(__func__);
    return *_rx_offsets;
}

size_t MBESRawDetectionData::get_number_of_soundings()
{
    if (!_number_of_soundings)
        throw_not_implemented(__func__);
    return *_number_of_soundings;
}

xt::xtensor<float, 1> MBESRawDetectionData::get_soundings_two_way_travel_times(
    const pingtools::BeamSelection& bs)
{
    if (!_soundings_two_way_travel_times)
        throw_not_implemented(__func__);
    return select_soundings(*_soundings_two_way_travel_times, bs);
}

xt::xtensor<float, 1> MBESRawDetectionData::get_soundings_rx_steering_angles(
    const pingtools::BeamSelection& bs)
{
    if (!_soundings_rx_steering_angles)
        throw_not_implemented(__func__);
    return select_soundings(*_soundings_rx_steering_angles, bs);
}

xt::xtensor<size_t, 1> MBESRawDetectionData::get_soundings_sector_numbers(
    const pingtools::BeamSelection& bs)
{
    if (!_soundings_sector_numbers)
        throw_not_implemented(__func__);
    return select_soundings(*_soundings_sector_numbers, bs);
}

// --------------------------- equality -----------------------------------------
bool MBESRawDetectionData::operator==(const MBESRawDetectionData& other) const
{
    return _ref_pingtime == other._ref_pingtime && _ref_pose == other._ref_pose &&
           _surface_sound_speed == other._surface_sound_speed &&
           opt_xtensor_equal(_sector_beam_numbers, other._sector_beam_numbers) &&
           opt_xtensor_equal(_sector_tx_time_delays, other._sector_tx_time_delays) &&
           opt_xtensor_equal(_sector_tx_steering_angles, other._sector_tx_steering_angles) &&
           _sector_tx_offsets == other._sector_tx_offsets && _rx_offsets == other._rx_offsets &&
           _number_of_soundings == other._number_of_soundings &&
           opt_xtensor_equal(_soundings_two_way_travel_times,
                             other._soundings_two_way_travel_times) &&
           opt_xtensor_equal(_soundings_rx_steering_angles, other._soundings_rx_steering_angles) &&
           opt_xtensor_equal(_soundings_sector_numbers, other._soundings_sector_numbers);
}

// --------------------------- from/to stream -----------------------------------
MBESRawDetectionData MBESRawDetectionData::from_stream(std::istream& is)
{
    MBESRawDetectionData data;
    data._ref_pingtime                   = read_opt_scalar<double>(is);
    data._ref_pose                       = read_opt_pose(is);
    data._surface_sound_speed            = read_opt_scalar<float>(is);
    data._sector_beam_numbers            = read_opt_xtensor<size_t>(is);
    data._sector_tx_time_delays          = read_opt_xtensor<float>(is);
    data._sector_tx_steering_angles      = read_opt_xtensor<float>(is);
    data._sector_tx_offsets              = read_opt_pose_vector(is);
    data._rx_offsets                     = read_opt_pose(is);
    data._number_of_soundings            = read_opt_scalar<size_t>(is);
    data._soundings_two_way_travel_times = read_opt_xtensor<float>(is);
    data._soundings_rx_steering_angles   = read_opt_xtensor<float>(is);
    data._soundings_sector_numbers       = read_opt_xtensor<size_t>(is);
    return data;
}

void MBESRawDetectionData::to_stream(std::ostream& os) const
{
    write_opt_scalar(os, _ref_pingtime);
    write_opt_pose(os, _ref_pose);
    write_opt_scalar(os, _surface_sound_speed);
    write_opt_xtensor(os, _sector_beam_numbers);
    write_opt_xtensor(os, _sector_tx_time_delays);
    write_opt_xtensor(os, _sector_tx_steering_angles);
    write_opt_pose_vector(os, _sector_tx_offsets);
    write_opt_pose(os, _rx_offsets);
    write_opt_scalar(os, _number_of_soundings);
    write_opt_xtensor(os, _soundings_two_way_travel_times);
    write_opt_xtensor(os, _soundings_rx_steering_angles);
    write_opt_xtensor(os, _soundings_sector_numbers);
}

// --------------------------- object printer -----------------------------------
tools::classhelper::ObjectPrinter MBESRawDetectionData::__printer__(
    unsigned int float_precision,
    bool         superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer(
        this->class_name(), float_precision, superscript_exponents);

    printer.register_section("Transmission reference");
    if (_ref_pingtime)
        printer.register_value("ref_pingtime", *_ref_pingtime, "s");
    if (_surface_sound_speed)
        printer.register_value("surface_sound_speed", *_surface_sound_speed, "m/s");

    printer.register_section("Sectors");
    if (_sector_beam_numbers)
        printer.register_value("number_of_sectors", _sector_beam_numbers->size());
    if (_sector_tx_time_delays)
        printer.register_container(
            "sector_tx_time_delays",
            std::vector<float>(_sector_tx_time_delays->begin(), _sector_tx_time_delays->end()),
            "s");
    if (_sector_tx_steering_angles)
        printer.register_container(
            "sector_tx_steering_angles",
            std::vector<float>(_sector_tx_steering_angles->begin(),
                               _sector_tx_steering_angles->end()),
            "°");

    printer.register_section("Soundings");
    if (_number_of_soundings)
        printer.register_value("number_of_soundings", *_number_of_soundings);

    return printer;
}

// ---- I_MBESRawDetectionInfos::get_mbesrawdetectiondata (needs the complete data type) ----
MBESRawDetectionData I_MBESRawDetectionInfos::get_mbesrawdetectiondata()
{
    MBESRawDetectionData data;
    data.set_ref_pingtime(get_ref_pingtime());
    data.set_ref_pose(get_ref_pose());
    data.set_surface_sound_speed(get_surface_sound_speed());
    data.set_sector_beam_numbers(get_sector_beam_numbers());
    data.set_sector_tx_time_delays(get_sector_tx_time_delays());
    data.set_sector_tx_steering_angles(get_sector_tx_steering_angles());
    data.set_sector_tx_offsets(get_sector_tx_offsets());
    data.set_rx_offsets(get_rx_offsets());
    data.set_number_of_soundings(get_number_of_soundings());
    data.set_soundings_two_way_travel_times(get_soundings_two_way_travel_times());
    data.set_soundings_rx_steering_angles(get_soundings_rx_steering_angles());
    data.set_soundings_sector_numbers(get_soundings_sector_numbers());
    return data;
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
