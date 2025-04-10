//sourcehash: 2183692d3a8d8dcaa92a44137a3a4939af5b8c5ef637f1f32c874832d3fac393

/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.

  This is a modified version which allows for more than 8 arguments and includes def-guard
 */

#pragma once

#ifndef __DOCSTRINGS_HPP__

#define __EXPAND(x)                                                  x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...) COUNT
#define __VA_SIZE(...)                                               __EXPAND(__COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                                 a##b
#define __CAT2(a, b)                                                 __CAT1(a, b)
#define __DOC1(n1)                                                   __doc_##n1
#define __DOC2(n1, n2)                                               __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                                           __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                                       __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                                   __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)                           __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define __DOC8(n1, n2, n3, n4, n5, n6, n7, n8)                                                     \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8
#define __DOC9(n1, n2, n3, n4, n5, n6, n7, n8, n9)                                                 \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9
#define __DOC10(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)                                           \
    __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7##_##n8##_##n9##_##n10
#define DOC(...) __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#endif // NEW_DOC_HEADER_HPP
#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder =
R"doc(Simple class to generate 3D grids (images) and interpolate xyz data
onto a grid using simple forward mapping algorithms (e.g. block mean,
weighted mean interpolation))doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_ForwardGridder =
R"doc(Initialize forward gridder class using grid parameters.

Parameter ``xres``:
    x resolution of the grid

Parameter ``yres``:
    y resolution of the grid

Parameter ``zres``:
    z resolution of the grid

Parameter ``min_x``:
    smallest x value that must be contained within the grid

Parameter ``max_x``:
    largest x value that must be contained within the grid

Parameter ``min_y``:
    smallest y value that must be contained within the grid

Parameter ``max_y``:
    largest y value that must be contained within the grid

Parameter ``min_z``:
    smallest z value that must be contained within the grid

Parameter ``max_z``:
    largest z value that must be contained within the grid

Parameter ``xbase``:
    x base position of the grid, by default 0.0

Parameter ``ybase``:
    y base position of the grid, by default 0.0

Parameter ``zbase``:
    z base position of the grid, by default 0.0)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_xmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_xmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_ymax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_ymin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_zmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_border_zmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_from_data =
R"doc(Create gridder with resolution "res" xmin,xmax,ymin,ymax,zmin,zmax are
determined to exactly contain the given data vectors (sx,sy,sz)

Parameter ``res``:
    x,y and z resolution of the grid

Parameter ``sx``:
    array with x data

Parameter ``sy``:
    array with y data

Parameter ``sz``:
    array with z data

Returns:
    ForwardGridder object)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_from_res =
R"doc(Create gridder setting xres,yres and zres to "res"

Parameter ``res``:
    x,y and z resolution of the grid

Parameter ``min_x``:
    smallest x value that must be contained within the grid

Parameter ``max_x``:
    largest x value that must be contained within the grid

Parameter ``min_y``:
    smallest y value that must be contained within the grid

Parameter ``max_y``:
    largest y value that must be contained within the grid

Parameter ``min_z``:
    smallest z value that must be contained within the grid

Parameter ``max_z``:
    largest z value that must be contained within the grid

Returns:
    ForwardGridder object)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_from_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_xmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_xmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_ymax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_ymin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_zmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_border_zmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_empty_grd_images =
R"doc(Create empty grid images

Returns:
    std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>>
    image_values, image_weights)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_extent = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_extent_x = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_extent_y = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_extent_z = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_min_and_offset = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_minmax =
R"doc(Returns the min/max value of three lists (same size).

Template parameter ``T_vector``:
    $Parameter ``sx``:

1D array with x positions

Parameter ``sy``:
    1D array with y positions

Parameter ``sz``:
    1D array with z positions

Returns:
    std::tuple<t_float, t_float, t_float, t_float, t_float, t_float>
    (xmin,xmax,ymin,ymax,zmin,zmax))doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_nx = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_ny = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_nz = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_x_coordinates = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_x_grd_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_x_index = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_x_index_fraction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_x_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_xbase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_xmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_xmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_xres = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_y_coordinates = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_y_grd_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_y_index = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_y_index_fraction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_y_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_ybase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_ymax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_ymin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_yres = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_z_coordinates = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_z_grd_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_z_index = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_z_index_fraction = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_z_value = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_zbase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_zmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_zmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_get_zres = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_interpolate_block_mean =
R"doc(Interpolate 3D points onto 3d images using block mean interpolation

Template parameter ``T_vector``:
    $Parameter ``sx``:

x values

Parameter ``sy``:
    y values

Parameter ``sz``:
    z values

Parameter ``s_val``:
    amplitudes / volume backscattering coefficients

Parameter ``image_values``:
    Image with values. If empty a new image will be created.

Parameter ``image_weights``:
    Image with weights. If empty a new image will be created.

Returns:
    std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>>
    image_values, image_weights)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_interpolate_weighted_mean =
R"doc(Interpolate 3D points onto 3d images using weighted mean interpolation

Template parameter ``T_vector``:
    $Parameter ``sx``:

x values

Parameter ``sy``:
    y values

Parameter ``sz``:
    z values

Parameter ``s_val``:
    amplitudes / volume backscattering coefficients

Parameter ``image_values``:
    Image with values. If empty a new image will be created.

Parameter ``image_weights``:
    Image with weights. If empty a new image will be created.

Returns:
    std::tuple<xt::xtensor<t_float, 3>, xt::xtensor<t_float, 3>>
    image_values, image_weights)doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_nx = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_ny = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_nz = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_operator_eq = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_printer = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_to_stream = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_xbase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_xmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_xmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_xres = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_ybase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_ymax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_ymin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_yres = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_zbase = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_zmax = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_zmin = R"doc()doc";

static const char *__doc_themachinethatgoesping_algorithms_gridding_ForwardGridder_zres = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


