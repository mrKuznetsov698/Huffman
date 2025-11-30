cmake_minimum_required(VERSION 3.27)
project(Huffman)

set(CMAKE_CXX_STANDARD 23)

add_executable(encoder encoder.cpp)
add_executable(decoder decoder.cpp)
