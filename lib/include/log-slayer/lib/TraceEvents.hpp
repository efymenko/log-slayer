#pragma once

#include <array>
#include <type_traits>

#include "StaticOptions.hpp"
#include "TraceEvent.hpp"

namespace log_slayer {

    using TraceEvents = typename std::conditional_t<
        options::SHARED_MEMORY_DYNAMIC,
        TraceEvent*,
        std::array<TraceEvent, options::TRACE_EVENTS_PER_THREAD>
    >;

}

