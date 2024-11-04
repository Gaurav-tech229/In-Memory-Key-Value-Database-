// src/core/cache_entry.cpp
#include "core/cache_entry.hpp"
#include <algorithm>
#include <iostream>

namespace redis {

    CacheEntry::CacheEntry(const RedisValue& val)
        : value(val)
        , last_access(std::chrono::system_clock::now())
        , access_count(0)
    {}

    CacheEntry::CacheEntry(CacheEntry&& other) noexcept
        : value(std::move(other.value))
        , last_access(other.last_access)
        , expiry(other.expiry)
        , access_count(other.access_count)
    {}

    CacheEntry::CacheEntry(const CacheEntry& other)
        : value(other.value)
        , last_access(other.last_access)
        , expiry(other.expiry)
        , access_count(other.access_count)
    {}

    CacheEntry& CacheEntry::operator=(CacheEntry&& other) noexcept {
        if (this != &other) {
            value = std::move(other.value);
            last_access = other.last_access;
            expiry = other.expiry;
            access_count = other.access_count;
        }
        return *this;
    }

    CacheEntry& CacheEntry::operator=(const CacheEntry& other) {
        if (this != &other) {
            value = other.value;
            last_access = other.last_access;
            expiry = other.expiry;
            access_count = other.access_count;
        }
        return *this;
    }

    bool CacheEntry::is_expired() const {
        if (!expiry) return false;
        return std::chrono::system_clock::now() >= *expiry;
    }

    void CacheEntry::update_access() {
        last_access = std::chrono::system_clock::now();
        ++access_count;
    }

    void CacheEntry::set_expiry(int seconds) {
        expiry = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
    }

    std::optional<int> CacheEntry::get_ttl() const {
        if (!expiry) return std::nullopt;

        auto now = std::chrono::system_clock::now();
        if (*expiry <= now) return 0;

        return std::chrono::duration_cast<std::chrono::seconds>(
            *expiry - now).count();
    }

    size_t CacheEntry::estimate_size() const {
        size_t size = 0;
        std::visit([&size, this](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                size = calculate_string_size(v);
            }
            else if constexpr (std::is_same_v<T, std::list<std::string>>) {
                for (const auto& item : v) {
                    size += calculate_string_size(item);
                }
            }
            else if constexpr (std::is_same_v<T, std::unordered_map<std::string, std::string>>) {
                for (const auto& [k, val] : v) {
                    size += calculate_string_size(k);
                    size += calculate_string_size(val);
                }
            }
            }, value);
        return size;
    }

    size_t CacheEntry::calculate_string_size(const std::string& str) const {
        return str.capacity() + sizeof(str);
    }

} // namespace redis