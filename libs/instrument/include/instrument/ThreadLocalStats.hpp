#pragma once

#include "common/FunctionAddress.hpp"

namespace log_slayer {

    struct ThreadLocalStats {
        FunctionAddress prevFunctionAddress = nullptr;
    };

}

