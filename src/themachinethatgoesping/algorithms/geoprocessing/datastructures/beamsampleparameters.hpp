// Kiel SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/beamsampleparameters.doc.hpp"

#include <map>
#include <vector>

#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

#include "sampledirections.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store directional parameters of multibeam system. Usefull as input for
 * raytracing water column images.
 */
class BeamSampleParameters
{
    xt::xtensor<float, 1> _alongtrack_angles; ///< in °, positive bow up, 0 == downwards
    xt::xtensor<float, 1> _crosstrack_angles; ///< in °, positive portside up, 0 == downwards
    xt::xtensor<float, 1>
        _first_sample_offset; ///< in s, time offset of the first sample (sample number 0)
    xt::xtensor<float, 1>        _sample_interval;   ///< in s, time interval between samples
    xt::xtensor<unsigned int, 1> _number_of_samples; ///< number of samples per beam

  public:
    /**
     * @brief Construct a new BeamSampleParameters object (all values uninitialized)
     *
     * @param number_of_beams number of beams
     */
    BeamSampleParameters(unsigned int number_of_beams)
        : _alongtrack_angles(xt::empty<float>({ number_of_beams }))
        , _crosstrack_angles(xt::empty<float>({ number_of_beams }))
        , _first_sample_offset(xt::empty<float>({ number_of_beams }))
        , _sample_interval(xt::empty<float>({ number_of_beams }))
        , _number_of_samples(xt::empty<unsigned int>({ number_of_beams }))
    {
    }

    /**
     * @brief Construct a new Sample Indices object from existing beam parameters
     *
     * @param alongtrack_angles alongtrack angles of the MBES in °
     * @param crosstrack_angles crosstrack angles of the MBES in °
     * @param first_sample_offset time offset of the first sample (sample number 0) in s
     * @param sample_interval time interval between samples in s
     * @param number_of_samples number of samples per beam
     *
     * @note all parameters must have the same size
     */
    BeamSampleParameters(xt::xtensor<float, 1>        alongtrack_angles,
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

    // ----- accessors -----
    const xt::xtensor<float, 1>& get_alongtrack_angles() const { return _alongtrack_angles; }
    const xt::xtensor<float, 1>& get_crosstrack_angles() const { return _crosstrack_angles; }
    const xt::xtensor<float, 1>& get_first_sample_offset() const { return _first_sample_offset; }
    const xt::xtensor<float, 1>& get_sample_interval() const { return _sample_interval; }
    const xt::xtensor<unsigned int, 1>& get_number_of_samples() const { return _number_of_samples; }

    // ----- setters -----
    void set_alongtrack_angles(xt::xtensor<float, 1> alongtrack_angles)
    {
        check_size(alongtrack_angles, "set_alongtrack_angles");
        this->_alongtrack_angles = std::move(alongtrack_angles);
    }
    void set_crosstrack_angles(xt::xtensor<float, 1> crosstrack_angles)
    {
        check_size(crosstrack_angles, "set_crosstrack_angles");
        this->_crosstrack_angles = std::move(crosstrack_angles);
    }
    void set_first_sample_offset(xt::xtensor<float, 1> first_sample_offset)
    {
        check_size(first_sample_offset, "set_first_sample_offset");
        this->_first_sample_offset = std::move(first_sample_offset);
    }
    void set_sample_interval(xt::xtensor<float, 1> sample_interval)
    {
        check_size(sample_interval, "set_sample_interval");
        this->_sample_interval = std::move(sample_interval);
    }
    void set_number_of_samples(const xt::xtensor<unsigned int, 1>& number_of_samples)
    {
        check_size(number_of_samples, "set_number_of_samples");
        this->_number_of_samples = std::move(number_of_samples);
    }

    bool operator==(const BeamSampleParameters& rhs) const = default;

  public:
    // ----- file I/O -----
    static BeamSampleParameters from_stream(std::istream& is)
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

    void to_stream(std::ostream& os) const
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

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("BeamSampleParameters", float_precision, superscript_exponents);

        printer.register_container("alongtrack_angles", _alongtrack_angles);
        printer.register_container("crosstrack_angles", _crosstrack_angles);
        printer.register_container("first_sample_offset", _first_sample_offset);
        printer.register_container("sample_interval", _sample_interval);
        printer.register_container("number_of_samples", _number_of_samples);

        return printer;
    }

  protected:
    void check_size(const xt::xtensor<float, 1>& data, const std::string& name) const
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

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(BeamSampleParameters)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping