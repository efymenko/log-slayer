#pragma once

#include <array>
#include <type_traits>

#include "TraceEvent.hpp"

namespace log_slayer {

    using TraceEvents = typename std::conditional_t<
        options::SHARED_MEMORY_DYNAMIC,
        TraceEvent*,
        std::array<TraceEvent, options::EVENTS_PER_THREAD>
    >;

}

