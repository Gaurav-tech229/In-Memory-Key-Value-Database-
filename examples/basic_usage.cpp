// examples/basic_usage.cpp
#include "core/redis_store.hpp"
#include <iostream>

int main() {
    redis::RedisStore store;

    // String operations
    store.set("user:1", redis::RedisValue("John Doe"));
    auto user = store.get("user:1");
    if (user) {
        std::cout << "User: " << std::get<std::string>(*user) << "\n";
    }

    // List operations
    store.lpush("notifications", "New message");
    store.lpush("notifications", "Friend request");

    // Hash operations
    store.hset("user:1:profile", "name", "John Doe");
    store.hset("user:1:profile", "email", "john@example.com");

    return 0;
}