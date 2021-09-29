cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(firefly)

include_directories("${PROJECT_SOURCE_DIR}/core/utility")
include_directories("${PROJECT_SOURCE_DIR}/core/network")
add_subdirectory(core)

add_subdirectory(test)

aux_source_directory(. DIR_SRCS)

add_executable(${PROJECT_NAME} main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} network util test)
