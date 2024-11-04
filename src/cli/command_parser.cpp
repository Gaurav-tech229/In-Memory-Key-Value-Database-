// src/cli/command_parser.cpp
#include "cli/command_parser.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace redis::cli {

    Command CommandParser::parse(const std::string& input) {
        auto tokens = tokenize(input);
        if (tokens.empty()) {
            return Command{ "", {} };
        }

        std::string name = normalize_command(tokens[0]);
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());

        return Command{ name, args };
    }

    std::vector<std::string> CommandParser::tokenize(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

    std::string CommandParser::normalize_command(const std::string& cmd) {
        std::string normalized = cmd;
        std::transform(normalized.begin(), normalized.end(),
            normalized.begin(), ::toupper);
        return normalized;
    }

} // namespace redis::cli