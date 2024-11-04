// include/core/lru_cache.hpp
#pragma once
#include <list>
#include <unordered_map>
#include <string>

namespace redis {

    class LRUCache {
    public:
        explicit LRUCache(size_t capacity);

        void access(const std::string& key);
        bool should_evict(const std::string& key) const;
        void remove(const std::string& key);
        void clear();

        size_t size() const;
        size_t get_capacity() const;

    private:
        size_t capacity;
        std::list<std::string> lru_list;  // Most recently used at front
        std::unordered_map<std::string, std::list<std::string>::iterator> lru_map;
    };

} // namespace redis