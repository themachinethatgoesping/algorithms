/**
 * @file cwsignalinfos.hpp
 * @brief Definition of the CWSignalInfos struct.
 */

#pragma once

/* generated doc strings */
#include ".docstrings/cwsignalinfos.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace signalprocessing {
namespace datastructures {

/**
 * @struct CWSignalInfos
 * @brief Struct representing information about a continuous wave signal.
 */
struct CWSignalInfos
{
    float center_frequency;         ///< Center frequency of the signal in Hz.
    float bandwidth;                ///< Bandwidth of the signal in Hz.
    float effective_pulse_duration; ///< Effective pulse duration of the signal in seconds.

  public:
    /**
     * @brief Default constructor.
     */
    CWSignalInfos() = default;

    /**
     * @brief Constructor.
     * @param center_frequency The center frequency of the signal in Hz.
     * @param bandwidth The bandwidth of the signal in Hz.
     * @param effective_pulse_duration The effective pulse duration of the signal in seconds.
     */
    CWSignalInfos(float center_frequency, float bandwidth, float effective_pulse_duration)
        : center_frequency(center_frequency)
        , bandwidth(bandwidth)
        , effective_pulse_duration(effective_pulse_duration)
    {
    }

    /**
     * @brief Destructor.
     */
    virtual ~CWSignalInfos() = default;

    /**
     * @brief Equality operator.
     * @param rhs The right-hand side of the operator.
     * @return True if the objects are equal, false otherwise.
     */
    bool operator==(const CWSignalInfos& rhs) const = default;

  public:
    // ----- file I/O -----

    /**
     * @brief Read CWSignalInfos from a stream.
     * @param is The input stream.
     * @return The read CWSignalInfos object.
     */
    static CWSignalInfos from_stream(std::istream& is)
    {
        CWSignalInfos data;

        is.read(reinterpret_cast<char*>(&data.center_frequency), sizeof(CWSignalInfos));

        return data;
    }

    /**
     * @brief Write CWSignalInfos to a stream.
     * @param os The output stream.
     */
    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&center_frequency), sizeof(CWSignalInfos));
    }

  public:
    /**
     * @brief Get the ObjectPrinter for CWSignalInfos.
     * @param float_precision The precision for floating-point values.
     * @return The ObjectPrinter.
     */
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision) const
    {
        tools::classhelper::ObjectPrinter printer("CWSignalInfos", float_precision);

        printer.register_value("center_frequency", center_frequency, "Hz");
        printer.register_value("bandwidth", bandwidth, "Hz");
        printer.register_value("effective_pulse_duration", effective_pulse_duration, "s");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(CWSignalInfos)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace signalprocessing
} // namespace algorithms
} // namespace themachinethatgoesping