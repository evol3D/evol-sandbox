project('evol-sandbox', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=gnu11'])

add_global_arguments('-fms-extensions', '-DEV_APP_CONFIG="' + meson.current_source_dir() + '/appconfig.meta"', language: 'c')

evol_subproj = subproject('evol')
evol_dep = dependency('evol')

subproject('evmod_glfw')
evmodglfw_dep = dependency('evmod_glfw')

subproject('evmod_ecs')
evmodecs_dep = dependency('evmod_ecs')

subproject('evmod_physics')
evmodphysics_dep = dependency('evmod_physics')

sandbox_incdir = []

configure_file(input: 'config.lua', output: 'config.lua', copy: true)

if build_machine.system() == 'windows'
    # evol dll needs to be copied for each build
    evoldllcopy = custom_target(
        'evolDLLCopy',
        depends: evol_subproj.get_variable('evol_lib'),
        output: 'evolDLL',
        command: ['powershell.exe', meson.current_source_dir() + '/buildscripts/evoldllcopy.ps1', meson.project_build_root()],
        build_by_default: true,
    )

    # flecs dll needs to be copied to root builddir
    flecsdllcopy = custom_target(
        'flecsDLLCopy',
        output: 'flecsDLL',
        command: ['powershell.exe', meson.current_source_dir() + '/buildscripts/flecsdllcopy.ps1', meson.project_build_root()],
        build_by_default: true,
    )

    # lua dll only needs to be copied in the environment setup
    run_command('powershell.exe', meson.current_source_dir() + '/buildscripts/luadllcopy.ps1', meson.project_source_root(), meson.project_build_root())
endif

sandbox_exe = executable(
  'sandbox',
  'src/main.c',
  dependencies: [
    evol_dep,
    evmodglfw_dep,
    evmodecs_dep,
    evmodphysics_dep,
  ],
  install : true,
  include_directories: sandbox_incdir,
)


