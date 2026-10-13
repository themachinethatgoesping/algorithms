// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0
//

#pragma once

/* generated doc strings */
#include ".docstrings/beamselection.doc.hpp"

/* std includes */
#include <iostream>
#include <vector>

/* ping includes */
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

#include "readsamplerange.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pingtools {

/**
 * @brief A class to hold the selected beams/sample range for a single transducer
 *
 */
class BeamSelection
{
  protected:
    std::vector<uint32_t> _beam_numbers; ///< selected beam numbers

  public:
    BeamSelection() {}

    /**
     * @brief Initialize a beam sample selection from a whole swath
     *
     * @param number_of_beams number of beams in the swath
     */
    BeamSelection(size_t number_of_beams);

    /**
     * @brief Initialize a beam sample selection from a whole swath
     *
     * @param number_of_beams number of beams in the swath
     */
    BeamSelection(std::vector<uint32_t> beam_numbers);

    // --- operators ---
    bool operator==(const BeamSelection& other) const = default;

    // ----- add beams/samples -----
    /**
     * @brief Add a beam to the selection
     *
     * @param beam_nr beam number
     * @param first_sample_number first sample number to select
     * @param last_sample_number_per_beam last sample number to select
     */
    void add_beam(uint32_t beam_nr) { _beam_numbers.push_back(beam_nr); }

    // ----- convenient data access -----
    /**
     * @brief Return the number of beams
     *
     * @return size_t
     */
    size_t get_number_of_beams() const { return _beam_numbers.size(); }

    /**
     * @brief return if the selection contains beams
     *
     * @return true
     * @return false
     */
    bool empty() const { return _beam_numbers.empty(); }

    /**
     * @brief Return the beam numbers
     *
     * @return std::vector<uint32_t>
     */
    const std::vector<uint32_t>& get_beam_numbers() const { return _beam_numbers; }

    // ----- from/to binary -----
    /**
     * @brief Return a BeamSelection read from a binary stream
     *
     * @param is input stream
     * @return BeamSelection
     */
    static BeamSelection from_stream(std::istream& is);

    /**
     * @brief Write a BeamSelection to a binary stream
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
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSelection)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

/**
 * @brief Provide a boost hash function for BeamSelection
 * - Note: this is needed to use BeamSelection as boost::flyweight
 *
 * @param data
 * @return std::size_t
 */
// IGNORE_DOC:mkd_doc_themachinethatgoesping_algorithms_pingtools_hash_value
inline size_t hash_value(const BeamSelection& data)
{
    return data.binary_hash();
}

} // namespace pingtools
} // namespace algorithms
} // namespace themachinethatgoesping