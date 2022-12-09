cmake_minimum_required(VERSION 3.23)
project(ddp)

set(CMAKE_CXX_STANDARD 20)

add_executable(ddp main.cpp ddt.cpp ddt.h)

string(APPEND CMAKE_CXX_FLAGS " -O2 -pipe -march=native")
string(APPEND CMAKE_C_FLAGS " -O2 -pipe -march=native")
add_compile_options(-O2 -pipe -march=native)
