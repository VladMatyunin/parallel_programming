cmake_minimum_required(VERSION 3.6)
project(bitonic_sort)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
add_executable(bitonic_sort ${SOURCE_FILES})
target_link_libraries(bitonic_sort m)