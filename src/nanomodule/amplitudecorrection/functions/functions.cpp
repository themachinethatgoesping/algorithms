// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <optional>

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>

#include <themachinethatgoesping/tools_nanobind/nanobind_xtensor.hpp>
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

#include <themachinethatgoesping/algorithms/amplitudecorrection/functions.hpp>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_amplitudecorrection {
namespace py_functions {

#define DOC_amplitudecorrection_functions(ARG)                                                                         \
    DOC(themachinethatgoesping, algorithms, amplitudecorrection, functions, ARG)

template<typename t_float>
void init_functions(nb::module_& m)
{
    using namespace amplitudecorrection::functions;

    // sound velocity
    m.def("calc_sound_velocity",
          &calc_sound_velocity,
          DOC_amplitudecorrection_functions(calc_sound_velocity),
          nb::arg("depth_m"),
          nb::arg("temperature_c"),
          nb::arg("salinity_psu"),
          nb::arg("latitude")  = 0.0,
          nb::arg("longitude") = 0.0);

    // absorption
    m.def("calc_absorption_coefficient_db_m",
          &calc_absorption_coefficient_db_m,
          DOC_amplitudecorrection_functions(calc_absorption_coefficient_db_m),
          nb::arg("frequency_hz"),
          nb::arg("depth_m"),
          nb::arg("sound_velocity_m_s"),
          nb::arg("temperature_c"),
          nb::arg("salinity_psu"),
          nb::arg("pH") = 8);

    // range correction
    m.def("get_sample_numbers_plus_half",
        &get_sample_numbers_plus_half<xt::xtensor<t_float, 1>, int32_t>,
          DOC_amplitudecorrection_functions(get_sample_numbers_plus_half),
          nb::arg("first_sample_nr"),
          nb::arg("last_sample_nr"),
          nb::arg("step") = 1);

    m.def("approximate_range_factor",
          &approximate_range_factor<t_float>,
          DOC_amplitudecorrection_functions(approximate_range_factor),
          nb::arg("sample_interval_s"),
          nb::arg("sound_velocity_m_s"));

    m.def("approximate_ranges",
          nb::overload_cast<t_float, t_float, int32_t, int32_t, int32_t>(
              &approximate_ranges<xt::xtensor<t_float, 1>, int32_t>),
          DOC_amplitudecorrection_functions(approximate_ranges),
          nb::arg("sample_interval_s"),
          nb::arg("sound_velocity_m_s"),
          nb::arg("first_sample_nr"),
          nb::arg("last_sample_nr"),
          nb::arg("step") = 1);

    m.def("approximate_ranges",
          nb::overload_cast<t_float, t_float, const xt::xtensor<int32_t, 1>&>(
              &approximate_ranges<xt::xtensor<t_float, 1>, xt::xtensor<int32_t, 1>>),
          DOC_amplitudecorrection_functions(approximate_ranges_2),
          nb::arg("sample_interval_s"),
          nb::arg("sound_velocity_m_s"),
          nb::arg("sample_numbers"));

    m.def("compute_cw_range_correction",
          &compute_cw_range_correction<xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(compute_cw_range_correction),
          nb::arg("ranges_m"),
          nb::arg("absorption_db_m"),
          nb::arg("tvg_factor"));

    m.def("apply_beam_sample_correction",
          &apply_beam_sample_correction<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_sample_correction),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);

