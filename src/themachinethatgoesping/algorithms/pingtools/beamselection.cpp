// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "beamselection.hpp"
#include <numeric>
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pingtools {

BeamSelection::BeamSelection(size_t number_of_beams)
    : _beam_numbers(std::vector<uint32_t>(number_of_beams))
{
    std::iota(std::begin(_beam_numbers),
              std::end(_beam_numbers),
              0); // Fill vector with 0, 1, ..., number_of_beams-1.
}

BeamSelection::BeamSelection(std::vector<uint32_t> beam_numbers)
    : _beam_numbers(std::move(beam_numbers))
{
}

BeamSelection BeamSelection::from_stream(std::istream& is)
{
    using themachinethatgoesping::tools::classhelper::stream::container_from_stream;

    BeamSelection object;
    object._beam_numbers = container_from_stream<std::vector<uint32_t>>(is);

    return object;
}

void BeamSelection::to_stream(std::ostream& os) const
{
    using themachinethatgoesping::tools::classhelper::stream::container_to_stream;

    container_to_stream(os, _beam_numbers);
}

tools::classhelper::ObjectPrinter BeamSelection::__printer__(unsigned int float_precision, bool superscript_exponents) const
{
    using themachinethatgoesping::tools::classhelper::ObjectPrinter;

    ObjectPrinter printer("BeamSelection", float_precision, superscript_exponents);

    printer.register_container("_beam_numbers", _beam_numbers);

    return printer;
}

} // namespace themachinethatgoesping
} // namespace algorithms
} // namespace pingtools
