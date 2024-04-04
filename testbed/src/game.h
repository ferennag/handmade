#pragma once

#include <defines.h>
#include <game_types.h>

typedef struct game_state {
    f32 delta_time;
} GameState;

b8 game_initialize(struct game *game_inst);

b8 game_update(struct game *game_inst, f64 delta_time);

b8 game_render(struct game *game_inst, f64 delta_time);

void game_on_resize(struct game *game_inst, u32 width, u32 height);
