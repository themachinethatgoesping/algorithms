//sourcehash: f30c576b62d1a9d66ee33c2e698a87138a02643c16a7b93a6dd5fb9e6a6ac284

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__
#define __DOCSTRINGS_HPP__

#define MKD_EXPAND(x)                                      x
#define MKD_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...)  COUNT
#define MKD_VA_SIZE(...)                                   MKD_EXPAND(MKD_COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define MKD_CAT1(a, b)                                     a ## b
#define MKD_CAT2(a, b)                                     MKD_CAT1(a, b)
#define MKD_DOC1(n1)                                       mkd_doc_##n1
#define MKD_DOC2(n1, n2)                                   mkd_doc_##n1##_##n2
#define MKD_DOC3(n1, n2, n3)                               mkd_doc_##n1##_##n2##_##n3
#define MKD_DOC4(n1, n2, n3, n4)                           mkd_doc_##n1##_##n2##_##n3##_##n4
#define MKD_DOC5(n1, n2, n3, n4, n5)                       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5
#define MKD_DOC6(n1, n2, n3, n4, n5, n6)                   mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define MKD_DOC7(n1, n2, n3, n4, n5, n6, n7)               mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define MKD_DOC8(n1, n2, n3, n4, n5, n6, n7, n8)           mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define MKD_DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)       mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define MKD_DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) mkd_doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...)                                           MKD_EXPAND(MKD_EXPAND(MKD_CAT2(MKD_DOC, MKD_VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#endif // __DOCSTRINGS_HPP__
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper =
R"doc(A feature mapper that enables conversion between different feature
spaces using nearest neighbor interpolation

This class manages multiple named features and provides functionality
to map values between different feature spaces or convert feature
values to/from indices. It's particularly useful for coordinate
transformations and feature space conversions in data processing
pipelines.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_NearestFeatureMapper =
R"doc(Constructor with optional initial features
Args:
    features: Map of feature names to their corresponding value
              vectors)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_clear_features = R"doc(Clear all features from the mapper)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_feature_to_feature =
R"doc(Convert a value from one feature space to another
Args:
    feature_from: Name of the source feature
    feature_to: Name of the target feature
    value: The value in the source feature space

Returns:
    The corresponding value in the target feature space

Raises:
    std::runtime_error: if either feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_feature_to_feature_2 =
R"doc(Convert multiple values from one feature space to another (vectorized)
Args:
    feature_from: Name of the source feature
    feature_to: Name of the target feature
    values: Container of values in the source feature space
    mp_cores: Number of cores to use for parallel processing (default:
              1)

Template Args:
    t_values: Type of the values container (must be xtensor
              compatible)

Returns:
    Container of corresponding values in the target feature space

Raises:
    std::runtime_error: if either feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_feature_to_index =
R"doc(Convert a feature value to its corresponding index
Args:
    feature: Name of the feature
    value: The value to convert to an index

Returns:
    The index of the nearest value in the feature

Raises:
    std::runtime_error: if the feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_feature_to_index_2 =
R"doc(Convert multiple feature values to their corresponding indices
(vectorized)
Args:
    feature: Name of the feature
    values: Container of values to convert to indices
    mp_cores: Number of cores to use for parallel processing (default:
              1)

Template Args:
    t_index: Type of the output index container (must be xtensor
             compatible)
    t_values: Type of the input values container (must be xtensor
              compatible)

Returns:
    Container of indices corresponding to the nearest values

Raises:
    std::runtime_error: if the feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_features = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_from_stream =
R"doc(Deserialize a mapper from an input stream
Args:
    is: Input stream to read from

Returns:
    NearestFeatureMapper object constructed from the stream)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_get_feature_indices =
R"doc(Get the indices for a specific feature
Args:
    feature: Name of the feature

Returns:
    Optional vector of feature indices, nullopt if feature doesn't
    exist)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_get_feature_values =
R"doc(Get the values for a specific feature
Args:
    feature: Name of the feature

Returns:
    Optional vector of feature values, nullopt if feature doesn't
    exist)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_has_feature =
R"doc(Check if a feature exists in the mapper
Args:
    feature: Name of the feature to check

Returns:
    true if the feature exists, false otherwise)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_index_to_feature =
R"doc(Convert an index to its corresponding feature value
Args:
    feature: Name of the feature
    index: The index to convert to a value

Returns:
    The interpolated value at the given index

Raises:
    std::runtime_error: if the feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_index_to_feature_2 =
R"doc(Convert multiple indices to their corresponding feature values
(vectorized)
Args:
    feature: Name of the feature
    indices: Container of indices to convert to values
    mp_cores: Number of cores to use for parallel processing (default:
              1)

Template Args:
    t_values: Type of the output values container (must be xtensor
              compatible)
    t_index: Type of the input index container (must be xtensor
             compatible)

Returns:
    Container of interpolated values at the given indices

Raises:
    std::runtime_error: if the feature is not found)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_keys =
R"doc(Get all feature names
Returns:
    Vector of all feature names currently stored in the mapper)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_operator_eq =
R"doc(Equality comparison operator
Args:
    other: Another NearestFeatureMapper to compare with

Returns:
    true if both mappers have the same features and values, false
    otherwise)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_printer =
R"doc(Create an ObjectPrinter for this mapper
Args:
    float_precision: Precision for floating point numbers
    superscript_exponents: Whether to use superscript for exponents

Returns:
    ObjectPrinter configured for this mapper)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_remove_feature =
R"doc(Remove a feature from the mapper
Args:
    feature: Name of the feature to remove)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_set_feature =
R"doc(Add or update a feature with the given values
Args:
    feature: Name of the feature to set
    values: Vector of values for the feature

Raises:
    std::invalid_argument: if values has less than 2 elements)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_NearestFeatureMapper_to_stream =
R"doc(Serialize the mapper to an output stream
Args:
    os: Output stream to write to)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator =
R"doc(A bidirectional nearest neighbor interpolator for mapping between
values and indices

This class provides functionality to map between continuous values and
discrete indices using nearest neighbor interpolation in both
directions (forward and reverse).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_ReversableNearestInterpolator = R"doc(Default constructor)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_ReversableNearestInterpolator_2 =
R"doc(Construct interpolator from a vector of values
Args:
    values: Vector of values to create the interpolator from)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_forward_interpolator = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_get_index =
R"doc(Get the nearest index for a given value
Args:
    x: The value to find the nearest index for

Returns:
    The index of the nearest value)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_get_index_data =
R"doc(Get the underlying index data
Returns:
    Reference to the vector of indices used for interpolation)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_get_value =
R"doc(Get the value at a given index (with interpolation)
Args:
    idx: The index to get the value for

Returns:
    The interpolated value at the given index)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_get_value_data =
R"doc(Get the underlying value data
Returns:
    Reference to the vector of values used for interpolation)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_featuremapping_ReversableNearestInterpolator_reverse_interpolator = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


