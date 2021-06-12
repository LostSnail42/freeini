cmake_minimum_required(VERSION 3.10)

project(freeini VERSION 0.1)

add_library(freeini INTERFACE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_include_directories(freeini INTERFACE include/)

add_subdirectory(test)
enable_testing()
add_test(BasicTest test/Test)

