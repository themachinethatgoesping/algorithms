// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/xyz.doc.hpp"

#include <utility> // std::pair

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <GeographicLib/UTMUPS.hpp>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store a georeferenced sample location. It is used as
 * output for the raytracers functions. This object stores local x (forward
 * coordinate), y (starboard coordinate) and z (depth). These coordinates
 * can be converted to UTM or Lat/Lon if a reference position (for coordinate 0)
 * is known.
 */
template<size_t Dim>
struct XYZ
{
  public:
    xt::xtensor<float, Dim> x; ///< x coordinate in m, positive forward
    xt::xtensor<float, Dim> y; ///< y coordinate in m, positive starboard
    xt::xtensor<float, Dim> z; ///< z coordinate in m, positive downwards

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    XYZ() = default;

    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    XYZ(const std::array<size_t, Dim>& shape)
    {
        x = xt::xtensor<float, Dim>::from_shape(shape);
        y = xt::xtensor<float, Dim>::from_shape(shape);
        z = xt::xtensor<float, Dim>::from_shape(shape);
    }

    // return shape of the internal tensors
    // type is: std::array<long unsigned int, Dim>
    const auto& shape() const { return x.shape(); }

    /**
     * @brief Construct a new XYZ object
     *
     * @param x in m, positive forward
     * @param y in m, positive starboard
     * @param z in m, positive downwards
     */
    XYZ(xt::xtensor<float, Dim> x_, xt::xtensor<float, Dim> y_, xt::xtensor<float, Dim> z_)
        : x(std::move(x_))
        , y(std::move(y_))
        , z(std::move(z_))
    {
        check_shape();
    }

    bool operator==(const XYZ& rhs) const = default;

    size_t size() const { return x.size(); }

    /**
     * @brief Concatenate multiple XYZ objects
     * Note: the dimensionality of the XYZ objects will be lost (transformed to XYZ<1>)
     *
     * @param vector of XYZ objects
     * @return XYZ<1>
     */
    static XYZ<1> concat(const std::vector<std::shared_ptr<const XYZ<Dim>>>& xyzs)
    {
        // create a new XYZ object with the summed size of all previous xyz objects
        size_t size = 0;
        for (const auto& xyz : xyzs)
            size += xyz->size();
        XYZ<1> xyz({ size });

        // copy the data from the xyz objects to the new xyz object
        // use std::copy
        auto itx = xyz.x.begin();
        auto ity = xyz.y.begin();
        auto itz = xyz.z.begin();
        for (const auto& xyz_ : xyzs)
        {
            itx = std::copy(xyz_->x.begin(), xyz_->x.end(), itx);
            ity = std::copy(xyz_->y.begin(), xyz_->y.end(), ity);
            itz = std::copy(xyz_->z.begin(), xyz_->z.end(), itz);
        }

        return xyz;
    }

    /**
     * @brief Rotate the XYZ object using a quaternion
     *
     * @param q quaternion
     *
     */
    void rotate(const Eigen::Quaternionf& q)
    {
        // rotate the xyz object
        for (size_t i = 0; i < size(); ++i)
        {
            auto rotated = tools::rotationfunctions::rotateXYZ(
                q, x.unchecked(i), y.unchecked(i), z.unchecked(i));
            x.unchecked(i) = rotated[0];
            y.unchecked(i) = rotated[1];
            z.unchecked(i) = rotated[2];
        }
    }

    /**
     * @brief Rotate the XYZ object using yaw, pitch, roll in °
     *
     * @param yaw in °
     * @param pitch in °
     * @param roll in °
     *
     */
    void rotate(float yaw, float pitch, float roll)
    {
        // rotate the xyz object
        for (size_t i = 0; i < size(); ++i)
        {
            auto rotated = tools::rotationfunctions::rotateXYZ(
                tools::rotationfunctions::quaternion_from_ypr(yaw, pitch, roll),
                x.unchecked(i),
                y.unchecked(i),
                z.unchecked(i));
            x.unchecked(i) = rotated[0];
            y.unchecked(i) = rotated[1];
            z.unchecked(i) = rotated[2];
        }
    }

    void translate(float x_, float y_, float z_)
    {
        // translate the xyz object
        for (size_t i = 0; i < size(); ++i)
        {
            x.unchecked(i) += x_;
            y.unchecked(i) += y_;
            z.unchecked(i) += z_;
        }
    }

    // ----- some convenient math -----
    std::array<float, 2> get_minmax_x() const { return xt::minmax(x)(); }
    std::array<float, 2> get_minmax_y() const { return xt::minmax(y)(); }
    std::array<float, 2> get_minmax_z() const { return xt::minmax(z)(); }

  public:
    // ----- file I/O -----
    static XYZ from_stream(std::istream& is)
    {
        XYZ data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.x = xt::xtensor<float, Dim>::from_shape(shape);
        data.y = xt::xtensor<float, Dim>::from_shape(shape);
        data.z = xt::xtensor<float, Dim>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data.x.data()), sizeof(float) * data.x.size());
        is.read(reinterpret_cast<char*>(data.y.data()), sizeof(float) * data.y.size());
        is.read(reinterpret_cast<char*>(data.z.data()), sizeof(float) * data.z.size());

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        // compare sizes
        if (x.shape() != y.shape() || x.shape() != z.shape())
        {
            throw std::runtime_error(fmt::format("XYZ::to_stream: x, y, z must have the same "
                                                 "shape. "
                                                 "x.size() = {}, y.size() = {}, z.size() = {}",
                                                 x.size(),
                                                 y.size(),
                                                 z.size()));
        }

        std::array<size_t, Dim> shape = x.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(x.data()), sizeof(float) * x.size());
        os.write(reinterpret_cast<const char*>(y.data()), sizeof(float) * y.size());
        os.write(reinterpret_cast<const char*>(z.data()), sizeof(float) * z.size());
    }

  private:
    virtual void check_shape() const
    {
        // compare shape
        if (x.shape() != y.shape() || x.shape() != z.shape())
        {
            throw std::runtime_error(fmt::format("XYZ: x, y, z must have the same shape. "
                                                 "x.size() = {}, y.size() = {}, z.size() = {}",
                                                 x.size(),
                                                 y.size(),
                                                 z.size()));
        }
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("XYZ", float_precision, superscript_exponents);

        printer.register_container("x", x, "positive forward, m");
        printer.register_container("y", y, "positive starboard, m");
        printer.register_container("z", z, "positive downwards, m");

        return printer;
    }

    std::pair<xt::xtensor<double, Dim>, xt::xtensor<double, Dim>> to_latlon(
        int  utm_zone,
        bool northern_hemisphere) const
    {
        auto LaLoZ = std::make_pair<xt::xtensor<double, Dim>, xt::xtensor<double, Dim>>(
            xt::xtensor<double, Dim>::from_shape(x.shape()),
            xt::xtensor<double, Dim>::from_shape(y.shape()));

        for (size_t i = 0; i < size(); ++i)
        {
            GeographicLib::UTMUPS::Reverse(utm_zone,
                                           northern_hemisphere,
                                           y.unchecked(i),
                                           x.unchecked(i),
                                           LaLoZ.first.unchecked(i),
                                           LaLoZ.second.unchecked(i));
        }

        return LaLoZ;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(XYZ)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping