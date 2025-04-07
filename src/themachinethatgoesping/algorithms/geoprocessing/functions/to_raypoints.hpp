#pragma once

/* generated doc strings */
#include ".docstrings/to_raypoints.doc.hpp"

#include <themachinethatgoesping/tools/helper.hpp>
#include <themachinethatgoesping/tools/helper/xtensor.hpp>
#include <themachinethatgoesping/tools/rotationfunctions/quaternions.hpp>
#include <xtensor/xfixed.hpp>

namespace themachinethatgoesping {
namespace algorithms {
namespace geoprocessing {
namespace functions {

template<tools::helper::c_xtensor    t_xtensor_out,
         tools::helper::c_xtensor_1d t_xtensor_1d,
         std::floating_point         t_float>
inline t_xtensor_out to_raypoints(
    /**
     * @brief Interpolates ray points between a base location and end locations.
     *
     * @param base_location Numeric value for the base location.
     * @param end_locations 1D array of end location values.
     * @param base_scale_value Numeric scale for the base location.
     * @param end_scale_values 1D array of scale values matching end_locations.
     * @param ray_scale_values 1D array of scale values for ray interpolation.
     * @param mp_cores Number of CPU cores to use in parallel processing.
     * @return Output array with interpolated ray points.
     */
    const t_float       base_location,
    const t_xtensor_1d& end_locations,
    const t_float       base_scale_value,
    const t_xtensor_1d& end_scale_values,
    const t_xtensor_1d& ray_scale_values,
    const int           mp_cores = 1)
{

    if (end_scale_values.size() != end_locations.size())
        throw std::runtime_error(
            fmt::format("to_raypoints: end_scale_values ({}) and end_locations ({}) must have size", end_scale_values.size(),end_locations.size()));

    std::array<size_t, 2> shape = { end_locations.size(), ray_scale_values.size() };

    auto output_locations = t_xtensor_out::from_shape(shape);

    t_xtensor_1d slopes = (end_locations - base_location) / (end_scale_values - base_scale_value);

#pragma omp parallel for num_threads(mp_cores) collapse(2)
    for (size_t r = 0; r < shape[0]; ++r)
    {
        for (size_t p = 0; p < shape[1]; ++p)
        {
            auto x                           = ray_scale_values.unchecked(p);
            output_locations.unchecked(r, p) = base_location + (x)*slopes.unchecked(r);
        }
    }

    return output_locations;
}

}
}
}
}