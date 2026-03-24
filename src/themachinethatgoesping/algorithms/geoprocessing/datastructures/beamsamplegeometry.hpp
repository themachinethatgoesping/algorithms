// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/beamsamplegeometry.doc.hpp"

#include <fmt/core.h>
#include <optional>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>

#include "beamaffine1d.hpp"
#include "xyz.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief Stores per-ping beam/sample geometry as linear affines.
 *
 * Each optional affine maps sample_nr → a coordinate (x, y, or z) per beam:
 *   coord[beam] = affine.offsets[beam] + affine.slopes[beam] * sample_nr
 *
 * The user can set only the affines they need (e.g. z-only for depth images).
 * Transformation functions will throw if the required affine is not set.
 */
struct BeamSampleGeometry
{
  private:
    size_t _n_beams = 0;

    xt::xtensor<float, 1>        _first_sample_numbers; ///< [n_beams] first valid sample nr
    xt::xtensor<unsigned int, 1> _number_of_samples;     ///< [n_beams] number of samples

    std::optional<BeamAffine1D> _affine_x; ///< sample_nr → x per beam
    std::optional<BeamAffine1D> _affine_y; ///< sample_nr → y per beam
    std::optional<BeamAffine1D> _affine_z; ///< sample_nr → z per beam

  public:
    /**
     * @brief Construct an empty BeamSampleGeometry
     */
    BeamSampleGeometry() = default;

    /**
     * @brief Construct a BeamSampleGeometry with beam metadata only (no affines set)
     *
     * @param first_sample_numbers first valid sample number per beam [n_beams]
     * @param number_of_samples number of samples per beam [n_beams]
     */
    BeamSampleGeometry(xt::xtensor<float, 1>        first_sample_numbers,
                       xt::xtensor<unsigned int, 1> number_of_samples)
        : _n_beams(first_sample_numbers.size())
        , _first_sample_numbers(std::move(first_sample_numbers))
        , _number_of_samples(std::move(number_of_samples))
    {
        if (_first_sample_numbers.size() != _number_of_samples.size())
            throw std::runtime_error(fmt::format(
                "BeamSampleGeometry: first_sample_numbers ({}) and number_of_samples ({}) must "
                "have same size",
                _first_sample_numbers.size(),
                _number_of_samples.size()));
    }

    bool operator==(const BeamSampleGeometry& rhs) const = default;

    // --- accessors ---
    size_t                              get_n_beams() const { return _n_beams; }
    const xt::xtensor<float, 1>&        get_first_sample_numbers() const { return _first_sample_numbers; }
    const xt::xtensor<unsigned int, 1>& get_number_of_samples() const { return _number_of_samples; }

    // --- last sample numbers (computed) ---
    xt::xtensor<float, 1> get_last_sample_numbers() const
    {
        return _first_sample_numbers + xt::cast<float>(_number_of_samples) - 1.0f;
    }

    // --- affine accessors ---
    bool has_x_affine() const { return _affine_x.has_value(); }
    bool has_y_affine() const { return _affine_y.has_value(); }
    bool has_z_affine() const { return _affine_z.has_value(); }

    const BeamAffine1D& get_x_affine() const
    {
        if (!_affine_x)
            throw std::runtime_error("BeamSampleGeometry: x affine not set");
        return *_affine_x;
    }

    const BeamAffine1D& get_y_affine() const
    {
        if (!_affine_y)
            throw std::runtime_error("BeamSampleGeometry: y affine not set");
        return *_affine_y;
    }

    const BeamAffine1D& get_z_affine() const
    {
        if (!_affine_z)
            throw std::runtime_error("BeamSampleGeometry: z affine not set");
        return *_affine_z;
    }

    // --- affine setters ---
    void set_x_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_x_affine");
        _affine_x = std::move(affine);
    }

    void set_y_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_y_affine");
        _affine_y = std::move(affine);
    }

    void set_z_affine(BeamAffine1D affine)
    {
        check_affine_size(affine, "set_z_affine");
        _affine_z = std::move(affine);
    }

    void set_xyz_affines(BeamAffine1D affine_x, BeamAffine1D affine_y, BeamAffine1D affine_z)
    {
        check_affine_size(affine_x, "set_xyz_affines (x)");
        check_affine_size(affine_y, "set_xyz_affines (y)");
        check_affine_size(affine_z, "set_xyz_affines (z)");
        _affine_x = std::move(affine_x);
        _affine_y = std::move(affine_y);
        _affine_z = std::move(affine_z);
    }

    // --- factory functions ---

    /**
     * @brief Create a BeamSampleGeometry from a base XYZ location and per-beam bottom XYZ
     * locations with their corresponding sample numbers.
     *
     * Computes affines such that:
     *   coord(sample_nr) = base_coord + (bottom_coord - base_coord) / bottom_sample_nr *
     * sample_nr
     *
     * @param base_xyz base location (single point, e.g. transducer position)
     * @param bottom_xyz per-beam bottom locations [n_beams]
     * @param bottom_sample_numbers per-beam sample number at the bottom [n_beams]
     * @param first_sample_numbers per-beam first valid sample number [n_beams]
     * @param number_of_samples per-beam number of samples [n_beams]
     * @return BeamSampleGeometry with x, y, z affines set
     */
    static BeamSampleGeometry from_bottom_xyz(
        const XYZ<1>&                      bottom_xyz,
        float                              base_x,
        float                              base_y,
        float                              base_z,
        const xt::xtensor<float, 1>&       bottom_sample_numbers,
        xt::xtensor<float, 1>              first_sample_numbers,
        xt::xtensor<unsigned int, 1>       number_of_samples)
    {
        BeamSampleGeometry geom(std::move(first_sample_numbers), std::move(number_of_samples));

        geom.set_x_affine(
            BeamAffine1D::from_base_and_endpoints(base_x, bottom_xyz.x, bottom_sample_numbers));
        geom.set_y_affine(
            BeamAffine1D::from_base_and_endpoints(base_y, bottom_xyz.y, bottom_sample_numbers));
        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(base_z, bottom_xyz.z, bottom_sample_numbers));

        return geom;
    }

    /**
     * @brief Create a BeamSampleGeometry with z affine only, from a base depth and per-beam
     * bottom depths at known sample numbers.
     *
     * @param base_z base depth (e.g. transducer depth)
     * @param bottom_depths per-beam bottom depths [n_beams]
     * @param bottom_sample_numbers per-beam sample number at the bottom [n_beams]
     * @param first_sample_numbers per-beam first valid sample number [n_beams]
     * @param number_of_samples per-beam number of samples [n_beams]
     * @return BeamSampleGeometry with z affine set
     */
    static BeamSampleGeometry from_bottom_z(
        float                              base_z,
        const xt::xtensor<float, 1>&       bottom_depths,
        const xt::xtensor<float, 1>&       bottom_sample_numbers,
        xt::xtensor<float, 1>              first_sample_numbers,
        xt::xtensor<unsigned int, 1>       number_of_samples)
    {
        BeamSampleGeometry geom(std::move(first_sample_numbers), std::move(number_of_samples));

        geom.set_z_affine(
            BeamAffine1D::from_base_and_endpoints(base_z, bottom_depths, bottom_sample_numbers));

        return geom;
    }

    // --- future factory stubs (not implemented yet) ---
    // static BeamSampleGeometry from_angles(
    //     float base_x, float base_y, float base_z,
    //     float yaw, float pitch, float roll,
    //     const xt::xtensor<float, 1>& alongtrack_angles,
    //     const xt::xtensor<float, 1>& crosstrack_angles,
    //     float sound_velocity,
    //     const xt::xtensor<float, 1>& sample_interval,
    //     xt::xtensor<float, 1> first_sample_numbers,
    //     xt::xtensor<unsigned int, 1> number_of_samples);

  public:
    // ----- file I/O -----
    static BeamSampleGeometry from_stream(std::istream& is)
    {
        BeamSampleGeometry data;

        is.read(reinterpret_cast<char*>(&data._n_beams), sizeof(size_t));

        data._first_sample_numbers = xt::xtensor<float, 1>::from_shape({ data._n_beams });
        data._number_of_samples    = xt::xtensor<unsigned int, 1>::from_shape({ data._n_beams });

        is.read(reinterpret_cast<char*>(data._first_sample_numbers.data()),
                sizeof(float) * data._n_beams);
        is.read(reinterpret_cast<char*>(data._number_of_samples.data()),
                sizeof(unsigned int) * data._n_beams);

        // read optional affines
        bool has_x, has_y, has_z;
        is.read(reinterpret_cast<char*>(&has_x), sizeof(bool));
        is.read(reinterpret_cast<char*>(&has_y), sizeof(bool));
        is.read(reinterpret_cast<char*>(&has_z), sizeof(bool));

        if (has_x)
            data._affine_x = BeamAffine1D::from_stream(is);
        if (has_y)
            data._affine_y = BeamAffine1D::from_stream(is);
        if (has_z)
            data._affine_z = BeamAffine1D::from_stream(is);

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_n_beams), sizeof(size_t));

        os.write(reinterpret_cast<const char*>(_first_sample_numbers.data()),
                 sizeof(float) * _n_beams);
        os.write(reinterpret_cast<const char*>(_number_of_samples.data()),
                 sizeof(unsigned int) * _n_beams);

        // write optional affines
        bool has_x = _affine_x.has_value();
        bool has_y = _affine_y.has_value();
        bool has_z = _affine_z.has_value();
        os.write(reinterpret_cast<const char*>(&has_x), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&has_y), sizeof(bool));
        os.write(reinterpret_cast<const char*>(&has_z), sizeof(bool));

        if (has_x)
            _affine_x->to_stream(os);
        if (has_y)
            _affine_y->to_stream(os);
        if (has_z)
            _affine_z->to_stream(os);
    }

  private:
    void check_affine_size(const BeamAffine1D& affine, const std::string& name) const
    {
        if (affine.size() != _n_beams)
            throw std::runtime_error(
                fmt::format("BeamSampleGeometry::{}: affine size ({}) does not match n_beams ({})",
                            name,
                            affine.size(),
                            _n_beams));
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BeamSampleGeometry", float_precision, superscript_exponents);

        printer.register_value("n_beams", _n_beams);
        printer.register_container("first_sample_numbers", _first_sample_numbers);
        printer.register_container("number_of_samples", _number_of_samples);
        printer.register_value("has_x_affine", has_x_affine());
        printer.register_value("has_y_affine", has_y_affine());
        printer.register_value("has_z_affine", has_z_affine());

        if (_affine_x)
            printer.register_section("x_affine");
        if (_affine_y)
            printer.register_section("y_affine");
        if (_affine_z)
            printer.register_section("z_affine");

        return printer;
    }

  public:
    // -- class helper function macros --
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSampleGeometry)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping
