project('evol-sandbox', 'c',
  version : '0.1',
  default_options : [
    'warning_level=3', 
    'c_std=gnu11',
  ],
)

evol_subproj = subproject('evol')
evol_dep = dependency('evol')

sandbox_args = []

cc = meson.get_compiler('c')
compiler_name = cc.get_id()
if compiler_name == 'gcc'
  sandbox_args += '-fms-extensions'
endif


# The subproject's sole purpose is to make sure that the modules build
subproject('evmod_glfw')
subproject('evmod_ecs')
subproject('evmod_physics')
subproject('evmod_script')
subproject('evmod_assets')
subproject('evmod_game')

evmodglfw_dep = dependency('evmod_glfw')
evmodecs_dep = dependency('evmod_ecs')
evmodphysics_dep = dependency('evmod_physics')
evmodscript_dep = dependency('evmod_script')
evmodgame_dep = dependency('evmod_game')
evmod_assets_dep = dependency('evmod_assets')

# Setup build directory
subdir('buildscripts')

sandbox_exe = executable(
  'sandbox',
  'src/main.c',
  dependencies: [
    evol_dep,

    evmodglfw_dep,
    evmodecs_dep,
    evmodphysics_dep,
    evmodscript_dep,
    evmodgame_dep,
    evmod_assets_dep,
  ],
  install : true,
  c_args: sandbox_args,
)

# Copy config file
configure_file(input: 'config.lua', output: 'config.lua', copy: true)
