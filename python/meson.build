# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: CC0-1.0

# -- python sources --
sources = [
  'themachinethatgoesping/algorithms/__init__.py',
  'themachinethatgoesping/algorithms/backscattermodels/__init__.py',
  'themachinethatgoesping/algorithms/backscattermodels/gasbubbles/__init__.py',
  'themachinethatgoesping/algorithms/backscattermodels/gasbubbles/li.py',
  'themachinethatgoesping/algorithms/backscattermodels/gasbubbles/parameters.py',
  'themachinethatgoesping/algorithms/backscattermodels/gasbubbles/thuraisingham.py',
]

init_configuration = configuration_data({
    'PROJECT_VERSION' : meson.project_version(),
})

if get_option('build_pythonmodule').enabled()
    
    if get_option('pydev_install').disabled() #don't install if pydev_install is activated
        foreach source : sources
            #create install path for file
            fields = []
            foreach field : source.split('/')
                if not field.endswith('.py')
                    fields += [field]
                endif
            endforeach
            pth = '/'.join(fields)

            # TODO: Not yet possible because of https://github.com/mesonbuild/meson/issues/2320
            # __init__.py may need configuration
            # if source.split('/')[-1] == '__init__.py'
            #     configure_file(
            #         input: source,
            #         output: pth / '__init__.py',
            #         configuration: init_configuration,
            #         install: true,
            #         install_dir: pymod.get_install_dir() / pth,
            #         install_tag: 'python-runtime'
            #     )
            # else
            #     pymod.install_sources(source, subdir: pth, install_tag: 'python-runtime')
            # endif

            pymod.install_sources(source, subdir: pth, install_tag: 'python-runtime')
                
        endforeach

    endif
endif
