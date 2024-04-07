cmake_minimum_required(VERSION 3.27)
project(engine LANGUAGES C CXX)
set(CMAKE_C_STANDARD 17)

include(CTest)

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
        src/containers/darray.c
        include/core/input.h
        src/core/input.c
        include/core/clock.h
        src/core/clock.c
        include/renderer/renderer_types.h
        include/renderer/renderer_frontend.h
        include/renderer/renderer_backend.h
        src/renderer/renderer_backend.c
        src/renderer/renderer_frontend.c
        include/renderer/vulkan/vulkan_backend.h
        src/renderer/vulkan/vulkan_backend.c
        include/renderer/vulkan/vulkan_types.h
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

add_subdirectory(tests)

enable_testing()
add_test(NAME engine_tests COMMAND check_engine)