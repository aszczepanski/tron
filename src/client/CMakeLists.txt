cmake_minimum_required(VERSION 2.8)

file(GLOB CLIENT_SRCS ./*.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(client ${CLIENT_SRCS})
