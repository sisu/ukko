cmake_minimum_required(VERSION 2.6)

project(net)

set(src Connection.cpp ServerSocket.cpp)
add_library(net ${src})
