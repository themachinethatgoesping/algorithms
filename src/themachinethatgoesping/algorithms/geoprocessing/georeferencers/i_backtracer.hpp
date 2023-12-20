// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/i_backtracer.doc.hpp"

#include "../datastructures.hpp"

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <xtensor/xview.hpp>

#include <themachinethatgoesping/navigation/datastructures.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace backtracers {

class I_Backtracer
{
    std::string _name;

    navigation::datastructures::Geolocation _sensor_location; ///< Location/Orientation of the senor
    float                                   _sensor_x;
    float                                   _sensor_y;

    Eigen::Quaternion<float> _sensor_orientation_quat; ///< Quaternion describing the orientation
                                                       ///< of the sensor

  public:
    std::string class_name() const { return _name; }

  public:
    I_Backtracer(navigation::datastructures::Geolocation sensor_location,
                 float                                   sensor_x,
                 float                                   sensor_y,
                 std::string                             name)
        : _name(std::move(name))
    {
        set_sensor_location(std::move(sensor_location), sensor_x, sensor_y);
    }
    virtual ~I_Backtracer() = default;

    // ----- operators -----
    bool operator==(const I_Backtracer& other) const
    {
        // do not compare sensor quaternion as this is derived from the sensor location
        if (_name == other._name)
            if (_sensor_location == other._sensor_location)
                return true;

        return false;
    }

    // ----- tracing -----

    /**
     * @brief Backtrace the location of a set of points.
     *
     * @param x in m, positive forward
     * @param y in m, positive starboard
     * @param z in m, positive downwards
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::SampleDirectionsRange
     */
    virtual datastructures::SampleDirectionsRange<1> backtrace_points(
        [[maybe_unused]] const xt::xtensor<float, 1>& x,
        [[maybe_unused]] const xt::xtensor<float, 1>& y,
        [[maybe_unused]] const xt::xtensor<float, 1>& z,
        [[maybe_unused]] unsigned int                 mp_cores = 1) const
    {
        throw not_implemented("backtrace(Multiple points)", _name);
    }

    /**
     * @brief Backtrace the location of a set of points.
     *
     * @param xyz structure with x/y/z coordinates
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::SampleDirectionsRange
     */
    datastructures::SampleDirectionsRange<1> backtrace_points(
        [[maybe_unused]] const datastructures::XYZ<1>& xyz,
        [[maybe_unused]] unsigned int                  mp_cores = 1) const
    {
        return backtrace_points(xyz.x, xyz.y, xyz.z, mp_cores);
    }

    /**
     * @brief Backtrace the location of an image specified by two coordinate vectors
     * x is assumed to be 0
     *
     * @param y_coordinates in m, positive starboard
     * @param z_coordinates in m, positive downwards
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::SampleDirectionsRange<2>, shape is (y_coordinates.size(),
     * z_coordinates.size())
     */
    virtual datastructures::SampleDirectionsRange<2> backtrace_image(
        [[maybe_unused]] const xt::xtensor<float, 1>& y_coordinates,
        [[maybe_unused]] const xt::xtensor<float, 1>& z_coordinates,
        [[maybe_unused]] unsigned int                 mp_cores = 1) const
    {
        throw not_implemented("backtrace(Multiple points)", _name);
    }

    // ----- setters -----
    void set_sensor_location(navigation::datastructures::Geolocation sensor_location,
                             float                                   sensor_x,
                             float                                   sensor_y)
    {
        _sensor_location = std::move(sensor_location);
        _sensor_x        = sensor_x;
        _sensor_y        = sensor_y;

        _sensor_orientation_quat = tools::rotationfunctions::quaternion_from_ypr(
            0.0f, _sensor_location.pitch, _sensor_location.roll);
    }

    // ----- accessors -----
    float       get_sensor_x() const { return _sensor_x; }
    float       get_sensor_y() const { return _sensor_y; }
    const auto& get_sensor_location() const { return _sensor_location; }
    auto        get_sensor_orientation_quat_ypr() const
    {
        return tools::rotationfunctions::ypr_from_quaternion(_sensor_orientation_quat);
    }
    const auto& get_sensor_orientation_quat() const { return _sensor_orientation_quat; }

    // ----- file I/O -----
    static I_Backtracer from_stream(std::istream& is)
    {
        auto name = tools::classhelper::stream::container_from_stream<std::string>(is);

        auto                 geolocation = navigation::datastructures::Geolocation::from_stream(is);
        std::array<float, 2> sensor_xy;
        is.read(reinterpret_cast<char*>(&sensor_xy), sizeof(sensor_xy));

        return I_Backtracer(std::move(geolocation), sensor_xy[0], sensor_xy[1], name);
    }

    void to_stream(std::ostream& os) const
    {
        tools::classhelper::stream::container_to_stream(os, _name);
        _sensor_location.to_stream(os);
        os.write(reinterpret_cast<const char*>(&_sensor_x), sizeof(_sensor_x) * 2);
    }

  public:
    // __printer__ function is necessary to support print() info_string() etc (defined by
    // __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro below)
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer(this->class_name(), float_precision);

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
            : std::runtime_error(fmt::format("method {} not implemented for backtracer type '{}'",
                                             method_name,
                                             name))
        {
        }
    };

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(I_Backtracer)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace backtracers
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping