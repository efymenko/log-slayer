configure_file(
  input: 'config.in',
  output: 'Config.hpp',
  configuration: configuration_data({
    'shm_max_threads': get_option('shm_max_threads'),
    'shm_dynamic': get_option('shm_dynamic').to_string(),
    'shm_name': get_option('shm_name'),
    'event_align': get_option('event_align'),
    'events_per_thread': get_option('events_per_thread'),
    'enable_event_timestamp': get_option('enable_event_timestamp').to_string(),
    'enable_event_type': get_option('enable_event_type').to_string(),
    'save_path': get_option('save_path'),
  })
)

