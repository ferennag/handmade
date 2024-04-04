#include "platform/platform.h"

#ifdef HN_PLATFORM_MACOSX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else

#include <unistd.h>

#endif

#include "core/logger.h"

typedef struct internal_state {
    Display *display;
    xcb_window_t window;
} InternalState;

b8 platform_init(PlatformState *platform_state, const char *app_name, i32 x, i32 y, i32 width, i32 height) {
    // https://alexvia.com/post/001_xlib_opening_window/

    platform_state->internal_state = calloc(1, sizeof(InternalState));
    InternalState *state = (InternalState *) platform_state->internal_state;

    Display *display = XOpenDisplay(NULL);
    state->display = display;

    Window window = XCreateSimpleWindow(
            display,
            DefaultRootWindow(display),    // parent
            x, y,                            // x, y
            width, height,                        // width, height
            0,                                // border width
            0x00000000,                        // border color
            0x00000000                        // background color
    );
    state->window = window;
    XStoreName(display, window, app_name);
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask);
    XMapWindow(display, window);
    XFlush(display);

    return TRUE;
}

void platform_shutdown(PlatformState *platform_state) {
    InternalState *state = (InternalState *) platform_state->internal_state;
    XCloseDisplay(state->display);
}

b8 platform_pump_messages(PlatformState *platform_state) {
    InternalState *state = (InternalState *) platform_state->internal_state;
    b8 quit_flagged = FALSE;

    while (XPending(state->display) > 0) {
        XEvent event = {0};
        XNextEvent(state->display, &event);
        if (event.type == KeyPress) {
            quit_flagged = TRUE;
        }
    }

    return quit_flagged;
}

void *platform_allocate(u64 size, b8 aligned) {
    return calloc(1, size);
}

void platform_free(void *block, b8 aligned) {
    free(block);
}

void *platform_zero_memory(void *block, u64 size) {
    return memset(block, 0, size);
}

void *platform_copy_memory(void *dest, const void *src, u64 size) {
    return memcpy(dest, src, size);
}

void *platform_set_memory(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char *message, u8 color) {
    const char *colors[] = {"0;41", "1;31", "1;33", "1;32", "0;41", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colors[color], message);
}

void platform_console_write_error(const char *message, u8 color) {
    const char *colors[] = {"0;41", "1;31", "1;33", "1;32", "0;41", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colors[color], message);
}

f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms > 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

#endif