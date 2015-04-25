cmake_minimum_required(VERSION 3.1)
project(hw7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=ansi")

set(SOURCE_FILES main.c utility.c parse_commands.c pipe_commands.c history.c linked_list.c commands.c builtin.c ls.c)
set(HEADER_FILES utility.h parse_commands.h pipe_commands.h history.h linked_list.h commands.h builtin.h ls.h)

add_executable(hw7 ${SOURCE_FILES} ${HEADER_FILES})
