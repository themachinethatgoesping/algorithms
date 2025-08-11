// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampledirections.doc.hpp"

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>




#include "raytraceresult.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store beamsample directions (along angle, across angle).
 */
template<size_t Dim>
struct SampleDirections
{
  public:
    xt::xtensor<float, Dim> alongtrack_angle; ///< in °, positive bow up, 0 == downwards
    xt::xtensor<float, Dim> crosstrack_angle; ///< in °, positive starboard up, 0 == downwards
    
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
    SampleDirections(const std::array<size_t, Dim>& shape)
    {
        alongtrack_angle = xt::xtensor<float, Dim>::from_shape(shape);
        crosstrack_angle = xt::xtensor<float, Dim>::from_shape(shape);
        SampleDirections::check_shape();
    }

    /**
     * @brief Construct a new SampleDirections object
     *
     * @param alongtrack_angle in °, positive bow up, 0 == downwards
     * @param crosstrack_angle in °, positive starboard up, 0 == downwards
     */
    SampleDirections(xt::xtensor<float, Dim> alongtrack_angle_,
                     xt::xtensor<float, Dim> crosstrack_angle_)
        : alongtrack_angle(std::move(alongtrack_angle_))
        , crosstrack_angle(std::move(crosstrack_angle_))
    {
        SampleDirections::check_shape();
    }

    bool operator==(const SampleDirections& rhs) const = default;

    size_t size() const
    {
        check_shape();

        return alongtrack_angle.size();
    }

    std::array<size_t, Dim> shape() const
    {
        check_shape();

        return alongtrack_angle.shape();
    }

  public:
    // ----- file I/O -----
    static SampleDirections from_stream(std::istream& is)
    {
        SampleDirections data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.alongtrack_angle = xt::xtensor<float, Dim>::from_shape(shape);
        data.crosstrack_angle = xt::xtensor<float, Dim>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data.alongtrack_angle.data()),
                sizeof(float) * data.alongtrack_angle.size());
        is.read(reinterpret_cast<char*>(data.crosstrack_angle.data()),
                sizeof(float) * data.crosstrack_angle.size());

        data.check_shape();
        return data;
    }

    void to_stream(std::ostream& os) const
    {
        check_shape();

        std::array<size_t, Dim> shape = alongtrack_angle.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(alongtrack_angle.data()),
                 sizeof(float) * alongtrack_angle.size());
        os.write(reinterpret_cast<const char*>(crosstrack_angle.data()),
                 sizeof(float) * crosstrack_angle.size());
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("SampleDirections", float_precision, superscript_exponents);

        printer.register_container("alongtrack_angle", alongtrack_angle, "°");
        printer.register_container("crosstrack_angle", crosstrack_angle, "°");

        return printer;
    }

  public:
    /**
     * @brief check if the internal variables have the same shape
     *
     */
    virtual void check_shape() const
    {
        // compare shape
        if (alongtrack_angle.shape() != crosstrack_angle.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleDirections::SampleDirections: alongtrack_angle, crosstrack_angle must have "
                "the same shape. alongtrack_angle.size() = {}, "
                "crosstrack_angle.size() = {}",
                alongtrack_angle.size(),
                crosstrack_angle.size()));
        }
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