#pragma once

#include "defines.h"

typedef struct platform_state {
    void *internal_state;
} PlatformState;

b8 platform_init(PlatformState *platform_state, const char *app_name, i32 x, i32 y, i32 width, i32 height);
void platform_shutdown(PlatformState *platform_state);

b8 platform_pump_messages(PlatformState *platform_state);

void *platform_allocate(u64 size, b8 aligned);
void platform_free(void *block, b8 aligned);
void *platform_zero_memory(void *block, u64 size);
void *platform_copy_memory(void *dest, const void *src, u64 size);
void *platform_set_memory(void *dest, i32 value, u64 size);

void platform_console_write(const char *message, u8 color);
void platform_console_write_error(const char *message, u8 color);

f64 platform_get_absolute_time();

void platform_sleep(u64 ms);
