project('evol-sandbox', 'c',
  version : '0.1',
  default_options : [
    'warning_level=3', 
    'c_std=gnu11',
  ],
)

evol_subproj = subproject('evol')
evol_dep = dependency('evol')

# The subproject's sole purpose is to make sure that the modules build
subproject('evmod_glfw')
subproject('evmod_ecs')
subproject('evmod_physics')

evmodglfw_dep = dependency('evmod_glfw')
evmodecs_dep = dependency('evmod_ecs')
evmodphysics_dep = dependency('evmod_physics')

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
  ],
  install : true,
)

# Copy config file
configure_file(input: 'config.lua', output: 'config.lua', copy: true)
