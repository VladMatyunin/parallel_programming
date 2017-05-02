cmake_minimum_required(VERSION 3.6)
project(pg_research)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER  "/usr/bin/mpic++")
set(CMAKE_C_COMPILER "/usr/bin/mpicc")
include_directories(/usr/include/mpi/)
set(SOURCE_FILES speedUp.c)
add_executable(pg_research ${SOURCE_FILES})