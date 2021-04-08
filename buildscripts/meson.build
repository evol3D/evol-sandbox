if build_machine.system() == 'windows'
    # evol dll needs to be copied for each build
    evoldllcopy = custom_target(
        'evolDLLCopy',
        depends: evol_subproj.get_variable('evol_lib'),
        output: 'evolDLL',
        command: ['powershell.exe', meson.current_source_dir() + '/evoldllcopy.ps1', meson.project_build_root()],
        build_by_default: true,
    )

    # flecs dll needs to be copied to root builddir
    flecsdllcopy = custom_target(
        'flecsDLLCopy',
        output: 'flecsDLL',
        command: ['powershell.exe', meson.current_source_dir() + '/flecsdllcopy.ps1', meson.project_build_root()],
        build_by_default: true,
    )

    # lua dll only needs to be copied in the environment setup
    run_command('python', 
        meson.project_source_root() / 'subprojects/evol/buildscripts/copy.py',
        meson.project_source_root() / 'subprojects/luajit/src/lua51.dll', # src
        meson.project_build_root())                                       # dst

endif

