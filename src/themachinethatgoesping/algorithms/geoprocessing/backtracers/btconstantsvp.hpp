// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/btconstantsvp.doc.hpp"

/* generated doc strings */
#include ".docstrings/btconstantsvp.doc.hpp"

#include "../datastructures.hpp"
#include "i_backtracer.hpp"

#include <cmath>

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <xtensor/xfixed.hpp>
#include <xtensor/xvectorize.hpp>

#include <themachinethatgoesping/navigation/datastructures.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace backtracers {

class BTConstantSVP : public I_Backtracer
{
    // navigation::datastructures::GeoLocation_sensor_location;
    // Eigen::Quaternion<float> _sensor_orientation_quat;

    float _sound_velocity;
    float _sound_velocity_2;

  public:
    /**
     * @brief Construct a new BTConstantSVP object
     *
     * @param sensor_location Orientation and depth of the echo sounder
     * @param sound_velocity Sound velocity in m/s
     */
    BTConstantSVP(navigation::datastructures::GeoLocation sensor_location, float sound_velocity)
        : I_Backtracer(std::move(sensor_location), "BTConstantSVP")
        , _sound_velocity(sound_velocity)
        , _sound_velocity_2(sound_velocity / 2)
    {
    }
    virtual ~BTConstantSVP() = default;

    // ----- operators -----
    bool operator==(const BTConstantSVP& other) const
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

    datastructures::SampleDirections<1> backtrace_points(
        const xt::xtensor<float, 1>&  x,
        const xt::xtensor<float, 1>&  y,
        const xt::xtensor<float, 1>&  z,
        [[maybe_unused]] unsigned int mp_cores = 1) const override
    {
        // check that all vectors have the same shape
        if (x.shape() != y.shape() || x.shape() != z.shape())
            throw std::runtime_error(
                fmt::format("BTConstantSVP::trace(Multiple points): input vectors must have the "
                            "same length: {} {} {}",
                            x.size(),
                            y.size(),
                            z.size()));

        // auto num_points = x.size();

        datastructures::SampleDirections<1> targets;

        auto dz = z - get_sensor_location().z;

        auto vec_hypot = xt::vectorize(static_cast<float (*)(float, float, float)>(std::hypot));

        auto r = vec_hypot(x, y, dz);

        // TODO: check if this subtraction is correct (or approx correct)
        targets.alongtrack_angle    = xt::degrees(xt::asin(x / r)) - get_sensor_location().pitch;
        targets.crosstrack_angle    = xt::degrees(-xt::asin(y / r)) - get_sensor_location().roll;
        targets.two_way_travel_time = 2 * r / _sound_velocity;

        return targets;
    }

    datastructures::SampleDirections<2> backtrace_image(const xt::xtensor<float, 1>& y_coordinates,
                                                        const xt::xtensor<float, 1>& z_coordinates,
                                                        unsigned int mp_cores = 1) const override
    {
        datastructures::SampleDirections<2> targets({ y_coordinates.size(), z_coordinates.size() });

        auto dz = xt::eval(z_coordinates - get_sensor_location().z);

#pragma omp parallel for num_threads(mp_cores)
        for (unsigned int i = 0; i < y_coordinates.size(); ++i)
        {
            auto r = xt::eval(xt::hypot(y_coordinates[i], dz));

            xt::row(targets.crosstrack_angle, i) =
                xt::eval(xt::degrees(-xt::asin(y_coordinates[i] / r)));
        }

        // TODO: check if this subtraction is approx correct enough
        targets.alongtrack_angle.fill(-get_sensor_location().pitch);
        targets.crosstrack_angle -= get_sensor_location().roll;

        return targets;
    }

    // ----- file I/O -----
    static BTConstantSVP from_stream(std::istream& is)
    {
        auto data = BTConstantSVP(I_Backtracer::from_stream(is));
        if (data.get_name() != "BTConstantSVP")
            throw std::runtime_error(
                fmt::format("BTConstantSVP::from_stream: wrong object name: {}", data.get_name()));

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
     * @brief Construct a new BTConstantSVP object from a base backtracer object leaving sound
     * velocity uninitialized (for reading from stream)
     *
     * @param backtracer base backtracer object
     */
    BTConstantSVP(I_Backtracer backtracer)
        : I_Backtracer(std::move(backtracer))
    {
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BTConstantSVP)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace backtracers
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping