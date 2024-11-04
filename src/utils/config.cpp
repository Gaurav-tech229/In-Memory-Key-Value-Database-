// src/utils/config.cpp
#include "utils/config.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace redis {

    Config* Config::instance_ = nullptr;

    Config& Config::instance() {
        if (!instance_) {
            instance_ = new Config();
        }
        return *instance_;
    }

    std::string Config::get(const std::string& key, const std::string& default_value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = settings.find(key);
        if (it != settings.end()) {
            return it->second;
        }
        return default_value;
    }

    void Config::set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex);
        settings[key] = value;
    }

    bool Config::load_file(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex);
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        settings.clear();
        std::string line;
        while (std::getline(file, line)) {
            parse_line(line);
        }

        return true;
    }

    bool Config::save_file(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex);
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& [key, value] : settings) {
            file << key << " = " << value << "\n";
        }

        return true;
    }

    void Config::parse_line(const std::string& line) {
        // Skip empty lines and comments
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            return;
        }

        // Find the equals sign
        size_t pos = trimmed.find('=');
        if (pos == std::string::npos) {
            return;
        }

        // Extract key and value
        std::string key = trim(trimmed.substr(0, pos));
        std::string value = trim(trimmed.substr(pos + 1));

        if (!key.empty()) {
            settings[key] = value;
        }
    }

    std::string Config::trim(const std::string& str) {
        const std::string whitespace = " \t\r\n";
        size_t start = str.find_first_not_of(whitespace);
        if (start == std::string::npos) {
            return "";
        }
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

} // namespace redis