cmake_minimum_required(VERSION 3.19)
project(Project)

set(CMAKE_CXX_STANDARD 14)

IF(WIN32)
    add_definitions(-D WINDOWS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
ELSE()
    add_definitions(-D DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -pthread -fopenmp")
ENDIF()

add_executable(Project main.cpp)
add_executable(packager packager.cpp)

