// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/samplelocationslocal.doc.hpp"

#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store a georeferenced sample location. It is used as
 * output for the raytracers functions. This object stores local x (forward
 * coordinate), y (starboard coordinate) depth and true range. These coordinates
 * can be converted to UTM or Lat/Lon if a reference position (for coordinate 0)
 * is known.
 */
template<size_t Dim>
struct SampleLocationsLocal
{
  public:
    xt::xtensor<float, Dim> x;          ///< in m, positive forward
    xt::xtensor<float, Dim> y;          ///< in m, positive starboard
    xt::xtensor<float, Dim> z;          ///< in m, positive downwards
    xt::xtensor<float, Dim> true_range; ///< in m, accumulated ray path length

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    SampleLocationsLocal() = default;

    /**
     * @brief Construct a new SampleLocationsLocal object
     *
     * @param x in m, positive forward
     * @param y in m, positive starboard
     * @param z in m, positive downwards
     * @param true_range in m, accumulated ray path length
     */
    SampleLocationsLocal(xt::xtensor<float, Dim> x_,
                         xt::xtensor<float, Dim> y_,
                         xt::xtensor<float, Dim> z_,
                         xt::xtensor<float, Dim> true_range_)
        : x(std::move(x_))
        , y(std::move(y_))
        , z(std::move(z_))
        , true_range(std::move(true_range_))
    {
        // compare shape
        if (x.shape() != y.shape() || x.shape() != z.shape() || x.shape() != true_range.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleLocationsLocal: x, y, z and true_range must have the same shape. "
                "x.size() = {}, y.size() = {}, z.size() = {}, true_range.size() = {}",
                x.size(),
                y.size(),
                z.size(),
                true_range.size()));
        }
    }

    bool operator==(const SampleLocationsLocal& rhs) const = default;

  public:
    // ----- file I/O -----
    static SampleLocationsLocal from_stream(std::istream& is)
    {
        SampleLocationsLocal data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.x          = xt::xtensor<float, Dim>::from_shape(shape);
        data.y          = xt::xtensor<float, Dim>::from_shape(shape);
        data.z          = xt::xtensor<float, Dim>::from_shape(shape);
        data.true_range = xt::xtensor<float, Dim>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data.x.data()), sizeof(float) * data.x.size());
        is.read(reinterpret_cast<char*>(data.y.data()), sizeof(float) * data.y.size());
        is.read(reinterpret_cast<char*>(data.z.data()), sizeof(float) * data.z.size());
        is.read(reinterpret_cast<char*>(data.true_range.data()),
                sizeof(float) * data.true_range.size());

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        // compare sizes
        if (x.shape() != y.shape() || x.shape() != z.shape() || x.shape() != true_range.shape())
        {
            throw std::runtime_error(fmt::format(
                "SampleLocationsLocal::to_stream: x, y, z and true_range must have the same shape. "
                "x.size() = {}, y.size() = {}, z.size() = {}, true_range.size() = {}",
                x.size(),
                y.size(),
                z.size(),
                true_range.size()));
        }

        std::array<size_t, Dim> shape = x.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(x.data()), sizeof(float) * x.size());
        os.write(reinterpret_cast<const char*>(y.data()), sizeof(float) * y.size());
        os.write(reinterpret_cast<const char*>(z.data()), sizeof(float) * z.size());
        os.write(reinterpret_cast<const char*>(true_range.data()),
                 sizeof(float) * true_range.size());
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("SampleLocationsLocal", float_precision);

        printer.register_container("x", x, "positive forward, m");
        printer.register_container("y", y, "positive starboard, m");
        printer.register_container("z", z, "positive downwards, m");
        printer.register_container("true_range", true_range, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SampleLocationsLocal)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping