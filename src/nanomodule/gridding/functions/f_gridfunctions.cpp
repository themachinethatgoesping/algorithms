// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <nanobind/nanobind.h>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/gridding/functions/gridfunctions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_gridding {
namespace py_functions {

#define DOC_gridding_functions(ARG) DOC(themachinethatgoesping, algorithms, gridding, functions, ARG)

template<typename t_float>
void init_gridfunctions(nanobind::module_& m)
{
    namespace nb = nanobind;
    namespace xnb = xt::nanobind;
    using namespace gridding::functions;

    m.def("get_minmax",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&, const int>(
              &get_minmax<xnb::pytensor<t_float, 1>>),
          DOC_gridding_functions(get_minmax),
          nb::arg("sb").noconvert(),
          nb::arg("mp_cores") = 1);
    m.def("get_minmax",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const int>(&get_minmax<xnb::pytensor<t_float, 1>>),
          DOC_gridding_functions(get_minmax_2),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("mp_cores") = 1);
    m.def("get_minmax",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const int>(&get_minmax<xnb::pytensor<t_float, 1>>),
          DOC_gridding_functions(get_minmax_3),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sz").noconvert(),
          nb::arg("mp_cores") = 1);

    m.def("get_index",
          nb::overload_cast<const t_float, const t_float, const t_float>(&get_index<t_float>),
          DOC_gridding_functions(get_minmax),
          nb::arg("val"),
          nb::arg("grd_val_min"),
          nb::arg("grd_res"));

    m.def("group_blocks",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xnb::pytensor<t_float, 1>, t_float, int>),
          DOC_gridding_functions(group_blocks),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sz").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"),
          nb::arg("zmin"),
          nb::arg("zres"),
          nb::arg("nz"));

    m.def("group_blocks",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xnb::pytensor<t_float, 1>, t_float, int>),
          DOC_gridding_functions(group_blocks_2),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"));

    m.def("group_blocks",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int>(&group_blocks<xnb::pytensor<t_float, 1>, t_float, int>),
          DOC_gridding_functions(group_blocks_3),
          nb::arg("sx").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"));

    m.def("get_index_fraction",
          nb::overload_cast<const t_float, const t_float, const t_float>(
              &get_index_fraction<t_float>),
          DOC_gridding_functions(get_index_fraction),
          nb::arg("val"),
          nb::arg("grd_val_min"),
          nb::arg("grd_res"));

    m.def("get_value",
          nb::overload_cast<const t_float, const t_float, const t_float>(&get_value<t_float>),
          DOC_gridding_functions(get_value),
          nb::arg("index"),
          nb::arg("grd_val_min"),
          nb::arg("grd_res"));

    m.def("get_grd_value",
          nb::overload_cast<const t_float, const t_float, const t_float>(&get_grd_value<t_float>),
          DOC_gridding_functions(get_grd_value),
          nb::arg("value"),
          nb::arg("grd_val_min"),
          nb::arg("grd_res"));

    m.def(
        "get_index_weights",
        nb::overload_cast<const t_float, const t_float, const t_float>(&get_index_weights<t_float>),
        DOC_gridding_functions(get_index_weights),
        nb::arg("frac_x"),
        nb::arg("frac_y"),
        nb::arg("frac_z"));
    m.def("get_index_weights",
          nb::overload_cast<const t_float, const t_float>(&get_index_weights<t_float>),
          DOC_gridding_functions(get_index_weights_2),
          nb::arg("frac_x"),
          nb::arg("frac_y"));
    m.def("get_index_weights",
          nb::overload_cast<const t_float>(&get_index_weights<t_float>),
          DOC_gridding_functions(get_index_weights_3),
          nb::arg("frac_x"));

    m.def("grd_weighted_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 3>&,
                            xnb::pytensor<t_float, 3>&>(
              &grd_weighted_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 3>, t_float, int>),
          DOC_gridding_functions(grd_weighted_mean),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sz").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"),
          nb::arg("zmin"),
          nb::arg("zres"),
          nb::arg("nz"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());
    m.def("grd_weighted_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 2>&,
                            xnb::pytensor<t_float, 2>&>(
              &grd_weighted_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 2>, t_float, int>),
          DOC_gridding_functions(grd_weighted_mean_2),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());

    m.def("grd_weighted_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 1>&,
                            xnb::pytensor<t_float, 1>&>(
              &grd_weighted_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 1>, t_float, int>),
          DOC_gridding_functions(grd_weighted_mean_3),
          nb::arg("sx").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());

    m.def("grd_block_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 3>&,
                            xnb::pytensor<t_float, 3>&>(
              &grd_block_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 3>, t_float, int>),
          DOC_gridding_functions(grd_block_mean),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sz").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"),
          nb::arg("zmin"),
          nb::arg("zres"),
          nb::arg("nz"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());
    m.def("grd_block_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 2>&,
                            xnb::pytensor<t_float, 2>&>(
              &grd_block_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 2>, t_float, int>),
          DOC_gridding_functions(grd_block_mean_2),
          nb::arg("sx").noconvert(),
          nb::arg("sy").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("ymin"),
          nb::arg("yres"),
          nb::arg("ny"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());
    m.def("grd_block_mean",
          nb::overload_cast<const xnb::pytensor<t_float, 1>&,
                            const xnb::pytensor<t_float, 1>&,
                            const t_float,
                            const t_float,
                            const int,
                            xnb::pytensor<t_float, 1>&,
                            xnb::pytensor<t_float, 1>&>(
              &grd_block_mean<xnb::pytensor<t_float, 1>, xnb::pytensor<t_float, 1>, t_float, int>),
          DOC_gridding_functions(grd_block_mean_3),
          nb::arg("sx").noconvert(),
          nb::arg("sv").noconvert(),
          nb::arg("xmin"),
          nb::arg("xres"),
          nb::arg("nx"),
          nb::arg("image_values").noconvert(),
          nb::arg("image_weights").noconvert());
}

void init_f_gridfunctions(nanobind::module_& m)
{
    init_gridfunctions<float>(m);
    init_gridfunctions<double>(m);
}

} // namespace py_functions
} // namespace py_gridding
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping