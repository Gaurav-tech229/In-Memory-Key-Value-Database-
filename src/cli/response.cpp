// src/cli/response.cpp
#include "cli/response.hpp"
#include <iostream>

namespace redis::cli {

    Response::Response(Type t, const std::string& msg, std::optional<RedisValue> val)
        : type(t), message(msg), value(val) {}

    std::string Response::format() const {
        switch (type) {
        case Type::OK:
            return "OK";
        case Type::ERROR:
            return "(error) " + message;
        case Type::NIL:
            return "(nil)";
        case Type::STRING:
            return message.empty() ? format_value() : message;
        case Type::INTEGER:
            return message;
        case Type::ARRAY:
            return format_value();
        default:
            return "(error) Unknown response type";
        }
    }

    bool Response::is_error() const {
        return type == Type::ERROR;
    }

    std::string Response::format_value() const {
        if (!value) return "(nil)";

        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return "\"" + v + "\"";
            }
            else if constexpr (std::is_same_v<T, std::list<std::string>>) {
                std::string result = "1) ";
                int i = 1;
                for (const auto& item : v) {
                    result += "\"" + item + "\"\n" +
                        std::to_string(++i) + ") ";
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, std::unordered_map<std::string, std::string>>) {
                std::string result = "1) ";
                int i = 1;
                for (const auto& [k, val] : v) {
                    result += "\"" + k + "\": \"" + val + "\"\n" +
                        std::to_string(++i) + ") ";
                }
                return result;
            }
            return "(unknown type)";
            }, *value);
    }

} // namespace redis::cli