#pragma once

#include "TraceEvent.hpp"

#include <array>
#include <type_traits>

namespace log_slayer {

    using TraceEvents = typename std::conditional_t<
        options::SHARED_MEMORY_DYNAMIC,
        TraceEvent*,
        std::array<TraceEvent, options::EVENTS_PER_THREAD>
    >;

}

