// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

// -----------------------------------------------------------------------------
// BeamSampleGeometryPiecewise — per-beam piecewise-linear sample geometry
// -----------------------------------------------------------------------------
// Generalises BeamSampleGeometry to K segments per beam:
//   coord_axis[beam](sample_nr) = offsets[seg, beam] + slopes[seg, beam] * sample_nr
// where seg is selected from the K shared sample-number knots so that
//   knot_sample_nrs[seg] <= sample_nr <= knot_sample_nrs[seg + 1].
//
// Built from a [K+1, n_beams, 3] tensor of world-frame XYZ knot points
// produced by a raytracer; each axis fits per-segment linear pieces exactly
// to the knot endpoints (zero error AT knots; chord-fit error WITHIN segments).
//
// Auto-collapse: if all segments collapse to one (slopes equal across
// segments and offsets line up at knot 0), the geometry can be exposed as a
// single-segment fast path equivalent to the existing BeamSampleGeometry.
// -----------------------------------------------------------------------------

#pragma once

#include ".docstrings/beamsamplegeometrypiecewise.doc.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <utility>

#include <fmt/core.h>

#include <xtensor/containers/xtensor.hpp>
#include <xtensor/core/xmath.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>

#include "beamaffine1d.hpp"
#include "beamsamplegeometry.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief Per-beam piecewise-linear sample geometry with K segments.
 *
 * Layout per axis:
 *   offsets : [K, n_beams]
 *   slopes  : [K, n_beams]
 * Shared:
 *   knot_sample_nrs : [K+1] monotonic, defines segment boundaries
 */
struct BeamSampleGeometryPiecewise
{
  private:
    size_t                       _n_beams = 0;
    size_t                       _n_segments = 0;

    xt::xtensor<float, 1>        _first_sample_numbers; ///< [n_beams] first valid sample nr
    xt::xtensor<unsigned int, 1> _number_of_samples;     ///< [n_beams] number of samples
    xt::xtensor<float, 1>        _knot_sample_nrs;       ///< [K+1] monotone segment boundaries

    // Per-axis piecewise affines: [K, n_beams]
    bool                  _has_x = false;
    bool                  _has_y = false;
    bool                  _has_z = false;
    xt::xtensor<float, 2> _off_x, _slp_x;
    xt::xtensor<float, 2> _off_y, _slp_y;
    xt::xtensor<float, 2> _off_z, _slp_z;

    static constexpr float COLLAPSE_TOL = 1e-3f; ///< absolute tolerance for is_single_affine

  public:
    BeamSampleGeometryPiecewise() = default;

    bool operator==(const BeamSampleGeometryPiecewise& rhs) const = default;

    // --- accessors ---
    size_t                              get_n_beams() const { return _n_beams; }
    size_t                              get_n_segments() const { return _n_segments; }
    const xt::xtensor<float, 1>&        get_first_sample_numbers() const { return _first_sample_numbers; }
    const xt::xtensor<unsigned int, 1>& get_number_of_samples() const { return _number_of_samples; }
    const xt::xtensor<float, 1>&        get_knot_sample_nrs() const { return _knot_sample_nrs; }

    bool has_x() const { return _has_x; }
    bool has_y() const { return _has_y; }
    bool has_z() const { return _has_z; }

    const xt::xtensor<float, 2>& get_offsets_x() const { return _off_x; }
    const xt::xtensor<float, 2>& get_slopes_x()  const { return _slp_x; }
    const xt::xtensor<float, 2>& get_offsets_y() const { return _off_y; }
    const xt::xtensor<float, 2>& get_slopes_y()  const { return _slp_y; }
    const xt::xtensor<float, 2>& get_offsets_z() const { return _off_z; }
    const xt::xtensor<float, 2>& get_slopes_z()  const { return _slp_z; }

