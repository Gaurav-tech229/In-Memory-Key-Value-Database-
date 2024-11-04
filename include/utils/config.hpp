// include/utils/config.hpp
#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>

namespace redis {

    class Config {
    public:
        static Config& instance();

        std::string get(const std::string& key, const std::string& default_value = "");
        void set(const std::string& key, const std::string& value);
        bool load_file(const std::string& filename);
        bool save_file(const std::string& filename);

    private:
        Config() = default;  // Private constructor for singleton
        Config(const Config&) = delete;  // Prevent copying
        Config& operator=(const Config&) = delete;  // Prevent assignment

        std::unordered_map<std::string, std::string> settings;
        mutable std::mutex mutex;
        static Config* instance_;

        // Helper methods
        void parse_line(const std::string& line);
        std::string trim(const std::string& str);
    };

} // namespace redis
/*#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

namespace redis {

    class Config {
    public:
        static Config& instance();

        std::string get(const std::string& key, const std::string& default_value = "");
        void set(const std::string& key, const std::string& value);
        bool load_file(const std::string& filename);
        bool save_file(const std::string& filename);

    private:
        Config() = default;
        std::unordered_map<std::string, std::string> settings;
        static Config* instance_;
    };

} // namespace redis*/