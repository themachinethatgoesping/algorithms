// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/backtracedwci.doc.hpp"

#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>
#include <themachinethatgoesping/tools/vectorinterpolators.hpp>

#include "../datastructures.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace backtracers {

/**
 * @brief A structure to store a watercolumn image together with the necessary informations for
 * raytracing.
 */
class BacktracedWCI
{
    using NearestInterpolatorFI =
        typename tools::vectorinterpolators::NearestInterpolator<float, uint16_t>;
    using LinearInterpolatorFF =
        typename tools::vectorinterpolators::LinearInterpolator<float, float>;

    xt::xtensor<float, 2> _wci; ///< water column image

    NearestInterpolatorFI             _angle_beamnumber_interpolator;
    std::vector<LinearInterpolatorFF> _range_samplenumber_interpolators;

    float    _min_angle;
    float    _max_angle;
    uint16_t _wci_first_sample_number;
    uint16_t _wci_sample_number_step;

    BacktracedWCI() = default;

  public:
    /**
     * @brief Construct a new sample location object (initialize all tensors using the specified
     * shape (empty))
     *
     * @param wci Water column image, shape: len(beam_reference_directions) x does not matter
     * @param beam_reference_directions beam reference directions: reference points that describe
     * beam angle and reference range for each beam
     *
     */
    BacktracedWCI(const xt::xtensor<float, 2>&                    wci,
                  const datastructures::SampleDirectionsRange<1>& beam_reference_directions,
                  const std::vector<uint16_t>&                    beam_reference_sample_numbers,
                  uint16_t                                        wci_first_sample_number,
                  uint16_t                                        wci_sample_number_step = 1)
        : _wci(wci)
        , _wci_first_sample_number(
              std::roundf(float(wci_first_sample_number) / wci_sample_number_step))
        , _wci_sample_number_step(wci_sample_number_step)
    {
        using tools::vectorinterpolators::LinearInterpolator;
        using tools::vectorinterpolators::NearestInterpolator;

        if (beam_reference_directions.size() == 0)
            throw std::runtime_error("lookup: beam_reference_directions is empty");

        if (_wci.shape()[0] < beam_reference_directions.size() ||
            _wci.shape()[0] < beam_reference_sample_numbers.size())
            throw std::runtime_error(
                fmt::format("lookup: wci.shape() = [{},{}], beam_reference_directions.size() = {}, "
                            "beam_reference_sample_numbers.size() = {}",
                            _wci.shape()[0],
                            _wci.shape()[1],
                            beam_reference_directions.size(),
                            beam_reference_sample_numbers.size()));

        // sort the beam indices by angle and add them to a nearest interpolator
        auto sorted_beam_index = xt::argsort(beam_reference_directions.crosstrack_angle);
        auto sorted_beam_angles =
            xt::index_view(beam_reference_directions.crosstrack_angle, sorted_beam_index);

        size_t last_index = sorted_beam_angles.size() - 1;
        float  delta_angle =
            (sorted_beam_angles.unchecked(last_index) - sorted_beam_angles.unchecked(0)) /
            (last_index);
        _min_angle = sorted_beam_angles.unchecked(0) - delta_angle / 2;
        _max_angle = sorted_beam_angles.unchecked(last_index) + delta_angle / 2;

        _angle_beamnumber_interpolator = NearestInterpolatorFI(
            std::vector<float>(sorted_beam_angles.begin(), sorted_beam_angles.end()),
            std::vector<uint16_t>(sorted_beam_index.begin(), sorted_beam_index.end()),
            tools::vectorinterpolators::t_extr_mode::nearest);

        // create sample interpolator for each beam
        _range_samplenumber_interpolators.clear();
        _range_samplenumber_interpolators.reserve(beam_reference_directions.size());
        for (size_t bn = 0; bn < beam_reference_directions.size(); ++bn)
        {
            _range_samplenumber_interpolators.emplace_back(
                std::vector<float>{ 0.f, float(beam_reference_directions.range[bn]) },
                std::vector<float>{
                    0.f,
                    std::roundf(float(beam_reference_sample_numbers[bn]) / wci_sample_number_step) });
        }

        check_shape();
    }

    float lookup(float beam_angle, float range)
    {
        if (beam_angle < _min_angle || beam_angle > _max_angle)
            return std::numeric_limits<float>::quiet_NaN();

        uint16_t bn = _angle_beamnumber_interpolator(beam_angle);
        int      sn = int(_range_samplenumber_interpolators[bn](range)) - _wci_first_sample_number;

        if (sn < 0 || sn >= int(_wci.shape()[1]))
            return std::numeric_limits<float>::quiet_NaN();

        return _wci.unchecked(bn, sn);
    }

    bool operator==(const BacktracedWCI& rhs) const = default;

    size_t size() const { return _wci.size(); }

    std::array<size_t, 2> shape() const { return _wci.shape(); }

    // getters
    const xt::xtensor<float, 2>& get_wci() const { return _wci; }
    const NearestInterpolatorFI& get_angle_beamnumber_interpolator() const
    {
        return _angle_beamnumber_interpolator;
    }

    const std::vector<LinearInterpolatorFF>& get_range_samplenumber_interpolators() const
    {
        return _range_samplenumber_interpolators;
    }
    float    get_min_angle() const { return _min_angle; }
    float    get_max_angle() const { return _max_angle; }
    uint16_t get_wci_first_sample_number() const
    {
        return _wci_first_sample_number * _wci_sample_number_step;
    }
    /**
     * @brief Get the internally saved wci first sample number.
     *        Internally: the first_sample_number of the image is divided by sample_number_step
     * 
     * @return uint16_t 
     */
    uint16_t get_wci_first_sample_number_internal() const { return _wci_first_sample_number; }
    uint16_t get_wci_sample_number_step() const { return _wci_sample_number_step; }

  public:
    // ----- file I/O -----
    static BacktracedWCI from_stream(std::istream& is)
    {
        BacktracedWCI data;

        std::array<size_t, 2> shape;

        is.read(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        data._wci = xt::xtensor<float, 2>::from_shape(shape);

        is.read(reinterpret_cast<char*>(data._wci.data()), sizeof(float) * data._wci.size());

        data._angle_beamnumber_interpolator = NearestInterpolatorFI::from_stream(is);
        for (size_t i = 0; i < shape[0]; ++i)
            data._range_samplenumber_interpolators.push_back(LinearInterpolatorFF::from_stream(is));

        is.read(reinterpret_cast<char*>(&data._min_angle),
                sizeof(data._min_angle) + sizeof(data._max_angle) +
                    sizeof(data._wci_first_sample_number) + sizeof(data._wci_sample_number_step));

        data.check_shape();

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        check_shape();

        std::array<size_t, 2> shape = _wci.shape();

        os.write(reinterpret_cast<char*>(shape.data()), sizeof(size_t) * shape.size());

        os.write(reinterpret_cast<const char*>(_wci.data()), sizeof(float) * _wci.size());

        _angle_beamnumber_interpolator.to_stream(os);
        for (size_t i = 0; i < shape[0]; ++i)
            _range_samplenumber_interpolators[i].to_stream(os);

        os.write(reinterpret_cast<const char*>(&_min_angle),
                 sizeof(_min_angle) + sizeof(_max_angle) + sizeof(_wci_first_sample_number) +
                     sizeof(_wci_sample_number_step));
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision,
                                                  bool         superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer(
            "BacktracedWCI", float_precision, superscript_exponents);

        printer.register_container("wci", _wci, "°");
        printer.register_value("min_angle", _min_angle, "°");
        printer.register_value("max_angle", _max_angle, "°");
        printer.register_value("wci_first_sample_number",
                               _wci_first_sample_number * _wci_sample_number_step,
                               std::to_string(_wci_first_sample_number));
        printer.register_value("wci_sample_number_step", _wci_sample_number_step, "");
        printer.append(
            _angle_beamnumber_interpolator.__printer__(float_precision, superscript_exponents));
        for (size_t i = 0; i < _range_samplenumber_interpolators.size(); ++i)
            printer.append(_range_samplenumber_interpolators[i].__printer__(float_precision,
                                                                            superscript_exponents));

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BacktracedWCI)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__

  private:
    void check_shape() const
    {
        // compare shape
        if (_wci.shape()[0] != _range_samplenumber_interpolators.size() ||
            //_angle_beamnumber_interpolator.get_y(_min_angle) < 0 ||
            uint16_t(_angle_beamnumber_interpolator.get_y(_min_angle)) >= _wci.shape()[0] ||
            //_angle_beamnumber_interpolator.get_y(_max_angle) < 0 ||
            uint16_t(_angle_beamnumber_interpolator.get_y(_max_angle)) >= _wci.shape()[0])
        {
            throw std::runtime_error(
                fmt::format("BacktraceWCI: shape missmatch!\n-wci.shape() = [{},{}]"
                            "\n-_range_samplenumber_interpolators.size() = {}"
                            "\n-beam range(min_angle) = {}"
                            "\n-beam range(max_angle) = {}",
                            _wci.shape()[0],
                            _wci.shape()[1],
                            _range_samplenumber_interpolators.size(),
                            _angle_beamnumber_interpolator.get_y(_min_angle),
                            _angle_beamnumber_interpolator.get_y(_max_angle)));
        }
    }
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping