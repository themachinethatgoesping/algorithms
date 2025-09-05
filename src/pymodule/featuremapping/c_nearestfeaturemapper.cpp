// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "module.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <themachinethatgoesping/algorithms/featuremapping/nearestfeaturemapper.hpp>
#include <themachinethatgoesping/tools_pybind/classhelper.hpp>

// -- include pybind11 headers
#include <pybind11/stl.h>
#include <xtensor-python/pytensor.hpp> // Numpy bindings

namespace py = pybind11;

namespace themachinethatgoesping {
namespace algorithms {
namespace pymodule {
namespace py_featuremapping {

#define DOC_NearestFeatureMapper(ARG)                                                              \
    DOC(themachinethatgoesping, algorithms, featuremapping, NearestFeatureMapper, ARG)

// -- create submodule --
void init_c_nearestfeaturemapper(py::module& m)
{
    using namespace featuremapping;
    namespace py = pybind11;

    auto c_nearestfeaturemapper =
        py::class_<NearestFeatureMapper>(
            m,
            "NearestFeatureMapper",
            DOC(themachinethatgoesping, algorithms, featuremapping, NearestFeatureMapper))
            // Constructors
            .def(py::init<const std::map<std::string, std::vector<double>>&>(),
                 DOC_NearestFeatureMapper(NearestFeatureMapper),
                 py::arg("features") = std::map<std::string, std::vector<double>>{})
            // Feature management methods
            .def("set_feature",
                 &NearestFeatureMapper::set_feature,
                 DOC_NearestFeatureMapper(set_feature),
                 py::arg("feature"),
                 py::arg("values"))
            .def("remove_feature",
                 &NearestFeatureMapper::remove_feature,
                 DOC_NearestFeatureMapper(remove_feature),
                 py::arg("feature"))
            .def("clear_features",
                 &NearestFeatureMapper::clear_features,
                 DOC_NearestFeatureMapper(clear_features))
            .def("has_feature",
                 &NearestFeatureMapper::has_feature,
                 DOC_NearestFeatureMapper(has_feature),
                 py::arg("feature"))
            // Converter methods
            .def("feature_to_index",
                 static_cast<size_t (NearestFeatureMapper::*)(const std::string&, double) const>(
                     &NearestFeatureMapper::feature_to_index),
                 DOC_NearestFeatureMapper(feature_to_index),
                 py::arg("feature"),
                 py::arg("value"))
            .def("index_to_feature",
                 static_cast<double (NearestFeatureMapper::*)(const std::string&, size_t) const>(
                     &NearestFeatureMapper::index_to_feature),
                 DOC_NearestFeatureMapper(index_to_feature),
                 py::arg("feature"),
                 py::arg("index"))
            .def("feature_to_feature",
                 static_cast<double (NearestFeatureMapper::*)(
                     const std::string&, const std::string&, double) const>(
                     &NearestFeatureMapper::feature_to_feature),
                 DOC_NearestFeatureMapper(feature_to_feature),
                 py::arg("feature_from"),
                 py::arg("feature_to"),
                 py::arg("value"))
            // vectorized versions
            .def("feature_to_index",
                 static_cast<xt::pytensor<size_t, 1> (NearestFeatureMapper::*)(
                     const std::string&, const xt::pytensor<double, 1>&, const int) const>(
                     &NearestFeatureMapper::feature_to_index),
                 DOC_NearestFeatureMapper(feature_to_index_2),
                 py::arg("feature"),
                 py::arg("values"),
                 py::arg("mp_cores") = 1)
            .def("index_to_feature",
                 static_cast<xt::pytensor<double, 1> (NearestFeatureMapper::*)(
                     const std::string&, const xt::pytensor<size_t, 1>&, const int) const>(
                     &NearestFeatureMapper::index_to_feature),
                 DOC_NearestFeatureMapper(index_to_feature_2),
                 py::arg("feature"),
                 py::arg("indices"),
                 py::arg("mp_cores") = 1)
            .def("feature_to_feature",
                 static_cast<xt::pytensor<double, 1> (NearestFeatureMapper::*)(
                     const std::string&,
                     const std::string&,
                     const xt::pytensor<double, 1>&,
                     const int) const>(&NearestFeatureMapper::feature_to_feature),
                 DOC_NearestFeatureMapper(feature_to_feature_2),
                 py::arg("feature_from"),
                 py::arg("feature_to"),
                 py::arg("values"),
                 py::arg("mp_cores") = 1)
            // Getter methods
            .def("get_feature_values",
                 &NearestFeatureMapper::get_feature_values,
                 DOC_NearestFeatureMapper(get_feature_values),
                 py::arg("feature"))
            .def("get_feature_indices",
                 &NearestFeatureMapper::get_feature_indices,
                 DOC_NearestFeatureMapper(get_feature_indices),
                 py::arg("feature"))
            .def("keys", &NearestFeatureMapper::keys, DOC_NearestFeatureMapper(keys))
            // ----- operators -----
            .def("__eq__",
                 &NearestFeatureMapper::operator==,
                 DOC_NearestFeatureMapper(operator_eq),
                 py::arg("other"))
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
