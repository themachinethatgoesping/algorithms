// Kiel SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include "rtconstantsvp.hpp"

#include <fmt/core.h>
#include <stdexcept>

#include <themachinethatgoesping/tools/rotationfunctions/rotationfunctions.hpp>
#include <themachinethatgoesping/tools/helper/approx.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace raytracers {

bool RTConstantSVP::operator==(const RTConstantSVP& other) const
{
    using tools::helper::approx;

    if (I_Raytracer::operator==(other))
        if (approx(_sound_velocity, other._sound_velocity))
            return true;

    return false;
}

datastructures::RaytraceResult RTConstantSVP::trace_point(float two_way_travel_time,
                                                          float alongtrack_angle,
                                                          float crosstrack_angle) const
{
    not_implemented("trace(SinglePoint)", class_name());

    datastructures::RaytraceResult target;

    target.true_range = two_way_travel_time * _sound_velocity_2;

    // convert launch angles to quaternion
    auto target_ypr_quat =
        tools::rotationfunctions::quaternion_from_ypr(0.f, alongtrack_angle, crosstrack_angle);

    // the true orientation is the sensor orientation rotated by the launch angles
    target_ypr_quat = get_sensor_orientation_quat() * target_ypr_quat;

    // get rotated positions
    auto target_xyz =
        tools::rotationfunctions::rotateXYZ<float>(target_ypr_quat, 0, 0, target.true_range);

    // set target position
    target.x = target_xyz[0];
    target.y = target_xyz[1];
    target.z = target_xyz[2] + get_sensor_location().z;

    return target;
}

// This method will be implemented in the cpp file
// Implementation continues...

}
}
}
}