cmake_minimum_required(VERSION 3.15)
project(class_scheduling_csp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(class_scheduling_csp main.cpp src/CSP.cpp src/Classes.cpp)