#pragma once

namespace log_slayer {

    namespace filter {

#ifdef LOG_SLAYER_FILTER_FUNCTION_REPEATS
        constexpr bool shouldSkipOnRepeat = true;
#else
        constexpr bool shouldSkipOnRepeat = false;
#endif

    }
}

