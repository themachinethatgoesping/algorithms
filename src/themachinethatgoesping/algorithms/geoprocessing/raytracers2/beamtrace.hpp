// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// BeamTrace — polyline of a single ray through a layered sound velocity profile
// -----------------------------------------------------------------------------
// Stores the path-defining points of one traced beam in the athwartships y-z
// plane (x/forward = 0). Point 0 is always the launch point; the remaining
// points are the vertices where the analytic ray path changes segment: every
// sound-velocity-profile layer crossing and every interior turning point, plus
// the final point at the requested two-way travel time.
//
// Frame / sign conventions (matching the geoprocessing xyz convention):
//   * z is depth, positive downwards.
//   * horizontal_offset is the signed athwartships coordinate y, positive
//     starboard.
//   * the incident angle is measured from straight down (+z): 0 deg points
//     down, +-90 deg is horizontal, +-180 deg points up. Following the
//     right-handed rotation about +x (forward), a direction with positive y
//     and positive z has a negative angle (i.e. starboard-down is negative,
//     port-down is positive).
//
// Only the quantities that the trace produces directly are stored (depth,
// horizontal offset, two-way travel time and the cosine of the ray angle).
// The incident angle in degrees and the cumulative along-ray range are derived
// on access, so no work is spent on them unless they are requested. Because
// every stored segment lies inside a single layer it is exactly one circular
// arc, which lets the range be reconstructed exactly from the stored points.
// -----------------------------------------------------------------------------

#pragma once

/* generated doc strings */
#include ".docstrings/beamtrace.doc.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/views/xview.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief Path of a single traced beam through a layered sound velocity profile.
 *
 * The stored points describe the ray polyline in the athwartships y-z plane
 * (forward coordinate x is zero). Point 0 is the launch point. Depth,
 * horizontal offset, two-way travel time and the cosine of the ray angle are
 * stored; incident angle (deg) and cumulative range (m) are derived on access.
 */
class BeamTrace
{
  private:
    // Stored per-point tables; point 0 is the launch point.
    xt::xtensor<float, 1> _depths;
    xt::xtensor<float, 1> _horizontal_offsets;
    xt::xtensor<float, 1> _two_way_travel_times;
    xt::xtensor<float, 1> _cos_incident_angles;

  public:
    /// @brief Construct an empty BeamTrace (no points stored).
    BeamTrace() = default;

    /**
     * @brief Construct from the four stored per-point tables (all must have the same length).
     * @param depths               depth z (m, positive down); point 0 is the launch point.
     * @param horizontal_offsets   signed athwartships offset y (m, positive starboard).
     * @param two_way_travel_times two-way travel time (s).
     * @param cos_incident_angles  cosine of the ray angle from +z (1=down, 0=turning, −1=up).
     */
    BeamTrace(xt::xtensor<float, 1> depths,
              xt::xtensor<float, 1> horizontal_offsets,
              xt::xtensor<float, 1> two_way_travel_times,
              xt::xtensor<float, 1> cos_incident_angles)
    {
        set(std::move(depths),
            std::move(horizontal_offsets),
            std::move(two_way_travel_times),
            std::move(cos_incident_angles));
    }

    /// @brief Equality comparison.
    bool operator==(const BeamTrace& other) const
    {
        return _depths == other._depths && _horizontal_offsets == other._horizontal_offsets &&
               _two_way_travel_times == other._two_way_travel_times &&
               _cos_incident_angles == other._cos_incident_angles;
    }

    /**
     * @brief Set all stored tables at once (all must have the same length).
     * @throws std::runtime_error if table sizes differ.
     */
    void set(xt::xtensor<float, 1> depths,
             xt::xtensor<float, 1> horizontal_offsets,
             xt::xtensor<float, 1> two_way_travel_times,
             xt::xtensor<float, 1> cos_incident_angles)
    {
        const size_t p = depths.size();
        if (horizontal_offsets.size() != p || two_way_travel_times.size() != p ||
            cos_incident_angles.size() != p)
            throw std::runtime_error(fmt::format(
                "BeamTrace: all tables must have the same size (got depths {}, horizontal_offsets "
                "{}, two_way_travel_times {}, cos_incident_angles {})",
                p,
                horizontal_offsets.size(),
                two_way_travel_times.size(),
                cos_incident_angles.size()));

        _depths               = std::move(depths);
        _horizontal_offsets   = std::move(horizontal_offsets);
        _two_way_travel_times = std::move(two_way_travel_times);
        _cos_incident_angles  = std::move(cos_incident_angles);
    }

    // --- stored-quantity accessors ---

    /// Number of stored points (>= 1 for a valid trace; point 0 is the launch point).
    size_t get_number_of_points() const { return _depths.size(); }

    /// Depth z (m, positive down) at each point.
    const xt::xtensor<float, 1>& get_depths_in_meters() const { return _depths; }
    /// Signed athwartships offset y (m, positive starboard) at each point.
    const xt::xtensor<float, 1>& get_horizontal_offsets_in_meters() const { return _horizontal_offsets; }
    /// Two-way travel time (s) at each point.
    const xt::xtensor<float, 1>& get_two_way_travel_times_in_seconds() const { return _two_way_travel_times; }
    /// Cosine of the ray angle from +z (1 down, 0 turning, -1 up) at each point.
    const xt::xtensor<float, 1>& get_cos_incident_angles() const { return _cos_incident_angles; }

