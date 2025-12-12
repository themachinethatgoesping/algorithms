# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

# -- library setup --
libname = projectnamespace + '_algorithms'
headerdir = projectnamespace + '/algorithms'

# -- source files --
sources = [
  'amplitudecorrection/functions/absorption.cpp',
  'pointprocessing/bubblestreams/zspine.cpp',
  'signalprocessing/types.cpp',
  'featuremapping/nearestfeaturemapper.cpp',
  'geoprocessing/datastructures/beamsampleparameters.cpp',
]

headers = [
  'helloping.hpp',
  'gridding/forwardgridder1d.hpp',
  'gridding/forwardgridder2d.hpp',
  'gridding/forwardgridder3d.hpp',
  'gridding/.docstrings/forwardgridder1d.doc.hpp',
  'gridding/.docstrings/forwardgridder2d.doc.hpp',
  'gridding/.docstrings/forwardgridder3d.doc.hpp',
  'gridding/functions/gridfunctions.hpp',
  'gridding/functions/resamplingfunctions.hpp',
  'gridding/functions/.docstrings/gridfunctions.doc.hpp',
  'gridding/functions/.docstrings/resamplingfunctions.doc.hpp',
  'amplitudecorrection/functions.hpp',
  'amplitudecorrection/.docstrings/functions.doc.hpp',
  'amplitudecorrection/functions/_shape_assertations.hpp',
  'amplitudecorrection/functions/absorption.hpp',
  'amplitudecorrection/functions/rangecorrection.hpp',
  'amplitudecorrection/functions/wcicorrection.hpp',
  'amplitudecorrection/functions/.docstrings/_shape_assertations.doc.hpp',
  'amplitudecorrection/functions/.docstrings/absorption.doc.hpp',
  'amplitudecorrection/functions/.docstrings/rangecorrection.doc.hpp',
  'amplitudecorrection/functions/.docstrings/wcicorrection.doc.hpp',
  'pointprocessing/functions.hpp',
  'pointprocessing/bubblestreams/zspine.hpp',
  'pointprocessing/bubblestreams/.docstrings/zspine.doc.hpp',
  'pointprocessing/.docstrings/functions.doc.hpp',
  'pointprocessing/functions/segment_in_weighted_quantiles.hpp',
  'pointprocessing/functions/weighted_median.hpp',
  'pointprocessing/functions/.docstrings/segment_in_weighted_quantiles.doc.hpp',
  'pointprocessing/functions/.docstrings/weighted_median.doc.hpp',
  'signalprocessing/datastructures.hpp',
  'signalprocessing/types.hpp',
  'signalprocessing/datastructures/cwsignalparameters.hpp',
  'signalprocessing/datastructures/fmsignalparameters.hpp',
  'signalprocessing/datastructures/genericsignalparameters.hpp',
  'signalprocessing/datastructures/.docstrings/cwsignalparameters.doc.hpp',
  'signalprocessing/datastructures/.docstrings/fmsignalparameters.doc.hpp',
  'signalprocessing/datastructures/.docstrings/genericsignalparameters.doc.hpp',
  'signalprocessing/.docstrings/datastructures.doc.hpp',
  'signalprocessing/.docstrings/types.doc.hpp',
  'featuremapping/nearestfeaturemapper.hpp',
  'featuremapping/.docstrings/nearestfeaturemapper.doc.hpp',
  '.docstrings/helloping.doc.hpp',
  'imageprocessing/functions.hpp',
  'imageprocessing/.docstrings/functions.doc.hpp',
  'imageprocessing/functions/backwardmapping.hpp',
  'imageprocessing/functions/find_local_maxima.hpp',
  'imageprocessing/functions/find_local_maxima2.hpp',
  'imageprocessing/functions/grow_regions.hpp',
  'imageprocessing/functions/.docstrings/backwardmapping.doc.hpp',
  'imageprocessing/functions/.docstrings/find_local_maxima.doc.hpp',
  'imageprocessing/functions/.docstrings/find_local_maxima2.doc.hpp',
  'imageprocessing/functions/.docstrings/grow_regions.doc.hpp',
  'geoprocessing/datastructures.hpp',
  'geoprocessing/raytracers.hpp',
  'geoprocessing/datastructures/beamsampleparameters.hpp',
  'geoprocessing/datastructures/raytraceresult.hpp',
  'geoprocessing/datastructures/raytraceresults.hpp',
  'geoprocessing/datastructures/sampledirections.hpp',
  'geoprocessing/datastructures/sampledirectionsrange.hpp',
  'geoprocessing/datastructures/sampledirectionstime.hpp',
  'geoprocessing/datastructures/sampleindices.hpp',
  'geoprocessing/datastructures/xyz.hpp',
  'geoprocessing/datastructures/.docstrings/beamsampleparameters.doc.hpp',
  'geoprocessing/datastructures/.docstrings/raytraceresult.doc.hpp',
  'geoprocessing/datastructures/.docstrings/raytraceresults.doc.hpp',
  'geoprocessing/datastructures/.docstrings/sampledirections.doc.hpp',
  'geoprocessing/datastructures/.docstrings/sampledirectionsrange.doc.hpp',
  'geoprocessing/datastructures/.docstrings/sampledirectionstime.doc.hpp',
  'geoprocessing/datastructures/.docstrings/sampleindices.doc.hpp',
  'geoprocessing/datastructures/.docstrings/xyz.doc.hpp',
  'geoprocessing/.docstrings/datastructures.doc.hpp',
  'geoprocessing/.docstrings/raytracers.doc.hpp',
  'geoprocessing/raytracers/i_raytracer.hpp',
  'geoprocessing/raytracers/rtconstantsvp.hpp',
  'geoprocessing/raytracers/.docstrings/i_raytracer.doc.hpp',
  'geoprocessing/raytracers/.docstrings/rtconstantsvp.doc.hpp',
  'geoprocessing/backtracers/backtracedwci.hpp',
  'geoprocessing/backtracers/btconstantsvp.hpp',
  'geoprocessing/backtracers/i_backtracer.hpp',
  'geoprocessing/backtracers/.docstrings/backtracedwci.doc.hpp',
  'geoprocessing/backtracers/.docstrings/btconstantsvp.doc.hpp',
  'geoprocessing/backtracers/.docstrings/i_backtracer.doc.hpp',
  'geoprocessing/sparsemaps/map_tools.hpp',
  'geoprocessing/sparsemaps/sparsemap.hpp',
  'geoprocessing/sparsemaps/xmap.hpp',
  'geoprocessing/sparsemaps/xymap.hpp',
  'geoprocessing/sparsemaps/xyzmap.hpp',
  'geoprocessing/sparsemaps/.docstrings/map_tools.doc.hpp',
  'geoprocessing/sparsemaps/.docstrings/sparsemap.doc.hpp',
  'geoprocessing/sparsemaps/.docstrings/xmap.doc.hpp',
  'geoprocessing/sparsemaps/.docstrings/xymap.doc.hpp',
  'geoprocessing/sparsemaps/.docstrings/xyzmap.doc.hpp',
  'geoprocessing/georeferencers/btconstantsvp.hpp',
  'geoprocessing/georeferencers/i_backtracer.hpp',
  'geoprocessing/georeferencers/.docstrings/btconstantsvp.doc.hpp',
  'geoprocessing/georeferencers/.docstrings/i_backtracer.doc.hpp',
  'geoprocessing/functions/to_raypoints.hpp',
  'geoprocessing/functions/.docstrings/to_raypoints.doc.hpp',
]

# -- targets --
# Now that we have .cpp sources, we need to build a library
algorithms_lib = library(
  libname,
  sources,
  dependencies: [boost_dep, navigation_dep, tools_dep, gswteos_10_dep],
  override_options: ['b_lundef=false'],
  cpp_pch : '../../pch/algorithms_themachinethatgoesping_algorithms.hpp',
  install: true,
  install_tag: 'library',
)

# -- internal dependency --
algorithms_dep = declare_dependency(
  dependencies: [boost_dep, navigation_dep, tools_dep, gswteos_10_dep],
  link_with: [algorithms_lib],
  include_directories: ['../../'],
)

# -- installation setup --

# install headers
if headers != ['']
  install_headers(headers, subdir: headerdir + '/', preserve_path: true)
endif

# # create pkgconfig file
# pkg = import('pkgconfig')
# pkg.generate(
#   algorithms_lib,
#   description: 'Library for processing themachinethatgoesping pings',
#   url: 'https://github.com/themachinethatgoesping/algorithms',
# )