cmake_minimum_required(VERSION 3.27)
project(engine C)
set(CMAKE_C_STANDARD 17)

find_package(Vulkan REQUIRED)
add_library(engine SHARED
        include/defines.h
        include/core/logger.h
        src/core/logger.c
        include/core/asserts.h
        include/platform/platform.h
        src/platform/platform_macosx.c
        include/core/application.h
        src/core/application.c
        include/game_types.h
        include/entry.h
        include/core/memory.h
        src/core/memory.c
        include/core/event.h
        src/core/event.c
        include/containers/darray.h
        include/containers/darray.c
)

if (APPLE)
    find_package(X11 REQUIRED)
    message(STATUS "Using X11 windowing system for Apple platforms.")
    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

    set(WINDOWING_LIBRARIES X11::X11 X11::xcb X11::X11_xcb)
endif ()

target_compile_options(engine PRIVATE -g -Wall -Werror -Wvarargs)
target_link_libraries(engine Vulkan::Vulkan ${WINDOWING_LIBRARIES})
target_include_directories(engine PUBLIC include)