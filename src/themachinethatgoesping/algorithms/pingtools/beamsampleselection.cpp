// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "beamsampleselection.hpp"

/* std includes */
#include <algorithm>
#include <iostream>
#include <ranges>

/* external includes */
#include <fmt/format.h>
#include <xtensor/views/xview.hpp>

/* ping includes */
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper/integermath.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace pingtools {

BeamSampleSelection::BeamSampleSelection(BeamSelection beam_selection)
    : BeamSelection(std::move(beam_selection))
{
    _first_sample_number_per_beam = std::vector<uint32_t>(get_number_of_beams(), 0);
    _last_sample_number_per_beam =
        std::vector<uint32_t>(get_number_of_beams(), std::numeric_limits<uint32_t>::max());

    _first_sample_number_ensemble = 0;
    _last_sample_number_ensemble  = std::numeric_limits<uint32_t>::max();
}

BeamSampleSelection::BeamSampleSelection(std::vector<uint32_t> first_sample_number_per_beam,
                                        std::vector<uint32_t> last_sample_number_per_beam,
                                        uint32_t              sample_step_ensemble)
    : BeamSelection(first_sample_number_per_beam.size())
    , _first_sample_number_per_beam(std::move(first_sample_number_per_beam))
    , _last_sample_number_per_beam(std::move(last_sample_number_per_beam))
    , _sample_step_ensemble(sample_step_ensemble)
{
    // check that size is equal
    if (_first_sample_number_per_beam.size() != _last_sample_number_per_beam.size())
        throw std::invalid_argument(fmt::format(
            "ERROR[BeamSampleSelection::init]: first_sample_number_per_beam.size() ({}) != "
            "last_sample_number_per_beam.size() ({})",
            _first_sample_number_per_beam.size(),
            _last_sample_number_per_beam.size()));

    // find min/max sample numbers
    _first_sample_number_ensemble = *std::ranges::min_element(_first_sample_number_per_beam);
    _last_sample_number_ensemble  = *std::ranges::max_element(_last_sample_number_per_beam);
}

ReadSampleRange BeamSampleSelection::get_read_sample_range(uint32_t beam_index,
                                                          uint32_t first_sample_offset_in_beam,
                                                          uint32_t number_of_samples_in_beam) const
{
    using tools::helper::substract_set_zero_if_negative;

    if (beam_index >= get_number_of_beams())
    {
        throw std::runtime_error(fmt::format(
            "BeamSampleSelection::get_read_sample_range: beam index {} out of range [0,{}]",
            beam_index,
            get_number_of_beams() - 1));
    }

    auto first_beam_sample_to_read = substract_set_zero_if_negative<uint32_t>(
        std::max(_first_sample_number_per_beam[beam_index], _first_sample_number_ensemble),
        first_sample_offset_in_beam);

    auto last_beam_sample_to_read = substract_set_zero_if_negative<uint32_t>(
        std::min(_last_sample_number_per_beam[beam_index], _last_sample_number_ensemble),
        first_sample_offset_in_beam);

    if (last_beam_sample_to_read >= number_of_samples_in_beam)
    {
        last_beam_sample_to_read = number_of_samples_in_beam - 1;
    }

    int number_of_samples_to_read =
        (last_beam_sample_to_read - first_beam_sample_to_read) / _sample_step_ensemble + 1;
    if (number_of_samples_to_read < 0)
    {
        number_of_samples_to_read = 0;
    }

    int first_read_sample_offset = first_beam_sample_to_read + first_sample_offset_in_beam;
    int last_read_sample_offset =
        first_read_sample_offset + (number_of_samples_to_read - 1) * _sample_step_ensemble;

    if (last_read_sample_offset < first_read_sample_offset)
    {
        last_read_sample_offset = first_read_sample_offset;
    }

    return ReadSampleRange(uint32_t(first_beam_sample_to_read),
                           uint32_t(number_of_samples_to_read),
                           uint32_t(first_read_sample_offset),
                           uint32_t(last_read_sample_offset));
}

void BeamSampleSelection::add_beam(uint32_t beam_nr,
                                  uint32_t first_sample_number,
                                  uint32_t last_sample_number)
{
    BeamSelection::add_beam(beam_nr);

    _first_sample_number_per_beam.push_back(first_sample_number);
    _last_sample_number_per_beam.push_back(last_sample_number);

    _first_sample_number_ensemble =
        std::min(_first_sample_number_ensemble, first_sample_number);
    _last_sample_number_ensemble = std::max(_last_sample_number_ensemble, last_sample_number);
}

uint32_t BeamSampleSelection::get_number_of_samples_ensemble() const
{
    // this check is necessary because of potential buffer overflows
    if (_last_sample_number_ensemble < _first_sample_number_ensemble)
        return 0;

    return (_last_sample_number_ensemble - _first_sample_number_ensemble) /
               _sample_step_ensemble +
           1;
}

bool BeamSampleSelection::empty() const
{
    if (BeamSelection::empty())
        return true;
    return get_number_of_samples_ensemble() == 0;
}

xt::xtensor<uint32_t, 1> BeamSampleSelection::get_sample_numbers_ensemble_1d() const
{
    xt::xtensor<uint32_t, 1> sample_numbers_ensemble =
        xt::arange<uint32_t>(_first_sample_number_ensemble,
                             _last_sample_number_ensemble + _sample_step_ensemble,
                             _sample_step_ensemble);

    return sample_numbers_ensemble;
}

xt::xtensor<uint32_t, 2> BeamSampleSelection::get_sample_numbers_ensemble_2d() const
{
    xt::xtensor<uint32_t, 1> sample_numbers_ensemble = get_sample_numbers_ensemble_1d();

    // return copy of sample_numbers_ensemble for each beam as 2d array
    xt::xtensor<uint32_t, 2> sample_numbers_ensemble_2d = xt::xtensor<uint32_t, 2>::from_shape(
        { get_number_of_beams(), get_number_of_samples_ensemble() });

    for (uint32_t bn = 0; bn < get_number_of_beams(); ++bn)
    {
        xt::view(sample_numbers_ensemble_2d, bn) = sample_numbers_ensemble;
    }

    return sample_numbers_ensemble_2d;
}

BeamSampleSelection BeamSampleSelection::from_stream(std::istream& is)
{
    using themachinethatgoesping::tools::classhelper::stream::container_from_stream;

    BeamSampleSelection object(BeamSelection::from_stream(is));
    object._first_sample_number_per_beam = container_from_stream<std::vector<uint32_t>>(is);
    object._last_sample_number_per_beam  = container_from_stream<std::vector<uint32_t>>(is);

    // read other variables
    is.read(reinterpret_cast<char*>(&object._sample_step_ensemble), sizeof(uint32_t) * 3);

    return object;
}

void BeamSampleSelection::to_stream(std::ostream& os) const
{
    using themachinethatgoesping::tools::classhelper::stream::container_to_stream;

    BeamSelection::to_stream(os);
    container_to_stream(os, _first_sample_number_per_beam);
    container_to_stream(os, _last_sample_number_per_beam);

    // write other variables
    os.write(reinterpret_cast<const char*>(&_sample_step_ensemble), sizeof(uint32_t) * 3);
}

tools::classhelper::ObjectPrinter BeamSampleSelection::__printer__(unsigned int float_precision, 
                                                                   bool         superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer("BeamSampleSelection", float_precision, superscript_exponents);

    printer.append(BeamSelection::__printer__(float_precision, superscript_exponents), true);
    printer.register_container("_first_sample_number_per_beam", _first_sample_number_per_beam);
    printer.register_container("_last_sample_number_per_beam", _last_sample_number_per_beam);

    printer.register_value("_sample_step_ensemble", _sample_step_ensemble);
    printer.register_value("_first_sample_number_ensemble", _first_sample_number_ensemble);
    printer.register_value("_last_sample_number_ensemble", _last_sample_number_ensemble);

    return printer;
}

} // namespace pingtools
} // namespace algorithms
} // namespace themachinethatgoesping
