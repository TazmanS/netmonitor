cmake_minimum_required(VERSION 3.16)

project(netmonitor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(
    netmonitor
    ${SOURCES}
)

target_compile_options(netmonitor PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wshadow
    -Wconversion
)

target_include_directories(
    netmonitor
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)