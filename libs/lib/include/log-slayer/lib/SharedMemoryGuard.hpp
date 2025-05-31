#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/mman.h>

namespace log_slayer {

    class SharedMemoryGuard {
    public:
        SharedMemoryGuard(std::string_view sharedMemoryName)
            : _sharedMemoryName(sharedMemoryName) {
            clear(_sharedMemoryName);
        }

        ~SharedMemoryGuard() {
            clear(_sharedMemoryName);
        }

    public:
        static void clear(std::string const& sharedMemoryName) {
            int unlinked = ::shm_unlink(sharedMemoryName.c_str());
            auto unlinkedErrno = errno;
            if (unlinked >= 0) {
                std::cout << "shared memory cleaned" << std::endl;
            } else if (unlinkedErrno == ENOENT) {
                std::cout << "shared memory non-exists" << std::endl;
            } else {
                std::cerr << "shm_unlink: " << std::strerror(errno) << std::endl;
                std::quick_exit(errno);
            }
        }

    private:
        std::string const _sharedMemoryName;

    };

}

