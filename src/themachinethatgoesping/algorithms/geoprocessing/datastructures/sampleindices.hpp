// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/sampleindices.doc.hpp"

#include <map>
#include <vector>

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
 * @brief A structure to store sample indices (sample nr, beam nr) for a set of beams. It is used
 * as output for the backmapper functions and as input for the get_wci_amplitude functions.
 */
struct SampleIndices
{
    std::map<unsigned int, std::vector<unsigned int>>
        beam_sample_map; ///< map <beam number, sample_number>

    /**
     * @brief Construct a new SampleIndices object (all values set to 0)
     *
     */
    SampleIndices() = default;

    /**
     * @brief Construct a new SampleIndices object
     *
     * @param beam_sample_map_ map <beam number, sample_number>

     */
    SampleIndices(std::map<unsigned int, std::vector<unsigned int>> beam_sample_map_)
        : beam_sample_map(std::move(beam_sample_map_))
    {
    }

    bool operator==(const SampleIndices& rhs) const = default;

  public:
    // ----- file I/O -----
    static SampleIndices from_stream(std::istream& is)
    {
        SampleIndices data;

        using tools::classhelper::stream::map_container_from_stream;
        data.beam_sample_map = map_container_from_stream<decltype(data.beam_sample_map)>(is);

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        using tools::classhelper::stream::map_container_to_stream;
        map_container_to_stream(os, beam_sample_map);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("SampleIndices", float_precision);

        for (const auto& [bn, sn] : beam_sample_map)
        {
            printer.register_container(fmt::format("Beam number ({})", bn), sn);
        }

        return printer;
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