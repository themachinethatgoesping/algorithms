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
  public:
    /**
     * @brief Construct a new BTConstantSVP object
     *
     * @param sensor_location Orientation and depth of the echo sounder
     * @param sound_velocity Sound velocity in m/s
     */
    BTConstantSVP(navigation::datastructures::GeoLocation sensor_location,
                  double                                  sensor_x,
                  double                                  sensor_y)
        : I_Backtracer(std::move(sensor_location), sensor_x, sensor_y, "BTConstantSVP")
    {
    }
    virtual ~BTConstantSVP() = default;

    // ----- operators -----
    bool operator==(const BTConstantSVP& other) const
    {
        using tools::helper::approx;

        if (I_Backtracer::operator==(other))
            return true;

        return false;
    }

    // ----- getters/setters -----
    datastructures::SampleDirectionsRange<1> backtrace_points(
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

        datastructures::SampleDirectionsRange<1> targets;

        auto dx = x - get_sensor_x();
        auto dy = y - get_sensor_y();
        auto dz = z - get_sensor_location().z;

        auto vec_hypot = xt::vectorize(static_cast<float (*)(float, float, float)>(std::hypot));

        auto r = vec_hypot(dx, dy, dz);

        // TODO: check if this subtraction is correct (or approx correct)
        targets.alongtrack_angle = xt::degrees(xt::asin(dx / r)) - get_sensor_location().pitch;
        targets.crosstrack_angle = xt::degrees(-xt::asin(dy / r)) - get_sensor_location().roll;
        targets.range            = r;

        return targets;
    }

    /**
     * Computes the backtrace image for a given set of y and z coordinates.
     *
     * @param y_coordinates The y coordinates of the backtrace image.
     * @param z_coordinates The z coordinates of the backtrace image.
     * @param mp_cores The number of cores to use for parallelization.
     * @return The backtrace image as a SampleDirectionsRange<2> object.
     */
    datastructures::SampleDirectionsRange<2> backtrace_image(
        const xt::xtensor<float, 1>& y_coordinates,
        const xt::xtensor<float, 1>& z_coordinates,
        unsigned int                 mp_cores = 1) const override
    {
        // Create a SampleDirectionsRange<2> object to store the backtrace image.
        datastructures::SampleDirectionsRange<2> targets(
            { y_coordinates.size(), z_coordinates.size() });

        // Compute the x, y, and z distances from the sensor to each point in the backtrace image.
        auto dx = -get_sensor_x();
        auto dy = xt::eval(y_coordinates - get_sensor_y());
        auto dz = xt::eval(z_coordinates - get_sensor_location().z);

        // Create a vectorized version of the hypot function for performance.
        auto vec_hypot = xt::vectorize(static_cast<float (*)(float, float, float)>(std::hypot));

        // Compute the alongtrack angle, crosstrack angle, and range for each point in the backtrace
        // image.
#pragma omp parallel for num_threads(mp_cores)
        for (size_t i = 0; i < dy.size(); ++i)
        {
            auto r = vec_hypot(
                dx, dy[i], dz); // Compute the distance from the sensor to the current point.

            // Compute the alongtrack angle, crosstrack angle, and range for the current point.
            xt::row(targets.alongtrack_angle, i) =
                xt::eval(xt::degrees(xt::degrees(xt::asin(dx / r)))) -
                get_sensor_location().pitch; // Subtract the sensor pitch from the alongtrack angle.
            xt::row(targets.crosstrack_angle, i) =
                xt::eval(xt::degrees(-xt::asin(xt::eval(dy[i] / r)))) -
                get_sensor_location().roll; // Subtract the sensor roll from the crosstrack angle.
            xt::row(targets.range, i) = std::move(r);
        }

        // Return the backtrace image.
        return targets;
    }

    // ----- file I/O -----
    static BTConstantSVP from_stream(std::istream& is)
    {
        auto data = BTConstantSVP(I_Backtracer::from_stream(is));
        if (data.class_name() != "BTConstantSVP")
            throw std::runtime_error(
                fmt::format("BTConstantSVP::from_stream: wrong object name: {}", data.class_name()));

        return data;
    }

    void to_stream(std::ostream& os) const { I_Backtracer::to_stream(os); }

  public:
    // __printer__ function is necessary to support print() info_string() etc (defined by
    // __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro below)
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer(this->class_name(), float_precision);

        printer.register_section("Basics", '*');
        printer.append(I_Backtracer::__printer__(float_precision));

        return printer;
    }

  private:
    /**
     * @brief Construct a new BTConstantSVP object from a base backtracer object
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