cmake_minimum_required(VERSION 2.6)

project(ukko)

set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++0x")

set(src slave.cpp master.cpp)

add_subdirectory(net)

add_library(ukko ${src})

link_libraries(ukko net)

add_executable(t t.cpp)
link_libraries(t ukko)
