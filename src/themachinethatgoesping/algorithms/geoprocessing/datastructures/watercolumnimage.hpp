// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/watercolumnimage.doc.hpp"

#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#include "raytraceresult.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store a watercolumn image together with the necessary informations for
 * raytracing.
 */
class WaterColumnImage
{
    using LinearInterpolator  = tools::vectorinterpolators::LinearInterpolator;
    using NearestInterpolator = tools::vectorinterpolators::NearestInterpolator;

  public:
    xt::xtensor<float, 2>   _wci;              ///< water column image
    xt::xtensor<float, Dim> beam_cross_angles; ///< cross axis angles in degrees
    xt::xtensor<float, Dim> beam_along_angles; ///< along axis angles in to_degrees
    float sample_interval                      ///< sample interval in seconds

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    WaterColumnImage() = default;

    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    WaterColumnImage(xt::xtensor<float, 2>                           wci,
                     const datastructures::SampleDirectionsRange<1>& beam_reference_directions,
                     const std::vector<uint16_t>&                    beam_reference_sample_numbers,
                     const std::vector<uint16_t>& beam_reference_max_sample_numbers)
    {
        using tools::vectorinterpolators::LinearInterpolator;
        using tools::vectorinterpolators::NearestInterpolator;

        if (beam_reference_directions.size() == 0)
            throw std::runtime_error("lookup: beam_reference_directions is empty");

        if (wci.shape()[0] < beam_reference_directions.size() ||
            wci.shape()[1] < *std::max_element(beam_reference_sample_numbers.begin(),
                                               beam_reference_sample_numbers.end()))
            throw std::runtime_error(
                fmt::format("lookup: wci shape ({}, {}) is smaller then beam_reference shape ({}, "
                            "{}), or beam_reference_sample_numbers",
                            wci.shape()[0],
                            wci.shape()[1],
                            beam_reference_directions.size(),
                            *std::max_element(beam_reference_sample_numbers.begin(),
                                              beam_reference_sample_numbers.end())));

        // datastructures::SampleIndices<2> indices(target_directions.shape());
        auto output_image = xt::xtensor<float, 2>::from_shape(target_directions.shape());

        // sort the beam indices by angle and add them to a nearest interpolator
        auto sorted_beam_index = xt::argsort(beam_reference_directions.crosstrack_angle);
        auto sorted_beam_angles =
            xt::index_view(beam_reference_directions.crosstrack_angle, sorted_beam_index);

        size_t last_index = sorted_beam_angles.size() - 1;
        float  delta_angle =
            (sorted_beam_angles.unchecked(last_index) - sorted_beam_angles.unchecked(0)) /
            (last_index);
        float min_angle = sorted_beam_angles.unchecked(0) - delta_angle / 2;
        float max_angle = sorted_beam_angles.unchecked(last_index) + delta_angle / 2;

        auto bi_interpolator = NearestInterpolator(
            std::vector<double>(sorted_beam_angles.begin(), sorted_beam_angles.end()),
            std::vector<uint16_t>(sorted_beam_index.begin(), sorted_beam_index.end()));

        // create sample interpolator for each beam
        std::vector<LinearInterpolator<double, double>> sample_interpolators;
        sample_interpolators.reserve(beam_reference_directions.size());
        for (size_t bn = 0; bn < beam_reference_directions.size(); ++bn)
        {
            sample_interpolators.emplace_back(
                std::vector<double>{ 0., double(beam_reference_directions.range[bn]) },
                std::vector<double>{ 0., double(beam_reference_sample_numbers[bn]) });
        }

        // loop through all target directions (flattened view)
        // TODO: use openmp for this: the problem isthat the interpolators are not thread safe at
        // the moment
        for (size_t ti = 0; ti < target_directions.size(); ++ti)
        {
            if (target_directions.crosstrack_angle.data()[ti] < min_angle ||
                target_directions.crosstrack_angle.data()[ti] > max_angle)
            {
                output_image.data()[ti] = std::numeric_limits<float>::quiet_NaN();
                continue;
            }

            auto bi = bi_interpolator(target_directions.crosstrack_angle.data()[ti]);

            if (target_directions.range.data()[ti] < 0)
            {
                output_image.data()[ti] = std::numeric_limits<float>::quiet_NaN();
                continue;
            }
            auto si = sample_interpolators[bi](target_directions.range.data()[ti]);

            if (si > beam_reference_max_sample_numbers[bi])
                output_image.data()[ti] = std::numeric_limits<float>::quiet_NaN();

            output_image.data()[ti] = wci.unchecked(bi, si);
        }

        return output_image;
    }

    /**
     * @brief Construct a new WaterColumnImage object
     *
     * @param beam_numbers in °, positive bow up, 0 == downwards
     * @param sample_numbers in °, positive starboard up, 0 == downwards
     */
    WaterColumnImage(xt::xtensor<uint16_t, Dim> beam_numbers_,
                     xt::xtensor<uint16_t, Dim> sample_numbers_)
        : beam_numbers(std::move(beam_numbers_))
        , sample_numbers(std::move(sample_numbers_))
    {
        WaterColumnImage::check_shape();
    }

    bool operator==(const WaterColumnImage& rhs) const = default;

    size_t size() const
    {
        check_shape();

        return beam_numbers.size();
    }

    std::array<size_t, Dim> shape() const
    {
        check_shape();

        return beam_numbers.shape();
    }

  public:
    // ----- file I/O -----
    static WaterColumnImage from_stream(std::istream& is)
    {
        WaterColumnImage data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.beam_numbers   = xt::xtensor<uint16_t, Dim>::from_shape(shape);
        data.sample_numbers = xt::xtensor<uint16_t, Dim>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data.beam_numbers.data()),
                sizeof(uint16_t) * data.beam_numbers.size());
        is.read(reinterpret_cast<char*>(data.sample_numbers.data()),
                sizeof(uint16_t) * data.sample_numbers.size());

        data.check_shape();
        return data;
    }

    void to_stream(std::ostream& os) const
    {
        check_shape();

        std::array<size_t, Dim> shape = beam_numbers.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(beam_numbers.data()),
                 sizeof(uint16_t) * beam_numbers.size());
        os.write(reinterpret_cast<const char*>(sample_numbers.data()),
                 sizeof(uint16_t) * sample_numbers.size());
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int uint16_t_precision) const
    {
        tools::classhelper::ObjectPrinter printer("WaterColumnImage", uint16_t_precision);

        printer.register_container("beam_numbers", beam_numbers, "°");
        printer.register_container("sample_numbers", sample_numbers, "°");

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
        if (beam_numbers.shape() != sample_numbers.shape())
        {
            throw std::runtime_error(fmt::format(
                "WaterColumnImage::WaterColumnImage: beam_numbers, sample_numbers must have "
                "the same shape. beam_numbers.size() = {}, "
                "sample_numbers.size() = {}",
                beam_numbers.size(),
                sample_numbers.size()));
        }
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(WaterColumnImage)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping