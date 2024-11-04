// src/core/lru_cache.cpp
#include "core/lru_cache.hpp"
#include <iostream>

namespace redis {

    LRUCache::LRUCache(size_t cap) : capacity(cap) {}

    void LRUCache::access(const std::string& key) {
        auto it = lru_map.find(key);
        if (it != lru_map.end()) {
            lru_list.erase(it->second);
        }
        else if (lru_list.size() >= capacity) {
            auto last = lru_list.back();
            lru_list.pop_back();
            lru_map.erase(last);
        }
        lru_list.push_front(key);
        lru_map[key] = lru_list.begin();
    }

    bool LRUCache::should_evict(const std::string& key) const {
        return lru_map.find(key) == lru_map.end();
    }

    void LRUCache::remove(const std::string& key) {
        auto it = lru_map.find(key);
        if (it != lru_map.end()) {
            lru_list.erase(it->second);
            lru_map.erase(it);
        }
    }

    void LRUCache::clear() {
        lru_list.clear();
        lru_map.clear();
    }

    size_t LRUCache::size() const {
        return lru_list.size();
    }

    size_t LRUCache::get_capacity() const {
        return capacity;
    }

} // namespace redis