// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp>

#include <themachinethatgoesping/algorithms/gridding/functions/gridfunctions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

#define DOC_functions(ARG) DOC(themachinethatgoesping, algorithms, gridding, functions, ARG)

template<typename t_float>
void init_gridfunctions(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace gridding::functions;

    m.def("get_minmax",
          py::overload_cast<const xt::pytensor<t_float, 1>&, const int>(
              &get_minmax<xt::pytensor<t_float, 1>>),
          DOC_functions(get_minmax),
          py::arg("sb").noconvert(),
          py::arg("mp_cores") = 1);
    m.def("get_minmax",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const int>(&get_minmax<xt::pytensor<t_float, 1>>),
          DOC_functions(get_minmax_2),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("mp_cores") = 1);
    m.def("get_minmax",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const int>(&get_minmax<xt::pytensor<t_float, 1>>),
          DOC_functions(get_minmax_3),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sz").noconvert(),
          py::arg("mp_cores") = 1);

    m.def("get_index",
          py::overload_cast<const t_float, const t_float, const t_float>(&get_index<t_float>),
          DOC_functions(get_minmax),
          py::arg("val"),
          py::arg("grd_val_min"),
          py::arg("grd_res"));

    m.def("group_blocks",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xt::pytensor<t_float, 1>, t_float, int>),
          DOC_functions(group_blocks),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sz").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"),
          py::arg("zmin"),
          py::arg("zres"),
          py::arg("nz"));

    m.def("group_blocks",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xt::pytensor<t_float, 1>, t_float, int>),
          DOC_functions(group_blocks_2),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"));

    m.def("group_blocks",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xt::pytensor<t_float, 1>, t_float, int>),
          DOC_functions(group_blocks_3),
          py::arg("sx").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"));

    m.def("get_index_fraction",
          py::overload_cast<const t_float, const t_float, const t_float>(
              &get_index_fraction<t_float>),
          DOC_functions(get_index_fraction),
          py::arg("val"),
          py::arg("grd_val_min"),
          py::arg("grd_res"));

    m.def("get_value",
          py::overload_cast<const t_float, const t_float, const t_float>(&get_value<t_float>),
          DOC_functions(get_value),
          py::arg("index"),
          py::arg("grd_val_min"),
          py::arg("grd_res"));

    m.def("get_grd_value",
          py::overload_cast<const t_float, const t_float, const t_float>(&get_grd_value<t_float>),
          DOC_functions(get_grd_value),
          py::arg("value"),
          py::arg("grd_val_min"),
          py::arg("grd_res"));

    m.def(
        "get_index_weights",
        py::overload_cast<const t_float, const t_float, const t_float>(&get_index_weights<t_float>),
        DOC_functions(get_index_weights),
        py::arg("frac_x"),
        py::arg("frac_y"),
        py::arg("frac_z"));
    m.def("get_index_weights",
          py::overload_cast<const t_float, const t_float>(&get_index_weights<t_float>),
          DOC_functions(get_index_weights_2),
          py::arg("frac_x"),
          py::arg("frac_y"));
    m.def("get_index_weights",
          py::overload_cast<const t_float>(&get_index_weights<t_float>),
          DOC_functions(get_index_weights_3),
          py::arg("frac_x"));

    m.def("grd_weighted_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 3>&,
                            xt::pytensor<t_float, 3>&>(
              &grd_weighted_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 3>, t_float, int>),
          DOC_functions(grd_weighted_mean),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sz").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"),
          py::arg("zmin"),
          py::arg("zres"),
          py::arg("nz"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());
    m.def("grd_weighted_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 2>&,
                            xt::pytensor<t_float, 2>&>(
              &grd_weighted_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 2>, t_float, int>),
          DOC_functions(grd_weighted_mean_2),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());

    m.def("grd_weighted_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 1>&,
                            xt::pytensor<t_float, 1>&>(
              &grd_weighted_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 1>, t_float, int>),
          DOC_functions(grd_weighted_mean_3),
          py::arg("sx").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());

    m.def("grd_block_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 3>&,
                            xt::pytensor<t_float, 3>&>(
              &grd_block_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 3>, t_float, int>),
          DOC_functions(grd_block_mean),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sz").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"),
          py::arg("zmin"),
          py::arg("zres"),
          py::arg("nz"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());
    m.def("grd_block_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 2>&,
                            xt::pytensor<t_float, 2>&>(
              &grd_block_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 2>, t_float, int>),
          DOC_functions(grd_block_mean_2),
          py::arg("sx").noconvert(),
          py::arg("sy").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("ymin"),
          py::arg("yres"),
          py::arg("ny"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());
    m.def("grd_block_mean",
          py::overload_cast<const xt::pytensor<t_float, 1>&,
                            const xt::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            xt::pytensor<t_float, 1>&,
                            xt::pytensor<t_float, 1>&>(
              &grd_block_mean<xt::pytensor<t_float, 1>, xt::pytensor<t_float, 1>, t_float, int>),
          DOC_functions(grd_block_mean_3),
          py::arg("sx").noconvert(),
          py::arg("sv").noconvert(),
          py::arg("xmin"),
          py::arg("xres"),
          py::arg("nx"),
          py::arg("image_values").noconvert(),
          py::arg("image_weights").noconvert());
}

void init_f_gridfunctions(pybind11::module& m)
{
    init_gridfunctions<float>(m);
    init_gridfunctions<double>(m);
}

} // namespace py_functions
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping