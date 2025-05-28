#pragma once

#include <cstdint>

namespace log_slayer {

    enum class HookType : std::uint64_t {
        ENTER,
        EXIT
    };

}

