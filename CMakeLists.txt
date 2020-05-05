cmake_minimum_required(VERSION 3.1)
project(chatroom)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(EXECUTABLE_LIBRARY_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(lion)
add_subdirectory(src)

