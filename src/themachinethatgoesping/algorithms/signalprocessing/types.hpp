#pragma once

/* generated doc strings */
#include ".docstrings/types.doc.hpp"

#include <magic_enum/magic_enum.hpp>
#include <variant>

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

inline std::string_view to_string(t_TxSignalType tx_signal_type)
{
    return magic_enum::enum_name(tx_signal_type);
}

inline t_TxSignalType tx_signal_type_from_string(std::string_view tx_signal_type_string)
{
    return magic_enum::enum_cast<t_TxSignalType>(tx_signal_type_string).value();
}

} // namespace types
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
