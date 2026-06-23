cmake_minimum_required(VERSION 3.16)

project(netmonitor C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(
    netmonitor
    ${SOURCES}
)

target_include_directories(
    netmonitor
    PRIVATE
    include
)