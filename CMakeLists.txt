cmake_minimum_required(VERSION 3.1)

INCLUDE(CMakeForceCompiler)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(firefly)

set(INCLUDE_DIR
        ./test
        ./Utility
        ./third_party
        )

aux_source_directory(./test/ TEST_DIR_SRCS)
aux_source_directory(./Utility CORE_UTILITY_DIR_SRCS)
aux_source_directory(./third_party THIRD_PARTY_DIR_SRCS)

file(GLOB ALL_SOURCE
        ${TEST_DIR_SRCS}
        ${CORE_NETWORK_DIR_SRCS}
        ${CORE_UTILITY_DIR_SRCS}
        ${THIRD_PARTY_DIR_SRCS}
        )

include_directories(${INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_executable(${PROJECT_NAME} main.cpp ${ALL_SOURCE})

find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
