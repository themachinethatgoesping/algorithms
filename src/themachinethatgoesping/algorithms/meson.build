# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

# -- library setup --
libname   = projectnamespace + '_algorithms'
headerdir = projectnamespace + '/algorithms'

# -- source files --
sources = [
]

headers = [
  'geoprocessing/datastructures/samplelocationlocal.hpp',
  'geoprocessing/datastructures/samplelocationslocal.hpp',
  'helloping.hpp',
]

# -- targets --
# for now force static linking on windows
if build_machine.system() != 'linux'
  algorithms_lib = static_library(libname,sources,
                        dependencies : [
                          boost_dep, 
                          omp_dep,
                          navigation_dep,
                          tools_dep
                          ],
                        override_options: [
                          'b_lundef=false'
                      ],
                        install : true, 
                        install_tag : 'library'
                        )
else  
  algorithms_lib = library(libname,sources,
                        dependencies : [
                          boost_dep, 
                          omp_dep,
                          navigation_dep,
                          tools_dep
                          ],
                        override_options: [
                          'b_lundef=false'
                      ],
                        install : true, 
                        install_tag : 'library'
                        )
endif
                      

# -- internal dependency --
algorithms_dep = declare_dependency(
  dependencies : [
    boost_dep,
    omp_dep,
    navigation_dep,
    tools_dep
  ],
  link_with : [algorithms_lib],
  include_directories : ['../../']
  )


# -- installation setup --

# install headers
if headers != ['']
    install_headers(headers, subdir: headerdir+'/', preserve_path: true)
endif

# create pkgconfig file
pkg = import('pkgconfig')
pkg.generate(algorithms_lib,
             description : 'Library for processing themachinethatgoesping pings',
             url : 'https://github.com/themachinethatgoesping/algorithms')
