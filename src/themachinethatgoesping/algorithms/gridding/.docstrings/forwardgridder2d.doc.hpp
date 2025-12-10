//sourcehash: 336f43246b8546952607c368151dd63f394778eda365ec48325c158760df66f5

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


static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D =
R"doc(Simple class to generate 2D grids (images) and interpolate xy data
onto a grid using simple forward mapping algorithms (e.g. block mean,
weighted mean interpolation))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_ForwardGridder2D =
R"doc(Initialize forward gridder class using grid parameters.

Args:
    xres: x resolution of the grid
    yres: y resolution of the grid
    min_x: smallest x value that must be contained within the grid
    max_x: largest x value that must be contained within the grid
    min_y: smallest y value that must be contained within the grid
    max_y: largest y value that must be contained within the grid
    xbase: x base position of the grid, by default 0.0
    ybase: y base position of the grid, by default 0.0)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_border_xmax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_border_xmin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_border_ymax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_border_ymin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_from_data =
R"doc(Create gridder with resolution "res" xmin,xmax,ymin,ymax are
determined to exactly contain the given data vectors (sx,sy)

Args:
    res: x and y resolution of the grid
    sx: array with x data
    sy: array with y data

Returns:
    ForwardGridder2D object)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_from_res =
R"doc(Create gridder setting xres and yres to "res"

Args:
    res: x and y resolution of the grid
    min_x: smallest x value that must be contained within the grid
    max_x: largest x value that must be contained within the grid
    min_y: smallest y value that must be contained within the grid
    max_y: largest y value that must be contained within the grid

Returns:
    ForwardGridder2D object)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_border_xmax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_border_xmin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_border_ymax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_border_ymin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_empty_grd_images =
R"doc(Create empty grid images

Returns:
    std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>>
        image_values, image_weights)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_extent = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_extent_x = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_extent_y = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_min_and_offset = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_minmax =
R"doc(Returns the min/max value of two lists (same size).

Args:
    sx: 1D array with x positions
    sy: 1D array with y positions

Template Args:
    T_vector: 

Returns:
    std::tuple<t_float, t_float, t_float, t_float>
        (xmin,xmax,ymin,ymax))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_nx = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_ny = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_x_coordinates = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_x_grd_value = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_x_index = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_x_index_fraction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_x_value = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_xbase = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_xmax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_xmin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_xres = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_y_coordinates = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_y_grd_value = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_y_index = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_y_index_fraction = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_y_value = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_ybase = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_ymax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_ymin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_get_yres = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_group_blocks = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_interpolate_block_mean =
R"doc(Interpolate 2D points onto 2d images using block mean interpolation

Args:
    sx: x values
    sy: y values
    s_val: amplitudes / volume backscattering coefficients

Template Args:
    T_vector: 

Returns:
    std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>>
        image_values, image_weights)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_interpolate_block_mean_inplace =
R"doc(Interpolate 2D points onto 2d images using block mean interpolation
(inplace version)

Args:
    sx: x values
    sy: y values
    s_val: amplitudes / volume backscattering coefficients
    image_values: Image with values will be edited inplace
    image_weights: Image with weights will be edited inplace

Template Args:
    T_vector:)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_interpolate_weighted_mean =
R"doc(Interpolate 2D points onto 2d images using weighted mean interpolation

Args:
    sx: x values
    sy: y values
    s_val: amplitudes / volume backscattering coefficients

Template Args:
    T_vector: 

Returns:
    std::tuple<xt::xtensor<t_float, 2>, xt::xtensor<t_float, 2>>
        image_values, image_weights)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_interpolate_weighted_mean_inplace =
R"doc(Interpolate 2D points onto 2d images using weighted mean interpolation
(inplace version)

Args:
    sx: x values
    sy: y values
    s_val: amplitudes / volume backscattering coefficients
    image_values: Image with values will be edited inplace
    image_weights: Image with weights will be edited inplace

Template Args:
    T_vector:)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_nx = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_ny = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_xbase = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_xmax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_xmin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_xres = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_ybase = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_ymax = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_ymin = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_gridding_ForwardGridder2D_yres = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