    /**
     * @brief Build from a [K+1, n_beams, 3] tensor of world-frame knot xyz.
     *
     * For each beam and axis, fits per-segment linear pieces to the K+1 knots:
     *   slopes[k, b]  = (knot_xyz[k+1, b, axis] - knot_xyz[k, b, axis])
     *                   / (knot_sample_nrs[k+1] - knot_sample_nrs[k])
     *   offsets[k, b] = knot_xyz[k, b, axis] - slopes[k, b] * knot_sample_nrs[k]
     *
     * All three axes (x, y, z) are populated.
     *
     * @param first_sample_numbers per-beam first valid sample [n_beams]
     * @param number_of_samples    per-beam sample count [n_beams]
     * @param knot_sample_nrs      [K+1] monotone segment boundaries
     * @param knot_xyz             [K+1, n_beams, 3] world-frame knot xyz
     */
    static BeamSampleGeometryPiecewise from_layer_xyz(
        xt::xtensor<float, 1>        first_sample_numbers,
        xt::xtensor<unsigned int, 1> number_of_samples,
        xt::xtensor<float, 1>        knot_sample_nrs,
        const xt::xtensor<float, 3>& knot_xyz)
    {
        if (first_sample_numbers.size() != number_of_samples.size())
            throw std::runtime_error("from_layer_xyz: first_sample_numbers and number_of_samples size mismatch");
        if (knot_sample_nrs.size() < 2)
            throw std::runtime_error("from_layer_xyz: need at least 2 knot_sample_nrs");
        for (size_t k = 1; k < knot_sample_nrs.size(); ++k)
            if (!(knot_sample_nrs.unchecked(k) > knot_sample_nrs.unchecked(k - 1)))
                throw std::runtime_error("from_layer_xyz: knot_sample_nrs must be strictly monotone");

        const size_t K1      = knot_sample_nrs.size();
        const size_t K       = K1 - 1;
        const size_t n_beams = first_sample_numbers.size();

        if (knot_xyz.shape(0) != K1 || knot_xyz.shape(1) != n_beams || knot_xyz.shape(2) != 3)
            throw std::runtime_error(fmt::format(
                "from_layer_xyz: knot_xyz shape ({},{},{}) must be ({},{}, 3)",
                knot_xyz.shape(0), knot_xyz.shape(1), knot_xyz.shape(2),
                K1, n_beams));

        BeamSampleGeometryPiecewise g;
        g._n_beams              = n_beams;
        g._n_segments           = K;
        g._first_sample_numbers = std::move(first_sample_numbers);
        g._number_of_samples    = std::move(number_of_samples);
        g._knot_sample_nrs      = std::move(knot_sample_nrs);

        g._off_x = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._slp_x = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._off_y = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._slp_y = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._off_z = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._slp_z = xt::xtensor<float, 2>::from_shape({ K, n_beams });
        g._has_x = g._has_y = g._has_z = true;

        for (size_t k = 0; k < K; ++k)
        {
            const float s0   = g._knot_sample_nrs.unchecked(k);
            const float s1   = g._knot_sample_nrs.unchecked(k + 1);
            const float invd = 1.f / (s1 - s0);
            for (size_t b = 0; b < n_beams; ++b)
            {
                const float x0 = knot_xyz(k, b, 0);
                const float x1 = knot_xyz(k + 1, b, 0);
                const float y0 = knot_xyz(k, b, 1);
                const float y1 = knot_xyz(k + 1, b, 1);
                const float z0 = knot_xyz(k, b, 2);
                const float z1 = knot_xyz(k + 1, b, 2);
                const float sx = (x1 - x0) * invd;
                const float sy = (y1 - y0) * invd;
                const float sz = (z1 - z0) * invd;
                g._slp_x.unchecked(k, b) = sx;
                g._slp_y.unchecked(k, b) = sy;
                g._slp_z.unchecked(k, b) = sz;
                g._off_x.unchecked(k, b) = x0 - sx * s0;
                g._off_y.unchecked(k, b) = y0 - sy * s0;
                g._off_z.unchecked(k, b) = z0 - sz * s0;
            }
        }
        return g;
    }

    /**
     * @brief Evaluate (x, y, z) for a given beam and sample number.
     *
     * Selects the segment k with knot_sample_nrs[k] <= sample_nr <= knot_sample_nrs[k+1].
     * Outside the knot range, clamps to the first/last segment (linear extrapolation).
     */
    std::array<float, 3> eval_xyz(size_t beam, float sample_nr) const
    {
        if (beam >= _n_beams)
            throw std::out_of_range("BeamSampleGeometryPiecewise.eval_xyz: beam out of range");
        const size_t k = locate_segment_(sample_nr);
        std::array<float, 3> r{};
        r[0] = _has_x ? _off_x.unchecked(k, beam) + _slp_x.unchecked(k, beam) * sample_nr : 0.f;
        r[1] = _has_y ? _off_y.unchecked(k, beam) + _slp_y.unchecked(k, beam) * sample_nr : 0.f;
        r[2] = _has_z ? _off_z.unchecked(k, beam) + _slp_z.unchecked(k, beam) * sample_nr : 0.f;
        return r;
    }

    /**
     * @brief Returns true if all K segments collapse (within tolerance) to a
     *        single affine per beam — i.e. piecewise representation is
     *        equivalent to a plain BeamSampleGeometry.
     */
    bool is_single_affine(float tol = COLLAPSE_TOL) const
    {
        if (_n_segments <= 1) return true;
        auto check = [&](const xt::xtensor<float, 2>& off,
                         const xt::xtensor<float, 2>& slp) {
            for (size_t b = 0; b < _n_beams; ++b)
            {
                const float s0 = slp.unchecked(0, b);
                const float o0 = off.unchecked(0, b);
                for (size_t k = 1; k < _n_segments; ++k)
                {
                    if (std::abs(slp.unchecked(k, b) - s0) > tol) return false;
                    if (std::abs(off.unchecked(k, b) - o0) > tol) return false;
                }
            }
            return true;
        };
        if (_has_x && !check(_off_x, _slp_x)) return false;
        if (_has_y && !check(_off_y, _slp_y)) return false;
        if (_has_z && !check(_off_z, _slp_z)) return false;
        return true;
    }

