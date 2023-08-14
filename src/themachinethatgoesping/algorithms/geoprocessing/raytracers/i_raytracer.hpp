// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/i_raytracer.doc.hpp"

#include "../datastructures.hpp"

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <themachinethatgoesping/navigation/datastructures.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers {

class I_Raytracer
{
    std::string _name;

    navigation::datastructures::GeoLocation _sensor_location; ///< Location/Orientation of the senor

    Eigen::Quaternion<float> _sensor_orientation_quat; ///< Quaternion describing the orientation
                                                       ///< of the sensor

  public:
    std::string get_name() const { return _name; }

  public:
    I_Raytracer(navigation::datastructures::GeoLocation sensor_location, std::string name)
        : _name(std::move(name))
    {
        set_sensor_location(std::move(sensor_location));
    }
    virtual ~I_Raytracer() = default;

    // ----- operators -----
    bool operator==(const I_Raytracer& other) const = default;

    // ----- tracing -----

    virtual datastructures::SamplelocationLocal trace(
        [[maybe_unused]] float alongtrack_angle,
        [[maybe_unused]] float crosstrack_angle,
        [[maybe_unused]] float two_way_travel_time) const
    {
        not_implemented("trace(SinglePoint)", _name);
    }

    // ----- setters -----
    void set_sensor_location(navigation::datastructures::GeoLocation sensor_location)
    {
        _sensor_location = std::move(sensor_location);

        _sensor_orientation_quat = tools::rotationfunctions::quaternion_from_ypr(
            0.0f, _sensor_location.pitch, _sensor_location.roll);
    }

    // ----- accessors -----
    const auto& get_sensor_location() const { return _sensor_location; }
    auto        get_sensor_orientation_quat_ypr() const
    {
        return tools::rotationfunctions::ypr_from_quaternion(_sensor_orientation_quat);
    }
    const auto& get_sensor_orientation_quat() const { return _sensor_orientation_quat; }

    // ----- file I/O -----
    static I_Raytracer from_stream(std::istream& is)
    {
        auto name = tools::classhelper::stream::container_from_stream<std::string>(is);

        auto geolocation = navigation::datastructures::GeoLocation::from_stream(is);

        return I_Raytracer(std::move(geolocation), name);
    }

    void to_stream(std::ostream& os) const
    {
        tools::classhelper::stream::container_to_stream(os, _name);
        _sensor_location.to_stream(os);
    }

  public:
    // __printer__ function is necessary to support print() info_string() etc (defined by
    // __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro below)
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer(this->get_name(), float_precision);

        printer.register_section("Sensor location", '*');
        printer.append(_sensor_location.__printer__(float_precision));

        printer.register_section("Sensor location converted", '*');
        printer.register_container("YPR", get_sensor_orientation_quat_ypr(), "°");

        return printer;
    }

  protected:
    struct not_implemented : public std::runtime_error
    {
        not_implemented(std::string method_name, std::string name)
            : std::runtime_error(fmt::format("method {} not implemented for raytracer type '{}'",
                                             method_name,
                                             name))
        {
        }
    };

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(I_Raytracer)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping