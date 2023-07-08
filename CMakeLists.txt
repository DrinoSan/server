cmake_minimum_required(VERSION 3.19)
project(cpp-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

## Use file globbing to find all source files in the src directory
#file(GLOB SOURCES "main.c" "src/*.c" "src/utils/*.c")
file(GLOB SOURCES "main.cpp" "src/*.cpp" "src/utils/*.cpp")
file(GLOB_RECURSE HEADERS src/*.h src/utils/*.h)

# Add your source files here
add_executable(cpp-server ${SOURCES})
