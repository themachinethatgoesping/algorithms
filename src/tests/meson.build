# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

#---------- general test case header --------------

testcomponent = 'algorithms'

sources = [
  'tutorial.test.cpp',
  'gridding/forwardgridder.test.cpp',
  'gridding/forwardgridder1d.test.cpp',
  'gridding/forwardgridder2d.test.cpp',
  'gridding/forwardgridder3d.test.cpp',
  'gridding/functions/gridfunctions.test.cpp',
  'pointprocessing/bubblestreams/zspine.test.cpp',
  'pointprocessing/functions/segment_in_weighted_quantiles.cpp',
  'pointprocessing/functions/weighted_median.cpp',
  'signalprocessing/datastructures/cwsignalparameters.test.cpp',
  'signalprocessing/datastructures/fmsignalparameters.test.cpp',
  'signalprocessing/datastructures/genericsignalparameters.test.cpp',
  'imageprocessing/find_local_maxima.test.cpp',
  'imageprocessing/find_local_maxima2.test.cpp',
  'imageprocessing/grow_regions.test.cpp',
  'algorithms/functions/absorption.test.cpp',
  'algorithms/functions/rangecorrection.test.cpp',
  'algorithms/functions/wcicorrection.test.cpp',
  'geoprocessing/datastructures/beamsampleparameters.test.cpp',
  'geoprocessing/datastructures/raytraceresult.test.cpp',
  'geoprocessing/datastructures/raytraceresults.test.cpp',
  'geoprocessing/datastructures/sampledirections.test.cpp',
  'geoprocessing/datastructures/sampledirectionsrange.test.cpp',
  'geoprocessing/datastructures/sampledirectionstime.test.cpp',
  'geoprocessing/datastructures/sampleindices.test.cpp',
  'geoprocessing/datastructures/xyz.test.cpp',
  'geoprocessing/raytracers/i_raytracer.test.cpp',
  'geoprocessing/raytracers/rtconstantsvp.test.cpp',
  'geoprocessing/backtracers/backtracedwci.test.cpp',
  'geoprocessing/backtracers/btconstantsvp.test.cpp',
  'geoprocessing/backtracers/i_backtracer.test.cpp',
  'geoprocessing/sparsemaps/sparsemap.test.cpp',
  'geoprocessing/functions/to_raypoints.test.cpp',
]

foreach source : sources
    testname = projectnamespace + '._' + testcomponent + '.' + source.split('.')[0] + '_.test'
    testname = testname.replace('/', '.')

    testexe = executable(testname,
            sources: [source],
            dependencies : [catch2, algorithms_dep],
            link_language : 'cpp',
            override_options: [
            ],
            #install : true
            )

    test(testname,testexe)
endforeach








