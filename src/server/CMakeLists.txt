cmake_minimum_required(VERSION 2.8)

file(GLOB SERVER_SRCS ./*.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(server ${SERVER_SRCS})
