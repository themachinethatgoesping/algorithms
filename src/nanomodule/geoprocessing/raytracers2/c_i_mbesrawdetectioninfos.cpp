// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/raytracers2/i_mbesrawdetectioninfos.hpp"
#include "py_i_mbesrawdetectioninfos.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_geoprocessing {
namespace py_raytracers2 {

void init_c_i_mbesrawdetectioninfos(nanobind::module_& m)
{
    namespace nb = nanobind;
    using namespace themachinethatgoesping::algorithms::geoprocessing::raytracers2;

    auto cls = nb::class_<I_MBESRawDetectionInfos>(
        m,
        "I_MBESRawDetectionInfos",
        DOC(themachinethatgoesping, algorithms, geoprocessing, raytracers2, I_MBESRawDetectionInfos));

    add_I_MBESRawDetectionInfos_interface<I_MBESRawDetectionInfos>(cls);

    cls __PYCLASS_DEFAULT_PRINTING__(I_MBESRawDetectionInfos);
}

} // namespace py_raytracers2
} // namespace py_geoprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping
