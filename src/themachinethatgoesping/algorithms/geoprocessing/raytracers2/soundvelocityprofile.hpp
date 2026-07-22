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
#include <vector>

#include <fmt/format.h>

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
    xt::xtensor<float, 1> _depths;
    xt::xtensor<float, 1> _sound_speeds;        // one per depth knot

    // Precomputed per-layer constants  (size L)
    xt::xtensor<float, 1> _gradients;
    xt::xtensor<float, 1> _inverse_gradients;
    xt::xtensor<bool, 1>  _isovelocity;

    // Optional metadata.
    std::optional<double> _timestamp;
    std::optional<double> _latitude;
    std::optional<double> _longitude;
    std::optional<double> _surface_sound_speed; // measured transducer/surface sound speed (m/s)

    static constexpr float ISO_EPS = 1e-6f; // |dc/dz| threshold for iso-velocity detection

  public:
    /// @brief Construct an empty SoundVelocityProfile.
    SoundVelocityProfile() = default;

    /**
     * @brief Construct from depth/sound-speed tables.
     * @param z  monotonically increasing depth knots (m, positive down).
     * @param c  corresponding sound speeds (m/s, must be positive).
     */
    SoundVelocityProfile(xt::xtensor<float, 1> z, xt::xtensor<float, 1> c)
    {
        set(std::move(z), std::move(c));
    }

    /**
     * @brief Constant-velocity profile from the surface to z_max.
     * @param c     sound speed (m/s).
     * @param z_max maximum depth (m); default 12 000 m.
     * @return SoundVelocityProfile with uniform sound speed.
     */
    static SoundVelocityProfile uniform(float c, float z_max = 12000.f)
    {
        xt::xtensor<float, 1> zs = { 0.f, z_max };
        xt::xtensor<float, 1> cs = { c, c };
        return SoundVelocityProfile(std::move(zs), std::move(cs));
    }

    /// @brief Equality comparison (metadata is ignored).
    bool operator==(const SoundVelocityProfile& other) const
    {
        return _depths == other._depths && _sound_speeds == other._sound_speeds &&
               _timestamp == other._timestamp &&
               _latitude == other._latitude && _longitude == other._longitude &&
               _surface_sound_speed == other._surface_sound_speed;
    }

    /**
     * @brief Set depth/sound-speed tables and recompute layer constants.
     * @param z  monotonically increasing depth knots (m, positive down).
     * @param c  corresponding sound speeds (m/s, must be positive).
     * @throws std::runtime_error if sizes differ, fewer than 2 entries, or non-monotone depths.
     */
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

    /// @brief Unix timestamp (s, UTC) when the profile was measured, or std::nullopt if unset.
    std::optional<double> get_timestamp() const { return _timestamp; }
    /// @brief Set the unix timestamp (s, UTC); pass std::nullopt to clear.
    void                  set_timestamp(std::optional<double> timestamp) { _timestamp = timestamp; }
    /// @brief True iff a timestamp is set.
    bool                  has_timestamp() const { return _timestamp.has_value(); }

    /// @brief Latitude (decimal degrees, +N) where the profile was measured, or std::nullopt if unset.
    std::optional<double> get_latitude() const { return _latitude; }
    /// @brief Set latitude (decimal degrees, +N); pass std::nullopt to clear.
    void                  set_latitude(std::optional<double> latitude) { _latitude = latitude; }

    /// @brief Longitude (decimal degrees, +E) where the profile was measured, or std::nullopt if unset.
    std::optional<double> get_longitude() const { return _longitude; }
    /// @brief Set longitude (decimal degrees, +E); pass std::nullopt to clear.
    void                  set_longitude(std::optional<double> longitude) { _longitude = longitude; }

    /// @brief Set both latitude (decimal degrees, +N) and longitude (decimal degrees, +E) at once.
    void set_location(std::optional<double> latitude, std::optional<double> longitude)
    {
        _latitude  = latitude;
        _longitude = longitude;
    }

    /// @brief True iff both latitude and longitude are set.
    bool has_location() const { return _latitude.has_value() && _longitude.has_value(); }

    // --- optional metadata: surface (transducer) sound speed ---

    /// @brief Measured transducer/surface sound speed (m/s), or std::nullopt if unset.
    std::optional<double> get_surface_sound_speed() const { return _surface_sound_speed; }
    /// @brief Set the measured transducer/surface sound speed (m/s); pass std::nullopt to clear.
    void set_surface_sound_speed(std::optional<double> surface_sound_speed)
    {
        _surface_sound_speed = surface_sound_speed;
    }
    /// @brief True iff a surface (transducer) sound speed is set.
    bool has_surface_sound_speed() const { return _surface_sound_speed.has_value(); }

    /**
     * @brief Return a copy of this profile with a measured surface (transducer) sound speed
     *        integrated at the transducer depth (Kongsberg "SHC=0" convention).
     *
     * The returned profile replaces every knot at or above @p transducer_depth_in_meters with an
     * iso-velocity segment at @p surface_sound_speed_in_meters_per_second (from depth 0 down to the
     * transducer depth) and keeps the archived knots strictly below the transducer depth. This makes
     * the sound speed at the transducer equal to the real-time measured surface sound speed (SSV),
     * which is what the echosounder uses when forming the beams; a beam launched at the transducer
     * depth is then self-consistent (the Snell launch/reference speed and the profile value at the
     * launch depth agree, removing the angle-dependent outer-beam depth bias that appears when the
     * archived profile value at the transducer differs from the measured SSV).
     *
     * The measured surface sound speed is also stored as metadata on the returned profile
     * (get_surface_sound_speed()).
     *
     * @param surface_sound_speed_in_meters_per_second measured sound speed at the transducer (m/s, >0).
     * @param transducer_depth_in_meters               transducer depth below the surface (m, >= 0).
     * @return SoundVelocityProfile extended with the surface sound speed.
     */
    SoundVelocityProfile get_profile_with_surface_sound_speed(
        float surface_sound_speed_in_meters_per_second,
        float transducer_depth_in_meters) const
    {
        if (!(surface_sound_speed_in_meters_per_second > 0.f))
            throw std::runtime_error("SoundVelocityProfile::get_profile_with_surface_sound_speed: "
                                     "surface sound speed must be positive");
        if (!(transducer_depth_in_meters >= 0.f))
            throw std::runtime_error("SoundVelocityProfile::get_profile_with_surface_sound_speed: "
                                     "transducer depth must be >= 0");

        std::vector<float> zs;
        std::vector<float> cs;
        zs.reserve(_depths.size() + 2);
        cs.reserve(_depths.size() + 2);

        // iso-velocity segment at the measured SSV from the surface to the transducer depth
        zs.push_back(0.f);
        cs.push_back(surface_sound_speed_in_meters_per_second);
        if (transducer_depth_in_meters > 0.f)
        {
            zs.push_back(transducer_depth_in_meters);
            cs.push_back(surface_sound_speed_in_meters_per_second);
        }

        // keep the archived knots strictly deeper than the transducer
        for (size_t i = 0; i < _depths.size(); ++i)
        {
            if (_depths.unchecked(i) > transducer_depth_in_meters)
            {
                zs.push_back(_depths.unchecked(i));
                cs.push_back(_sound_speeds.unchecked(i));
            }
        }

        // if the transducer is at/below the whole archived profile, keep the deepest knot so the
        // result still spans a positive depth range (and stays strictly increasing)
        if (zs.size() < 2)
        {
            const size_t last   = _depths.size() - 1;
            const float  deep_z = std::max(_depths.unchecked(last), transducer_depth_in_meters + 1.f);
            zs.push_back(deep_z);
            cs.push_back(_sound_speeds.unchecked(last));
        }

        SoundVelocityProfile extended;
        xt::xtensor<float, 1> z_out = xt::xtensor<float, 1>::from_shape({ zs.size() });
        xt::xtensor<float, 1> c_out = xt::xtensor<float, 1>::from_shape({ cs.size() });
        std::copy(zs.begin(), zs.end(), z_out.begin());
        std::copy(cs.begin(), cs.end(), c_out.begin());
        extended.set(std::move(z_out), std::move(c_out));
        extended._timestamp           = _timestamp;
        extended._latitude            = _latitude;
        extended._longitude           = _longitude;
        extended._surface_sound_speed = double(surface_sound_speed_in_meters_per_second);
        return extended;
    }

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
        if (_surface_sound_speed.has_value())
            printer.register_value("surface_sound_speed", *_surface_sound_speed, "m/s");
        else
            printer.register_string("surface_sound_speed", "not set");
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
        svp._timestamp           = read_optional_(is);
        svp._latitude            = read_optional_(is);
        svp._longitude           = read_optional_(is);
        svp._surface_sound_speed = read_optional_(is);
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
        write_optional_(os, _surface_sound_speed);
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
