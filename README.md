# Redis-Like In-Memory Store

A lightweight C++ implementation of an in-memory key-value store, inspired by Redis. This project provides a command-line interface for interacting with the store and supports basic Redis-like operations.

## Features

### Core Functionality
- In-memory key-value storage
- Command-line interface (CLI)
- Thread-safe operations
- TTL (Time To Live) support
- LRU (Least Recently Used) cache implementation

### Supported Commands
- `SET key value` - Store key-value pairs
- `GET key` - Retrieve values
- `DEL key` - Remove key-value pairs
- `EXPIRE key seconds` - Set expiration time
- `TTL key` - Check remaining time
- `HELP` - Display command list
- `EXIT/QUIT` - Exit the server

### Technical Features
- Modern C++17 implementation
- Thread-safe operations using mutex locks
- Memory-efficient storage with automatic cleanup
- Configurable settings via config files
- Extensible command parser
- Robust error handling

## Project Structure
```
redis-like-store/
├── include/            # Header files
│   ├── core/          # Core functionality
│   ├── cli/           # CLI interface
│   └── utils/         # Utilities
├── src/               # Source files
│   ├── core/          # Core implementations
│   ├── cli/           # CLI implementations
│   └── utils/         # Utility implementations
└── CMakeLists.txt     # Build configuration
```

## Building
Requires:
- CMake 3.10 or higher
- C++17 compliant compiler
- Visual Studio 2022 (for Windows)

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Usage
After building, run the server:
```bash
./build/bin/Release/redis-server.exe
```

Basic commands:
```
redis> SET mykey "Hello World"
OK
redis> GET mykey
"Hello World"
redis> EXPIRE mykey 60
1
redis> TTL mykey
60
```

## Future Enhancements
- Data persistence
- Additional data structures (Lists, Hashes, Sets)
- Network interface
- Cluster support
- Publish/Subscribe messaging
- Transaction support
