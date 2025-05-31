#pragma once

#include "Context.hpp"
#include "Statistics.hpp"
#include "Recorder.hpp"

#include "log-slayer/config/Config.hpp"

#include <thread>

namespace log_slayer {

    class EventHandler {
    public:
        EventHandler(Context& context, Recorder& recorder)
            : _context(context)
            , _recorder(recorder)
            , _stopFlag(false) {
            _thread = std::thread(&EventHandler::run, this);
    }

    public:
        ~EventHandler() {
            if (_thread.joinable()) {
                _stopFlag = true;
                _thread.join();
            }
        }

    private:
        void run() {
            constexpr auto maxSize = options::EVENTS_PER_THREAD;
            while (!_stopFlag) {
                for (std::size_t i = 0; i < _context.communicationTable.threadRegions.size(); ++i) {
                    auto& threadRegion = _context.communicationTable.threadRegions[i];
                    auto tail = threadRegion.tail.load(std::memory_order_relaxed);
                    auto head = threadRegion.head.load(std::memory_order_acquire);
                    if (tail == head)
                        continue;
                    auto& traceEvents = threadRegion.traceEvents;
                    std::uint64_t count = 0;
                    if (__builtin_expect(tail < head, 1)) {
                        count = head - tail;
                        _recorder.write(&traceEvents[tail], &traceEvents[head], i);
                    } else {
                        count = maxSize - tail + head;
                        _recorder.write(&traceEvents[tail], &traceEvents[0] + maxSize, i);
                        _recorder.write(&traceEvents[0], &traceEvents[head], i);
                    }
                    threadRegion.tail.store(head, std::memory_order_release);
                    _context.statistics.store(_statistics, std::memory_order_release);
                    _statistics.traceEventCount += count;
                }
            }
        }

    private:
        Context& _context;
        Recorder& _recorder;
        std::thread _thread;
        std::atomic<bool> _stopFlag;
        Statistics _statistics;

    };

}

