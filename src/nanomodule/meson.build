# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

# -- module name
module_name = 'algorithms_nanopy'
        
# -- sources
sources = [
  'module.cpp',
  'featuremapping/c_nearestfeaturemapper.cpp',
  'featuremapping/module.cpp',
  'imageprocessing/functions/functions.cpp',
]

# define python module
py_algorithms = pymod.extension_module(
    module_name,
    sources: sources,
    dependencies : [
        tools_nanobind_dep,
        algorithms_dep,
        gswteos_10_dep,
        nanobind_dep,
        python3_dep],
    link_language : 'cpp',
    # cpp_pch : 'pch/pch.hpp',
    override_options: [
        #'cpp_rtti=true',
    ],
    cpp_args : ['-DMODULE_NAME='+module_name, '-DMODULE_VERSION=' + '"' + meson.project_version() + '"'],
    install : true,
    subdir : projectnamespace,
    install_tag : 'python-runtime',
    override_options: ['unity=off']
)



        
