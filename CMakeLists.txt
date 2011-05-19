cmake_minimum_required(VERSION 2.6)

project(ukko)

set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++0x")

set(src ukko.cpp)

add_subdirectory(net)

add_library(ukko ${src})

link_libraries(net)
