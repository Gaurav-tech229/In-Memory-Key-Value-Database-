#pragma once
// include/core/redis_value.hpp
#pragma once
#include <variant>
#include <string>
#include <list>
#include <unordered_map>
#include <optional>
#include <chrono>
#include <iostream>

namespace redis {

    // Define the possible value types
    using RedisValue = std::variant<
        std::string,
        std::list<std::string>,
        std::unordered_map<std::string, std::string>
    >;

    // Value type enumeration
    enum class ValueType {
        STRING,
        LIST,
        HASH,
        NONE
    };

    // Value type helper functions
    ValueType get_value_type(const RedisValue& value);
    std::string value_type_to_string(ValueType type);

} // namespace redis