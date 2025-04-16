// SPDX-FileCopyrightText: 2024 Peter Urban, Ghent University
// SPDX-FileCopyrightText: 2022 GEOMAR Helmholtz Centre for Ocean Research Kiel
//
// SPDX-License-Identifier: MPL-2.0

/**
 * @brief These functions are used for shape verifications in wcicorrection.hpp
 *
 */

#pragma once

/* generated doc strings */
#include ".docstrings/_shape_assertations.doc.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <fmt/core.h>
#include <optional>
#include <xsimd/xsimd.hpp>
#include <xtensor/containers/xadapt.hpp>



#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/helper/xtensor.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace amplitudecorrection {
namespace functions {

// --- assertions ---

template<size_t axis, tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void assert_wci_axis_shape(const t_xtensor_2d& wci,
                                  const t_xtensor_1d& per_element_offset,
                                  std::string_view    axis_name)
{
    static_assert(tools::helper::c_xtensor_2d<t_xtensor_2d>,
                  "Template parameter must be a 2D tensor");
    static_assert(tools::helper::c_xtensor_1d<t_xtensor_1d>,
                  "Template parameter must be a 1D tensor");

    if (axis >= wci.dimension())
        throw std::invalid_argument(fmt::format("ERROR[{}]: axis {} out of range", __func__, axis));

    if (wci.shape(axis) != per_element_offset.shape(0))
        throw std::invalid_argument(fmt::format("ERROR[{}]: wci.shape({}) [{}] != {}.shape(0) [{}]",
                                                __func__,
                                                axis,
                                                wci.shape(axis),
                                                axis_name,
                                                per_element_offset.shape(0)));
}

template<tools::helper::c_xtensor t_xtensor_2d, tools::helper::c_xtensor t_xtensor_1d>
inline void assert_wci_beam_sample_shape(const t_xtensor_2d& wci,
                                         const t_xtensor_1d& per_beam_offset,
                                         const t_xtensor_1d& per_sample_offset)
{
    assert_wci_axis_shape<0>(wci, per_beam_offset, "per_beam_offset");
    assert_wci_axis_shape<1>(wci, per_sample_offset, "per_sample_offset");
}

}
}
}
}
