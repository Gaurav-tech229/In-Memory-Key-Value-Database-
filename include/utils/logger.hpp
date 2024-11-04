// include/utils/logger.hpp
#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iostream>

namespace redis {

    class Logger {
    public:
        enum class Level {
            DEBUG,
            INFO,
            WARNING,
            ERROR
        };

        explicit Logger(const std::string& filename = "redis.log");
        ~Logger();

        void log(Level level, const std::string& message);
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);

    private:
        std::ofstream log_file;
        mutable std::mutex mutex;

        std::string level_to_string(Level level) const;
        std::string get_timestamp() const;
    };

} // namespace redis