project('evol-sandbox', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=gnu11'])

add_global_arguments('-fms-extensions', '-DEV_APP_CONFIG="' + meson.current_source_dir() + '/appconfig.meta"', language: 'c')

evol_dep = dependency('evol')

subproject('evmod_glfw')
evmodglfw_dep = dependency('evmod_glfw')

sandbox_incdir = []

configure_file(input: 'config.lua', output: 'config.lua', copy: true)

exe = executable(
  'sandbox',
  'src/main.c',
  dependencies: [
    evol_dep,
    evmodglfw_dep,
  ],
  install : true,
  include_directories: sandbox_incdir,
)

test('basic', exe)
