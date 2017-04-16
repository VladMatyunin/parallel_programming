cmake_minimum_required(VERSION 3.6)
project(page_rank)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(page_rank ${SOURCE_FILES})
