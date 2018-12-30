# Initialize project
cmake_minimum_required(VERSION 3.9)
project(initok_example)

# Set proper build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Compile examples
add_executable(initok_c initok_example.c)
add_executable(initok_cxx initok_example.cpp)