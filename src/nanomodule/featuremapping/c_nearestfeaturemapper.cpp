// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "module.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>

#include <themachinethatgoesping/algorithms/featuremapping/nearestfeaturemapper.hpp>
#include <themachinethatgoesping/tools_nanobind/classhelper.hpp>

// -- include nanobind headers
#include <themachinethatgoesping/tools_nanobind/pytensor_nanobind.hpp>

namespace nb = nanobind;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_featuremapping {

#define DOC_NearestFeatureMapper(ARG)                                                              \
    DOC(themachinethatgoesping, algorithms, featuremapping, NearestFeatureMapper, ARG)

// -- create submodule --
void init_c_nearestfeaturemapper(nb::module_& m)
{
    using namespace featuremapping;

    auto c_nearestfeaturemapper =
        nb::class_<NearestFeatureMapper>(
            m,
            "NearestFeatureMapper",
            DOC(themachinethatgoesping, algorithms, featuremapping, NearestFeatureMapper))
            // Constructors
            .def(nb::init<const std::map<std::string, std::vector<double>>&>(),
                 DOC_NearestFeatureMapper(NearestFeatureMapper),
                 nb::arg("features") = std::map<std::string, std::vector<double>>{})
            // Feature management methods
            .def("set_feature",
                 &NearestFeatureMapper::set_feature,
                 DOC_NearestFeatureMapper(set_feature),
                 nb::arg("feature"),
                 nb::arg("values"))
            .def("remove_feature",
                 &NearestFeatureMapper::remove_feature,
                 DOC_NearestFeatureMapper(remove_feature),
                 nb::arg("feature"))
            .def("clear_features",
                 &NearestFeatureMapper::clear_features,
                 DOC_NearestFeatureMapper(clear_features))
            .def("has_feature",
                 &NearestFeatureMapper::has_feature,
                 DOC_NearestFeatureMapper(has_feature),
                 nb::arg("feature"))
            // Converter methods
            .def("feature_to_index",
                 static_cast<size_t (NearestFeatureMapper::*)(const std::string&, double) const>(
                     &NearestFeatureMapper::feature_to_index),
                 DOC_NearestFeatureMapper(feature_to_index),
                 nb::arg("feature"),
                 nb::arg("value"))
            .def("index_to_feature",
                 static_cast<double (NearestFeatureMapper::*)(const std::string&, size_t) const>(
                     &NearestFeatureMapper::index_to_feature),
                 DOC_NearestFeatureMapper(index_to_feature),
                 nb::arg("feature"),
                 nb::arg("index"))
            .def("feature_to_feature",
                 static_cast<double (NearestFeatureMapper::*)(
                     const std::string&, const std::string&, double) const>(
                     &NearestFeatureMapper::feature_to_feature),
                 DOC_NearestFeatureMapper(feature_to_feature),
                 nb::arg("feature_from"),
                 nb::arg("feature_to"),
                 nb::arg("value"))
            // vectorized versions
            .def("feature_to_index",
                 static_cast<xt::nanobind::pytensor<size_t, 1> (NearestFeatureMapper::*)(
                     const std::string&, const xt::nanobind::pytensor<double, 1>&, const int) const>(
                     &NearestFeatureMapper::feature_to_index),
                 DOC_NearestFeatureMapper(feature_to_index_2),
                 nb::arg("feature"),
                 nb::arg("values"),
                 nb::arg("mp_cores") = 1)
            .def("index_to_feature",
                 static_cast<xt::nanobind::pytensor<double, 1> (NearestFeatureMapper::*)(
                     const std::string&, const xt::nanobind::pytensor<size_t, 1>&, const int) const>(
                     &NearestFeatureMapper::index_to_feature),
                 DOC_NearestFeatureMapper(index_to_feature_2),
                 nb::arg("feature"),
                 nb::arg("indices"),
                 nb::arg("mp_cores") = 1)
            .def("feature_to_feature",
                 static_cast<xt::nanobind::pytensor<double, 1> (NearestFeatureMapper::*)(
                     const std::string&,
                     const std::string&,
                     const xt::nanobind::pytensor<double, 1>&,
                     const int) const>(&NearestFeatureMapper::feature_to_feature),
                 DOC_NearestFeatureMapper(feature_to_feature_2),
                 nb::arg("feature_from"),
                 nb::arg("feature_to"),
                 nb::arg("values"),
                 nb::arg("mp_cores") = 1)
            // Getter methods
            .def("get_feature_values",
                 &NearestFeatureMapper::get_feature_values,
                 DOC_NearestFeatureMapper(get_feature_values),
                 nb::arg("feature"))
            .def("get_feature_indices",
                 &NearestFeatureMapper::get_feature_indices,
                 DOC_NearestFeatureMapper(get_feature_indices),
                 nb::arg("feature"))
            .def("keys", &NearestFeatureMapper::keys, DOC_NearestFeatureMapper(keys))
            // ----- operators -----
            .def("__eq__",
                 &NearestFeatureMapper::operator==,
                 DOC_NearestFeatureMapper(operator_eq),
                 nb::arg("other"))
        // ----- pybind macros -----
        // default copy functions
        __PYCLASS_DEFAULT_COPY__(NearestFeatureMapper)
        // default binary functions
        __PYCLASS_DEFAULT_BINARY__(NearestFeatureMapper)
        // default printing functions
        __PYCLASS_DEFAULT_PRINTING__(NearestFeatureMapper)
        // end ForwardGridder1D
        ;
    // Vectorized methods
    // add_vectorization_functions<float>(c_nearestfeaturemapper);
    // add_vectorization_functions<double>(c_nearestfeaturemapper);
}

}
}
}
}
