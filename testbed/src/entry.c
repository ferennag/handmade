#include <entry.h>
#include "game.h"
#include <core/memory.h>

b8 create_game(Game *out_game) {
    out_game->application_config.start_pos_x = 100;
    out_game->application_config.start_pos_y = 100;
    out_game->application_config.start_width = 1280;
    out_game->application_config.start_height = 760;
    out_game->application_config.name = "Handmade Hero";

    // Hook up the callback functions
    out_game->initialize = game_initialize;
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->on_resize = game_on_resize;

    // Create the game state
    GameState *game_state = hm_alloc(sizeof(GameState), MEMORY_TAG_GAME);
    out_game->state = game_state;

    return TRUE;
}