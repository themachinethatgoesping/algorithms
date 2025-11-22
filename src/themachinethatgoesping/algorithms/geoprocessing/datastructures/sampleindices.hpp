// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampleindices.doc.hpp"

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
struct SampleIndices
{
  public:
    xt::xtensor<uint16_t, Dim> beam_numbers; ///< beam number of each sample
    xt::xtensor<uint16_t, Dim> sample_numbers; ///< sample number of each sample

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    SampleIndices() = default;

    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param shape shape of the internal tensors
     *
     */
    SampleIndices(const std::array<size_t, Dim>& shape)
    {
        beam_numbers = xt::xtensor<uint16_t, Dim>::from_shape(shape);
        sample_numbers = xt::xtensor<uint16_t, Dim>::from_shape(shape);
        SampleIndices::check_shape();
    }

    /**
     * @brief Construct a new SampleIndices object
     *
     * @param beam_numbers in °, positive bow up, 0 == downwards
     * @param sample_numbers in °, positive starboard up, 0 == downwards
     */
    SampleIndices(xt::xtensor<uint16_t, Dim> beam_numbers_,
                     xt::xtensor<uint16_t, Dim> sample_numbers_)
        : beam_numbers(std::move(beam_numbers_))
        , sample_numbers(std::move(sample_numbers_))
    {
        SampleIndices::check_shape();
    }

    bool operator==(const SampleIndices& rhs) const = default;

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
    static SampleIndices from_stream(std::istream& is)
    {
        SampleIndices data;

        std::array<size_t, Dim> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data.beam_numbers = xt::xtensor<uint16_t, Dim>::from_shape(shape);
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

        os.write(reinterpret_cast<const char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(beam_numbers.data()),
                 sizeof(uint16_t) * beam_numbers.size());
        os.write(reinterpret_cast<const char*>(sample_numbers.data()),
                 sizeof(uint16_t) * sample_numbers.size());
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("SampleIndices", float_precision, superscript_exponents);

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
                "SampleIndices::SampleIndices: beam_numbers, sample_numbers must have "
                "the same shape. beam_numbers.size() = {}, "
                "sample_numbers.size() = {}",
                beam_numbers.size(),
                sample_numbers.size()));
        }
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SampleIndices)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping