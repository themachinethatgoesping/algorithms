// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_imageprocessing {
namespace py_functions {

#define DOC_functions(ARG) DOC(themachinethatgoesping, algorithms, imageprocessing, functions, ARG)

template<typename t_float>
void init_find_local_maxima(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace imageprocessing::functions;

    // find_local_maxima
    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 3>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 3>>),
          DOC_functions(find_local_maxima),
          py::arg("data"),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 2>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 2>>),
          DOC_functions(find_local_maxima_2),
          py::arg("data"),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

    m.def("find_local_maxima",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const std::optional<t_float>,
                            const bool,
                            const int>(&find_local_maxima<xt::pytensor<t_float, 1>>),
          DOC_functions(find_local_maxima_3),
          py::arg("data"),
          py::arg("threshold")   = std::nullopt,
          py::arg("accept_nans") = true,
          py::arg("mp_cores")    = 1);

          // find_local_maxima2
    m.def("find_local_maxima2",
      &find_local_maxima2<xt::pytensor<t_float, 3>>,
      DOC_functions(find_local_maxima2),
      py::arg("data"),
      py::arg("threshold")   = std::nullopt,
      py::arg("accept_nans") = true,
      py::arg("mp_cores")    = 1);

m.def("find_local_maxima2",
      &find_local_maxima2<xt::pytensor<t_float, 2>>,
      DOC_functions(find_local_maxima2_2),
      py::arg("data"),
      py::arg("threshold")   = std::nullopt,
      py::arg("accept_nans") = true,
      py::arg("mp_cores")    = 1);

m.def("find_local_maxima2",
      &find_local_maxima2<xt::pytensor<t_float, 1>>,
      DOC_functions(find_local_maxima2_3),
      py::arg("data"),
      py::arg("threshold")   = std::nullopt,
      py::arg("accept_nans") = true,
      py::arg("mp_cores")    = 1);
}

void init_m_functions(pybind11::module& m)
{
    pybind11::module submodule = m.def_submodule("functions");

    submodule.doc() = "M that holds functions used for amplitude corrections";

    init_find_local_maxima<double>(submodule);
    init_find_local_maxima<float>(submodule);
}

} // namespace py_functions
} // namespace py_imageprocessing
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping