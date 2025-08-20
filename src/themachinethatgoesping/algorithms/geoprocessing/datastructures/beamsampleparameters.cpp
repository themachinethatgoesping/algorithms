// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "beamsampleparameters.hpp"

#include <stdexcept>
#include <fmt/core.h>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

BeamSampleParameters::BeamSampleParameters(xt::xtensor<float, 1>        alongtrack_angles,
                                           xt::xtensor<float, 1>        crosstrack_angles,
                                           xt::xtensor<float, 1>        first_sample_offset,
                                           xt::xtensor<float, 1>        sample_interval,
                                           xt::xtensor<unsigned int, 1> number_of_samples)
    : _alongtrack_angles(std::move(alongtrack_angles))
    , _crosstrack_angles(std::move(crosstrack_angles))
    , _first_sample_offset(std::move(first_sample_offset))
    , _sample_interval(std::move(sample_interval))
    , _number_of_samples(std::move(number_of_samples))
{
    // verify the sizes
    if (this->_alongtrack_angles.size() != this->_crosstrack_angles.size() ||
        this->_alongtrack_angles.size() != this->_first_sample_offset.size() ||
        this->_alongtrack_angles.size() != this->_sample_interval.size() ||
        this->_alongtrack_angles.size() != this->_number_of_samples.size())
    {
        throw std::runtime_error(
            fmt::format("ERROR[BeamSampleParameters]:: All parameters must have the same size! "
                        "({} != {} != {} != {} != {} )",
                        this->_alongtrack_angles.size(),
                        this->_crosstrack_angles.size(),
                        this->_first_sample_offset.size(),
                        this->_sample_interval.size(),
                        this->_number_of_samples.size()));
    }
}

BeamSampleParameters BeamSampleParameters::from_stream(std::istream& is)
{
    size_t number_of_beams;
    is.read(reinterpret_cast<char*>(&number_of_beams), sizeof(size_t));

    BeamSampleParameters data(number_of_beams);

    is.read(reinterpret_cast<char*>(data._alongtrack_angles.data()),
            data._alongtrack_angles.size() * sizeof(float));
    is.read(reinterpret_cast<char*>(data._crosstrack_angles.data()),
            data._crosstrack_angles.size() * sizeof(float));
    is.read(reinterpret_cast<char*>(data._first_sample_offset.data()),
            data._first_sample_offset.size() * sizeof(float));
    is.read(reinterpret_cast<char*>(data._sample_interval.data()),
            data._sample_interval.size() * sizeof(float));
    is.read(reinterpret_cast<char*>(data._number_of_samples.data()),
            data._number_of_samples.size() * sizeof(unsigned int));

    return data;
}

void BeamSampleParameters::to_stream(std::ostream& os) const
{
    size_t number_of_beams = _alongtrack_angles.size();
    os.write(reinterpret_cast<const char*>(&number_of_beams), sizeof(number_of_beams));

    os.write(reinterpret_cast<const char*>(_alongtrack_angles.data()),
             _alongtrack_angles.size() * sizeof(float));
    os.write(reinterpret_cast<const char*>(_crosstrack_angles.data()),
             _crosstrack_angles.size() * sizeof(float));
    os.write(reinterpret_cast<const char*>(_first_sample_offset.data()),
             _first_sample_offset.size() * sizeof(float));
    os.write(reinterpret_cast<const char*>(_sample_interval.data()),
             _sample_interval.size() * sizeof(float));
    os.write(reinterpret_cast<const char*>(_number_of_samples.data()),
             _number_of_samples.size() * sizeof(unsigned int));
}

tools::classhelper::ObjectPrinter BeamSampleParameters::__printer__(unsigned int float_precision, 
                                                                    bool         superscript_exponents) const
{
    tools::classhelper::ObjectPrinter printer("BeamSampleParameters", float_precision, superscript_exponents);

    printer.register_container("alongtrack_angles", _alongtrack_angles);
    printer.register_container("crosstrack_angles", _crosstrack_angles);
    printer.register_container("first_sample_offset", _first_sample_offset);
    printer.register_container("sample_interval", _sample_interval);
    printer.register_container("number_of_samples", _number_of_samples);

    return printer;
}

void BeamSampleParameters::check_size(const xt::xtensor<float, 1>& data, const std::string& name) const
{
    if (data.size() != _number_of_samples.size())
    {
        throw std::runtime_error(fmt::format("ERROR[BeamSampleParameters::{}]: All parameters "
                                             "must have the same size! ({} != {})",
                                             name,
                                             data.size(),
                                             _number_of_samples.size()));
    }
}

}
}
}
}