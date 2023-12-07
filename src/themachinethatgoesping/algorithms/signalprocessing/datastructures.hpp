#pragma once

/* generated doc strings */
#include ".docstrings/datastructures.doc.hpp"

#include <variant>

#include "datastructures/genericsignalparameters.hpp"
#include "datastructures/cwsignalparameters.hpp"
#include "datastructures/fmsignalparameters.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

using TxSignalParameters = std::variant<CWSignalParameters, FMSignalParameters, GenericSignalParameters>;


} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping