cmake_minimum_required(VERSION 3.19)
project(Project)

set(CMAKE_CXX_STANDARD 14)

IF(WIN32)
    add_definitions(-D WINDOWS)
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp")
ENDIF()

add_executable(Project
        main.cpp)

