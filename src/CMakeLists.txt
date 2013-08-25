cmake_minimum_required(VERSION 2.8)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(application)

set(EXECUTABLE_OUTPUT_PATH ..)

set(LIBS application server client)
set(LIBS ${LIBS} pthread)

file(GLOB COMMON_SRCS common/*.cpp)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_definitions("-Wall")

add_executable(main main.cpp ${COMMON_SRCS})
target_link_libraries(main ${LIBS})
