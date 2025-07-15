// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#pragma once

/* generated doc strings */
#include ".docstrings/raytraceresult.doc.hpp"

#include <themachinethatgoesping/tools/classhelper/objectprinter.hpp>
#include <themachinethatgoesping/tools/classhelper/stream.hpp>
#include <themachinethatgoesping/tools/helper/approx.hpp>

#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace datastructures {

/**
 * @brief A structure to store a georeferenced sample location. It is used as
 * output for the raytracers functions. This object stores local x (forward
 * coordinate), y (starboard coordinate) depth and true range. These coordinates
 * can be converted to UTM or Lat/Lon if a reference position (for coordinate 0)
 * is known.
 */
struct RaytraceResult
{
    float x          = 0.0;  ///< in m, positive forward
    float y          = 0.0;  ///< in m, positive starboard
    float z          = 0.0f; ///< in m, positive downwards
    float true_range = 0.0f; ///< in m, accumulated ray path length

    /**
     * @brief Construct a new sample location object (all values set to 0)
     *
     */
    RaytraceResult() = default;

    /**
     * @brief Construct a new RaytraceResult object
     *
     * @param x in m, positive forward
     * @param y in m, positive starboard
     * @param z in m, positive downwards
     * @param true_range in m, accumulated ray path length
     */
    RaytraceResult(float x_, float y_, float z_, float true_range_)
        : x(std::move(x_))
        , y(std::move(y_))
        , z(std::move(z_))
        , true_range(std::move(true_range_))
    {
    }

    bool operator!=(const RaytraceResult& rhs) const { return !(operator==(rhs)); }
    bool operator==(const RaytraceResult& rhs) const
    {
        if (tools::helper::approx(x, rhs.x))
            if (tools::helper::approx(y, rhs.y))
                if (tools::helper::approx(z, rhs.z))
                    if (tools::helper::approx(true_range, rhs.true_range))
                        return true;

        return false;
    }

  public:
    // ----- file I/O -----
    static RaytraceResult from_stream(std::istream& is)
    {
        RaytraceResult data;

        is.read(reinterpret_cast<char*>(&data.x), sizeof(RaytraceResult));

        return data;
    }

    void to_stream(std::ostream& os) const
    {
        os.write(reinterpret_cast<const char*>(&x), sizeof(RaytraceResult));
    }

  public:
    tools::classhelper::ObjectPrinter __printer__(unsigned int float_precision, bool superscript_exponents) const
    {
        tools::classhelper::ObjectPrinter printer("RaytraceResult", float_precision, superscript_exponents);

        printer.register_value("x", x, "positive forward, m");
        printer.register_value("y", y, "positive starboard, m");
        printer.register_value("z", z, "positive downwards, m");
        printer.register_value("true_range", true_range, "ray path length, m");

        return printer;
    }

  public:
    // -- class helper function macros --
    // define to_binary and from_binary functions (needs the serialize function)
    __STREAM_DEFAULT_TOFROM_BINARY_FUNCTIONS__(RaytraceResult)
    // define info_string and print functions (needs the __printer__ function)
    __CLASSHELPER_DEFAULT_PRINTING_FUNCTIONS__
};

} // namespace datastructures
} // namespace geoprocessing
} // namespace algorithms
} // namespace themachinethatgoesping