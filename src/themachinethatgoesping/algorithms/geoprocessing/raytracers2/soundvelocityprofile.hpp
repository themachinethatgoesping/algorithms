// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// SoundVelocityProfile — 1-D layered sound velocity profile c(z)
// -----------------------------------------------------------------------------
// Stores depth/sound-speed knots and precomputes per-layer constants used by
// the LayerRaytracer for closed-form Snell raytracing.
//
// Depths are stored in **absolute world coordinates** (positive down,
// e.g. depth below the sea surface), not relative to a launch point. The
// LayerRaytracer launches each beam at the absolute depth carried in the
// TX pose (``pose.z``); the SVP must cover that depth.
//
// Layer i covers z ∈ [z_i, z_{i+1}] with linear gradient:
//   c(z) = c_i + g_i * (z - z_i),       g_i = (c_{i+1} - c_i) / (z_{i+1} - z_i)
//
// Iso-velocity layers (g_i ≈ 0) are flagged so the raytracer can use the
// straight-ray formulas instead of the constant-gradient ones.
//
// In addition to the depth/speed tables, the profile may optionally carry a
// timestamp (unix seconds) and latitude/longitude where it was measured.
// These are exposed as ``std::optional<double>``. Naming conventions for
// accessors mirror the ``SoundSpeedProfileDatagram`` (``get_depths_in_meters``,
// ``get_sound_speeds_in_meters_per_second``, ``get_timestamp``,
// ``get_date_string`` …).
// -----------------------------------------------------------------------------

#pragma once

#include ".docstrings/soundvelocityprofile.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/timeconv.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers2 {

/**
 * @brief 1-D depth-dependent sound velocity profile with layered analytic
 *        precomputations for use by the LayerRaytracer.
 *
 * Depths are absolute (e.g. metres below the sea surface). Optional metadata
 * (timestamp, latitude, longitude) is stored as ``std::optional<double>``.
 */
class SoundVelocityProfile
{
  private:
    xt::xtensor<float, 1> _depths;              ///< [L+1] depth knots (m), monotonically increasing, absolute
    xt::xtensor<float, 1> _sound_speeds;        ///< [L+1] sound speed knots (m/s), corresponding to _depths

    // Precomputed per-layer constants  (size L)
    xt::xtensor<float, 1> _gradients;           ///< sound-speed gradient dc/dz (s⁻¹) per layer
    xt::xtensor<float, 1> _inverse_gradients;   ///< 1/gradient (s) per layer (0.0f for iso-velocity layers)
    xt::xtensor<bool, 1>  _isovelocity;         ///< iso-velocity flag per layer (|gradient| < ISO_EPS)

    // Optional metadata.
    std::optional<double> _timestamp;
    std::optional<double> _latitude;
    std::optional<double> _longitude;

    static constexpr float ISO_EPS = 1e-6f; ///< |dc/dz| below which a layer counts as iso-velocity

  public:
    SoundVelocityProfile() = default;

    /**
     * @brief Construct from depth/speed tables.
     * @param z depths (m), monotonically increasing (absolute coordinates)
     * @param c sound speeds (m/s)
     */
    SoundVelocityProfile(xt::xtensor<float, 1> z, xt::xtensor<float, 1> c)
    {
        set(std::move(z), std::move(c));
    }

    /// Convenience: constant SVP from surface to z_max.
    static SoundVelocityProfile uniform(float c, float z_max = 12000.f)
    {
        xt::xtensor<float, 1> zs = { 0.f, z_max };
        xt::xtensor<float, 1> cs = { c, c };
        return SoundVelocityProfile(std::move(zs), std::move(cs));
    }

    bool operator==(const SoundVelocityProfile& other) const
    {
        return _depths == other._depths && _sound_speeds == other._sound_speeds &&
               _timestamp == other._timestamp &&
               _latitude == other._latitude && _longitude == other._longitude;
    }

    void set(xt::xtensor<float, 1> z, xt::xtensor<float, 1> c)
    {
        if (z.size() != c.size())
            throw std::runtime_error(fmt::format(
                "SoundVelocityProfile: z and c must have same size ({} vs {})", z.size(), c.size()));
        if (z.size() < 2)
            throw std::runtime_error("SoundVelocityProfile: need at least 2 layer knots");
        for (size_t i = 1; i < z.size(); ++i)
        {
            if (!(z.unchecked(i) > z.unchecked(i - 1)))
                throw std::runtime_error("SoundVelocityProfile: z must be strictly increasing");
            if (!(c.unchecked(i) > 0.f) || !(c.unchecked(i - 1) > 0.f))
                throw std::runtime_error("SoundVelocityProfile: c must be positive");
        }
        _depths       = std::move(z);
        _sound_speeds = std::move(c);
        recompute_layer_constants_();
    }

    // --- accessors (SoundSpeedProfileDatagram-style names) ---
    /// All depth knots (m), absolute coordinates.
    const xt::xtensor<float, 1>& get_depths_in_meters() const { return _depths; }
    /// All sound speeds (m/s), one per depth knot.
    const xt::xtensor<float, 1>& get_sound_speeds_in_meters_per_second() const { return _sound_speeds; }
    /// Sound-speed gradient dc/dz (s⁻¹) per layer (size = number_of_layers).
    const xt::xtensor<float, 1>& get_sound_speed_gradients_in_per_second() const { return _gradients; }
    /// 1 / gradient (s) per layer; 0 for iso-velocity layers (size = number_of_layers).
    const xt::xtensor<float, 1>& get_inverse_sound_speed_gradients_in_seconds() const { return _inverse_gradients; }
    /// Per-layer iso-velocity flag: true when |gradient| < ISO_EPS (size = number_of_layers).
    const xt::xtensor<bool, 1>&  get_isovelocity_flags() const { return _isovelocity; }
    /// Number of layers (= number of knots − 1).
    size_t get_number_of_layers() const { return _depths.size() - 1; }
    /// Depth (m) at the given knot index.
    float get_depth_in_meters(size_t i) const
    {
        if (i >= _depths.size())
            throw std::out_of_range(fmt::format(
                "SoundVelocityProfile::get_depth_in_meters: index {} out of range [0, {})",
                i, _depths.size()));
        return _depths.unchecked(i);
    }
    /// Sound speed (m/s) at the given knot index.
    float get_sound_speed_in_meters_per_second(size_t i) const
    {
        if (i >= _sound_speeds.size())
            throw std::out_of_range(fmt::format(
                "SoundVelocityProfile::get_sound_speed_in_meters_per_second: index {} "
                "out of range [0, {})", i, _sound_speeds.size()));
        return _sound_speeds.unchecked(i);
    }
    /// Number of (depth, sound speed) entries (= number of layers + 1).
    size_t get_number_of_entries() const { return _depths.size(); }

    /// Sound speed at depth z (linear interp inside layers, clamped at ends).
    float get_sound_speed(float z) const
    {
        if (_depths.size() < 2)
            throw std::runtime_error("SoundVelocityProfile: not initialized");
        if (z <= _depths.unchecked(0))
            return _sound_speeds.unchecked(0);
        const size_t L = _depths.size();
        if (z >= _depths.unchecked(L - 1))
            return _sound_speeds.unchecked(L - 1);
        // binary search for layer
        size_t lo = 0, hi = L - 1;
        while (hi - lo > 1)
        {
            size_t mid = (lo + hi) / 2;
            (z < _depths.unchecked(mid) ? hi : lo) = mid;
        }
        const float z0 = _depths.unchecked(lo);
        const float c0 = _sound_speeds.unchecked(lo);
        return c0 + _gradients.unchecked(lo) * (z - z0);
    }

    // --- optional metadata: timestamp, lat/lon ---

    /// Unix timestamp (seconds since epoch, UTC) when the profile was measured.
    std::optional<double> get_timestamp() const { return _timestamp; }
    void                  set_timestamp(std::optional<double> timestamp) { _timestamp = timestamp; }
    bool                  has_timestamp() const { return _timestamp.has_value(); }

    /// Latitude (decimal degrees, +N) where the profile was measured.
    std::optional<double> get_latitude() const { return _latitude; }
    void                  set_latitude(std::optional<double> latitude) { _latitude = latitude; }

    /// Longitude (decimal degrees, +E) where the profile was measured.
    std::optional<double> get_longitude() const { return _longitude; }
    void                  set_longitude(std::optional<double> longitude) { _longitude = longitude; }

    /// Convenience: set both latitude and longitude at once.
    void set_location(std::optional<double> latitude, std::optional<double> longitude)
    {
        _latitude  = latitude;
        _longitude = longitude;
    }

    /// True iff both latitude and longitude are set.
    bool has_location() const { return _latitude.has_value() && _longitude.has_value(); }

    /**
     * @brief Format ``_timestamp`` as a date string.
     *
     * Returns ``"no timestamp"`` if no timestamp is set.
     *
     * @param fractionalSecondsDigits passed to ``timeconv::unixtime_to_datestring``
     * @param format                  passed to ``timeconv::unixtime_to_datestring``
     */
    std::string get_date_string(unsigned int       fractionalSecondsDigits = 2,
                                const std::string& format = "%z__%d-%m-%Y__%H:%M:%S") const
    {
        if (!_timestamp.has_value())
            return "no timestamp";
        return tools::timeconv::unixtime_to_datestring(
            *_timestamp, fractionalSecondsDigits, format);
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "SoundVelocityProfile", float_precision, superscript_exponents);
        printer.register_container("depths", _depths, "m");
        printer.register_container("sound_speeds", _sound_speeds, "m/s");
        printer.register_section("layers (precomputed)");
        printer.register_value("number_of_layers", get_number_of_layers());
        printer.register_section("metadata (optional)");
        if (_timestamp.has_value())
        {
            printer.register_value("timestamp", *_timestamp, "s (unix)");
            printer.register_string("date", get_date_string());
        }
        else
        {
            printer.register_string("timestamp", "not set");
        }
        if (_latitude.has_value())
            printer.register_value("latitude", *_latitude, "deg");
        else
            printer.register_string("latitude", "not set");
        if (_longitude.has_value())
            printer.register_value("longitude", *_longitude, "deg");
        else
            printer.register_string("longitude", "not set");
        return printer;
    }

  private:
    // Helpers to (de)serialize std::optional<double> as a [flag(uint8), value(double)] pair.
    static void write_optional_(std::ostream& os, const std::optional<double>& v)
    {
        std::uint8_t flag = v.has_value() ? 1u : 0u;
        os.write(reinterpret_cast<const char*>(&flag), sizeof(flag));
        double value = v.value_or(0.0);
        os.write(reinterpret_cast<const char*>(&value), sizeof(double));
    }
    static std::optional<double> read_optional_(std::istream& is)
    {
        std::uint8_t flag = 0;
        is.read(reinterpret_cast<char*>(&flag), sizeof(flag));
        double value = 0.0;
        is.read(reinterpret_cast<char*>(&value), sizeof(double));
        if (flag)
            return value;
        return std::nullopt;
    }

  public:
    static SoundVelocityProfile from_stream(std::istream& is)
    {
        size_t n = 0;
        is.read(reinterpret_cast<char*>(&n), sizeof(size_t));
        SoundVelocityProfile svp;
        svp._depths       = xt::xtensor<float, 1>::from_shape({ n });
        svp._sound_speeds = xt::xtensor<float, 1>::from_shape({ n });
        is.read(reinterpret_cast<char*>(svp._depths.data()),       sizeof(float) * n);
        is.read(reinterpret_cast<char*>(svp._sound_speeds.data()), sizeof(float) * n);
        svp._timestamp = read_optional_(is);
        svp._latitude  = read_optional_(is);
        svp._longitude = read_optional_(is);
        if (n >= 2)
            svp.recompute_layer_constants_();
        return svp;
    }

    void to_stream(std::ostream& os) const
    {
        size_t n = _depths.size();
        os.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(_depths.data()),       sizeof(float) * n);
        os.write(reinterpret_cast<const char*>(_sound_speeds.data()), sizeof(float) * n);
        write_optional_(os, _timestamp);
        write_optional_(os, _latitude);
        write_optional_(os, _longitude);
    }

  private:
    void recompute_layer_constants_()
    {
        const size_t L = _depths.size() - 1;
        _gradients          = xt::xtensor<float, 1>::from_shape({ L });
        _inverse_gradients  = xt::xtensor<float, 1>::from_shape({ L });
        _isovelocity        = xt::xtensor<bool, 1>::from_shape({ L });
        for (size_t i = 0; i < L; ++i)
        {
            const float dz = _depths.unchecked(i + 1)       - _depths.unchecked(i);
            const float dc = _sound_speeds.unchecked(i + 1) - _sound_speeds.unchecked(i);
            const float g  = dc / dz;
            _gradients.unchecked(i)         = g;
            _isovelocity.unchecked(i)       = std::abs(g) < ISO_EPS;
            _inverse_gradients.unchecked(i) = _isovelocity.unchecked(i) ? 0.f : 1.f / g;
        }
    }

  public:
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(SoundVelocityProfile)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__

    /**
     * @brief Hash that depends **only** on the depth/sound-speed tables
     *        (i.e. the actual SVP content), ignoring optional metadata
     *        such as timestamp / latitude / longitude.
     *
     * This is what ``hash_value`` (and therefore ``boost::flyweight``) uses,
     * so two profiles with identical depth/sound-speed tables but different
     * acquisition timestamps share a single flyweight entry.
     */
    xxh::hash_t<64> hash_content_only() const
    {
        xxh::hash3_state_t<64> hash;
        const size_t           n = _depths.size();
        hash.update(&n, sizeof(n));
        if (n > 0)
        {
            hash.update(_depths.data(),       n * sizeof(float));
            hash.update(_sound_speeds.data(), n * sizeof(float));
        }
        return hash.digest();
    }
};

/**
 * @brief boost::flyweight requires a free ``hash_value`` for the value type.
 *        We hash only the depth/sound-speed tables (see ``hash_content_only``)
 *        so that profiles that differ only in timestamp / location metadata
 *        deduplicate to the same flyweight entry.
 */
inline std::size_t hash_value(const SoundVelocityProfile& svp)
{
    return svp.hash_content_only();
}

} // namespace raytracers2
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
