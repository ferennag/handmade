cmake_minimum_required(VERSION 3.27)
project(testbed C)

set(CMAKE_C_STANDARD 17)

add_executable(testbed src/entry.c
        src/game.h
        src/game.c)
target_link_libraries(testbed engine)