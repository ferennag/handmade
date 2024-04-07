#include "platform/platform.h"

#ifdef HN_PLATFORM_MACOSX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else

#include <unistd.h>

#endif

#include "core/logger.h"
#include "core/input.h"
#include "core/event.h"

typedef struct internal_state {
    Display *display;
    xcb_window_t window;
} InternalState;

Keys translate_keycode(u32 keycode);

void handle_key_event(XKeyEvent event, b8 pressed);

void handle_button_event(XButtonEvent event, b8 pressed);
void handle_motion_event(XMotionEvent event);

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
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | PointerMotionMask);
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

        switch (event.type) {
            case KeyPress: {
                handle_key_event(event.xkey, TRUE);
                break;
            }
            case KeyRelease: {
                handle_key_event(event.xkey, FALSE);
                break;
            }
            case ButtonPress: {
                handle_button_event(event.xbutton, TRUE);
                break;
            }
            case ButtonRelease: {
                handle_button_event(event.xbutton, FALSE);
                break;
            }
            case MotionNotify: {
                handle_motion_event(event.xmotion);
                break;
            }
        }
    }

    return quit_flagged;
}

void handle_key_event(XKeyEvent event, b8 pressed) {
    char buffer[16];
    KeySym keysym;
    XComposeStatus status;
    XLookupString(&event, buffer, 16, &keysym, &status);

    Keys key = translate_keycode(keysym);
    input_process_key(key, pressed);
}

void handle_button_event(XButtonEvent event, b8 pressed) {
    MouseButtons button = MOUSE_MAX_BUTTONS;

    switch (event.button) {
        case Button1:
            button = MOUSE_BUTTON_LEFT;
            break;
        case Button2:
            button = MOUSE_BUTTON_MIDDLE;
            break;
        case Button3:
            button = MOUSE_BUTTON_RIGHT;
            break;
        case Button4:
            button = MOUSE_BUTTON_4;
            break;
        case Button5:
            button = MOUSE_BUTTON_5;
            break;
    }

    if (button == MOUSE_MAX_BUTTONS) {
        HM_WARN("Unknown mouse button was pressed!");
        return;
    }

    input_process_button(button, pressed);
}

