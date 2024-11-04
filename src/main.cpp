// src/main.cpp
#include "core/redis_store.hpp"
#include "cli/redis_cli.hpp"
#include "utils/config.hpp"
#include "utils/logger.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        redis::RedisStore store;
        redis::cli::RedisCLI cli(store);

        std::cout << "Redis-like server starting...\n";
        cli.start();

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}