// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
//

#pragma once

/* generated doc strings */
#include ".docstrings/beamsampleselection.doc.hpp"

/* std includes */
#include <limits>
#include <vector>

/* external includes */
#include <xtensor/containers/xtensor.hpp>

/* ping includes */
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

#include "beamselection.hpp"
#include "readsamplerange.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pingtools {

/**
 * @brief A class to hold the selected beams/sample range for a single transducer
 *
 */
class BeamSampleSelection : public BeamSelection
{
    using BeamSelection::_beam_numbers;                  ///< selected beam numbers
    std::vector<uint32_t> _first_sample_number_per_beam; ///< first sample number per beam
    std::vector<uint32_t> _last_sample_number_per_beam;  ///< last sample number per beam

    uint32_t _sample_step_ensemble = 1; ///< sample step size (same for the entire ensemble)
    uint32_t _first_sample_number_ensemble =
        std::numeric_limits<uint32_t>::max();  ///< minimum sample number
                                               ///< (min(first_sample_number_per_beam))
    uint32_t _last_sample_number_ensemble = 0; ///< maximum sample number
                                               ///< (max(last_sample_number_per_beam))

  public:
    BeamSampleSelection(uint32_t sample_step_ensemble = 1)
        : _sample_step_ensemble(sample_step_ensemble)
    {
    }

    /**
     * @brief Construct a new Beam Sample Selection object
     *
     * @param beam_selection
     */
    BeamSampleSelection(BeamSelection beam_selection);

    /**
     * @brief Initialize a beam sample selection from a whole swath
     *
     * @param first_sample_number_per_beam first sample number per beam
     * @param last_sample_number_per_beam last sample number per beam
     * @param sample_step_ensemble sample step size (same for the entire ensemble)
     */
    BeamSampleSelection(std::vector<uint32_t> first_sample_number_per_beam,
                        std::vector<uint32_t> last_sample_number_per_beam,
                        uint32_t              sample_step_ensemble = 1);

    // --- operators ---
    bool operator==(const BeamSampleSelection& other) const = default;

    // --- get read sample range ---
    /**
     * @brief Return the read sample range for a given beam
     *
     * @param beam_index index of the beam within the beam sample selection
     * @param first_sample_offset offset to the first sample (often 0)
     * @param number_of_samples number of samples in the real beam structure
     * @return ReadSampleRange read sample range
     */
    ReadSampleRange get_read_sample_range(uint32_t beam_index,
                                          uint32_t first_sample_offset_in_beam,
                                          uint32_t number_of_samples_in_beam) const;

    // ----- add beams/samples -----
    /**
     * @brief Add a beam to the selection
     *
     * @param beam_nr beam number
     * @param first_sample_number first sample number to select
     * @param last_sample_number last sample number to select
     */
    void add_beam(uint32_t beam_nr,
                  uint32_t first_sample_number = 0,
                  uint32_t last_sample_number  = std::numeric_limits<uint32_t>::max());

    // ----- convenient data access -----
    using BeamSelection::get_beam_numbers;
    using BeamSelection::get_number_of_beams;

    /**
     * @brief Return the first sample number per beam
     *
     * @return std::vector<uint32_t>
     */
    const std::vector<uint32_t>& get_first_sample_number_per_beam() const
    {
        return _first_sample_number_per_beam;
    }

    /**
     * @brief Return the max number of samples per beam
     *
     * @return std::vector<uint32_t>
     */
    const std::vector<uint32_t>& get_last_sample_number_per_beam() const
    {
        return _last_sample_number_per_beam;
    }

    // ----- ensemble data -----
    void set_sample_step_ensemble(uint32_t sample_step_ensemble)
    {
        _sample_step_ensemble = sample_step_ensemble;
    }

    void set_first_sample_number_ensemble(uint32_t first_sample_number_ensemble)
    {
        _first_sample_number_ensemble = first_sample_number_ensemble;
    }

    void set_last_sample_number_ensemble(uint32_t last_sample_number_ensemble)
    {
        _last_sample_number_ensemble = last_sample_number_ensemble;
    }

    /**
     * @brief Return the sample step size
     *
     * @return uint32_t
     */
    uint32_t get_sample_step_ensemble() const { return _sample_step_ensemble; }

    /**
     * @brief Return the first sample number of the ensemble
     *
     * @return uint32_t
     */
    uint32_t get_first_sample_number_ensemble() const { return _first_sample_number_ensemble; }

    /**
     * @brief Return the last sample number of the ensemble
     *
     * @return uint32_t
     */
    uint32_t get_last_sample_number_ensemble() const { return _last_sample_number_ensemble; }

    /**
     * @brief return the number of samples within the ensemble
     *
     * @return uint32_t
     */
    uint32_t get_number_of_samples_ensemble() const;

    /**
     * @brief return if the selection contains beams and samples
     *
     * @return true
     * @return false
     */
    bool empty() const;

    xt::xtensor<uint32_t, 1> get_sample_numbers_ensemble_1d() const;

    xt::xtensor<uint32_t, 2> get_sample_numbers_ensemble_2d() const;

    // ----- from/to binary -----
    /**
     * @brief Return a BeamSampleSelection read from a binary stream
     *
     * @param is input stream
     * @return BeamSampleSelection
     */
    static BeamSampleSelection from_stream(std::istream& is);

    /**
     * @brief Write a BeamSampleSelection to a binary stream
     *
     * @param os output stream
     */
    void to_stream(std::ostream& os) const;

    // ----- printing -----
    /**
     * @brief Print function, needs __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__ macro
     * See also: classhelper/objectprinter.hpp
     *
     * @param float_precision Precision of floating point numbers
     * @return classhelper::ObjectPrinter
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, 
                                                  bool         superscript_exponents) const;

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs to_stream and from_stream)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSampleSelection)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

/**
 * @brief Provide a boost hash function for BeamSampleSelection
 * - Note: this is needed to use BeamSampleSelection as boost::flyweight
 *
 * @param data
 * @return std::size_t
 */
// IGNORE_DOC:mkd_doc_themachinethatgoesping_algorithms_pingtools_hash_value
inline size_t hash_value(const BeamSampleSelection& data)
{
    return data.binary_hash();
}

} // namespace pingtools
} // namespace algorithms
} // namespace themachinethatgoesping