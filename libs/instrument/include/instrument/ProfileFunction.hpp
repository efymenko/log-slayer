#pragma once

#include "common/ThreadRegion.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>

namespace log_slayer {

    using Cursor = ThreadRegion::Cursor;

    template<std::size_t bufferSize>
    static inline auto distance(Cursor head, Cursor tail) {
        if (__builtin_expect(head >= tail, 1)) {
            return head - tail;
        } else {
            return (bufferSize - tail) + head;
        }
    }

    template<HookType hookType>
    void profileFunction(
        FunctionAddress functionAddress,
        ThreadRegion& threadRegion,
        Cursor& localHead,
        Cursor& localTail
    ) {
        // thread_local ThreadLocalStats threadLocalStats;
        // using Filter = FunctionFilter<hookType, filter::shouldSkipOnRepeat>;
        // if (Filter::filter(functionAddress, threadLocalStats))
        //     return;
        // thread_local std::size_t skip = 0;
        // if (skip) {
        //     --skip;
        //     return;
        // }
        constexpr auto size =
            sizeof(threadRegion.traceEvents) / sizeof(threadRegion.traceEvents[0]);
        constexpr auto delta = size / 1000;

        Cursor nextIndex = (localHead + 1) % size;
        if (__builtin_expect(nextIndex == localTail, 0)) {
            localTail = threadRegion.tail.load(std::memory_order_acquire);
            if (__builtin_expect(nextIndex == localTail, 0)) {
                // skip = delta;
                return;
            }
        }
        auto dist = distance<size>(localHead, localTail);
        if (__builtin_expect(dist >= delta && dist % delta == 0 , 0)) {
            threadRegion.head.store(localHead, std::memory_order_release);
        }
        threadRegion.traceEvents[localHead] = TraceEvent(functionAddress, hookType);
        localHead = nextIndex;
    }

}

