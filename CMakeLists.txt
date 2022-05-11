cmake_minimum_required(VERSION 3.1)

set(PACKAGE_VERSION 0.0.1)

project(Installation VERSION ${PACKAGE_VERSION})

INCLUDE(CMakeForceCompiler)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(firefly)

set(INCLUDE_DIR
        ./firefly/Utility/headers
        ./firefly/third_party
        )

aux_source_directory(./firefly/test/ TEST_DIR_SRCS)
aux_source_directory(./firefly/Utility/src UTILITY_DIR_SRCS)
aux_source_directory(./firefly/third_party THIRD_PARTY_DIR_SRCS)

file(GLOB ALL_SOURCE
        ${UTILITY_DIR_SRCS}
        ${THIRD_PARTY_DIR_SRCS}
        )

include_directories(${INCLUDE_DIR})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

#add_executable(${PROJECT_NAME} main.cpp ${ALL_SOURCE} ${TEST_DIR_SRCS})

add_library(${PROJECT_NAME} ${ALL_SOURCE})

file(GLOB HEADERS_FILES
        CONFIGURE_DEPENDS LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/firefly/**/**/*.h*")

foreach(header ${HEADERS_FILES})
    string(APPEND header_path "${header}\;")
endforeach()

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${header_path})

find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        fireflyConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS ${PROJECT_NAME}
        EXPORT fireflyTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib #static lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include/firefly
        )

install(EXPORT fireflyTargets
        FILE fireflyTargets.cmake
        NAMESPACE firefly::
        DESTINATION lib/cmake/firefly
        )

configure_file(fireflyConfig.cmake.in fireflyConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/fireflyConfigVersion.cmake"
        DESTINATION lib/cmake/firefly
        )