    // --- derived quantities (computed on access) ---

    /**
     * @brief Incident angle (deg) from straight down at each point.
     *
     * 0 deg points down, +-90 deg is horizontal, +-180 deg points up. Positive
     * angles correspond to the port side (negative y), consistent with a
     * right-handed rotation about +x.
     */
    xt::xtensor<float, 1> get_incident_angles_in_degrees() const
    {
        constexpr float rad_to_deg = float(180.0 / M_PI);

        const float                 hsign = horizontal_side_();
        const xt::xtensor<float, 1>& cz    = _cos_incident_angles;
        const auto sin_mag = xt::sqrt(xt::maximum(0.f, 1.f - cz * cz));

        return xt::eval(xt::atan2(-hsign * sin_mag, cz) * rad_to_deg);
    }

    /**
     * @brief Cumulative along-ray range (m) from the launch point.
     * @return Range array; element 0 is always 0 (launch point).
     */
    xt::xtensor<float, 1> get_ranges_in_meters() const
    {
        const size_t p = get_number_of_points();

        xt::xtensor<float, 1> ranges = xt::xtensor<float, 1>::from_shape({ p });
        ranges.fill(0.f);
        if (p < 2)
            return ranges;

        // Per-segment chord length and change in ray angle.
        const auto z0 = xt::view(_depths, xt::range(0, p - 1));
        const auto z1 = xt::view(_depths, xt::range(1, p));
        const auto h0 = xt::view(_horizontal_offsets, xt::range(0, p - 1));
        const auto h1 = xt::view(_horizontal_offsets, xt::range(1, p));

        const auto dz    = z1 - z0;
        const auto dh    = h1 - h0;
        const auto chord = xt::sqrt(dz * dz + dh * dh);

        const auto theta = xt::acos(xt::clip(_cos_incident_angles, -1.f, 1.f));
        const auto half  = 0.5f * xt::abs(xt::view(theta, xt::range(1, p)) -
                                          xt::view(theta, xt::range(0, p - 1)));

        // arc = chord * (half / sin(half)); the ratio tends to 1 for straight
        // segments (half -> 0), which is used directly to avoid a 0/0.
        constexpr float eps    = 1e-6f;
        const auto segment_len = xt::eval(
            chord * xt::where(half < eps, 1.f, half / xt::sin(half)));

        for (size_t i = 1; i < p; ++i)
            ranges.unchecked(i) = ranges.unchecked(i - 1) + segment_len.unchecked(i - 1);

        return ranges;
    }

  private:
    // Returns +1 (starboard) or -1 (port) based on the net horizontal offset;
    // +1 for a vertical beam.
    float horizontal_side_() const
    {
        // The beam keeps its athwartships direction along the whole path, so the
        // point with the largest |offset| (the last one) carries a robust sign.
        float largest = 0.f;
        for (size_t i = 0; i < _horizontal_offsets.size(); ++i)
        {
            const float offset = _horizontal_offsets.unchecked(i);
            if (std::abs(offset) > std::abs(largest))
                largest = offset;
        }
        return largest < 0.f ? -1.f : 1.f;
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamTrace", float_precision, superscript_exponents);

        printer.register_value("number_of_points", get_number_of_points());
        printer.register_container("depths", _depths, "m");
        printer.register_container("horizontal_offsets", _horizontal_offsets, "m");
        printer.register_container("two_way_travel_times", _two_way_travel_times, "s");
        printer.register_section("derived");
        printer.register_container("incident_angles", get_incident_angles_in_degrees(), "deg");
        printer.register_container("ranges", get_ranges_in_meters(), "m");

        return printer;
    }

  public:
    static BeamTrace from_stream(std::istream& is)
    {
        size_t p = 0;
        is.read(reinterpret_cast<char*>(&p), sizeof(size_t));

        BeamTrace trace;
        trace._depths               = xt::xtensor<float, 1>::from_shape({ p });
        trace._horizontal_offsets   = xt::xtensor<float, 1>::from_shape({ p });
        trace._two_way_travel_times = xt::xtensor<float, 1>::from_shape({ p });
        trace._cos_incident_angles  = xt::xtensor<float, 1>::from_shape({ p });

        is.read(reinterpret_cast<char*>(trace._depths.data()), sizeof(float) * p);
        is.read(reinterpret_cast<char*>(trace._horizontal_offsets.data()), sizeof(float) * p);
        is.read(reinterpret_cast<char*>(trace._two_way_travel_times.data()), sizeof(float) * p);
        is.read(reinterpret_cast<char*>(trace._cos_incident_angles.data()), sizeof(float) * p);

        return trace;
    }

    void to_stream(std::ostream& os) const
    {
        size_t p = _depths.size();
        os.write(reinterpret_cast<const char*>(&p), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(_depths.data()), sizeof(float) * p);
        os.write(reinterpret_cast<const char*>(_horizontal_offsets.data()), sizeof(float) * p);
        os.write(reinterpret_cast<const char*>(_two_way_travel_times.data()), sizeof(float) * p);
        os.write(reinterpret_cast<const char*>(_cos_incident_angles.data()), sizeof(float) * p);
    }

  public:
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamTrace)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