    m.def("apply_beam_correction",
          &apply_beam_correction<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_correction),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("mp_cores") = 1);

    m.def("apply_sample_correction",
          &apply_sample_correction<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_sample_correction),
          nb::arg("wci"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);

    m.def("apply_system_offset",
          &apply_system_offset<xt::xtensor<t_float, 2>>,
          DOC_amplitudecorrection_functions(apply_sample_correction),
          nb::arg("wci"),
          nb::arg("system_offset"),
          nb::arg("mp_cores") = 1);

    m.def(
        "inplace_beam_sample_correction",
        // &inplace_beam_sample_correction<xt::pytensor<t_float, 2>, xt::pytensor<t_float,1 >> ,
        // use lambda here because for some reason xt::pytensor is seen as reference from python on
        // xt::pytensor& is not seen as reference
          [](xt::nanobind::pytensor<t_float, 2>        wci,
              const xt::nanobind::pytensor<t_float, 1>& per_beam_offset,
              const xt::nanobind::pytensor<t_float, 1>& per_sample_offset,
           std::optional<size_t>           min_beam_index = std::nullopt,
           std::optional<size_t>           max_beam_index = std::nullopt,
           int                             mp_cores       = 1) {
            inplace_beam_sample_correction(
                wci, per_beam_offset, per_sample_offset, min_beam_index, max_beam_index, mp_cores);
        },
        DOC_amplitudecorrection_functions(inplace_beam_sample_correction),
    nb::arg("wci").noconvert(),
    nb::arg("per_beam_offset"),
    nb::arg("per_sample_offset"),
    nb::arg("min_beam_index") = std::nullopt,
    nb::arg("max_beam_index") = std::nullopt,
    nb::arg("mp_cores")       = 1);

    m.def(
        "inplace_beam_correction",
        //&inplace_beam_correction<xt::pytensor<t_float, 2>, xt::pytensor<t_float, 1>>,
        // use lambda here because for some reason xt::pytensor is seen as reference from python on
        // xt::pytensor& is not seen as reference
          [](xt::nanobind::pytensor<t_float, 2>        wci,
              const xt::nanobind::pytensor<t_float, 1>& per_beam_offset,
           std::optional<size_t>           min_beam_index = std::nullopt,
           std::optional<size_t>           max_beam_index = std::nullopt,
           int                             mp_cores       = 1) {
            inplace_beam_correction(wci, per_beam_offset, min_beam_index, max_beam_index, mp_cores);
        },
        DOC_amplitudecorrection_functions(inplace_beam_correction),
    nb::arg("wci").noconvert(),
    nb::arg("per_beam_offset"),
    nb::arg("min_beam_index") = std::nullopt,
    nb::arg("max_beam_index") = std::nullopt,
    nb::arg("mp_cores")       = 1);

    m.def(
        "inplace_sample_correction",
        //&inplace_sample_correction<xt::pytensor<t_float, 2>, xt::pytensor<t_float, 1>>,
        // use lambda here because for some reason xt::pytensor is seen as reference from python on
        // xt::pytensor& is not seen as reference
          [](xt::nanobind::pytensor<t_float, 2>        wci,
              const xt::nanobind::pytensor<t_float, 1>& per_sample_offset,
           std::optional<size_t>           min_beam_index = std::nullopt,
           std::optional<size_t>           max_beam_index = std::nullopt,
           int                             mp_cores       = 1) {
            inplace_sample_correction(
                wci, per_sample_offset, min_beam_index, max_beam_index, mp_cores);
        },
        DOC_amplitudecorrection_functions(inplace_sample_correction),
    nb::arg("wci").noconvert(),
    nb::arg("per_sample_offset"),
    nb::arg("min_beam_index") = std::nullopt,
    nb::arg("max_beam_index") = std::nullopt,
    nb::arg("mp_cores")       = 1);

    m.def(
        "inplace_system_offset",
        //&inplace_system_offset<xt::pytensor<t_float, 2>>,
        // use lambda here because for some reason xt::pytensor is seen as reference from python on
        // xt::pytensor& is not seen as reference
    [](xt::nanobind::pytensor<t_float, 2> wci,
           t_float                  system_offset,
           std::optional<size_t>    min_beam_index = std::nullopt,
           std::optional<size_t>    max_beam_index = std::nullopt,
           int                      mp_cores       = 1) {
            inplace_system_offset(wci, system_offset, min_beam_index, max_beam_index, mp_cores);
        },
        DOC_amplitudecorrection_functions(inplace_system_offset),
    nb::arg("wci").noconvert(),
    nb::arg("system_offset"),
    nb::arg("min_beam_index") = std::nullopt,
    nb::arg("max_beam_index") = std::nullopt,
    nb::arg("mp_cores")       = 1);

    m.def("apply_beam_sample_correction_loop",
          &apply_beam_sample_correction_loop<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_sample_correction_loop),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);

    m.def("apply_beam_sample_correction_xtensor2",
          &apply_beam_sample_correction_xtensor2<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_sample_correction_xtensor2),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);
    m.def("apply_beam_sample_correction_xtensor3",
          &apply_beam_sample_correction_xtensor3<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_sample_correction_xtensor3),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);

    m.def("apply_beam_sample_correction_xsimd",
          &apply_beam_sample_correction_xsimd<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 1>>,
          DOC_amplitudecorrection_functions(apply_beam_sample_correction_xsimd),
          nb::arg("wci"),
          nb::arg("per_beam_offset"),
          nb::arg("per_sample_offset"),
          nb::arg("mp_cores") = 1);
}

void init_m_functions(nb::module_& m)
{
    nb::module_ submodule = m.def_submodule("functions");

    submodule.doc() = "M that holds functions used for amplitude corrections";

    init_functions<float>(submodule);
    init_functions<double>(submodule);
}

} // namespace py_functions
} // namespace py_amplitudecorrection
} // namespace pymodule
} // namespace algorithms
} // namespace themachinethatgoesping