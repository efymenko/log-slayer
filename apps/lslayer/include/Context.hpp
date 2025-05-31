#pragma once

#include "common/CommunicationTable.hpp"
#include "Statistics.hpp"

namespace log_slayer {

    struct Context {
        CommunicationTable& communicationTable;
        std::atomic<Statistics> statistics;
    };

}

