#include "cli/redis_cli.hpp"
#include "cli/command_parser.hpp"
#include <iostream>
#include <string>

namespace redis::cli {

    RedisCLI::RedisCLI(RedisStore& store_instance)
        : store(store_instance)
        , running(true) {
    }

    void RedisCLI::start() {
        std::cout << "Redis-like CLI (press Ctrl+C to exit)\n";
        std::cout << "Type 'HELP' for commands list\n";

        while (running) {
            std::cout << "redis> ";
            std::string input;
            std::getline(std::cin, input);

            if (input.empty()) continue;

            handle_input(input);
        }
    }

    void RedisCLI::stop() {
        running = false;
    }

    void RedisCLI::handle_input(const std::string& input) {
        auto command = CommandParser::parse(input);
        auto response = execute_command(command);
        print_response(response);
    }

    Response RedisCLI::execute_command(const Command& cmd) {
        if (cmd.name == "HELP") {
            print_help();
            return Response{ Response::Type::OK, "Help displayed" };
        }

        if (cmd.name == "EXIT" || cmd.name == "QUIT") {
            running = false;  // This will stop the command loop
            return Response{ Response::Type::OK, "Bye!" };
        }

        if (cmd.name == "SET") {
            if (cmd.args.size() < 2) {
                return Response{ Response::Type::ERROR, "ERR wrong number of arguments for 'set' command" };
            }
            store.set(cmd.args[0], RedisValue(cmd.args[1]));
            return Response{ Response::Type::OK };
        }

        if (cmd.name == "GET") {
            if (cmd.args.empty()) {
                return Response{ Response::Type::ERROR, "ERR wrong number of arguments for 'get' command" };
            }
            auto value = store.get(cmd.args[0]);
            if (!value) {
                return Response{ Response::Type::NIL };
            }
            return Response{ Response::Type::STRING, "", value };
        }

        return Response{ Response::Type::ERROR, "ERR unknown command '" + cmd.name + "'" };
    }

    void RedisCLI::print_response(const Response& response) {
        std::cout << response.format() << std::endl;
    }

    void RedisCLI::print_help() {
        std::cout << "\nAvailable Commands:\n"
            << "SET key value     -- Set key to hold string value\n"
            << "GET key          -- Get value of key\n"
            << "DEL key          -- Delete key\n"
            << "EXPIRE key sec   -- Set key TTL in seconds\n"
            << "TTL key          -- Get remaining TTL of key\n"
            << "HELP             -- Show this help\n\n";
            << "EXIT             -- Exit the server (or use Ctrl+C)\n"
            << "QUIT             -- Exit the server (or use Ctrl+C)\n\n";
    }

} // namespace redis::cli