#pragma once

#include <chrono>

namespace log_slayer {

    using Timestamp = std::chrono::steady_clock::time_point;

    static inline Timestamp getTimestamp() {
        return std::chrono::steady_clock::now();
    }

}

