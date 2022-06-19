cmake_minimum_required(VERSION 3.18)
project(strace C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCE_FILES "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")

add_executable(strace ${SOURCE_FILES} ${HEADER_FILES})
include_directories(include)
