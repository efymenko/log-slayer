#pragma once

#include <cstdint>

namespace log_slayer {

    using Timestamp = std::uint64_t;

    static inline std::uint64_t getTimestamp() {
        std::uint64_t hi, lo;
        __asm volatile ("rdtsc" : "=a" (lo), "=d"(hi));
        return lo | (hi << 32);
    }

}

