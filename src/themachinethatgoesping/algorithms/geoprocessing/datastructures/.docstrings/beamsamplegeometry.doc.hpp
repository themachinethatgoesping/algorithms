//sourcehash: 300f6c038c424536c04843c20ab894127c2fd030027fbd6b595193a404d6662a

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry =
R"doc(Stores per-ping beam/sample geometry as linear affines.

Each optional affine maps sample_nr → a coordinate (x, y, or z) per
beam:
  coord[beam] = affine.offsets[beam] + affine.slopes[beam] * sample_nr

The user can set only the affines they need (e.g. z-only for depth
images). Transformation functions will throw if the required affine is
not set.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_BeamSampleGeometry = R"doc(Construct an empty BeamSampleGeometry)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_BeamSampleGeometry_2 =
R"doc(Construct a BeamSampleGeometry with beam metadata only (no affines
set)

Args:
    first_sample_numbers: first valid sample number per beam [n_beams]
    number_of_samples: number of samples per beam [n_beams])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds =
R"doc(Bounding box of all beam endpoints in (x, y, z) space.

Coordinates are NaN for dimensions without a set affine.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_x_max = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_x_min = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_y_max = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_y_min = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_z_max = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_Bounds_z_min = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_x = R"doc(sample_nr → x per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_y = R"doc(sample_nr → y per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_affine_z = R"doc(sample_nr → z per beam)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_check_affine_size = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_first_sample_numbers = R"doc([n_beams] first valid sample nr)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_coord_all =
R"doc(Compute the full flat coordinate array for all beams and samples.

For each beam b, fills positions [flat_offsets[b] ..
+number_of_samples[b]-1]
with: affine.offset[b] + affine.slope[b] * (first_sample_numbers[b] +
      j)
One SIMD FMA call per beam.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_coord_flat = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_coord_range = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_x =
R"doc(Compute x coordinate for (beam_index, sample_number) pairs.

When beam_indices are sorted (consecutive same-beam entries), each run
is dispatched as a single SIMD FMA call for maximum throughput.

Args:
    beam_indices: beam index per entry [N], values in [0, n_beams)
    sample_numbers: float sample number per entry [N]

Returns:
    xt::xtensor<float, 1> x coordinate per entry [N])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_x_2 =
R"doc(Compute x coordinate for sample ranges per beam.

For each selected beam, generates sample numbers [first, first+step,
..., <= last] and computes x = offset + slope * sample_nr via SIMD
FMA. Result is 2D [n_selected_beams x max_samples]; trailing unused
cells are NaN.

Args:
    beam_indices: per selected beam [B], values in [0, n_beams)
    first_sample_numbers: first sample number per beam [B]
    last_sample_numbers: last sample number per beam [B]
    sample_step: step between consecutive samples (default 1)

Returns:
    xt::xtensor<float, 2> [B x max_samples])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_x_flat =
R"doc(Compute the full flat x coordinate array for all beams and samples.

The result has get_total_samples() elements laid out contiguously per
beam. Index with:
  flat_index = get_flat_offsets()[beam] + (sample_nr -
  first_sample_numbers[beam]))doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_y =
R"doc(@copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_sn&)
const)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_y_2 =
R"doc(@copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_fs&,
const t_xtensor_1d_ls&, uint32_t) const)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_y_flat = R"doc(@copydoc forward_x_flat)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_z =
R"doc(@copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_sn&)
const)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_z_2 =
R"doc(@copydoc forward_x(const t_xtensor_1d_bi&, const t_xtensor_1d_fs&,
const t_xtensor_1d_ls&, uint32_t) const)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_forward_z_flat = R"doc(@copydoc forward_x_flat)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_angle_and_range =
R"doc(Create a BeamSampleGeometry from crosstrack angles and ranges.

Computes y and z affines from the crosstrack angle and range at a
known sample number per beam (no x affine set since no alongtrack
info):
  y(sample_nr) = (-range * sin(crosstrack_angle)) / range_sample_nr *
  sample_nr z(sample_nr) = ( range * cos(crosstrack_angle)) /
  range_sample_nr * sample_nr

Args:
    crosstrack_angles: in °, positive portside up, 0 == downwards
                       [n_beams]
    ranges: in m, per beam [n_beams]
    range_sample_numbers: sample number at which the range was
                          measured [n_beams]
    first_sample_numbers: first valid sample number per beam [n_beams]
    number_of_samples: number of samples per beam [n_beams]

Returns:
    BeamSampleGeometry with y and z affines set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_angles_and_range =
