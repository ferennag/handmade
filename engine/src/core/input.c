#include <core/input.h>
#include <core/event.h>
#include <core/memory.h>
#include <core/logger.h>

typedef struct KeyboardState {
    b8 keys[256];
} KeyboardState;

typedef struct MouseState {
    i16 x;
    i16 y;
    u8 buttons[MOUSE_MAX_BUTTONS];
} MouseState;

typedef struct InputState {
    KeyboardState keyboard_previous, keyboard_current;
    MouseState mouse_previous, mouse_current;
} InputState;

static b8 initialized = FALSE;
static InputState input_state = {0};

b8 input_init() {
    HM_INFO("Initializing input subsystem...");
    initialized = TRUE;
    return TRUE;
}

void input_shutdown() {
    initialized = FALSE;
}

void input_update(f64 delta_time) {
    if (!initialized) {
        return;
    }

    hm_copy_memory(&input_state.keyboard_previous, &input_state.keyboard_current, sizeof(KeyboardState));
    hm_copy_memory(&input_state.mouse_previous, &input_state.mouse_current, sizeof(MouseState));
}

void input_process_key(Keys key, b8 pressed) {
    if (input_state.keyboard_current.keys[key] != pressed) {
        input_state.keyboard_current.keys[key] = pressed;

        EventContext context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_button(MouseButtons button, b8 pressed) {
    if (input_state.mouse_current.buttons[button] != pressed) {
        input_state.mouse_current.buttons[button] = pressed;

        EventContext context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y) {
    if (input_state.mouse_current.x != x || input_state.mouse_current.y != y) {
        input_state.mouse_current.x = x;
        input_state.mouse_current.y = y;

        EventContext context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 delta) {
    // Note: Internal state is not stored, we are just firing an event
    EventContext context;
    context.data.u8[0] = delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

b8 input_is_key_down(Keys key) {
    if (!initialized) {
        return FALSE;
    }

    return input_state.keyboard_current.keys[key] == TRUE;
}

b8 input_is_key_up(Keys key) {
    if (!initialized) {
        return TRUE;
    }

    return input_state.keyboard_current.keys[key] == FALSE;
}

b8 input_was_key_down(Keys key) {
    if (!initialized) {
        return FALSE;
    }

    return input_state.keyboard_previous.keys[key] == TRUE;
}

b8 input_was_key_up(Keys key) {
    if (!initialized) {
        return TRUE;
    }

    return input_state.keyboard_previous.keys[key] == FALSE;
}

b8 input_is_button_down(MouseButtons button) {
    if (!initialized) {
        return FALSE;
    }

    return input_state.mouse_current.buttons[button] == TRUE;
}

b8 input_is_button_up(MouseButtons button) {
    if (!initialized) {
        return TRUE;
    }

    return input_state.mouse_current.buttons[button] == FALSE;
}

b8 input_was_button_down(MouseButtons button) {
    if (!initialized) {
        return FALSE;
    }

    return input_state.mouse_previous.buttons[button] == TRUE;
}

b8 input_was_button_up(MouseButtons button) {
    if (!initialized) {
        return TRUE;
    }

    return input_state.mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(i32 *x, i32 *y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = input_state.mouse_current.x;
    *y = input_state.mouse_current.y;
}

void input_get_previous_mouse_position(i32 *x, i32 *y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = input_state.mouse_previous.x;
    *y = input_state.mouse_previous.y;
}
