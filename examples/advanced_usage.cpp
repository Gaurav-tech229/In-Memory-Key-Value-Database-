// examples/advanced_usage.cpp
#include "core/redis_store.hpp"
#include <iostream>
#include <thread>

void monitor_key(redis::RedisStore& store, const std::string& key) {
    while (true) {
        auto value = store.get(key);
        if (value) {
            std::cout << "Key " << key << " = " <<
                std::get<std::string>(*value) << "\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    redis::RedisStore store;

    // Set with expiry
    store.set("temp_key", redis::RedisValue("temporary value"));
    store.expire("temp_key", 5);

    // Monitor in separate thread
    std::thread monitor_thread(monitor_key, std::ref(store), "temp_key");

    // Wait for expiry
    std::this_thread::sleep_for(std::chrono::seconds(6));

    if (monitor_thread.joinable()) {
        monitor_thread.join();
    }

    return 0;
}