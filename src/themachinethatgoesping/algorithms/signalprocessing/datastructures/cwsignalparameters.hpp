/**
 * @file cwsignalparameters.hpp
 * @brief Definition of the CWSignalParameters struct.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/cwsignalparameters.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>



#include "../types.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

/**
 * @class CWSignalParameters
 * @brief Class representing information about a continuous wave signal.
 */
class CWSignalParameters
{
    float _center_frequency;         ///< Center frequency of the signal in Hz.
    float _bandwidth;                ///< Bandwidth of the signal in Hz.
    float _effective_pulse_duration; ///< Effective pulse duration of the signal in seconds.

    /**
     * @brief Default constructor.
     */
    CWSignalParameters() = default;

  public:
    /**
     * @brief Constructor.
     * @param center_frequency The center frequency of the signal in Hz.
     * @param bandwidth The bandwidth of the signal in Hz.
     * @param effective_pulse_duration The effective pulse duration of the signal in seconds.
     */
    CWSignalParameters(float center_frequency, float bandwidth, float effective_pulse_duration)
        : _center_frequency(center_frequency)
        , _bandwidth(bandwidth)
        , _effective_pulse_duration(effective_pulse_duration)
    {
    }

    /**
     * @brief Destructor.
     */
    virtual ~CWSignalParameters() = default;

    /**
     * @brief Equality operator.
     * @param rhs The right-hand side of the operator.
     * @return True if the objects are equal, false otherwise.
     */
    bool operator==(const CWSignalParameters& rhs) const
    {
        if (_center_frequency != rhs._center_frequency)
            if (!std::isnan(_center_frequency) && !std::isnan(rhs._center_frequency))
                return false;

        if (_bandwidth != rhs._bandwidth)
            if (!std::isnan(_bandwidth) && !std::isnan(rhs._bandwidth))
                return false;

        if (_effective_pulse_duration != rhs._effective_pulse_duration)
            if (!std::isnan(_effective_pulse_duration) &&
                !std::isnan(rhs._effective_pulse_duration))
                return false;

        return true;
    }

  public:
    // ----- getters -----
    float                 get_center_frequency() const { return _center_frequency; }
    float                 get_bandwidth() const { return _bandwidth; }
    float                 get_effective_pulse_duration() const { return _effective_pulse_duration; }
    types::o_TxSignalType get_tx_signal_type() const { return types::t_TxSignalType::CW; }

    // ----- setters -----
    void set_center_frequency(float center_frequency)
    {
        this->_center_frequency = center_frequency;
    }
    void set_bandwidth(float bandwidth) { this->_bandwidth = bandwidth; }
    void set_effective_pulse_duration(float effective_pulse_duration)
    {
        this->_effective_pulse_duration = effective_pulse_duration;
    }

    // ----- file I/O -----
    static constexpr size_t binary_size() { return sizeof(float) * 3; }

    /**
     * @brief Read CWSignalParameters from a stream.
     * @param is The input stream.
     * @return The read CWSignalParameters object.
     */
    static CWSignalParameters from_stream(std::istream& is)
    {
        CWSignalParameters data;

        is.read(reinterpret_cast<char*>(&data._center_frequency), binary_size());

        return data;
    }

    /**
     * @brief Write CWSignalParameters to a stream.
     * @param os The output stream.
     */
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&_center_frequency), binary_size());
    }

  public:
    /**
     * @brief Get the ObjectPrinter for CWSignalParameters.
     * @param float_precision The precision for floating-point values.
     * @return The ObjectPrinter.
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("CWSignalParameters", float_precision, superscript_exponents);

        printer.register_value("center_frequency", _center_frequency, "Hz");
        printer.register_value("bandwidth", _bandwidth, "Hz");
        printer.register_value("effective_pulse_duration", _effective_pulse_duration, "s");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(CWSignalParameters)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

// IGNORE_DOC:mkd_doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_hash_value
/**
 * @brief Boost hash function
 *
 * @param object object to hash
 * @return std::size_t
 */
inline std::size_t hash_value(const CWSignalParameters& object)
{
    return object.binary_hash();
}

} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping