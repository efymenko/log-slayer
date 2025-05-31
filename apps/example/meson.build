executable(
  'example',
  'main.cpp',
  cpp_args: ['-finstrument-functions'],
  link_args: ['-rdynamic', '-lrt'],
  link_with: client_lib,
  dependencies: [threads_dep]
)

