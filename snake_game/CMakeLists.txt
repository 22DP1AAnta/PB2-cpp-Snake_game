cmake_minimum_required(VERSION 3.23)
project(snake_game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(snake_game main.cpp)
