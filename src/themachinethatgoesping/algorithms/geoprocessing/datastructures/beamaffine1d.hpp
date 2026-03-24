// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/beamaffine1d.doc.hpp"

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/core/xnoalias.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A 1D affine transform per beam: value = offset + slope * sample_nr
 *
 * Stores one offset and one slope per beam in contiguous arrays (SoA layout),
 * enabling SIMD vectorization across beams.
 */
struct BeamAffine1D
{
    xt::xtensor<float, 1> offsets; ///< [n_beams] base value per beam
    xt::xtensor<float, 1> slopes;  ///< [n_beams] slope (change per sample) per beam

    /**
     * @brief Construct an empty BeamAffine1D
     */
    BeamAffine1D() = default;

    /**
     * @brief Construct a BeamAffine1D for a given number of beams (uninitialized values)
     *
     * @param n_beams number of beams
     */
    BeamAffine1D(size_t n_beams)
        : offsets(xt::xtensor<float, 1>::from_shape({ n_beams }))
        , slopes(xt::xtensor<float, 1>::from_shape({ n_beams }))
    {
    }

    /**
     * @brief Construct a BeamAffine1D from existing offsets and slopes
     *
     * @param offsets_ base value per beam [n_beams]
     * @param slopes_ slope per beam [n_beams]
     */
    BeamAffine1D(xt::xtensor<float, 1> offsets_, xt::xtensor<float, 1> slopes_)
        : offsets(std::move(offsets_))
        , slopes(std::move(slopes_))
    {
        if (offsets.size() != slopes.size())
            throw std::runtime_error(
                fmt::format("BeamAffine1D: offsets ({}) and slopes ({}) must have same size",
                            offsets.size(),
                            slopes.size()));
    }

    bool operator==(const BeamAffine1D& rhs) const = default;

    size_t size() const { return offsets.size(); }

    /**
     * @brief Compute value = offset + slope * sample_nr for a single beam
     *
     * @param beam_index beam index
     * @param sample_nr sample number (float for sub-sample interpolation)
     * @return float
     */
    float forward(size_t beam_index, float sample_nr) const
    {
        return offsets.unchecked(beam_index) + slopes.unchecked(beam_index) * sample_nr;
    }

    /**
     * @brief Compute sample_nr = (value - offset) / slope for a single beam
     *
     * @param beam_index beam index
     * @param value the value to invert
     * @return float sample number
     */
    float inverse(size_t beam_index, float value) const
    {
        return (value - offsets.unchecked(beam_index)) / slopes.unchecked(beam_index);
    }

    // --- factory functions ---

    /**
     * @brief Create a BeamAffine1D from a base value and per-beam end values at known sample
     * numbers.
     *
     * slope = (end_values - base_value) / end_sample_numbers
     * offset = base_value (for all beams)
     *
     * @param base_value scalar base value (at sample 0)
     * @param end_values per-beam values at the end sample [n_beams]
     * @param end_sample_numbers per-beam sample number of the end values [n_beams]
     * @return BeamAffine1D
     */
    static BeamAffine1D from_base_and_endpoints(float                         base_value,
                                                const xt::xtensor<float, 1>&  end_values,
                                                const xt::xtensor<float, 1>&  end_sample_numbers)
    {
        if (end_values.size() != end_sample_numbers.size())
            throw std::runtime_error(fmt::format(
                "BeamAffine1D::from_base_and_endpoints: end_values ({}) and end_sample_numbers "
                "({}) must have same size",
                end_values.size(),
                end_sample_numbers.size()));

        xt::xtensor<float, 1> offsets_ = xt::ones<float>({ end_values.size() }) * base_value;
        xt::xtensor<float, 1> slopes_  = (end_values - base_value) / end_sample_numbers;

        return BeamAffine1D(std::move(offsets_), std::move(slopes_));
    }

  public:
    // ----- file I/O -----
    static BeamAffine1D from_stream(std::istream& is)
    {
        BeamAffine1D data;

        size_t n_beams;
        is.read(reinterpret_cast<char*>(&n_beams), sizeof(size_t));

        data.offsets = xt::xtensor<float, 1>::from_shape({ n_beams });
        data.slopes  = xt::xtensor<float, 1>::from_shape({ n_beams });

        is.read(reinterpret_cast<char*>(data.offsets.data()), sizeof(float) * n_beams);
        is.read(reinterpret_cast<char*>(data.slopes.data()), sizeof(float) * n_beams);

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        size_t n_beams = offsets.size();
        os.write(reinterpret_cast<const char*>(&n_beams), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(offsets.data()), sizeof(float) * n_beams);
        os.write(reinterpret_cast<const char*>(slopes.data()), sizeof(float) * n_beams);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamAffine1D", float_precision, superscript_exponents);

        printer.register_container("offsets", offsets);
        printer.register_container("slopes", slopes);

        return printer;
    }

  public:
    // -- class helper function macros --
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamAffine1D)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
