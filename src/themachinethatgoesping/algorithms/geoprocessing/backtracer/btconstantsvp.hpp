// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/btconstantsvp.doc.hpp"

/* generated doc strings */
#include ".docstrings/rtconstantsvp.doc.hpp"

#include "../datastructures.hpp"
#include "i_backtracer.hpp"

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <xtensor/xfixed.hpp>

#include <themachinethatgoesping/navigation/datastructures.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace backtracers {

class RTConstantSVP : public I_Backtracer
{
    // navigation::datastructures::GeoLocation_sensor_location;
    // Eigen::Quaternion<float> _sensor_orientation_quat;

    float _sound_velocity;
    float _sound_velocity_2;

  public:
    /**
     * @brief Construct a new RTConstantSVP object
     *
     * @param sensor_location Orientation and depth of the echo sounder
     * @param sound_velocity Sound velocity in m/s
     */
    RTConstantSVP(navigation::datastructures::GeoLocation sensor_location, float sound_velocity)
        : I_Backtracer(std::move(sensor_location), "RTConstantSVP")
        , _sound_velocity(sound_velocity)
        , _sound_velocity_2(sound_velocity / 2)
    {
    }
    virtual ~RTConstantSVP() = default;

    // ----- operators -----
    bool operator==(const RTConstantSVP& other) const
    {
        using tools::helper::approx;

        if (I_Backtracer::operator==(other))
            if (approx(_sound_velocity, other._sound_velocity))
                return true;

        return false;
    }

    // ----- getters/setters -----
    void  set_sound_velocity(float sound_velocity) { _sound_velocity = sound_velocity; }
    float get_sound_velocity() const { return _sound_velocity; }

    datastructures::RaytraceResult trace_point(float two_way_travel_time,
                                                    float alongtrack_angle,
                                                    float crosstrack_angle) const override
    {
        not_implemented("trace(SinglePoint)", get_name());

        datastructures::RaytraceResult target;

        target.true_range = two_way_travel_time * _sound_velocity_2;

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

    datastructures::RaytraceResults<1> trace_points(
        const xt::xtensor<float, 1>& two_way_travel_times,
        const xt::xtensor<float, 1>& alongtrack_angles,
        const xt::xtensor<float, 1>& crosstrack_angles,
        unsigned int                 mp_cores = 1) const override
    {
        // check that all vectors have the same length
        if (crosstrack_angles.size() != two_way_travel_times.size() ||
            alongtrack_angles.size() != two_way_travel_times.size())
            throw std::runtime_error(
                fmt::format("RTConstantSVP::trace(Multiple points): input vectors must have the "
                            "same length: {} {} {}",
                            crosstrack_angles.size(),
                            alongtrack_angles.size(),
                            two_way_travel_times.size()));

        auto num_points = two_way_travel_times.size();

        datastructures::RaytraceResults<1> targets;

        targets.true_range = xt::eval(two_way_travel_times * _sound_velocity_2);
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

    datastructures::RaytraceResults<1> trace_points(
        const xt::xtensor<float, 1>& two_way_travel_times,
        float                        alongtrack_angle,
        const xt::xtensor<float, 1>& crosstrack_angles,
        unsigned int                 mp_cores = 1) const override
    {
        // check that all vectors have the same length
        if (crosstrack_angles.size() != two_way_travel_times.size())
            throw std::runtime_error(
                fmt::format("RTConstantSVP::trace(Multiple points): input vectors must have the "
                            "same length: {} {}",
                            crosstrack_angles.size(),
                            two_way_travel_times.size()));

        auto num_points = two_way_travel_times.size();

        datastructures::RaytraceResults<1> targets;

        targets.true_range = xt::eval(two_way_travel_times * _sound_velocity_2);
        targets.x          = xt::xtensor<float, 1>::from_shape({ num_points });
        targets.y          = xt::xtensor<float, 1>::from_shape({ num_points });
        targets.z          = xt::xtensor<float, 1>::from_shape({ num_points });

#pragma omp parallel for num_threads(mp_cores)
        for (unsigned int i = 0; i < num_points; ++i)
        {
            // convert launch angles to quaternion
            auto target_ypr_quat = tools::rotationfunctions::quaternion_from_ypr(
                0.f, alongtrack_angle, crosstrack_angles[i]);

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

    datastructures::RaytraceResults<1> trace_beam(
        const xt::xtensor<unsigned int, 1>& sample_numbers,
        float                               sampling_time,
        float                               sampling_time_offset,
        float                               alongtrack_angle,
        float                               crosstrack_angle) const override
    {
        float last_sample_time = (sample_numbers.back() * sampling_time) + sampling_time_offset;
        auto  scale_target     = trace_point(last_sample_time, alongtrack_angle, crosstrack_angle);

        return scale_beam(
            sample_numbers, sampling_time, sampling_time_offset, scale_target, last_sample_time);
    }

    datastructures::RaytraceResults<2> trace_swath(
        const xt::xtensor<unsigned int, 2>& sample_numbers,
        float                               sampling_time,
        float                               sampling_time_offset,
        float                               alongtrack_angle,
        const xt::xtensor<float, 1>&        crosstrack_angles,
        unsigned int                        mp_cores = 1) const override
    {
        xt::xtensor<float, 1> last_sample_times =
            xt::view(sample_numbers, xt::all(), -1) * sampling_time + sampling_time_offset;

        auto scale_targets =
            trace_points(last_sample_times, alongtrack_angle, crosstrack_angles, mp_cores);

        return scale_swath(sample_numbers,
                           sampling_time,
                           sampling_time_offset,
                           scale_targets,
                           last_sample_times,
                           mp_cores);
    }

    /**
     * @brief Compute the sample locations of a single beam by scaling between the transducer
     * location and a known target location
     *
     * @param sample_numbers Sample numbers to trace (starting from 0)
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param scale_x known target x position at scale_time
     * @param scale_y known target y position at scale_time
     * @param scale_z known target z position at scale_time
     * @param scale_true_range known target range at scale_time
     * @param scale_time known target two way travel time
     * @return datastructures::RaytraceResults<1>
     */
    datastructures::RaytraceResults<1> scale_beam(
        const xt::xtensor<unsigned int, 1>& sample_numbers,
        float                               sampling_time,
        float                               sampling_time_offset,
        float                               scale_x,
        float                               scale_y,
        float                               scale_z,
        float                               scale_true_range,
        float                               scale_time) const
    {
        xt::xtensor<float, 1> two_way_travel_times =
            ((sample_numbers * sampling_time) + sampling_time_offset);

        datastructures::RaytraceResults<1> targets;

        xt::xtensor_fixed<float, xt::xshape<2>> xp = { 0.f, scale_time };
        xt::xtensor_fixed<float, xt::xshape<2>> fp = { 0.f, scale_true_range };
        targets.true_range                         = xt::interp(two_way_travel_times, xp, fp);
        fp[1]                                      = scale_x;
        targets.x                                  = xt::interp(two_way_travel_times, xp, fp);
        fp[1]                                      = scale_y;
        targets.y                                  = xt::interp(two_way_travel_times, xp, fp);
        fp                                         = { get_sensor_location().z, scale_z };
        targets.z                                  = xt::interp(two_way_travel_times, xp, fp);

        return targets;
    }

    /**
     * @brief Compute the sample locations of a single beam by scaling between the transducer
     * location and a known target location
     *
     * @param sample_numbers Sample numbers to trace (starting from 0)
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param scale_target known target location at scale_time
     * @param scale_time known target two way travel time
     * @return datastructures::RaytraceResults<1>
     */
    datastructures::RaytraceResults<1> scale_beam(
        const xt::xtensor<unsigned int, 1>&        sample_numbers,
        float                                      sampling_time,
        float                                      sampling_time_offset,
        const datastructures::RaytraceResult& scale_target,
        float                                      scale_time) const
    {
        return scale_beam(sample_numbers,
                          sampling_time,
                          sampling_time_offset,
                          scale_target.x,
                          scale_target.y,
                          scale_target.z,
                          scale_target.true_range,
                          scale_time);
    }

    /**
     * @brief Compute the sample locations of a swath by scaling between the transducer
     * location and known target locations
     *
     * @param sample_numbers Sample numbers to trace (starting from 0)
     * @param sampling_time Time betweens samples in s
     * @param sampling_time_offset Time offset for sample number 0 in s
     * @param scale_target known target location at scale_time
     * @param scale_time two way travel time for the known target location
     * @param mp_cores number of cores to use for parallelization
     * @return datastructures::RaytraceResults<1>
     */
    datastructures::RaytraceResults<2> scale_swath(
        const xt::xtensor<unsigned int, 2>&            sample_numbers,
        float                                          sampling_time,
        float                                          sampling_time_offset,
        const datastructures::RaytraceResults<1>& scale_targets,
        const xt::xtensor<float, 1>&                   scale_times,
        unsigned int                                   mp_cores = 1) const
    {

        // test that beam numbers are consistent
        if (sample_numbers.shape()[0] != scale_times.size() ||
            scale_targets.size() != scale_times.size())
            throw std::runtime_error(fmt::format("RTConstantSVP::scale_swath: sample_numbers, "
                                                 "scale_targets and scale_times must have "
                                                 "the same number of beams: {} {} {}",
                                                 sample_numbers.shape()[0],
                                                 scale_targets.size(),
                                                 scale_times.size()));

        unsigned int number_of_beams   = sample_numbers.shape()[0];
        unsigned int number_of_samples = sample_numbers.shape()[1];

        datastructures::RaytraceResults<2> targets({ number_of_beams, number_of_samples });
#pragma omp parallel for num_threads(mp_cores)
        for (unsigned int bn = 0; bn < number_of_beams; ++bn)
        {
            auto beam_targets = scale_beam(xt::view(sample_numbers, bn, xt::all()),
                                           sampling_time,
                                           sampling_time_offset,
                                           scale_targets.x[bn],
                                           scale_targets.y[bn],
                                           scale_targets.z[bn],
                                           scale_targets.true_range[bn],
                                           scale_times[bn]);

            // TODO: this seems to be slow, maybe there is a better way to do this
            xt::row(targets.true_range, bn) = beam_targets.true_range;
            xt::row(targets.x, bn)          = beam_targets.x;
            xt::row(targets.y, bn)          = beam_targets.y;
            xt::row(targets.z, bn)          = beam_targets.z;
        }

        return targets;
    }

    

    // ----- file I/O -----
    static RTConstantSVP from_stream(std::istream& is)
    {
        auto data = RTConstantSVP(I_Backtracer::from_stream(is));
        if (data.get_name() != "RTConstantSVP")
            throw std::runtime_error(
                fmt::format("RTConstantSVP::from_stream: wrong object name: {}", data.get_name()));

        is.read(reinterpret_cast<char*>(&data._sound_velocity), sizeof(float));

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        I_Backtracer::to_stream(os);
        os.write(reinterpret_cast<const char*>(&_sound_velocity), sizeof(float));
    }

  public:
    // __printer__ function is necessary to support print() info_string() etc (defined by
    // __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro below)
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer(this->get_name(), float_precision);

        printer.register_section("Basics", '*');
        printer.append(I_Backtracer::__printer__(float_precision));

        printer.register_section("Profile", '*');
        printer.register_value("sound_velocity", _sound_velocity, "m/s");

        return printer;
    }

  private:
    /**
     * @brief Construct a new RTConstantSVP object from a base backtracer object leaving sound
     * velocity uninitialized (for reading from stream)
     *
     * @param backtracer base backtracer object
     */
    RTConstantSVP(I_Backtracer backtracer)
        : I_Backtracer(std::move(backtracer))
    {
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(RTConstantSVP)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace backtracers
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping