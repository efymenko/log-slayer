#pragma once

#include "log-slayer/lib/TraceEvent.hpp"

#include <vector>
#include <atomic>

namespace log_slayer {

    class Recorder {
    public:
        Recorder() : _recordingFlag(false) {}

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

        void write(TraceEvent* begin, TraceEvent* end, std::size_t regionId) {

        }

    private:
        std::atomic<bool> _recordingFlag;

    };

}