    /**
     * @brief Convert to a single-segment BeamSampleGeometry by taking segment 0.
     *
     * Caller is responsible for checking is_single_affine() if equivalence
     * matters. Otherwise this returns the first segment's affine (useful for
     * shallow-water / fast path).
     */
    BeamSampleGeometry to_single_affine() const
    {
        BeamSampleGeometry geom(_first_sample_numbers, _number_of_samples);
        if (_has_x)
            geom.set_x_affine(BeamAffine1D(
                xt::xtensor<float, 1>(xt::view(_off_x, 0, xt::all())),
                xt::xtensor<float, 1>(xt::view(_slp_x, 0, xt::all()))));
        if (_has_y)
            geom.set_y_affine(BeamAffine1D(
                xt::xtensor<float, 1>(xt::view(_off_y, 0, xt::all())),
                xt::xtensor<float, 1>(xt::view(_slp_y, 0, xt::all()))));
        if (_has_z)
            geom.set_z_affine(BeamAffine1D(
                xt::xtensor<float, 1>(xt::view(_off_z, 0, xt::all())),
                xt::xtensor<float, 1>(xt::view(_slp_z, 0, xt::all()))));
        return geom;
    }

  private:
    size_t locate_segment_(float sample_nr) const
    {
        // returns segment index k in [0, _n_segments-1]
        if (_n_segments == 0) return 0;
        const size_t K1 = _knot_sample_nrs.size();
        if (sample_nr <= _knot_sample_nrs.unchecked(0)) return 0;
        if (sample_nr >= _knot_sample_nrs.unchecked(K1 - 1)) return _n_segments - 1;
        // binary search
        size_t lo = 0, hi = K1 - 1;
        while (hi - lo > 1)
        {
            size_t mid = (lo + hi) / 2;
            (sample_nr < _knot_sample_nrs.unchecked(mid) ? hi : lo) = mid;
        }
        return lo;
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamSampleGeometryPiecewise", float_precision, superscript_exponents);
        printer.register_value("n_beams", _n_beams);
        printer.register_value("n_segments", _n_segments);
        printer.register_container("knot_sample_nrs", _knot_sample_nrs);
        printer.register_value("has_x", _has_x);
        printer.register_value("has_y", _has_y);
        printer.register_value("has_z", _has_z);
        return printer;
    }

  public:
    static BeamSampleGeometryPiecewise from_stream(std::istream& is)
    {
        BeamSampleGeometryPiecewise g;
        is.read(reinterpret_cast<char*>(&g._n_beams), sizeof(size_t));
        is.read(reinterpret_cast<char*>(&g._n_segments), sizeof(size_t));
        const size_t n = g._n_beams;
        const size_t K = g._n_segments;
        g._first_sample_numbers = xt::xtensor<float, 1>::from_shape({ n });
        g._number_of_samples    = xt::xtensor<unsigned int, 1>::from_shape({ n });
        g._knot_sample_nrs      = xt::xtensor<float, 1>::from_shape({ K + 1 });
        is.read(reinterpret_cast<char*>(g._first_sample_numbers.data()), sizeof(float) * n);
        is.read(reinterpret_cast<char*>(g._number_of_samples.data()), sizeof(unsigned int) * n);
        is.read(reinterpret_cast<char*>(g._knot_sample_nrs.data()), sizeof(float) * (K + 1));
        is.read(reinterpret_cast<char*>(&g._has_x), sizeof(bool));
        is.read(reinterpret_cast<char*>(&g._has_y), sizeof(bool));
        is.read(reinterpret_cast<char*>(&g._has_z), sizeof(bool));
        auto rd = [&](xt::xtensor<float, 2>& t) {
            t = xt::xtensor<float, 2>::from_shape({ K, n });
            is.read(reinterpret_cast<char*>(t.data()), sizeof(float) * K * n);
        };
        if (g._has_x) { rd(g._off_x); rd(g._slp_x); }
        if (g._has_y) { rd(g._off_y); rd(g._slp_y); }
        if (g._has_z) { rd(g._off_z); rd(g._slp_z); }
        return g;
    }

    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_n_beams), sizeof(size_t));
        os.write(reinterpret_cast<const char*>(&_n_segments), sizeof(size_t));
        const size_t n = _n_beams;
        const size_t K = _n_segments;
        os.write(reinterpret_cast<const char*>(_first_sample_numbers.data()), sizeof(float) * n);
        os.write(reinterpret_cast<const char*>(_number_of_samples.data()), sizeof(unsigned int) * n);
        os.write(reinterpret_cast<const char*>(_knot_sample_nrs.data()), sizeof(float) * (K + 1));
        os.write(reinterpret_cast<const char*>(&_has_x), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&_has_y), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&_has_z), sizeof(bool));
        auto wr = [&](const xt::xtensor<float, 2>& t) {
            os.write(reinterpret_cast<const char*>(t.data()), sizeof(float) * t.size());
        };
        if (_has_x) { wr(_off_x); wr(_slp_x); }
        if (_has_y) { wr(_off_y); wr(_slp_y); }
        if (_has_z) { wr(_off_z); wr(_slp_z); }
    }

    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSampleGeometryPiecewise)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
