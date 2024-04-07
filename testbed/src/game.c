#include <core/logger.h>
#include <core/event.h>
#include "game.h"

b8 on_keypress (EventCode code, void *sender, void *listener_inst, EventContext context) {
    HM_DEBUG("Key pressed: %d", context.data.u16[0]);
    return TRUE;
}

b8 on_mouse_move (EventCode code, void *sender, void *listener_inst, EventContext context) {
    HM_DEBUG("Mouse moved: %d, %d", context.data.u16[0], context.data.u16[1]);
    return TRUE;
}

b8 game_initialize(struct game *game_inst) {
    HM_DEBUG("Game Initialize called");

    event_subscribe(EVENT_CODE_KEY_PRESSED, 0, on_keypress);
    event_subscribe(EVENT_CODE_MOUSE_MOVED, 0, on_mouse_move);

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
