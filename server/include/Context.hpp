#pragma once

#include "log-slayer/lib/CommunicationTable.hpp"
#include "Statistics.hpp"

namespace log_slayer {

    struct Context {
        CommunicationTable& communicationTable;
        std::atomic<Statistics> statistics;
    };

}

