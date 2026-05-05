//sourcehash: 752b8a278ebebea29c807745a371a8830fabc429a391b63761226a155a0e16bd

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer =
R"doc(Closed-form Snell raytracer through a 1-D layered SVP.

Output is a [K+1, n_beams, 3] world-frame xyz tensor that plugs
directly into BeamSampleGeometryPiecewise::from_layer_xyz.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_LayerRaytracer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_LayerRaytracer_2 = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_get_svp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_set_svp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_svp = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_trace_at_times =
R"doc(Trace beams to the given one-way travel times.

Args:
    launch_dirs: [n_beams, 3] unit vectors in vehicle/transducer frame
                 (forward, starboard, down). Need NOT be normalized;
                 will be normalized internally. dz must be > 0
                 (downward), otherwise the beam gets NaN output.
    knot_times: [K+1] one-way travel times since TX (seconds),
                monotonically increasing, knot_times[0] >= 0.
    tx_poses: length K+1, world-frame TX pose at each knot
              (yaw/pitch/roll + xyz). xyz is the TX origin used for
              that knot's leg.
    rx_poses: length K+1, world-frame RX pose at each knot. Used only
              for translation interpolation across the round-trip. May
              be equal to tx_poses for the single-pose case.
    mp_cores: number of OpenMP threads; 1 = serial.

Returns:
    [K+1, n_beams, 3] world-frame xyz (positive z down), in the same
    units as the poses' xyz (typically metres).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_LayerRaytracer_trace_at_times_2 = R"doc(Convenience overload: same TX and RX pose at each knot.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


