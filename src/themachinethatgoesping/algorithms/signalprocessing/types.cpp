// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "types.hpp"

// Explicit template instantiation for o_TxSignalType
template class themachinethatgoesping::tools::classhelper::OptionFrozen<
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_values.size(),
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_values,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_names,
    themachinethatgoesping::algorithms::signalprocessing::types::t_TxSignalType_alt_names>;
