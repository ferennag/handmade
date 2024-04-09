cmake_minimum_required(VERSION 3.27)
project(engine LANGUAGES C)
set(CMAKE_C_STANDARD 17)

include(CTest)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_library(engine SHARED
        include/defines.h
        include/core/logger.h
        src/core/logger.c
        include/core/asserts.h
        include/platform/platform.h
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
        include/core/string.h
        src/core/string.c
        include/renderer/vulkan/vulkan_platform.h
        include/renderer/vulkan/vulkan_device.h
        src/renderer/vulkan/vulkan_device.c
        src/platform/platform_sdl.c
)

target_compile_options(engine PRIVATE -g -Wall -Werror -Wvarargs)
target_link_libraries(engine Vulkan::Vulkan ${SDL2_LIBRARIES})
target_include_directories(engine PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

add_subdirectory(tests)

enable_testing()
add_test(NAME engine_tests COMMAND check_engine)