// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/rtconstantsvp.doc.hpp"

#include "../datastructures.hpp"
#include "i_raytracer.hpp"

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <themachinethatgoesping/navigation/datastructures.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers {

class RTConstantSVP : public I_Raytracer
{
    // navigation::datastructures::GeoLocation_sensor_location;
    // Eigen::Quaternion<float> _sensor_orientation_quat;

    float _sound_velocity;

  public:
    /**
     * @brief Construct a new RTConstantSVP object
     *
     * @param sensor_location Orientation and depth of the echo sounder
     * @param sound_velocity Sound velocity in m/s
     */
    RTConstantSVP(navigation::datastructures::GeoLocation sensor_location, float sound_velocity)
        : I_Raytracer(std::move(sensor_location), "RTConstantSVP")
        , _sound_velocity(std::move(sound_velocity))
    {
    }
    virtual ~RTConstantSVP() = default;

    // ----- operators -----
    bool operator==(const RTConstantSVP& other) const = default;

    // ----- getters/setters -----
    void  set_sound_velocity(float sound_velocity) { _sound_velocity = sound_velocity; }
    float get_sound_velocity() const { return _sound_velocity; }

    datastructures::SamplelocationLocal trace_point(float two_way_travel_time,
                                                    float alongtrack_angle,
                                                    float crosstrack_angle) const override
    {
        not_implemented("trace(SinglePoint)", get_name());

        datastructures::SamplelocationLocal target;

        target.true_range = two_way_travel_time * _sound_velocity * 0.5f;

        // convert launch angles to quaternion
        auto target_ypr_quat =
            tools::rotationfunctions::quaternion_from_ypr(0.f, alongtrack_angle, crosstrack_angle);

        // the true orientation is the sensor orientation rotated by the launch angles
        target_ypr_quat = get_sensor_orientation_quat() * target_ypr_quat;

        // get rotated positions
        auto target_xyz =
            tools::rotationfunctions::rotateXYZ<float>(target_ypr_quat, 0, 0, target.true_range);

        // set target position
        target.x = target_xyz[0];
        target.y = target_xyz[1];
        target.z = target_xyz[2] + get_sensor_location().z;

        return target;
    }

    datastructures::SampleLocationsLocal<1> trace_points(
        [[maybe_unused]] const xt::xtensor<float, 1>& two_way_travel_times,
        [[maybe_unused]] const xt::xtensor<float, 1>& alongtrack_angles,
        [[maybe_unused]] const xt::xtensor<float, 1>& crosstrack_angles,
        [[maybe_unused]] int                          mp_cores = 1) const override
    {
        // check that all vectors have the same length
        if (alongtrack_angles.size() != crosstrack_angles.size() ||
            alongtrack_angles.size() != two_way_travel_times.size())
            throw std::runtime_error(
                fmt::format("RTConstantSVP::trace(Multiple points): input vectors must have the "
                            "same length: {} {} {}",
                            alongtrack_angles.size(),
                            crosstrack_angles.size(),
                            two_way_travel_times.size()));

        auto num_points = two_way_travel_times.size();

        datastructures::SampleLocationsLocal<1> targets;

        targets.true_range = xt::eval(two_way_travel_times * _sound_velocity * 0.5f);
        targets.x          = xt::xtensor<float, 1>::from_shape({ num_points });
        targets.y          = xt::xtensor<float, 1>::from_shape({ num_points });
        targets.z          = xt::xtensor<float, 1>::from_shape({ num_points });

#pragma omp parallel for num_threads(mp_cores)
        for (unsigned int i = 0; i < num_points; ++i)
        {
            // convert launch angles to quaternion
            auto target_ypr_quat = tools::rotationfunctions::quaternion_from_ypr(
                0.f, alongtrack_angles[i], crosstrack_angles[i]);

            // the true orientation is the sensor orientation rotated by the launch angles
            target_ypr_quat = get_sensor_orientation_quat() * target_ypr_quat;

            // get rotated positions
            auto target_xyz = tools::rotationfunctions::rotateXYZ<float>(
                target_ypr_quat, 0, 0, targets.true_range[i]);

            // set target position
            targets.x[i] = target_xyz[0];
            targets.y[i] = target_xyz[1];
            targets.z[i] = target_xyz[2];
        }
        targets.z += get_sensor_location().z;

        return targets;
    }

    // ----- file I/O -----
    static RTConstantSVP from_stream(std::istream& is)
    {
        auto data = RTConstantSVP(I_Raytracer::from_stream(is));
        if (data.get_name() != "RTConstantSVP")
            throw std::runtime_error(
                fmt::format("RTConstantSVP::from_stream: wrong object name: {}", data.get_name()));

        is.read(reinterpret_cast<char*>(&data._sound_velocity), sizeof(float));

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        I_Raytracer::to_stream(os);
        os.write(reinterpret_cast<const char*>(&_sound_velocity), sizeof(float));
    }

  public:
    // __printer__ function is necessary to support print() info_string() etc (defined by
    // __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro below)
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer(this->get_name(), float_precision);

        printer.register_section("Basics", '*');
        printer.append(I_Raytracer::__printer__(float_precision));

        printer.register_section("Profile", '*');
        printer.register_value("sound_velocity", _sound_velocity, "m/s");

        return printer;
    }

  private:
    /**
     * @brief Construct a new RTConstantSVP object from a base raytracer object leaving sound
     * velocity uninitialized (for reading from stream)
     *
     * @param raytracer base raytracer object
     */
    RTConstantSVP(I_Raytracer raytracer)
        : I_Raytracer(std::move(raytracer))
    {
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(RTConstantSVP)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping