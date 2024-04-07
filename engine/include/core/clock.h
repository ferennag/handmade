#pragma once

#include "defines.h"

typedef struct Clock {
    f64 target_fps;
    f64 start_time;
    f64 elapsed;

    f64 frame_start;
    f64 frame_end;
    f64 frame_elapsed;
    f64 last_time;
} Clock;

void clock_destroy(Clock *clock);

void clock_update(Clock *clock);

void clock_start(Clock *clock, f64 target_fps);

void clock_stop(Clock *clock);

f64 clock_delta_time(Clock *clock);

void clock_start_frame(Clock *clock);

void clock_end_frame(Clock *clock);