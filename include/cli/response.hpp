// include/cli/response.hpp
#pragma once
#include "../core/redis_value.hpp"
#include <string>
#include <optional>
#include <iostream>

namespace redis::cli {

    class Response {
    public:
        enum class Type {
            OK,
            ERROR,
            NIL,
            STRING,
            INTEGER,
            ARRAY
        };

        Response(Type type, const std::string& msg = "",
            std::optional<RedisValue> val = std::nullopt);

        std::string format() const;
        bool is_error() const;

        Type type;
        std::string message;
        std::optional<RedisValue> value;

    private:
        std::string format_value() const;
    };

} // namespace redis::cli