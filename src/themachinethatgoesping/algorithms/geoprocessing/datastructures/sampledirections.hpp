// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampledirections.doc.hpp"

#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#include "samplelocationlocal.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store beamsample directsion (along angle, across angle and range).
 */
template<size_t Dim>
struct SampleDirections
{
  public:
    xt::xtensor<float, Dim> alongtrack_angle;  ///< in °, positive bow up, 0 == downwards
    xt::xtensor<float, Dim> crosstrack_angle; ///< in °, positive starboard up, 0 == downwards
    xt::xtensor<float, Dim> two_way_travel_time;       ///< in m, accumulated ray path length

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    SampleDirections() = default;

    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    SampleDirections(const std::array<unsigned int, Dim>& shape)
    {
        alongtrack_angle  = xt::xtensor<float, Dim>::from_shape(shape);
        crosstrack_angle = xt::xtensor<float, Dim>::from_shape(shape);
        two_way_travel_time       = xt::xtensor<float, Dim>::from_shape(shape);
    }

    /**
     * @brief Construct a new SampleDirections object
     *
     * @param alongtrack_angle in °, positive bow up, 0 == downwards
     * @param crosstrack_angle in °, positive starboard up, 0 == downwards
     * @param two_way_travel_time in m, accumulated ray path length
     */
    SampleDirections(xt::xtensor<float, Dim> alongtrack_angle_,
                     xt::xtensor<float, Dim> crosstrack_angle_,
                     xt::xtensor<float, Dim> two_way_travel_time_)
        : alongtrack_angle(std::move(alongtrack_angle_))
        , crosstrack_angle(std::move(crosstrack_angle_))
        , two_way_travel_time(std::move(two_way_travel_time_))
    {
        // compare shape
        if (alongtrack_angle.shape() != crosstrack_angle.shape() ||
            alongtrack_angle.shape() != two_way_travel_time.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirections::SampleDirections: alongtrack_angle, crosstrack_angle and "
                "two_way_travel_time must have the same shape. alongtrack_angle.shape() = {}, "
                "crosstrack_angle.shape() = {}, two_way_travel_time.shape() = {}",
                alongtrack_angle.shape(),
                crosstrack_angle.shape(),
                two_way_travel_time.shape()));
        }
    }

    bool operator==(const SampleDirections& rhs) const = default;

    size_t size() const
    {
        // test if all arrays have the same size
        if (alongtrack_angle.size() != crosstrack_angle.size() ||
            alongtrack_angle.size() != two_way_travel_time.size())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirections::size: alongtrack_angle, crosstrack_angle and two_way_travel_time must "
                "have the same size. alongtrack_angle.size() = {}, crosstrack_angle.size() = {}, "
                "two_way_travel_time.size() = {}",
                alongtrack_angle.size(),
                crosstrack_angle.size(),
                two_way_travel_time.size()));
        }

        return alongtrack_angle.size();
    }

  public:
    // ----- file I/O -----
    static SampleDirections from_stream(std::istream& is)
    {
        SampleDirections data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.alongtrack_angle  = xt::xtensor<float, Dim>::from_shape(shape);
        data.crosstrack_angle = xt::xtensor<float, Dim>::from_shape(shape);
        data.two_way_travel_time = xt::xtensor<float, Dim>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data.alongtrack_angle.data()),
                sizeof(float) * data.alongtrack_angle.size());
        is.read(reinterpret_cast<char*>(data.crosstrack_angle.data()),
                sizeof(float) * data.crosstrack_angle.size());
        is.read(reinterpret_cast<char*>(data.two_way_travel_time.data()),
                sizeof(float) * data.two_way_travel_time.size());

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        // compare sizes
        if (alongtrack_angle.size() != crosstrack_angle.size() ||
            alongtrack_angle.size() != two_way_travel_time.size())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirections::to_stream: alongtrack_angle, crosstrack_angle and two_way_travel_time "
                "must have the same size. alongtrack_angle.size() = {}, crosstrack_angle.size() = "
                "{}, two_way_travel_time.size() = {}",
                alongtrack_angle.size(),
                crosstrack_angle.size(),
                two_way_travel_time.size()));
        }

        std::array<size_t, Dim> shape = alongtrack_angle.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(alongtrack_angle.data()),
                 sizeof(float) * alongtrack_angle.size());
        os.write(reinterpret_cast<const char*>(crosstrack_angle.data()),    
                    sizeof(float) * crosstrack_angle.size());
        os.write(reinterpret_cast<const char*>(two_way_travel_time.data()),
                 sizeof(float) * two_way_travel_time.size());
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("SampleDirections", float_precision);

        printer.register_container("alongtrack_angle", alongtrack_angle, "°");
        printer.register_container("crosstrack_angle", crosstrack_angle, "°");
        printer.register_container("two_way_travel_time", two_way_travel_time, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SampleDirections)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping