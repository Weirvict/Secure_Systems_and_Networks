cmake_minimum_required(VERSION 3.10)
project(sim)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(sim node0.cc node1.cc node2.cc node3.cc prog3.cc)
