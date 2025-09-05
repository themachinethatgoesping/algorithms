// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "nearestfeaturemapper.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <map>
#include <numeric>
#include <stdexcept>

namespace themachinethatgoesping {
namespace algorithms {
namespace featuremapping {

// ReversableNearestInterpolator implementation
ReversableNearestInterpolator::ReversableNearestInterpolator(const std::vector<double>& values)
{
    std::vector<size_t> indices(values.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::vector<double> indices_float(values.size());
    std::iota(indices_float.begin(), indices_float.end(), 0);

    _forward_interpolator =
        tools::vectorinterpolators::NearestInterpolator<double, size_t>(values, indices);
    _reverse_interpolator =
        tools::vectorinterpolators::NearestInterpolator<double, double>(indices_float, values);
}

size_t ReversableNearestInterpolator::get_index(double x) const
{
    return _forward_interpolator(x);
}

double ReversableNearestInterpolator::get_value(double idx) const
{
    return _reverse_interpolator(idx);
}

const std::vector<double>& ReversableNearestInterpolator::get_value_data() const
{
    return _forward_interpolator.get_data_X();
}

const std::vector<size_t>& ReversableNearestInterpolator::get_index_data() const
{
    return _forward_interpolator.get_data_Y();
}

// NearestFeatureMapper implementation
void NearestFeatureMapper::set_feature(const std::string&         feature,
                                       const std::vector<double>& values)
{
    if (values.size() < 2)
        throw std::invalid_argument(
            fmt::format("Feature '{}' must have at least two values!", feature));

    _features[feature] = ReversableNearestInterpolator(values);
}

void NearestFeatureMapper::remove_feature(const std::string& feature)
{
    _features.erase(feature);
}

void NearestFeatureMapper::clear_features()
{
    _features.clear();
}

bool NearestFeatureMapper::has_feature(const std::string& feature) const
{
    return _features.find(feature) != _features.end();
}

size_t NearestFeatureMapper::feature_to_index(const std::string& feature, double value) const
{
    auto it = _features.find(feature);
    if (it == _features.end())
        throw std::runtime_error("ERROR[NearestFeatureMapper::feature_to_index]: feature '" +
                                 feature + "' not found!");

    return it->second.get_index(value);
}

double NearestFeatureMapper::index_to_feature(const std::string& feature, size_t index) const
{
    auto it = _features.find(feature);
    if (it == _features.end())
        throw std::runtime_error("ERROR[NearestFeatureMapper::index_to_feature]: feature '" +
                                 feature + "' not found!");

    return it->second.get_value(index);
}

double NearestFeatureMapper::feature_to_feature(const std::string& feature_from,
                                                const std::string& feature_to,
                                                double             value) const
{
    return index_to_feature(feature_to, feature_to_index(feature_from, value));
}

std::optional<std::vector<double>> NearestFeatureMapper::get_feature_values(
    const std::string& feature) const
{
    if (has_feature(feature))
        return _features.at(feature).get_value_data();
    return std::nullopt;
}

std::optional<std::vector<size_t>> NearestFeatureMapper::get_feature_indices(
    const std::string& feature) const
{
    if (has_feature(feature))
        return _features.at(feature).get_index_data();
    return std::nullopt;
}

std::vector<std::string> NearestFeatureMapper::keys() const
{
    std::vector<std::string> keys;
    for (const auto& [key, _] : _features)
        keys.push_back(key);
    return keys;
}

void NearestFeatureMapper::to_stream(std::ostream& os) const
{
    auto keys = this->keys();
    tools::classhelper::stream::container_container_to_stream<1>(os, this->keys());

    for (const auto& key : keys)
    {
        tools::classhelper::stream::container_to_stream(os, _features.at(key).get_value_data());
    }
}

NearestFeatureMapper NearestFeatureMapper::from_stream(std::istream& is)
{
    NearestFeatureMapper data;
    auto keys = tools::classhelper::stream::container_container_from_stream<1, std::vector<std::string>>(is);
    for (const auto& key : keys)
    {
        data.set_feature(
            key, tools::classhelper::stream::container_from_stream<std::vector<double>>(is));
    }

    return data;
}

bool NearestFeatureMapper::operator==(const NearestFeatureMapper& other) const
{
    if (_features.size() != other._features.size())
        return false;

    for (const auto& [key, interpolator] : _features)
    {
        auto it = other._features.find(key);
        if (it == other._features.end())
            return false;

        if (interpolator.get_value_data() != it->second.get_value_data())
            return false;
    }

    return true;
}

tools::classhelper::ObjectPrinter NearestFeatureMapper::__printer__(
    unsigned int float_precision,
    bool         superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer(
        "NearestFeatureMapper", float_precision, superscript_exponents);
    printer.register_section("features");
    for (const auto& key : this->keys())
        printer.register_container(key, this->get_feature_values(key).value());

    return printer;
}

} // namespace featuremapping
} // namespace algorithms
} // namespace themachinethatgoesping
