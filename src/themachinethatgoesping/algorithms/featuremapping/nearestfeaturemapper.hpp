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

/**
 * @brief A bidirectional nearest neighbor interpolator for mapping between values and indices
 *
 * This class provides functionality to map between continuous values and discrete indices
 * using nearest neighbor interpolation in both directions (forward and reverse).
 */
class ReversableNearestInterpolator
{
    tools::vectorinterpolators::NearestInterpolator<double, size_t> _forward_interpolator;
    tools::vectorinterpolators::NearestInterpolator<double, double> _reverse_interpolator;

  public:
    /**
     * @brief Default constructor
     */
    ReversableNearestInterpolator() = default;

    /**
     * @brief Construct interpolator from a vector of values
     * @param values Vector of values to create the interpolator from
     */
    ReversableNearestInterpolator(const std::vector<double>& values);

    /**
     * @brief Get the nearest index for a given value
     * @param x The value to find the nearest index for
     * @return The index of the nearest value
     */
    size_t get_index(double x) const;

    /**
     * @brief Get the value at a given index (with interpolation)
     * @param idx The index to get the value for
     * @return The interpolated value at the given index
     */
    double get_value(double idx) const;

    /**
     * @brief Get the underlying value data
     * @return Reference to the vector of values used for interpolation
     */
    const std::vector<double>& get_value_data() const;

    /**
     * @brief Get the underlying index data
     * @return Reference to the vector of indices used for interpolation
     */
    const std::vector<size_t>& get_index_data() const;
};

/**
 * @brief A feature mapper that enables conversion between different feature spaces using nearest
 * neighbor interpolation
 *
 * This class manages multiple named features and provides functionality to map values between
 * different feature spaces or convert feature values to/from indices. It's particularly useful
 * for coordinate transformations and feature space conversions in data processing pipelines.
 */
class NearestFeatureMapper
{
    /* x/time/ping_nr coordinate conversion */
    std::map<std::string, ReversableNearestInterpolator> _features;

  public:
    /**
     * @brief Constructor with optional initial features
     * @param features Map of feature names to their corresponding value vectors
     */
    NearestFeatureMapper(const std::map<std::string, std::vector<double>>& features = {})
    {
        for (const auto& [name, values] : features)
            set_feature(name, values);
    }

    // set features
    /**
     * @brief Add or update a feature with the given values
     * @param feature Name of the feature to set
     * @param values Vector of values for the feature
     * @throws std::invalid_argument if values has less than 2 elements
     */
    void set_feature(const std::string& feature, const std::vector<double>& values);

    /**
     * @brief Remove a feature from the mapper
     * @param feature Name of the feature to remove
     */
    void remove_feature(const std::string& feature);

    /**
     * @brief Clear all features from the mapper
     */
    void clear_features();

    /**
     * @brief Check if a feature exists in the mapper
     * @param feature Name of the feature to check
     * @return true if the feature exists, false otherwise
     */
    bool has_feature(const std::string& feature) const;

    // converters
    /**
     * @brief Convert a feature value to its corresponding index
     * @param feature Name of the feature
     * @param value The value to convert to an index
     * @return The index of the nearest value in the feature
     * @throws std::runtime_error if the feature is not found
     */
    size_t feature_to_index(const std::string& feature, double value) const;

    /**
     * @brief Convert an index to its corresponding feature value
     * @param feature Name of the feature
     * @param index The index to convert to a value
     * @return The interpolated value at the given index
     * @throws std::runtime_error if the feature is not found
     */
    double index_to_feature(const std::string& feature, size_t index) const;

    /**
     * @brief Convert a value from one feature space to another
     * @param feature_from Name of the source feature
     * @param feature_to Name of the target feature
     * @param value The value in the source feature space
     * @return The corresponding value in the target feature space
     * @throws std::runtime_error if either feature is not found
     */
    double feature_to_feature(const std::string& feature_from,
                              const std::string& feature_to,
                              double             value) const;

    /**
     * @brief Convert multiple feature values to their corresponding indices (vectorized)
     * @tparam t_index Type of the output index container (must be xtensor compatible)
     * @tparam t_values Type of the input values container (must be xtensor compatible)
     * @param feature Name of the feature
     * @param values Container of values to convert to indices
     * @param mp_cores Number of cores to use for parallel processing (default: 1)
     * @return Container of indices corresponding to the nearest values
     * @throws std::runtime_error if the feature is not found
     */
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

    /**
     * @brief Convert multiple indices to their corresponding feature values (vectorized)
     * @tparam t_values Type of the output values container (must be xtensor compatible)
     * @tparam t_index Type of the input index container (must be xtensor compatible)
     * @param feature Name of the feature
     * @param indices Container of indices to convert to values
     * @param mp_cores Number of cores to use for parallel processing (default: 1)
     * @return Container of interpolated values at the given indices
     * @throws std::runtime_error if the feature is not found
     */
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

    /**
     * @brief Convert multiple values from one feature space to another (vectorized)
     * @tparam t_values Type of the values container (must be xtensor compatible)
     * @param feature_from Name of the source feature
     * @param feature_to Name of the target feature
     * @param values Container of values in the source feature space
     * @param mp_cores Number of cores to use for parallel processing (default: 1)
     * @return Container of corresponding values in the target feature space
     * @throws std::runtime_error if either feature is not found
     */
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
    /**
     * @brief Get the values for a specific feature
     * @param feature Name of the feature
     * @return Optional vector of feature values, nullopt if feature doesn't exist
     */
    std::optional<std::vector<double>> get_feature_values(const std::string& feature) const;

    /**
     * @brief Get the indices for a specific feature
     * @param feature Name of the feature
     * @return Optional vector of feature indices, nullopt if feature doesn't exist
     */
    std::optional<std::vector<size_t>> get_feature_indices(const std::string& feature) const;

    /**
     * @brief Get all feature names
     * @return Vector of all feature names currently stored in the mapper
     */
    std::vector<std::string> keys() const;

    // ----- serialization -----
    /**
     * @brief Serialize the mapper to an output stream
     * @param os Output stream to write to
     */
    void to_stream(std::ostream& os) const;

    /**
     * @brief Deserialize a mapper from an input stream
     * @param is Input stream to read from
     * @return NearestFeatureMapper object constructed from the stream
     */
    static NearestFeatureMapper from_stream(std::istream& is);

    /**
     * @brief Equality comparison operator
     * @param other Another NearestFeatureMapper to compare with
     * @return true if both mappers have the same features and values, false otherwise
     */
    bool operator==(const NearestFeatureMapper& other) const;

    // ----- objectprinter -----
    /**
     * @brief Create an ObjectPrinter for this mapper
     * @param float_precision Precision for floating point numbers
     * @param superscript_exponents Whether to use superscript for exponents
     * @return ObjectPrinter configured for this mapper
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const;

    // ----- class helper macros -----
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(NearestFeatureMapper)
};

} // namespace featuremapping
} // namespace algorithms
} // namespace themachinethatgoesping