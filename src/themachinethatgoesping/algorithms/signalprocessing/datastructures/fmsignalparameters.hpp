/**
 * @file cwsignalparameters.hpp
 * @brief Definition of the FMSignalParameters struct.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/fmsignalparameters.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

#include "../types.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

/**
 * @struct FMSignalParameters
 * @brief Struct representing information about a frequency modulated wave signal (chirp).
 */
struct FMSignalParameters
{
    float center_frequency;         ///< Center frequency of the signal in Hz.
    float bandwidth;                ///< Bandwidth of the signal in Hz.
    float effective_pulse_duration; ///< Effective pulse duration of the signal in seconds.
    bool  up_sweep = false;         ///< True if the signal is an up sweep, false otherwise.

  public:
    /**
     * @brief Default constructor.
     */
    FMSignalParameters() = default;

    /**
     * @brief Constructor.
     * @param center_frequency The center frequency of the signal in Hz.
     * @param bandwidth The bandwidth of the signal in Hz.
     * @param effective_pulse_duration The effective pulse duration of the signal in seconds.
     */
    FMSignalParameters(float center_frequency,
                       float bandwidth,
                       float effective_pulse_duration,
                       bool  up_sweep)
        : center_frequency(center_frequency)
        , bandwidth(bandwidth)
        , effective_pulse_duration(effective_pulse_duration)
        , up_sweep(up_sweep)
    {
    }

    /**
     * @brief Constructor.
     * @param center_frequency The center frequency of the signal in Hz.
     * @param bandwidth The bandwidth of the signal in Hz.
     * @param effective_pulse_duration The effective pulse duration of the signal in seconds.
     */
    FMSignalParameters(float                 center_frequency,
                       float                 bandwidth,
                       float                 effective_pulse_duration,
                       types::t_TxSignalType signal_type)
        : center_frequency(center_frequency)
        , bandwidth(bandwidth)
        , effective_pulse_duration(effective_pulse_duration)
    {
        switch (signal_type)
        {
            case types::t_TxSignalType::FM_UP_SWEEP:
                up_sweep = true;
                break;
            case types::t_TxSignalType::FM_DOWN_SWEEP:
                up_sweep = false;
                break;
            default:
                throw std::runtime_error("Invalid signal type for FMSignalParameters");
        }
    }

    /**
     * @brief Destructor.
     */
    virtual ~FMSignalParameters() = default;

    /**
     * @brief Equality operator.
     * @param rhs The right-hand side of the operator.
     * @return True if the objects are equal, false otherwise.
     */
    bool operator==(const FMSignalParameters& rhs) const = default;

  public:
    types::t_TxSignalType get_tx_signal_type() const
    {
        if (up_sweep)
            return types::t_TxSignalType::FM_UP_SWEEP;
        else
            return types::t_TxSignalType::FM_DOWN_SWEEP;
    }

    // ----- file I/O -----

    /**
     * @brief Read FMSignalParameters from a stream.
     * @param is The input stream.
     * @return The read FMSignalParameters object.
     */
    static FMSignalParameters from_stream(std::istream& is)
    {
        FMSignalParameters data;

        is.read(reinterpret_cast<char*>(&data.center_frequency), sizeof(float) * 3 + sizeof(bool));

        return data;
    }

    /**
     * @brief Write FMSignalParameters to a stream.
     * @param os The output stream.
     */
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&center_frequency),
                 sizeof(float) * 3 + sizeof(bool));
    }

  public:
    /**
     * @brief Get the ObjectPrinter for FMSignalParameters.
     * @param float_precision The precision for floating-point values.
     * @return The ObjectPrinter.
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("FMSignalParameters", float_precision);

        printer.register_value("center_frequency", center_frequency, "Hz");
        printer.register_value("bandwidth", bandwidth, "Hz");
        printer.register_value("effective_pulse_duration", effective_pulse_duration, "s");
        printer.register_value("Sweep direction", types::to_string(get_tx_signal_type()));

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(FMSignalParameters)
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
inline std::size_t hash_value(const FMSignalParameters& object)
{
    return object.binary_hash();
}

} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping