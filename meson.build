project('evol-sandbox', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=gnu11'])

evol_dep = dependency('evol')

subproject('evmod_glfw')

configure_file(input: 'config.lua', output: 'config.lua', copy: true)

exe = executable(
  'sandbox',
  'src/main.c',
  dependencies: [
    evol_dep,
  ],
  install : true,
)

test('basic', exe)
