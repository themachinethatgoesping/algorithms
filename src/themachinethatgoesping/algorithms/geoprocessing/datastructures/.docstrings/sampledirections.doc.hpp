//sourcehash: f037162e7cc56de214b1e4eb74d56a56738a64d0b409b317d3e6aeaa9787f4ed

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections =
R"doc(A structure to store beamsample directions (along angle, across
angle).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_SampleDirections = R"doc(Construct a new sample location object (all values set to 0))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_SampleDirections_2 =
R"doc(Construct a new sample location object (initialize all tensors using
the specified shape (empty))

Args:
    shape: shape of the internal tensors)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_SampleDirections_3 =
R"doc(Construct a new SampleDirections object

Args:
    alongtrack_angle: in °, positive bow up, 0 == downwards
    crosstrack_angle: in °, positive starboard up, 0 == downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_alongtrack_angle = R"doc(in °, positive bow up, 0 == downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_check_shape = R"doc(check if the internal variables have the same shape)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_crosstrack_angle = R"doc(in °, positive starboard up, 0 == downwards)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_shape = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_SampleDirections_to_stream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


