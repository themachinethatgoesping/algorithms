//sourcehash: fbeb1dd47d5e9abdb938c6418c4ba401cbb27aa574072a757c89d26610d179d1

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


static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos =
R"doc(Common interface that provides the raw MBES detection information (per
transmission / per sector / per sounding) required by raytracers.

The getters that take a pingtools::BeamSelection are virtual and throw
not_implemented by default. Concrete classes (e.g. the ping
bottom/watercolumn interfaces or MBESRawDetectionData) override the
subset they can provide. The getters without a BeamSelection are
implemented in terms of the virtual ones.)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_I_MBESRawDetectionInfos = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_class_name =
R"doc(Name of the (derived) class, used by the object printer.

Returns:
    std::string

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_beam_numbers_per_tx_sector =
R"doc(Beam (sounding) indices grouped by their transmit sector number.

Returns:
    std::vector<std::vector<size_t>> (outer index: sector number)

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_beam_numbers_per_tx_sector_2 =
R"doc(Beam (sounding) indices of the selection grouped by their transmit
sector number.

Args:
    bs: beam selection

Returns:
    std::vector<std::vector<size_t>> (outer index: sector number)

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_mbesrawdetectiondata =
R"doc(Copy the raw detection information into an owning MBESRawDetectionData
object.

Throws (clear exception) if any required getter is not implemented.

Returns:
    MBESRawDetectionData

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_mbesrawdetectioninfos =
R"doc(Return a reference to this object as the abstract
I_MBESRawDetectionInfos interface.

No data is copied; the (virtual) getters stay connected to the
concrete class.

Returns:
    I_MBESRawDetectionInfos&

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_number_of_sectors =
R"doc(Number of transmit sectors in this transmission (derived from
get_sector_beam_numbers).

Returns:
    size_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_number_of_soundings =
R"doc(Number of soundings (beams) in this transmission.

Returns:
    size_t

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_ref_pingtime =
R"doc(Reference ping time of the transmission (unix timestamp in seconds).

Equal to the tx time for a single sector transmission.

Returns:
    double

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_ref_pose =
R"doc(Reference pose that soundings generated from this transmission are
referenced to.

Rotating/translating the soundings by this pose converts them into the
world coordinate system.

Returns:
    navigation::datastructures::SensorPose

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_rx_offsets =
R"doc(Receive transducer offset/pose (common to all soundings of this
transmission).

Returns:
    navigation::datastructures::SensorPose

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_sector_beam_numbers =
R"doc(Reference beam number for each transmit sector (length: number of
sectors).

Returns:
    xt::xtensor<size_t, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_sector_tx_offsets =
R"doc(Transmit transducer offset/pose for each sector.

Returns:
    std::vector<navigation::datastructures::SensorPose>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_sector_tx_steering_angles =
R"doc(Transmit steering angle of each sector (degrees).

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_sector_tx_time_delays =
R"doc(Transmit time delay of each sector relative to the reference ping time
(s).

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_sector_tx_times =
R"doc(Absolute transmit time of each sector (derived: ref ping time + tx
time delay).

Returns:
    xt::xtensor<double, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_rx_steering_angles =
R"doc(Receive steering angle of all soundings (degrees).

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_rx_steering_angles_2 =
R"doc(Receive steering angle of the selected soundings (degrees).

Args:
    bs: beam selection

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_rx_times =
R"doc(Absolute receive time of all soundings (derived: sector tx time + two
way travel time).

Returns:
    xt::xtensor<double, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_rx_times_2 =
R"doc(Absolute receive time of the selected soundings (derived).

Args:
    bs: beam selection

Returns:
    xt::xtensor<double, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_sector_numbers =
R"doc(Transmit sector number of all soundings (index into the per-sector
arrays).

Returns:
    xt::xtensor<size_t, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_sector_numbers_2 =
R"doc(Transmit sector number of the selected soundings (index into the per-
sector arrays).

Args:
    bs: beam selection

Returns:
    xt::xtensor<size_t, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_two_way_travel_times =
R"doc(Two way travel time of all soundings (s).

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_soundings_two_way_travel_times_2 =
R"doc(Two way travel time of the selected soundings (s).

Args:
    bs: beam selection

Returns:
    xt::xtensor<float, 1>

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_get_surface_sound_speed =
R"doc(Sound speed at the transducer face (m/s), used to convert steering
angles to directions.

Returns:
    float

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_number_of_soundings = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_ref_pingtime = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_ref_pose = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_rx_offsets = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_sector_beam_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_sector_tx_offsets = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_sector_tx_steering_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_sector_tx_time_delays = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_soundings_rx_steering_angles = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_soundings_sector_numbers = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_soundings_two_way_travel_times = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_has_surface_sound_speed = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_info_string =
R"doc(                                                                                           \
return an info string using the class __printer__ object
\

Args:
    float_precision: number of digits for floating point values
                     \
    superscript_exponents: print exponents in superscript
                           \

Returns:
    std::string
    \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_print =
R"doc(                                                                                           \
print the object information to the given outpustream
\ \

Args:
    os: output stream, e.g. file stream or std::out or std::cerr
        \
    float_precision: number of digits for floating point values
                     \
    superscript_exponents: print exponents in superscript
                           \

)doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_printer = R"doc()doc";

static const char *mkd_doc_themachinethatgoesping_algorithms_geoprocessing_raytracers2_I_MBESRawDetectionInfos_throw_not_implemented =
R"doc(Throw a descriptive exception for a raw getter that is not implemented
by a derived class. Uniquely named to avoid clashing with
I_PingCommon::not_implemented under multiple inheritance (I_PingBottom
/ I_PingWatercolumn).

)doc";


#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif


