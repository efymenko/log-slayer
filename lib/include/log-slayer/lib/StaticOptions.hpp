#pragma once

#include "log-slayer/config/Config.hpp"

#include <cstdint>

namespace log_slayer {

    namespace options {

        constexpr auto SHARED_MEMORY_NAME = LOG_SLAYER_SHARED_MEMORY_NAME;

        constexpr bool SHARED_MEMORY_DYNAMIC = LOG_SLAYER_SHARED_MEMORY_DYNAMIC;

        constexpr std::size_t SHARED_MEMORY_MAX_THREADS = LOG_SLAYER_SHARED_MEMORY_MAX_THREADS;

        constexpr std::size_t TRACE_EVENT_ALIGN = LOG_SLAYER_TRACE_EVENT_ALIGN;

        constexpr std::size_t TRACE_EVENTS_PER_THREAD = LOG_SLAYER_TRACE_EVENTS_PER_THREAD;
    }

}

