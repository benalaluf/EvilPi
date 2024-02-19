cmake_minimum_required(VERSION 3.12)
project(EvilPi)

set(CMAKE_CXX_STANDARD 14)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
add_subdirectory(cpp)
