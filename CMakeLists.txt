cmake_minimum_required(VERSION 3.10)
project(RedisLikeStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Runtime Library to MD for all configurations
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/core
    ${PROJECT_SOURCE_DIR}/include/cli
    ${PROJECT_SOURCE_DIR}/include/utils
)

# Source files
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp"
)

file(GLOB_RECURSE CLI_SOURCES 
    "src/cli/*.cpp"
)

file(GLOB_RECURSE UTILS_SOURCES 
    "src/utils/*.cpp"
)

# Create library from sources
add_library(redis_lib STATIC
    ${CORE_SOURCES}
    ${CLI_SOURCES}
    ${UTILS_SOURCES}
)

# Main executable
add_executable(redis-server src/main.cpp)
target_link_libraries(redis-server PRIVATE redis_lib)

# Set output directories
set_target_properties(redis-server redis_lib
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)