void handle_motion_event(XMotionEvent event) {
    input_process_mouse_move(event.x, event.y);
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

Keys translate_keycode(u32 keycode) {
    switch (keycode) {
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Return:
            return KEY_ENTER;
        case XK_Tab:
            return KEY_TAB;
            //case XK_Shift: return KEY_SHIFT;
            //case XK_Control: return KEY_CONTROL;

        case XK_Pause:
            return KEY_PAUSE;
        case XK_Caps_Lock:
            return KEY_CAPITAL;

        case XK_Escape:
            return KEY_ESCAPE;

            // Not supported
            // case : return KEY_CONVERT;
            // case : return KEY_NONCONVERT;
            // case : return KEY_ACCEPT;

        case XK_Mode_switch:
            return KEY_MODECHANGE;

        case XK_space:
            return KEY_SPACE;
        case XK_Prior:
            return KEY_PRIOR;
        case XK_Next:
            return KEY_NEXT;
        case XK_End:
            return KEY_END;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Execute:
            return KEY_EXECUTE;
            // case XK_snapshot: return KEY_SNAPSHOT; // not supported
        case XK_Insert:
            return KEY_INSERT;
        case XK_Delete:
            return KEY_DELETE;
        case XK_Help:
            return KEY_HELP;

        case XK_Meta_L:
            return KEY_LWIN;  // TODO: not sure this is right
        case XK_Meta_R:
            return KEY_RWIN;
            // case XK_apps: return KEY_APPS; // not supported

            // case XK_sleep: return KEY_SLEEP; //not supported

        case XK_KP_0:
            return KEY_NUMPAD0;
        case XK_KP_1:
            return KEY_NUMPAD1;
        case XK_KP_2:
            return KEY_NUMPAD2;
        case XK_KP_3:
            return KEY_NUMPAD3;
        case XK_KP_4:
            return KEY_NUMPAD4;
        case XK_KP_5:
            return KEY_NUMPAD5;
        case XK_KP_6:
            return KEY_NUMPAD6;
        case XK_KP_7:
            return KEY_NUMPAD7;
        case XK_KP_8:
            return KEY_NUMPAD8;
        case XK_KP_9:
            return KEY_NUMPAD9;
        case XK_multiply:
            return KEY_MULTIPLY;
        case XK_KP_Add:
            return KEY_ADD;
        case XK_KP_Separator:
            return KEY_SEPARATOR;
        case XK_KP_Subtract:
            return KEY_SUBTRACT;
        case XK_KP_Decimal:
            return KEY_DECIMAL;
        case XK_KP_Divide:
            return KEY_DIVIDE;
        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
        case XK_F13:
            return KEY_F13;
        case XK_F14:
            return KEY_F14;
        case XK_F15:
            return KEY_F15;
        case XK_F16:
            return KEY_F16;
        case XK_F17:
            return KEY_F17;
        case XK_F18:
            return KEY_F18;
        case XK_F19:
            return KEY_F19;
        case XK_F20:
            return KEY_F20;
        case XK_F21:
            return KEY_F21;
        case XK_F22:
            return KEY_F22;
        case XK_F23:
            return KEY_F23;
        case XK_F24:
            return KEY_F24;

        case XK_Num_Lock:
            return KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return KEY_SCROLL;

        case XK_KP_Equal:
            return KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return KEY_LSHIFT;
        case XK_Shift_R:
            return KEY_RSHIFT;
        case XK_Control_L:
            return KEY_LCONTROL;
        case XK_Control_R:
            return KEY_RCONTROL;
            // case XK_Menu: return KEY_LMENU;
        case XK_Menu:
            return KEY_RMENU;

        case XK_semicolon:
            return KEY_SEMICOLON;
        case XK_plus:
            return KEY_PLUS;
        case XK_comma:
            return KEY_COMMA;
        case XK_minus:
            return KEY_MINUS;
        case XK_period:
            return KEY_PERIOD;
        case XK_slash:
            return KEY_SLASH;
        case XK_grave:
            return KEY_GRAVE;

        case XK_a:
        case XK_A:
            return KEY_A;
        case XK_b:
        case XK_B:
            return KEY_B;
        case XK_c:
        case XK_C:
            return KEY_C;
        case XK_d:
        case XK_D:
            return KEY_D;
        case XK_e:
        case XK_E:
            return KEY_E;
        case XK_f:
        case XK_F:
            return KEY_F;
        case XK_g:
        case XK_G:
            return KEY_G;
        case XK_h:
        case XK_H:
            return KEY_H;
        case XK_i:
        case XK_I:
            return KEY_I;
        case XK_j:
        case XK_J:
            return KEY_J;
        case XK_k:
        case XK_K:
            return KEY_K;
        case XK_l:
        case XK_L:
            return KEY_L;
        case XK_m:
        case XK_M:
            return KEY_M;
        case XK_n:
        case XK_N:
            return KEY_N;
        case XK_o:
        case XK_O:
            return KEY_O;
        case XK_p:
        case XK_P:
            return KEY_P;
        case XK_q:
        case XK_Q:
            return KEY_Q;
        case XK_r:
        case XK_R:
            return KEY_R;
        case XK_s:
        case XK_S:
            return KEY_S;
        case XK_t:
        case XK_T:
            return KEY_T;
        case XK_u:
        case XK_U:
            return KEY_U;
        case XK_v:
        case XK_V:
            return KEY_V;
        case XK_w:
        case XK_W:
            return KEY_W;
        case XK_x:
        case XK_X:
            return KEY_X;
        case XK_y:
        case XK_Y:
            return KEY_Y;
        case XK_z:
        case XK_Z:
            return KEY_Z;

        default:
            return 0;
    }
}

#endif