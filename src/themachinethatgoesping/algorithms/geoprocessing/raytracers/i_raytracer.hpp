// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/i_raytracer.doc.hpp"

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
namespace raytracers {

class I_Raytracer
{
    std::string _name;

    navigation::datastructures::GeoLocation _sensor_location; ///< Location/Orientation of the senor

    Eigen::Quaternion<float> _sensor_orientation_quat; ///< Quaternion describing the orientation
                                                       ///< of the sensor

  public:
    std::string class_name() const { return _name; }

  public:
    I_Raytracer(navigation::datastructures::GeoLocation sensor_location, std::string name)
        : _name(std::move(name))
    {
        set_sensor_location(std::move(sensor_location));
    }
    virtual ~I_Raytracer() = default;

    // ----- operators -----
    bool operator==(const I_Raytracer& other) const
    {
        // do not compare sensor quaternion as this is derived from the sensor location
        if (_name == other._name)
            if (_sensor_location == other._sensor_location)
                return true;

        return false;
    }

    // ----- tracing -----
    /**
     * @brief Trace the location of a single point.
     *
     * @param two_way_travel_time Two way travel time in s
     * @param alongtrack_angle Along track angle in °
     * @param crosstrack_angle Across track angle in °
     * @return datastructures::RaytraceResult
     */
    virtual datastructures::RaytraceResult trace_point(
        [[maybe_unused]] float two_way_travel_time,
        [[maybe_unused]] float alongtrack_angle,
        [[maybe_unused]] float crosstrack_angle) const
    {
        throw not_implemented("trace(SinglePoint)", _name);
    }

    /**
     * @brief Trace the location of a set of points.
     *
     * @param two_way_travel_time Two way travel time in s
     * @param alongtrack_angle Along track angle in °
     * @param crosstrack_angles Across track angle in °
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::RaytraceResult
     */
    virtual datastructures::RaytraceResults<1> trace_points(
        [[maybe_unused]] const xt::xtensor<float, 1>& two_way_travel_times,
        [[maybe_unused]] const xt::xtensor<float, 1>& alongtrack_angles,
        [[maybe_unused]] const xt::xtensor<float, 1>& crosstrack_angles,
        [[maybe_unused]] unsigned int                 mp_cores = 1) const
    {
        throw not_implemented("trace(Multiple points)", _name);
    }

    /**
     * @brief Trace the location of a set of points.
     *
     * @param sampledirections One dimensional sample directions array
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::RaytraceResult
     */
    datastructures::RaytraceResults<1> trace_points(
        const datastructures::SampleDirectionsTime<1>& sampledirections,
        unsigned int                                   mp_cores = 1) const
    {
        return trace_points(sampledirections.two_way_travel_time,
                            sampledirections.alongtrack_angle,
                            sampledirections.crosstrack_angle,
                            mp_cores);
    }

    /**
     * @brief Trace the sample locations of a single beam.
     *
     * @param sample_numbers Sample numbers to trace (starting from 0)
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param alongtrack_angle Along track angle in °
     * @param crosstrack_angle Across track angle in °
     * @return datastructures::RaytraceResults<1>
     */
    virtual datastructures::RaytraceResults<1> trace_beam(
        [[maybe_unused]] const xt::xtensor<unsigned int, 1>& sample_numbers,
        [[maybe_unused]] float                               sampling_time,
        [[maybe_unused]] float                               sampling_time_offset,
        [[maybe_unused]] float                               alongtrack_angle,
        [[maybe_unused]] float                               crosstrack_angle) const
    {
        throw not_implemented("trace(SingleBeam)", _name);
    }

    /**
     * @brief Trace the sample locations of a multiple beams in a swath.
     * Note: a 2d Array for sample numbers is expected where the first dimension is the beam and the
     * second dimension is the sample number. The beam dimension must be the same as for
     * "crosstrack_angles"
     *
     * @param sample_numbers Sample numbers to trace (starting from 0)
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param alongtrack_angle Along track angle of the swath in °
     * @param crosstrack_angles Across track angle of each beam in °
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::RaytraceResults<2>
     */
    virtual datastructures::RaytraceResults<2> trace_swath(
        [[maybe_unused]] const xt::xtensor<unsigned int, 2>& sample_numbers,
        [[maybe_unused]] float                               sampling_time,
        [[maybe_unused]] float                               sampling_time_offset,
        [[maybe_unused]] const xt::xtensor<float, 1>&        alongtrack_angle,
        [[maybe_unused]] const xt::xtensor<float, 1>&        crosstrack_angles,
        [[maybe_unused]] unsigned int                        mp_cores = 1) const
    {
        throw not_implemented("trace(Swath)", _name);
    }

    /**
     * @brief Trace the sample locations of a single beam.
     *
     * @param first_sample_number First sample number to trace
     * @param number_of_samples Number of samples to trace
     * @param sample_step Step between samples
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param alongtrack_angle Along track angle in °
     * @param crosstrack_angle Across track angle in °
     * @return datastructures::RaytraceResults<1>
     */
    virtual datastructures::RaytraceResults<1> trace_beam(unsigned int first_sample_number,
                                                          unsigned int number_of_samples,
                                                          unsigned int sample_step,
                                                          float        sampling_time,
                                                          float        sampling_time_offset,
                                                          float        alongtrack_angle,
                                                          float        crosstrack_angle) const
    {
        return trace_beam(xt::arange<unsigned int>(first_sample_number,
                                                   first_sample_number + number_of_samples,
                                                   sample_step),
                          sampling_time,
                          sampling_time_offset,
                          alongtrack_angle,
                          crosstrack_angle);
    }

    /**
     * @brief Trace the sample locations of a multiple beams in a swath.
     * Note: The number of beams is controlled by the dimension of crosstrack_angles
     *
     * @param first_sample_number First sample number to trace
     * @param number_of_samples Number of samples to trace
     * @param sample_step Step between samples
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param alongtrack_angle Along track angle of the swath in °
     * @param crosstrack_angles Across track angle of each beam in °
     * @param mp_cores Number of cores to use for parallelization
     * @return datastructures::RaytraceResults<2>
     */
    virtual datastructures::RaytraceResults<2> trace_swath(
        unsigned int                 first_sample_number,
        unsigned int                 number_of_samples,
        unsigned int                 sample_step,
        float                        sampling_time,
        float                        sampling_time_offset,
        const xt::xtensor<float, 1>& alongtrack_angles,
        const xt::xtensor<float, 1>& crosstrack_angles,
        unsigned int                 mp_cores = 1) const
    {
        xt::xtensor<unsigned int, 1> sample_numbers_per_beam = xt::arange<unsigned int>(
            first_sample_number, first_sample_number + number_of_samples, sample_step);

        auto sample_numbers = xt::xtensor<unsigned int, 2>::from_shape(
            { crosstrack_angles.size(), sample_numbers_per_beam.size() });

        for (unsigned int bn = 0; bn < crosstrack_angles.size(); bn++)
        {
            xt::view(sample_numbers, bn, xt::all()) = sample_numbers_per_beam;
        }

        return trace_swath(sample_numbers,
                           sampling_time,
                           sampling_time_offset,
                           alongtrack_angles,
                           crosstrack_angles,
                           mp_cores);
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