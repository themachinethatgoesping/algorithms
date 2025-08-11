// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/raytraceresults.doc.hpp"

#include <xtensor/containers/xadapt.hpp>
#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>


#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#include "xyz.hpp"

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
struct RaytraceResults : public XYZ<Dim>
{
  public:
    using t_base = XYZ<Dim>;
    using t_base::shape;
    using t_base::size;
    using t_base::x;
    using t_base::y;
    using t_base::z;

    xt::xtensor<float, Dim> true_range; ///< in m, accumulated ray path length

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    RaytraceResults() = default;

    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    RaytraceResults(std::array<size_t, Dim> shape)
        : t_base(std::move(shape))
    {
        true_range = xt::xtensor<float, Dim>::from_shape(t_base::shape());
    }

    /**
     * @brief Construct a new RaytraceResults object
     *
     * @param x in m, positive forward
     * @param y in m, positive starboard
     * @param z in m, positive downwards
     * @param true_range in m, accumulated ray path length
     */
    RaytraceResults(xt::xtensor<float, Dim> x_,
                    xt::xtensor<float, Dim> y_,
                    xt::xtensor<float, Dim> z_,
                    xt::xtensor<float, Dim> true_range_)
        : t_base(std::move(x_), std::move(y_), std::move(z_))
        , true_range(std::move(true_range_))
    {
        // compare shape
        check_shape();
    }

    /**
     * @brief Construct a new RaytraceResults object
     *
     * @param xyz XYZ object
     * @param true_range in m, accumulated ray path length
     */
    RaytraceResults(t_base xyz, xt::xtensor<float, Dim> true_range_)
        : t_base(std::move(xyz))
        , true_range(std::move(true_range_))
    {
        // compare shape
        check_shape();
    }

    bool operator==(const RaytraceResults& rhs) const = default;

    /**
     * @brief Concatenate multiple RaytraceResults objects
     * Note: the dimensionality of the RaytraceResults objects will be lost (transformed
     * RaytraceResults XYZ<1>)
     *
     * @param vector of RaytraceResults objects
     * @return RaytraceResults<1>
     */
    static RaytraceResults<1> concat(
        const std::vector<std::shared_ptr<const RaytraceResults<Dim>>>& objects)
    {
        // create a new XYZ object with the summed size of all previous xyz objects
        size_t size = 0;
        for (const auto& obj_ : objects)
            size += obj_->size();
        RaytraceResults<1> obj({size});

        // copy the data from the xyz objects to the new xyz object
        // use std::copy
        auto itx = obj.x.begin();
        auto ity = obj.y.begin();
        auto itz = obj.z.begin();
        auto itr = obj.true_range.begin();
        for (const auto& obj_ : objects)
        {
            itx = std::copy(obj_->x.begin(), obj_->x.end(), itx);
            ity = std::copy(obj_->y.begin(), obj_->y.end(), ity);
            itz = std::copy(obj_->z.begin(), obj_->z.end(), itz);
            itr = std::copy(obj_->true_range.begin(), obj_->true_range.end(), itr);
        }

        return obj;
    }

  public:
    // ----- file I/O -----
    static RaytraceResults from_stream(std::istream& is)
    {
        RaytraceResults data(t_base::from_stream(is));

        data.true_range = xt::xtensor<float, Dim>::from_shape(data.shape());

        is.read(reinterpret_cast<char*>(data.true_range.data()),
                sizeof(float) * data.true_range.size());

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        t_base::to_stream(os);
        os.write(reinterpret_cast<const char*>(true_range.data()),
                 sizeof(float) * true_range.size());
    }

  private:
    void check_shape() const override
    {
        // compare shape
        if (t_base::shape() != true_range.shape())
        {
            throw std::runtime_error(fmt::format("XYZ: x, y, z must have the same shape. "
                                                 "xyz.size() = {}, z.size() = {}",
                                                 t_base::size(),
                                                 true_range.size()));
        }
    }

    /**
     * @brief Construct a new Raytrace Results object with an uninitialized true_range tensor
     * This is private and only used for the from_stream function
     *
     * @param xyz
     */
    RaytraceResults(const t_base& xyz)
        : t_base(xyz)
    {
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("RaytraceResults", float_precision, superscript_exponents);

        printer.append(t_base::__printer__(float_precision, superscript_exponents), true);

        printer.register_container("true_range", true_range, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(RaytraceResults)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping