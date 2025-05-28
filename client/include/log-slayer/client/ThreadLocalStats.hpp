#pragma once

#include "log-slayer/lib/FunctionAddress.hpp"

namespace log_slayer {

    struct ThreadLocalStats {
        FunctionAddress prevFunctionAddress = nullptr;
    };

}

