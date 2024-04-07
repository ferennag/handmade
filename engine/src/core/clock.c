#include "core/clock.h"
#include "core/memory.h"
#include "platform/platform.h"

void clock_destroy(Clock *clock) {
    if (clock != 0) {
        hm_free(clock, sizeof(Clock), MEMORY_TAG_APPLICATION);
    }
}

void clock_update(Clock *clock) {
    if (clock->start_time != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->start_time;

        if (clock->last_time == 0) {
            clock->last_time = clock->elapsed;
        }
    }
}

void clock_start(Clock *clock, f64 target_fps) {
    clock->start_time = platform_get_absolute_time();
    clock->target_fps = target_fps;
    clock->elapsed = 0;
}

void clock_stop(Clock *clock) {
    clock->start_time = 0;
}

f64 clock_delta_time(Clock *clock) {
    return clock->elapsed - clock->last_time;
}

void clock_start_frame(Clock *clock) {
    clock->frame_start = platform_get_absolute_time();
}

void clock_end_frame(Clock *clock) {
    clock->frame_end = platform_get_absolute_time();
    clock->frame_elapsed = clock->frame_end - clock->frame_start;
    clock->last_time = clock->frame_start;

    if (clock->target_fps != 0) {
        f64 remaining_seconds = clock->target_fps - clock->frame_elapsed;

        if (remaining_seconds > 0) {
            f64 remaining_ms = remaining_seconds * 1000;
            platform_sleep(remaining_ms - 1);
        }
    }
}