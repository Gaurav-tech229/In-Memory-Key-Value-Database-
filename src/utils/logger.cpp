#include "utils/logger.hpp"
#include <iomanip>
#include <sstream>
#include <iostream>

namespace redis {

    Logger::Logger(const std::string& filename) {
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }

    Logger::~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void Logger::log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);

        log_file << get_timestamp() << " "
            << "[" << level_to_string(level) << "] "
            << message << std::endl;

        // Also print to console for ERROR level
        if (level == Level::ERROR) {
            std::cerr << get_timestamp() << " "
                << "[" << level_to_string(level) << "] "
                << message << std::endl;
        }
    }

    void Logger::debug(const std::string& message) {
        log(Level::DEBUG, message);
    }

    void Logger::info(const std::string& message) {
        log(Level::INFO, message);
    }

    void Logger::warning(const std::string& message) {
        log(Level::WARNING, message);
    }

    void Logger::error(const std::string& message) {
        log(Level::ERROR, message);
    }

    std::string Logger::level_to_string(Level level) const {
        switch (level) {
        case Level::DEBUG:   return "DEBUG";
        case Level::INFO:    return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR:   return "ERROR";
        default:            return "UNKNOWN";
        }
    }

    std::string Logger::get_timestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }

} // namespace redis