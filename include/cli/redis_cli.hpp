// include/cli/redis_cli.hpp
#pragma once
#include "../core/redis_store.hpp"
#include "command_parser.hpp"
#include "response.hpp"
#include <string>

namespace redis::cli {

    class RedisCLI {
    public:
        explicit RedisCLI(RedisStore& store);
        void start();
        void stop();

    private:
        RedisStore& store;
        bool running;

        Response execute_command(const Command& cmd);
        void print_response(const Response& response);
        void print_help();
        void handle_input(const std::string& input);
    };

} // namespace redis::cli