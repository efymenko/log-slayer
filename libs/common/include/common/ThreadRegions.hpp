#pragma once

#include "ThreadRegion.hpp"

#include <array>
#include <type_traits>

namespace log_slayer {

    using ThreadRegions = std::conditional_t<
        options::SHARED_MEMORY_DYNAMIC,
        ThreadRegion*,
        std::array<ThreadRegion, options::SHARED_MEMORY_MAX_THREADS>
    >;

}

