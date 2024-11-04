#include "core/redis_store.hpp"
#include "utils/logger.hpp"
#include <chrono>
#include <thread>
#include <iostream>

namespace redis {

    RedisStore::RedisStore(size_t cache_size)
        : lru_cache(cache_size)
        , running(true)
        , current_memory(0) {
        cleanup_thread = std::thread(&RedisStore::cleanup_expired_keys, this);
    }

    RedisStore::~RedisStore() {
        running = false;
        if (cleanup_thread.joinable()) {
            cleanup_thread.join();
        }
    }

    bool RedisStore::set(const std::string& key, const RedisValue& value) {
        std::lock_guard<std::mutex> lock(mutex);
        // Explicitly create CacheEntry with the value
        store.insert_or_assign(key, CacheEntry(value));
        lru_cache.access(key);
        return true;
    }

    std::optional<RedisValue> RedisStore::get(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = store.find(key);
        if (it != store.end() && !it->second.is_expired()) {
            it->second.update_access();
            lru_cache.access(key);
            return it->second.value;
        }
        return std::nullopt;
    }

    bool RedisStore::del(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex);
        return store.erase(key) > 0;
    }

    bool RedisStore::expire(const std::string& key, int seconds) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = store.find(key);
        if (it != store.end()) {
            it->second.set_expiry(seconds);
            return true;
        }
        return false;
    }

    int RedisStore::ttl(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = store.find(key);
        if (it != store.end()) {
            auto ttl = it->second.get_ttl();
            return ttl.value_or(-1);
        }
        return -2;
    }

    bool RedisStore::lpush(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = store.find(key);
        if (it == store.end()) {
            std::list<std::string> list{ value };
            return set(key, RedisValue(list));
        }

        if (auto* list = std::get_if<std::list<std::string>>(&it->second.value)) {
            list->push_front(value);
            return true;
        }
        return false;
    }

    std::optional<std::string> RedisStore::lpop(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = store.find(key);
        if (it != store.end()) {
            if (auto* list = std::get_if<std::list<std::string>>(&it->second.value)) {
                if (!list->empty()) {
                    auto value = list->front();
                    list->pop_front();
                    return value;
                }
            }
        }
        return std::nullopt;
    }

    void RedisStore::cleanup_expired_keys() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::lock_guard<std::mutex> lock(mutex);

            for (auto it = store.begin(); it != store.end();) {
                if (it->second.is_expired()) {
                    it = store.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
    }

    size_t RedisStore::size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return store.size();
    }

    void RedisStore::clear() {
        std::lock_guard<std::mutex> lock(mutex);
        store.clear();
        lru_cache.clear();
    }

} // namespace redis