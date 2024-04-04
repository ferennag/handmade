#pragma once

#include "core/application.h"

/**
 * Represents the basic game state.
 */
typedef struct game {
    ApplicationConfig application_config;

    b8 (*initialize)(struct game *game_inst);

    b8 (*update)(struct game *game_inst, f64 delta_time);

    b8 (*render)(struct game *game_inst, f64 delta_time);

    void (*on_resize)(struct game *game_inst, u32 width, u32 height);

    /// Game specific state, created and managed by the game
    void *state;
} Game;