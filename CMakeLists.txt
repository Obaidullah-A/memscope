cmake_minimum_required(VERSION 3.20)
project(memscope)

set(CMAKE_CXX_STANDARD 17)

add_executable(memscope
    src/main.cpp
    src/memory.cpp
)
