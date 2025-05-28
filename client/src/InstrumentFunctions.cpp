#include "log-slayer/client/ProfileFunction.hpp"
#include "log-slayer/lib/CommunicationTable.hpp"
#include "log-slayer/lib/SharedMemory.hpp"
#include "log-slayer/lib/StaticOptions.hpp"

using namespace log_slayer;

static inline CommunicationTable& getCommunicationTable() {
    static CommunicationTable& _communicationTable =
        *SharedMemory::open(options::SHARED_MEMORY_NAME);
    return _communicationTable;
}

static std::atomic<std::uint64_t> _regionId = 0;

thread_local ThreadRegion& _threadRegion = getCommunicationTable().threadRegions.at(_regionId++);

thread_local Cursor _localHead = 0;
thread_local Cursor _localTail = 0;

extern "C" {

    using namespace log_slayer;

    __attribute__((no_instrument_function)) void __cyg_profile_func_enter(void* functionAddress, void*) {
        profileFunction<HookType::ENTER>(functionAddress, _threadRegion, _localHead, _localTail);
    }

    __attribute__((no_instrument_function)) void __cyg_profile_func_exit(void* functionAddress, void*) {
        profileFunction<HookType::EXIT>(functionAddress, _threadRegion, _localHead, _localTail);
    }

}


