// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampledirectionstime.doc.hpp"

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#include "sampledirections.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store beamsample directions (along angle, across angle and range).
 */
template<size_t Dim>
struct SampleDirectionsTime : public SampleDirections<Dim>
{
  public:
    using SampleDirections<Dim>::alongtrack_angle;
    using SampleDirections<Dim>::crosstrack_angle;
    xt::xtensor<float, Dim> two_way_travel_time; ///< in s, accumulated ray path time

    using SampleDirections<Dim>::size;
    using SampleDirections<Dim>::shape;

    /**
     * @brief Construct a new SampleDirections object (all values set to 0)
     *
     */
    SampleDirectionsTime() = default;

    /**
     * @brief Construct a new SampleDirections object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    SampleDirectionsTime(const std::array<size_t, Dim>& shape)
        : SampleDirections<Dim>(shape)
    {
        two_way_travel_time = xt::xtensor<float, Dim>::from_shape(shape);
    }

    /**
     * @brief Construct a new SampleDirections object (from a SampleDirections object)
     *
     * @param two_way_travel_time in s, accumulated ray path time
     *
     */
    SampleDirectionsTime(SampleDirections<Dim>   sample_directions,
                         xt::xtensor<float, Dim> two_way_travel_time)
        : SampleDirections<Dim>(std::move(sample_directions))
        , two_way_travel_time(std::move(two_way_travel_time))
    {
        check_shape();
    }

    /**
     * @brief Construct a new SampleDirectionsTime object
     *
     * @param alongtrack_angle in °, positive bow up, 0 == downwards
     * @param crosstrack_angle in °, positive starboard up, 0 == downwards
     * @param two_way_travel_time in m, accumulated ray path length
     */
    SampleDirectionsTime(xt::xtensor<float, Dim> alongtrack_angle_,
                         xt::xtensor<float, Dim> crosstrack_angle_,
                         xt::xtensor<float, Dim> two_way_travel_time_)
        : SampleDirections<Dim>(std::move(alongtrack_angle_), std::move(crosstrack_angle_))
        , two_way_travel_time(std::move(two_way_travel_time_))
    {
        // compare shape
        check_shape();
    }

    bool operator==(const SampleDirectionsTime& rhs) const = default;

  public:
    // ----- file I/O -----
    static SampleDirectionsTime from_stream(std::istream& is)
    {
        SampleDirectionsTime data(SampleDirections<Dim>::from_stream(is));

        is.read(reinterpret_cast<char*>(data.two_way_travel_time.data()),
                sizeof(float) * data.two_way_travel_time.size());

        data.check_shape();
        return data;
    }

    void to_stream(std::ostream& os) const
    {
        SampleDirections<Dim>::to_stream(os);

        os.write(reinterpret_cast<const char*>(two_way_travel_time.data()),
                 sizeof(float) * two_way_travel_time.size());
    }

  public:
    void check_shape() const override
    {
        // compare shape
        if (alongtrack_angle.shape() != two_way_travel_time.shape() ||
            crosstrack_angle.shape() != two_way_travel_time.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirectionsTime::SampleDirectionsTime: alongtrack_angle, crosstrack_angle "
                "and two_way_travel_time must have the same shape. alongtrack_angle.size() = {}, "
                "crosstrack_angle.size() = {}, "
                "two_way_travel_time.size() = {}",
                alongtrack_angle.size(),
                crosstrack_angle.size(),
                two_way_travel_time.size()));
        }
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("SampleDirectionsTime", float_precision, superscript_exponents);

        printer.register_container("alongtrack_angle", alongtrack_angle, "°");
        printer.register_container("crosstrack_angle", crosstrack_angle, "°");
        printer.register_container(
            "two_way_travel_time", two_way_travel_time, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SampleDirectionsTime)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__

  private:
    /**
     * @brief Construct object from an existing SampleDirections object (leaving two_way_travel_time
     * uninitialized)
     *
     * Note: this is only used internally
     *
     * @param SampleDirections sample directions object
     *
     */
    SampleDirectionsTime(SampleDirections<Dim> sample_directions)
        : SampleDirections<Dim>(std::move(sample_directions))
        , two_way_travel_time(xt::xtensor<float, Dim>::from_shape(alongtrack_angle.shape()))
    {
    }
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping