#pragma once

#include "Context.hpp"
#include "Statistics.hpp"
#include "log-slayer/lib/StaticOptions.hpp"
#include "log-slayer/lib/TraceEvent.hpp"

#include <thread>
#include <vector>

namespace log_slayer {

    class EventHandler {
    public:
        EventHandler(Context& context)
            : _context(context), _stopFlag(false), _recordingFlag(false) {
            _thread = std::thread(&EventHandler::run, this);
    }

    public:
        ~EventHandler() {
            if (_thread.joinable()) {
                _stopFlag = true;
                _thread.join();
            }
        }

    public:

        bool startRecord() {
            if (!_recordingFlag) {
                _recordingFlag = true;
                return true;
            }
            return false;
        }

        bool saveRecord() {
            if (_recordingFlag) {
                _recordingFlag = false;
                return true;
            }
            return false;
        }

    private:
        void run() {
            constexpr auto maxSize = options::TRACE_EVENTS_PER_THREAD;
            std::vector<TraceEvent> eventBuffer;
            eventBuffer.reserve(maxSize);
            while (!_stopFlag) {
                for (auto& threadRegion : _context.communicationTable.threadRegions) {
                    auto tail = threadRegion.tail.load(std::memory_order_relaxed);
                    auto head = threadRegion.head.load(std::memory_order_acquire);
                    if (tail == head)
                        continue;
                    auto& traceEvents = threadRegion.traceEvents;
                    if (__builtin_expect(tail < head, 1)) {
                        auto count = head - tail;
                        eventBuffer.resize(count);
                        std::copy(
                            std::next(traceEvents.begin(), tail),
                            std::next(traceEvents.begin(), head),
                            eventBuffer.begin()
                        );
                    } else {
                        auto count = maxSize - tail + head;
                        eventBuffer.resize(count);
                        std::copy(
                            std::next(traceEvents.begin(), tail),
                            traceEvents.end(),
                            eventBuffer.begin()
                        );
                        std::copy(
                            traceEvents.begin(),
                            std::next(traceEvents.begin(), head),
                            std::next(eventBuffer.begin(), eventBuffer.size() - head)
                        );
                    }
                    threadRegion.tail.store(head, std::memory_order_release);
                    _statistics.traceEventCount += eventBuffer.size();
                    eventBuffer.clear();
                    _context.statistics.store(_statistics, std::memory_order_release);
                }
            }
        }

    private:
        Context& _context;
        std::thread _thread;
        std::atomic<bool> _stopFlag;
        std::atomic<bool> _recordingFlag;
        Statistics _statistics;

    };

}

