# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

#---------- general test case header --------------

testcomponent = 'algorithms'

sources = [
    'geoprocessing/datastructures/samplelocationslocal.test.cpp',
    'geoprocessing/datastructures/samplelocationlocal.test.cpp',
    'geoprocessing/datastructures/sampledirections.test.cpp',
    'geoprocessing/datastructures/sampleindices.test.cpp',
    'geoprocessing/raytracers/i_raytracer.test.cpp',
    'geoprocessing/raytracers/rtconstantsvp.test.cpp',
    'tutorial.test.cpp'
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








