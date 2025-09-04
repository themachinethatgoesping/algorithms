// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/functions/resamplingfunctions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

#define DOC_gridding_functions(ARG)                                                                \
    DOC(themachinethatgoesping, algorithms, gridding, functions, ARG)

template<typename t_float>
void init_resamplingfunctions(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace gridding::functions;

    m.def("compute_resampled_coordinates",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            size_t>(&compute_resampled_coordinates<xt::pytensor<t_float, 1>>),
          DOC_gridding_functions(compute_resampled_coordinates),
          py::arg("values_min").noconvert(),
          py::arg("values_max").noconvert(),
          py::arg("values_res").noconvert(),
          py::arg("grid_min")  = std::numeric_limits<t_float>::quiet_NaN(),
          py::arg("grid_max")  = std::numeric_limits<t_float>::quiet_NaN(),
          py::arg("max_steps") = 1024);
}

void init_f_resamplingfunctions(pybind11::module& m)
{
    init_resamplingfunctions<float>(m);
    init_resamplingfunctions<double>(m);
}

} // namespace py_functions
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping