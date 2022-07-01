cmake_minimum_required(VERSION 3.18)
project(ft_strace C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

file(GLOB_RECURSE SOURCE_FILES "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")

add_executable(strace ${SOURCE_FILES} ${HEADER_FILES})
add_subdirectory(libft)
include_directories(include libft/include)
