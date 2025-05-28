configure_file(
  input: 'config.in',
  output: 'Config.hpp',
  configuration: configuration_data({
    'LOG_SLAYER_SHARED_MEMORY_MAX_THREADS': get_option('shm_max_threads'),
    'LOG_SLAYER_SHARED_MEMORY_DYNAMIC': get_option('shm_dynamic'),
    'LOG_SLAYER_SHARED_MEMORY_NAME': get_option('shm_name'),
    'LOG_SLAYER_TRACE_EVENT_ALIGN': get_option('trace_event_align'),
    'LOG_SLAYER_TRACE_EVENTS_PER_THREAD': get_option('trace_events_per_thread'),
  })
)

