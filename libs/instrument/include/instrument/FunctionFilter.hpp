#pragma once

#include "common/FunctionAddress.hpp"
#include "common/HookType.hpp"
#include "ThreadLocalStats.hpp"

namespace log_slayer {

    template<HookType hookType, bool skipOnRepeat>
    struct FunctionFilter {
        constexpr static bool filter(
            FunctionAddress functionAddress,
            ThreadLocalStats& threadLocalStats
        ) {
            if constexpr (skipOnRepeat) {
                if (threadLocalStats.prevFunctionAddress == functionAddress) {
                    return true;
                }
                threadLocalStats.prevFunctionAddress = functionAddress;
            }
            return false;
        }
    };

}

