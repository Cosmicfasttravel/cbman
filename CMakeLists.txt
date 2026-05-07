cmake_minimum_required(VERSION 4.3)
project(cbman)

set(CMAKE_CXX_STANDARD 23)

add_executable(cbman
        src/main.cpp
        src/threadManager.h
)
