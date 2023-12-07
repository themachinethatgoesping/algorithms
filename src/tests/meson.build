# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

#---------- general test case header --------------

testcomponent = 'algorithms'

sources = [
    'geoprocessing/datastructures/xyz.test.cpp',
    'geoprocessing/datastructures/raytraceresults.test.cpp',
    'geoprocessing/datastructures/raytraceresult.test.cpp',
    'geoprocessing/datastructures/sampledirections.test.cpp',
    'geoprocessing/datastructures/sampledirectionsrange.test.cpp',
    'geoprocessing/datastructures/sampledirectionstime.test.cpp',
    'geoprocessing/datastructures/sampleindices.test.cpp',
    'geoprocessing/datastructures/beamsampleparameters.test.cpp',
    'geoprocessing/raytracers/i_raytracer.test.cpp',
    'geoprocessing/raytracers/rtconstantsvp.test.cpp',
    'geoprocessing/backtracers/i_backtracer.test.cpp',
    'geoprocessing/backtracers/btconstantsvp.test.cpp',
    'signalprocessing/datastructures/genericsignalparameters.test.cpp',
    'signalprocessing/datastructures/cwsignalparameters.test.cpp',
    'signalprocessing/datastructures/fmsignalparameters.test.cpp',
    'tutorial.test.cpp'
]

foreach source : sources
    testname = projectnamespace + '._' + testcomponent + '.' + source.split('.')[0] + '_.test'
    testname = testname.replace('/', '.')

    testexe = executable(testname,
            sources: [source],
            dependencies : [catch2, algorithms_dep, omp_dep],
            link_language : 'cpp',
            override_options: [
            ],
            #install : true
            )

    test(testname,testexe)
endforeach








