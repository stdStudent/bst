cmake_minimum_required(VERSION 3.23)
project(ddp)

set(CMAKE_CXX_STANDARD 20)

add_executable(ddp main.cpp ddt.cpp ddt.h)
