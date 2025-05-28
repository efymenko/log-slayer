#pragma once

#include "StaticOptions.hpp"
#include "FunctionAddress.hpp"
#include "HookType.hpp"
#include "Timestamp.hpp"

namespace log_slayer {

    struct alignas(options::TRACE_EVENT_ALIGN) TraceEvent {
        HookType hookType;
        Timestamp timestamp;
        FunctionAddress functionAddress;
    };

}

