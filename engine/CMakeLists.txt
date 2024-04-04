cmake_minimum_required(VERSION 3.27)
project(engine C)
set(CMAKE_C_STANDARD 17)

find_package(Vulkan REQUIRED)
add_library(engine SHARED
        include/defines.h
        include/core/logger.h
        src/core/logger.c
        include/core/asserts.h
)
target_compile_options(engine PRIVATE -g -Wall -Werror -Wvarargs)
target_link_libraries(engine Vulkan::Vulkan)
target_include_directories(engine PUBLIC include)