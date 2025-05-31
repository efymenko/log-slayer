#pragma once

#include "config/Config.hpp"
#include "FunctionAddress.hpp"
#include "HookType.hpp"
#include "Timestamp.hpp"

#include <type_traits>

namespace log_slayer {

    struct TraceEventBase {
        TraceEventBase(FunctionAddress functionAddress, HookType)
            : functionAddress(functionAddress) {}
        TraceEventBase() = default;
        FunctionAddress functionAddress;
    };

    struct alignas(options::EVENT_ALIGN) TraceEventFull : TraceEventBase {
        TraceEventFull(FunctionAddress functionAddress, HookType hookType)
            : TraceEventBase(functionAddress, hookType)
                , hookType(hookType)
                , timestamp(getTimestamp()) {}
        TraceEventFull() = default;
        HookType hookType;
        Timestamp timestamp;
    };

    struct alignas(options::EVENT_ALIGN) TraceEventHookOnly : TraceEventBase {
        TraceEventHookOnly(FunctionAddress functionAddress, HookType hookType)
            : TraceEventBase(functionAddress, hookType)
                , hookType(hookType) {}
        TraceEventHookOnly() = default;
        HookType hookType;
    };

    struct alignas(options::EVENT_ALIGN) TraceEventTimestampOnly : TraceEventBase {
        TraceEventTimestampOnly(FunctionAddress functionAddress, HookType hookType)
            : TraceEventBase(functionAddress, hookType)
                , timestamp(getTimestamp()) {}
        TraceEventTimestampOnly() = default;
        Timestamp timestamp;
    };

    template <bool EnableHookType, bool EnableTimestamp>
    using TraceEventSelector = typename std::conditional<
        EnableHookType && EnableTimestamp,
        TraceEventFull,
        typename std::conditional<
            EnableHookType,
            TraceEventHookOnly,
            typename std::conditional<
                EnableTimestamp,
                TraceEventTimestampOnly,
                TraceEventBase
            >::type
        >::type
    >::type;

    using TraceEvent = TraceEventSelector<
        options::ENABLE_EVENT_TYPE,
        options::ENABLE_EVENT_TIMESTAMP
    >;

}

