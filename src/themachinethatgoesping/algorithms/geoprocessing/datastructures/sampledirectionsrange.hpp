// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampledirectionsrange.doc.hpp"

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>




#include "sampledirections.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store beamsample directions (along angle, across angle and range).
 */
template<size_t Dim>
struct SampleDirectionsRange : public SampleDirections<Dim>
{
  public:
    using SampleDirections<Dim>::alongtrack_angle;
    using SampleDirections<Dim>::crosstrack_angle;
    xt::xtensor<float, Dim> range; ///< in m, accumulated ray path time

    using SampleDirections<Dim>::size;
    using SampleDirections<Dim>::shape;

    /**
     * @brief Construct a new SampleDirections object (all values set to 0)
     *
     */
    SampleDirectionsRange() = default;

    /**
     * @brief Construct a new SampleDirections object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    SampleDirectionsRange(const std::array<size_t, Dim>& shape)
        : SampleDirections<Dim>(shape)
    {
        range = xt::xtensor<float, Dim>::from_shape(shape);
    }

    /**
     * @brief Construct a new SampleDirections object (from a SampleDirections object)
     *
     * @param range in m, accumulated ray path time
     *
     */
    SampleDirectionsRange(SampleDirections<Dim> sample_directions, xt::xtensor<float, Dim> range)
        : SampleDirections<Dim>(std::move(sample_directions))
        , range(std::move(range))
    {
        check_shape();
    }

    /**
     * @brief Construct a new SampleDirectionsRange object
     *
     * @param alongtrack_angle in °, positive bow up, 0 == downwards
     * @param crosstrack_angle in °, positive starboard up, 0 == downwards
     * @param range in m, accumulated ray path length
     */
    SampleDirectionsRange(xt::xtensor<float, Dim> alongtrack_angle_,
                          xt::xtensor<float, Dim> crosstrack_angle_,
                          xt::xtensor<float, Dim> range_)
        : SampleDirections<Dim>(std::move(alongtrack_angle_), std::move(crosstrack_angle_))
        , range(std::move(range_))
    {
        // compare shape
        check_shape();
    }

    bool operator==(const SampleDirectionsRange& rhs) const = default;

  public:
    // ----- file I/O -----
    static SampleDirectionsRange from_stream(std::istream& is)
    {
        SampleDirectionsRange data(SampleDirections<Dim>::from_stream(is));

        is.read(reinterpret_cast<char*>(data.range.data()), sizeof(float) * data.range.size());

        data.check_shape();
        return data;
    }

    void to_stream(std::ostream& os) const
    {
        SampleDirections<Dim>::to_stream(os);

        os.write(reinterpret_cast<const char*>(range.data()), sizeof(float) * range.size());
    }

  public:
    void check_shape() const override
    {
        // compare shape
        if (alongtrack_angle.shape() != range.shape() || crosstrack_angle.shape() != range.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirectionsRange::SampleDirectionsRange: alongtrack_angle, crosstrack_angle "
                "and range must have the same shape. alongtrack_angle.size() = {}, "
                "crosstrack_angle.size() = {}, "
                "range.size() = {}",
                alongtrack_angle.size(),
                crosstrack_angle.size(),
                range.size()));
        }
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("SampleDirectionsRange", float_precision, superscript_exponents);

        printer.register_container("alongtrack_angle", alongtrack_angle, "°");
        printer.register_container("crosstrack_angle", crosstrack_angle, "°");
        printer.register_container("range", range, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SampleDirectionsRange)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__

  private:
    /**
     * @brief Construct object from an existing SampleDirections object (leaving range
     * uninitialized)
     *
     * Note: this is only used internally
     *
     * @param SampleDirections sample directions object
     *
     */
    SampleDirectionsRange(SampleDirections<Dim> sample_directions)
        : SampleDirections<Dim>(std::move(sample_directions))
        , range(xt::xtensor<float, Dim>::from_shape(alongtrack_angle.shape()))
    {
    }
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping