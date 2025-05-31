#pragma once

#include "CommunicationTable.hpp"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace log_slayer {

    class SharedMemory {
    public:
        static CommunicationTable* open(const char* sharedMemoryName) {
            int opened = ::shm_open(sharedMemoryName, O_RDWR, 0);
            if (opened < 0)
                handleError("shm_open");

            auto communicationTable = memoryMap(opened);

            ::close(opened);

            return communicationTable;
        }

        static CommunicationTable* create(const char* sharedMemoryName) {
            int opened = ::shm_open(sharedMemoryName, O_CREAT | O_EXCL | O_RDWR, 0600);
            if (opened < 0)
                handleError("shm_open");

            if (::ftruncate(opened, sizeof(CommunicationTable)) < 0)
                handleError("ftruncate");

            auto communicationTable = memoryMap(opened);

            ::close(opened);

            return communicationTable;

        }

    private:
        static void handleError(const char* functionName) {
            printf("%s: %s\n", functionName, std::strerror(errno));
            std::quick_exit(errno);
        }

        static CommunicationTable* memoryMap(int fileDescriptor) {
            constexpr auto size = sizeof(CommunicationTable);
            constexpr int prot = PROT_READ | PROT_WRITE;
            constexpr int flags = MAP_SHARED;
            auto mapped = ::mmap(nullptr, size, prot, flags, fileDescriptor, 0);
            if (mapped  == MAP_FAILED)
                handleError("mmap");
            if (reinterpret_cast<intptr_t>(mapped) % alignof(CommunicationTable) != 0) {
                printf("mmap returned pointer to unaligned memory\n");
                quick_exit(1);
            }

            return new (mapped) CommunicationTable;
        }

    };

}
