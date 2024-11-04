// include/core/redis_store.hpp
#pragma once
#include "redis_value.hpp"
#include "cache_entry.hpp"
#include "lru_cache.hpp"
#include "../utils/logger.hpp"
#include <unordered_map>
#include <mutex>
#include <thread>

namespace redis {

    class RedisStore {
    public:
        RedisStore(size_t cache_size = 1000);
        ~RedisStore();

        // Basic operations
        bool set(const std::string& key, const RedisValue& value);
        std::optional<RedisValue> get(const std::string& key);
        bool del(const std::string& key);

        // TTL operations
        bool expire(const std::string& key, int seconds);
        int ttl(const std::string& key) const;

        // List operations
        bool lpush(const std::string& key, const std::string& value);
        std::optional<std::string> lpop(const std::string& key);

        // Hash operations
        bool hset(const std::string& key, const std::string& field, const std::string& value);
        std::optional<std::string> hget(const std::string& key, const std::string& field);

        // Persistence
        bool save_snapshot();
        bool load_snapshot();

        // Stats
        size_t size() const;
        size_t memory_usage() const;
        void clear();

    private:
        std::unordered_map<std::string, CacheEntry> store;
        mutable std::mutex mutex;
        LRUCache lru_cache;
        Logger logger;

        std::thread cleanup_thread;
        bool running;

        const size_t MAX_MEMORY = 1024 * 1024 * 1024;  // 1GB
        size_t current_memory;

        void cleanup_expired_keys();
        void evict_if_needed(size_t required_size);
        bool append_to_aof(const std::string& command, const std::string& key);
    };

} // namespace redis