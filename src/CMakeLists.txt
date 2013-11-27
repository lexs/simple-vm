cmake_minimum_required(VERSION 2.8)

project(vm)

add_definitions(-std=c++11)
add_executable(vm main.cpp vm.cpp value.cpp builder.cpp)