// include/core/cache_entry.hpp
#pragma once
#include "redis_value.hpp"
#include <chrono>
#include <optional>

namespace redis {

    class CacheEntry {
    public:
        // Constructor with required value
        explicit CacheEntry(const RedisValue& val);

        // Add move constructor
        CacheEntry(CacheEntry&& other) noexcept;

        // Add copy constructor
        CacheEntry(const CacheEntry& other);

        // Add assignment operators
        CacheEntry& operator=(CacheEntry&& other) noexcept;
        CacheEntry& operator=(const CacheEntry& other);

        RedisValue value;
        std::chrono::system_clock::time_point last_access;
        std::optional<std::chrono::system_clock::time_point> expiry;
        size_t access_count;

        bool is_expired() const;
        void update_access();
        void set_expiry(int seconds);
        std::optional<int> get_ttl() const;
        size_t estimate_size() const;

    private:
        size_t calculate_string_size(const std::string& str) const;
    };

} // namespace redis