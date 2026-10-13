// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "readsamplerange.hpp"
#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pingtools {

// Implementation file for readsamplerange.hpp

ReadSampleRange ReadSampleRange::from_stream(std::istream& is)
{
    ReadSampleRange object;
    is.read(reinterpret_cast<char*>(&object), sizeof(object));

    return object;
}

void ReadSampleRange::to_stream(std::ostream& os) const
{
    // write other variables
    os.write(reinterpret_cast<const char*>(this), sizeof(ReadSampleRange));
}

tools::classhelper::ObjectPrinter ReadSampleRange::__printer__(unsigned int float_precision, bool superscript_exponents) const
{
    using themachinethatgoesping::tools::classhelper::ObjectPrinter;

    ObjectPrinter printer("ReadSampleRange", float_precision, superscript_exponents);

    printer.register_value("first_sample_to_read", _first_sample_to_read, "beamlocal");
    printer.register_value(
        "number_of_samples_to_read", _number_of_samples_to_read, "beamlocal");
    printer.register_value(
        "first_read_sample_offset", _first_read_sample_offset, "swathglobal");
    printer.register_value("last_read_sample_offset", _last_read_sample_offset, "swathglobal");

    return printer;
}

} // namespace themachinethatgoesping
} // namespace algorithms
} // namespace pingtools
