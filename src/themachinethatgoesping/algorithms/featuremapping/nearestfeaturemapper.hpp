// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/nearestfeaturemapper.doc.hpp"

#include <fmt/format.h>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/vectorinterpolators/nearestinterpolator.hpp>

/**
 * @brief Index converter for ping data coordinate transformations
 *
 */

namespace themachinethatgoesping {
namespace algorithms {
namespace featuremapping {

class ReversableNearestInterpolator
{
    tools::vectorinterpolators::NearestInterpolator<double, size_t> _forward_interpolator;
    tools::vectorinterpolators::NearestInterpolator<double, double> _reverse_interpolator;

  public:
    ReversableNearestInterpolator() = default;
    ReversableNearestInterpolator(const std::vector<double>& values);

    size_t                     get_index(double x) const;
    double                     get_value(double idx) const;
    const std::vector<double>& get_value_data() const;
    const std::vector<size_t>& get_index_data() const;
};

class NearestFeatureMapper
{
    /* x/time/ping_nr coordinate conversion */
    std::map<std::string, ReversableNearestInterpolator> _features;

  public:
    NearestFeatureMapper(const std::map<std::string, std::vector<double>>& features = {})
    {
        for (const auto& [name, values] : features)
            set_feature(name, values);
    }

    // set features
    void set_feature(const std::string& feature, const std::vector<double>& values);
    void remove_feature(const std::string& feature);
    void clear_features();
    bool has_feature(const std::string& feature) const;

    // converters
    size_t feature_to_index(const std::string& feature, double value) const;
    double index_to_feature(const std::string& feature, size_t index) const;
    double feature_to_feature(const std::string& feature_from,
                              const std::string& feature_to,
                              double             value) const;

    template<tools::helper::c_xtensor_1d t_index, tools::helper::c_xtensor_1d t_values>
    t_index feature_to_index(const std::string& feature,
                             const t_values&    values,
                             const int          mp_cores = 1) const
    {
        t_index indices = xt::empty<typename t_index::value_type>(values.shape());

        auto it = _features.find(feature);
        if (it == _features.end())
            throw std::runtime_error("ERROR[NearestFeatureMapper::index_to_feature]: feature '" +
                                     feature + "' not found!");

#pragma omp parallel for if (mp_cores > 1) num_threads(mp_cores)
        for (size_t i = 0; i < values.size(); ++i)
            indices[i] = it->second.get_index(values[i]);

        return indices;
    }

    template<tools::helper::c_xtensor_1d t_values, tools::helper::c_xtensor_1d t_index>
    t_values index_to_feature(const std::string& feature,
                              const t_index&     indices,
                              const int          mp_cores = 1) const
    {
        t_values values = xt::empty<typename t_values::value_type>(indices.shape());

        auto it = _features.find(feature);
        if (it == _features.end())
            throw std::runtime_error("ERROR[NearestFeatureMapper::index_to_feature]: feature '" +
                                     feature + "' not found!");

#pragma omp parallel for if (mp_cores > 1) num_threads(mp_cores)
        for (size_t i = 0; i < indices.size(); ++i)
            values[i] = it->second.get_value(indices[i]);

        return values;
    }

    template<tools::helper::c_xtensor_1d t_values>
    t_values feature_to_feature(const std::string& feature_from,
                                const std::string& feature_to,
                                const t_values&    values,
                                const int          mp_cores = 1) const
    {
        t_values result = xt::empty<typename t_values::value_type>(values.shape());

        auto it_from = _features.find(feature_from);
        if (it_from == _features.end())
            throw std::runtime_error("ERROR[NearestFeatureMapper::feature_to_feature]: feature '" +
                                     feature_from + "' not found!");

        auto it_to = _features.find(feature_to);
        if (it_to == _features.end())
            throw std::runtime_error("ERROR[NearestFeatureMapper::feature_to_feature]: feature '" +
                                     feature_to + "' not found!");

#pragma omp parallel for if (mp_cores > 1) num_threads(mp_cores)
        for (size_t i = 0; i < values.size(); ++i)
            result[i] = it_to->second.get_value(it_from->second.get_index(values[i]));

        return result;
    }

    // getters / setters
    std::optional<std::vector<double>> get_feature_values(const std::string& feature) const;

    std::vector<std::string> keys() const;

    // ----- serialization -----
    void to_stream(std::ostream& os) const;

    static NearestFeatureMapper from_stream(std::istream& is);

    bool operator==(const NearestFeatureMapper& other) const;

    // ----- objectprinter -----
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

    // ----- class helper macros -----
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(NearestFeatureMapper)
};

} // namespace featuremapping
} // namespace algorithms
} // namespace themachinethatgoesping