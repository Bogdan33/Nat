cmake_minimum_required(VERSION 3.31)
project(NaT)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

add_library(NaT_lib
        src/Nat.cpp)

add_executable(NaT main.cpp)

target_link_libraries(NaT NaT_lib)