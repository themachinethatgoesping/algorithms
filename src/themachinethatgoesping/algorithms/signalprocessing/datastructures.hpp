#pragma once

/* generated doc strings */
#include ".docstrings/datastructures.doc.hpp"

#include <variant>

#include "datastructures/cwsignalinfos.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

using TxSignalInfos = std::variant<CWSignalInfos>;


} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping