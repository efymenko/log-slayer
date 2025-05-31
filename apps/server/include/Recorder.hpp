#pragma once

#include "log-slayer/config/Config.hpp"
#include "log-slayer/lib/TraceEvent.hpp"

#include <filesystem>
#include <fstream>
#include <array>
#include <atomic>

namespace log_slayer {

    class Recorder {
    public:
        Recorder() : _recordingFlag(false), _writtingFlag(false) {}

    public:
        bool startRecord(std::string& message) {
            if (!_recordingFlag) {
                if (!prepareDir(message)) {
                    return false;
                }
                _recordingFlag = true;
                message = "Recording started";
                return true;
            }
            message = "Recording is alredy in progress";
            return false;
        }

        bool saveRecord(std::string& message) {
            if (_recordingFlag) {
                _recordingFlag = false;
                closeFiles();
                message = "Recording saved to " + _currentRootPath.string();
                return true;
            }
            message = "Recording has not started";
            return false;
        }

        void write(TraceEvent* begin, TraceEvent* end, std::size_t regionId) {
            if (begin == end)
                return;
            while (_writtingFlag.test_and_set(std::memory_order_acquire)) {}
            if (_recordingFlag.load(std::memory_order_acquire)) {
                auto& file = files[regionId];
                if (!file.is_open()) {
                    file.open(getFilePath(regionId), std::ios::out | std::ios::binary);
                }
                auto count = std::distance(begin, end);
                file.write(reinterpret_cast<char const*>(begin), count * sizeof(TraceEvent));
            }
            _writtingFlag.clear(std::memory_order_release);
        }

    private:
        bool prepareDir(std::string& error) {
            std::filesystem::path rootDir(options::SAVE_PATH);
            auto pathToDir = rootDir / generateDirName();
            bool created = false;
            while (!created) {
                std::error_code ec;
                created = std::filesystem::create_directories(pathToDir, ec);
                if (ec) {
                    error = ec.message();
                    return false;
                }
            }
            _currentRootPath = pathToDir;
            return true;
        }

        std::filesystem::path getFilePath(std::size_t regionId) {
            return _currentRootPath / ("thread_" + std::to_string(regionId) + ".bin");
        }

        void closeFiles() {
            while (_writtingFlag.test_and_set(std::memory_order_acquire)) {}
            for (auto& file : files) {
                if (file.is_open()) {
                    file.close();
                }
                file.clear();
            }
            _writtingFlag.clear(std::memory_order_release);
        }

        std::string generateDirName() {
            auto now = std::chrono::system_clock::now();
            auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
            auto fraction = now - seconds;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm tm;

            localtime_r(&t, &tm);

            char date_time[24];
            strftime(date_time, sizeof(date_time), "%Y.%m.%d-%H:%M:%S", &tm);

            std::ostringstream oss;
            oss << date_time << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();
            return oss.str();
        }

    private:
        std::atomic<bool> _recordingFlag;
        std::atomic_flag _writtingFlag;
        std::array<std::ofstream, options::SHARED_MEMORY_MAX_THREADS> files;
        std::filesystem::path _currentRootPath;

    };

}