R"doc(Create a BeamSampleGeometry from alongtrack angles, crosstrack angles,
and ranges.

Uses the independent / Mills Cross formulation: the TX and RX arrays
measure angles independently in perpendicular planes, so each angle
contributes one horizontal displacement:
  x(sample_nr) = ( range * sin(at_angle))  / range_sample_nr *
  sample_nr y(sample_nr) = (-range * sin(ct_angle))  / range_sample_nr
  * sample_nr z(sample_nr) = sqrt(range² - x² - y²)    /
  range_sample_nr * sample_nr

Args:
    alongtrack_angles: in °, positive bow up, 0 == downwards [n_beams]
    crosstrack_angles: in °, positive portside up, 0 == downwards
                       [n_beams]
    ranges: in m, per beam [n_beams]
    range_sample_numbers: sample number at which the range was
                          measured [n_beams]
    first_sample_numbers: first valid sample number per beam [n_beams]
    number_of_samples: number of samples per beam [n_beams]

Returns:
    BeamSampleGeometry with x, y, z affines set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_bottom_xyz =
R"doc(Create a BeamSampleGeometry from a base XYZ location and per-beam
bottom XYZ locations with their corresponding sample numbers.

Computes affines such that:
  coord(sample_nr) = base_coord + (bottom_coord - base_coord) /
  bottom_sample_nr *
sample_nr

Args:
    base_xyz: base location (single point, e.g. transducer position)
    bottom_xyz: per-beam bottom locations [n_beams]
    bottom_sample_numbers: per-beam sample number at the bottom
                           [n_beams]
    first_sample_numbers: per-beam first valid sample number [n_beams]
    number_of_samples: per-beam number of samples [n_beams]

Returns:
    BeamSampleGeometry with x, y, z affines set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_bottom_z =
R"doc(Create a BeamSampleGeometry with z affine only, from a base depth and
per-beam bottom depths at known sample numbers.

Args:
    base_z: base depth (e.g. transducer depth)
    bottom_depths: per-beam bottom depths [n_beams]
    bottom_sample_numbers: per-beam sample number at the bottom
                           [n_beams]
    first_sample_numbers: per-beam first valid sample number [n_beams]
    number_of_samples: per-beam number of samples [n_beams]

Returns:
    BeamSampleGeometry with z affine set)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_from_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_bounds =
R"doc(Compute the bounding box of all beam/sample coordinates.

Evaluates the forward transform at the first and last sample of every
beam and takes min/max.  Only populates bounds for dimensions that
have a set affine; others stay NaN.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_first_sample_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_flat_offsets =
R"doc(Compute per-beam cumulative offsets into a flat sample array.

flat_offsets[b] = sum(number_of_samples[0..b-1])

Use with: flat_index = flat_offsets[beam] + (sample_nr -
first_sample_numbers[beam])

Returns:
    xt::xtensor<unsigned int, 1> [n_beams])doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_last_sample_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_number_of_samples = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_total_samples = R"doc(Total number of samples across all beams.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_get_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_has_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_n_beams = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_number_of_samples = R"doc([n_beams] number of samples)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_operator_eq = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_x_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_xyz_affines = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_y_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_set_z_affine = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_to_stream = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_with_geolocation =
R"doc(Apply a Geolocation (depth + ypr) to the geometry.

Equivalent to with_rigid_transform(g.yaw, g.pitch, g.roll, 0, 0, g.z).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_with_geolocation_2 =
R"doc(Apply a GeolocationLocal (northing/easting/depth + ypr) to the
geometry.

The resulting (x, y, z) will be expressed as (northing, easting,
depth).
Note: x ↔ northing and y ↔ easting (northing-first convention).)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_with_offset =
R"doc(Bake a translation (dx, dy, dz) into the geometry's affines.

Only the per-beam offsets of set affines are modified; slopes are
untouched. Axes whose affine is unset are ignored silently.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_datastructures_BeamSampleGeometry_with_rigid_transform =
R"doc(Bake a rigid transform (yaw/pitch/roll rotation + translation) into
the geometry's affines.

Requires that all three (x, y, z) affines are set, since a rotation
mixes the axes. After this call, evaluating the geometry at any sample
number yields the rotated-then-translated coordinate of that point.

Args:
    yaw_deg: yaw in °, 0° == north, 90° == east
    pitch_deg: pitch in °, positive bow up
    roll_deg: roll in °, positive port up
    tx: ,ty,tz     translation applied AFTER rotation)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


