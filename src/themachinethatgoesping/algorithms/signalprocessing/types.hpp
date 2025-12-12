#pragma once

/* generated doc strings */
#include ".docstrings/types.doc.hpp"

#include <array>

#include <magic_enum/magic_enum.hpp>

#include <themachinethatgoesping/tools/classhelper/option_frozen.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace types {

enum class t_TxSignalType
{
    CW,
    FM_UP_SWEEP,
    FM_DOWN_SWEEP,
    UNKNOWN
};

inline constexpr std::array<t_TxSignalType, 4> t_TxSignalType_values = {
    t_TxSignalType::CW,
    t_TxSignalType::FM_UP_SWEEP,
    t_TxSignalType::FM_DOWN_SWEEP,
    t_TxSignalType::UNKNOWN
};

inline constexpr std::array<std::string_view, 4> t_TxSignalType_names = {
    "CW",
    "FM_UP_SWEEP",
    "FM_DOWN_SWEEP",
    "UNKNOWN"
};

inline constexpr std::array<std::string_view, 4> t_TxSignalType_alt_names = {
    "Continuous Wave",
    "Frequency Modulated up sweep",
    "Frequency Modulated down sweep",
    "Unknown"
};

using o_TxSignalType = themachinethatgoesping::tools::classhelper::OptionFrozen<
    t_TxSignalType,
    t_TxSignalType_values.size(),
    t_TxSignalType_values,
    t_TxSignalType_names,
    t_TxSignalType_alt_names>;

} // namespace types
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping

extern template class themachinethatgoesping::tools::classhelper::OptionFrozen<
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_values.size(),
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_values,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_names,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_alt_names>;
