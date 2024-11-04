// include/cli/command_parser.hpp
#pragma once
#include <string>
#include <vector>

namespace redis::cli {

    struct Command {
        std::string name;
        std::vector<std::string> args;
    };

    class CommandParser {
    public:
        static Command parse(const std::string& input);
        static bool validate(const Command& cmd);
        static std::string get_command_help(const std::string& command);

    private:
        static std::vector<std::string> tokenize(const std::string& input);
        static std::string normalize_command(const std::string& cmd);
    };

} // namespace redis::cli