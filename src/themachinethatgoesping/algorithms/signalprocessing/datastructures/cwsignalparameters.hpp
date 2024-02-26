/**
 * @file cwsignalparameters.hpp
 * @brief Definition of the CWSignalParameters struct.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/cwsignalparameters.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

#include "../types.hpp"

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

/**
 * @struct CWSignalParameters
 * @brief Struct representing information about a continuous wave signal.
 */
struct CWSignalParameters
{
    float center_frequency;         ///< Center frequency of the signal in Hz.
    float bandwidth;                ///< Bandwidth of the signal in Hz.
    float effective_pulse_duration; ///< Effective pulse duration of the signal in seconds.

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
        : center_frequency(center_frequency)
        , bandwidth(bandwidth)
        , effective_pulse_duration(effective_pulse_duration)
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
    bool operator==(const CWSignalParameters& rhs) const = default;

  public:
    types::t_TxSignalType get_tx_signal_type() const { return types::t_TxSignalType::CW; }

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

        is.read(reinterpret_cast<char*>(&data.center_frequency), binary_size());

        return data;
    }

    /**
     * @brief Write CWSignalParameters to a stream.
     * @param os The output stream.
     */
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&center_frequency), binary_size());
    }

  public:
    /**
     * @brief Get the ObjectPrinter for CWSignalParameters.
     * @param float_precision The precision for floating-point values.
     * @return The ObjectPrinter.
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("CWSignalParameters", float_precision);

        printer.register_value("center_frequency", center_frequency, "Hz");
        printer.register_value("bandwidth", bandwidth, "Hz");
        printer.register_value("effective_pulse_duration", effective_pulse_duration, "s");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(CWSignalParameters)
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
inline std::size_t hash_value(const CWSignalParameters& object)
{
    return object.binary_hash();
}

} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping