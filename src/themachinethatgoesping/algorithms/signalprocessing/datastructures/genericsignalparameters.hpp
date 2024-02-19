/**
 * @file cwsignalparameters.hpp
 * @brief Definition of the GenericSignalParameters struct.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/genericsignalparameters.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

#include "../types.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

/**
 * @struct GenericSignalParameters
 * @brief Struct representing information about a generic type.
 */
struct GenericSignalParameters
{
    float center_frequency;         ///< Center frequency of the signal in Hz.
    float bandwidth;                ///< Bandwidth of the signal in Hz.
    float effective_pulse_duration; ///< Effective pulse duration of the signal in seconds.
    types::t_TxSignalType signal_type = types::t_TxSignalType::UNKNOWN; ///< Signal type

    /**
     * @brief Default constructor.
     */
    GenericSignalParameters() = default;
  public:

    /**
     * @brief Constructor.
     * @param center_frequency The center frequency of the signal in Hz.
     * @param bandwidth The bandwidth of the signal in Hz.
     * @param effective_pulse_duration The effective pulse duration of the signal in seconds.
     */
    GenericSignalParameters(float                 center_frequency,
                            float                 bandwidth,
                            float                 effective_pulse_duration,
                            types::t_TxSignalType signal_type)
        : center_frequency(center_frequency)
        , bandwidth(bandwidth)
        , effective_pulse_duration(effective_pulse_duration)
        , signal_type(signal_type)
    {
    }

    /**
     * @brief Destructor.
     */
    virtual ~GenericSignalParameters() = default;

    /**
     * @brief Equality operator.
     * @param rhs The right-hand side of the operator.
     * @return True if the objects are equal, false otherwise.
     */
    bool operator==(const GenericSignalParameters& rhs) const
    {
        if (center_frequency != rhs.center_frequency)
            if (!std::isnan(center_frequency) && !std::isnan(rhs.center_frequency))
                return false;

        if (bandwidth != rhs.bandwidth)
            if (!std::isnan(bandwidth) && !std::isnan(rhs.bandwidth))
                return false;

        if (effective_pulse_duration != rhs.effective_pulse_duration)
            if (!std::isnan(effective_pulse_duration) && !std::isnan(rhs.effective_pulse_duration))
                return false;

        return signal_type == rhs.signal_type;
    }

  public:
    types::t_TxSignalType get_tx_signal_type() const { return signal_type; }

    // ----- file I/O -----

    /**
     * @brief Read GenericSignalParameters from a stream.
     * @param is The input stream.
     * @return The read GenericSignalParameters object.
     */
    static GenericSignalParameters from_stream(std::istream& is)
    {
        GenericSignalParameters data;

        is.read(reinterpret_cast<char*>(&data.center_frequency),
                sizeof(float) * 3 + sizeof(types::t_TxSignalType));

        return data;
    }

    /**
     * @brief Write GenericSignalParameters to a stream.
     * @param os The output stream.
     */
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&center_frequency),
                 sizeof(float) * 3 + sizeof(types::t_TxSignalType));
    }

  public:
    /**
     * @brief Get the ObjectPrinter for GenericSignalParameters.
     * @param float_precision The precision for floating-point values.
     * @return The ObjectPrinter.
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("GenericSignalParameters", float_precision);

        printer.register_value("center_frequency", center_frequency, "Hz");
        printer.register_value("bandwidth", bandwidth, "Hz");
        printer.register_value("effective_pulse_duration", effective_pulse_duration, "s");
        printer.register_value("signal_type", types::to_string(get_tx_signal_type()));

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(GenericSignalParameters)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

// IGNORE_DOC: __doc_themachinethatgoesping_algorithms_signalprocessing_datastructures_hash_value
/**
 * @brief Boost hash function
 *
 * @param object object to hash
 * @return std::size_t
 */
inline std::size_t hash_value(const GenericSignalParameters& object)
{
    return object.binary_hash();
}

} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping