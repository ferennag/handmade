#include <core/logger.h>
#include "game.h"

b8 game_initialize(struct game *game_inst) {
    HM_DEBUG("Game Initialize called");
    return TRUE;
}

b8 game_update(struct game *game_inst, f64 delta_time) {
    return TRUE;
}

b8 game_render(struct game *game_inst, f64 delta_time) {
    return TRUE;
}

void game_on_resize(struct game *game_inst, u32 width, u32 height) {
}
