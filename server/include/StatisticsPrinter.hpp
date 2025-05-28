#pragma once

#include "Context.hpp"

#include <thread>
#include <atomic>
#include <iostream>

namespace log_slayer {

    class StatitsticsPrinter {
    public:
        StatitsticsPrinter(Context& context) : _context(context), _stopFlag(false) {
            _thread = std::thread(&StatitsticsPrinter::run, this);
        }

    public:
        ~StatitsticsPrinter() {
            if (_thread.joinable()) {
                _stopFlag = true;
                _thread.join();
            }
        }

    private:
        void run() {
            std::uint64_t prevTraceEventCount = 0;
            while (!_stopFlag) {
                auto [currTraceEventCount]
                    = _context.statistics.load(std::memory_order_acquire);
                auto delta = currTraceEventCount - prevTraceEventCount;
                if (delta) {
                    std::cout << delta << " events/s" << std::endl;
                    prevTraceEventCount = currTraceEventCount;

                }
                std::this_thread::sleep_for(std::chrono::seconds{ 1 });
            }
        }

    private:
        Context& _context;
        std::thread _thread;
        std::atomic<bool> _stopFlag;
    };

}

