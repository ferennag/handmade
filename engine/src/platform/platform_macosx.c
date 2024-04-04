#include "platform/platform.h"

#ifdef HN_PLATFORM_MACOSX

b8 platform_init(PlatformState *state, const char *app_name, i32 x, i32 y, i32 width, i32 height) {
    return TRUE;
}

void platform_shutdown(PlatformState *state) {

}

b8 platform_pump_messages(PlatformState *state) {
    return FALSE;
}

void *platform_allocate(u64 size, b8 aligned) {
    return 0;
}

void platform_free(void *block, b8 aligned) {

}

void *platform_zero_memory(void *block, u64 size) {
    return 0;
}

void *platform_copy_memory(void *dest, const void *src, u64 size) {
    return 0;
}

void *platform_set_memory(void *dest, i32 value, u64 size) {
 return 0;
}

void platform_console_write(const char *message, u8 color) {

}

void platform_console_write_error(const char *message, u8 color) {

}

f64 platform_get_absolute_time() {
    return 0;
}

void platform_sleep(u64 ms) {

}

#endif