#pragma once

#include <atomic>

namespace log_slayer {

    struct ControlBlock {
        std::atomic_flag tailIsLocked = ATOMIC_FLAG_INIT;
    };

}

