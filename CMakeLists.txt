cmake_minimum_required(VERSION 2.8)

project(vm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_definitions(-std=c++11)

add_subdirectory(src)