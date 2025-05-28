#pragma once

#include "StaticOptions.hpp"
#include "log-slayer/lib/TraceEvents.hpp"

#include <atomic>
#include <type_traits>

namespace log_slayer {

    struct ThreadRegion {
        using Cursor = std::uint64_t;
        
        alignas(64) std::atomic<Cursor> head;
        alignas(64) std::atomic<Cursor> tail;
        TraceEvents traceEvents;

        static_assert(std::atomic<Cursor>::is_always_lock_free);
    };

    static_assert(std::is_trivially_copyable_v<ThreadRegion>);

}

