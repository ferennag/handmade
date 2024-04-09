#include "platform/platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include "core/input.h"
#include "core/logger.h"
#include "renderer/vulkan/vulkan_platform.h"
#include "renderer/vulkan/vulkan_types.h"
#include "containers/darray.h"

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else

#include <unistd.h>

#endif

#ifdef HN_PLATFORM_MACOSX

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>

#endif

typedef struct InternalState {
    SDL_Window *window;
} InternalState;

b8 platform_init(PlatformState *platform_state, const char *app_name, i32 x, i32 y, i32 width, i32 height) {
    InternalState *state = calloc(1, sizeof(InternalState));
    platform_state->internal_state = state;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        HM_FATAL("SDL could not be initialized: %s", SDL_GetError());
        return FALSE;
    }

    if (SDL_Vulkan_LoadLibrary(0) < 0) {
        HM_FATAL("SDL Vulkan could not be initialized: %s", SDL_GetError());
        return FALSE;
    }

    state->window = SDL_CreateWindow(app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                     SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                                     SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    return TRUE;
}

void platform_shutdown(PlatformState *platform_state) {
    InternalState *state = (InternalState *) platform_state->internal_state;
    SDL_DestroyWindow(state->window);
    SDL_Quit();
}

b8 platform_create_vulkan_surface(PlatformState *platform_state, VulkanContext *vulkan_context) {
    InternalState *state = (InternalState *) platform_state->internal_state;

    if (!SDL_Vulkan_CreateSurface(state->window, vulkan_context->instance, &vulkan_context->surface)) {
        HM_FATAL("Could not create vulkan surface: %s", SDL_GetError());
        return FALSE;
    }

    return TRUE;
}

MouseButtons map_mouse_button(SDL_MouseButtonEvent event) {
    switch (event.button) {
        case SDL_BUTTON_LEFT:
            return MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_RIGHT:
            return MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE:
            return MOUSE_BUTTON_MIDDLE;
        case SDL_BUTTON_X1:
            return MOUSE_BUTTON_4;
        case SDL_BUTTON_X2:
            return MOUSE_BUTTON_5;
        default:
            HM_WARN("Unknown mouse button pressed: %d", event.button);
            return 0;
    }
}

Keys map_sdl_key(SDL_KeyboardEvent event) {
    switch (event.keysym.sym) {
        case SDLK_a:
            return KEY_A;
        case SDLK_b:
            return KEY_B;
        case SDLK_c:
            return KEY_C;
        case SDLK_d:
            return KEY_D;
        case SDLK_e:
            return KEY_E;
        case SDLK_f:
            return KEY_F;
        case SDLK_g:
            return KEY_G;
        case SDLK_h:
            return KEY_H;
        case SDLK_i:
            return KEY_I;
        case SDLK_j:
            return KEY_J;
        case SDLK_k:
            return KEY_K;
        case SDLK_l:
            return KEY_L;
        case SDLK_m:
            return KEY_M;
        case SDLK_n:
            return KEY_N;
        case SDLK_o:
            return KEY_O;
        case SDLK_p:
            return KEY_P;
        case SDLK_q:
            return KEY_Q;
        case SDLK_r:
            return KEY_R;
        case SDLK_s:
            return KEY_S;
        case SDLK_t:
            return KEY_T;
        case SDLK_u:
            return KEY_U;
        case SDLK_v:
            return KEY_V;
        case SDLK_w:
            return KEY_W;
        case SDLK_x:
            return KEY_X;
        case SDLK_y:
            return KEY_Y;
        case SDLK_z:
            return KEY_Z;

        case SDLK_BACKSPACE:
            return KEY_BACKSPACE;
        case SDLK_RETURN:
            return KEY_ENTER;
        case SDLK_TAB:
            return KEY_TAB;
            //case XK_Shift: return KEY_SHIFT;
            //case XK_Control: return KEY_CONTROL;

        case SDLK_PAUSE:
            return KEY_PAUSE;
        case SDLK_CAPSLOCK:
            return KEY_CAPITAL;

        case SDLK_ESCAPE:
            return KEY_ESCAPE;

            // Not supported
            // case : return KEY_CONVERT;
            // case : return KEY_NONCONVERT;
            // case : return KEY_ACCEPT;

        case SDLK_MODE:
            return KEY_MODECHANGE;

        case SDLK_SPACE:
            return KEY_SPACE;
        case SDLK_PRIOR:
            return KEY_PRIOR;
        case SDLK_END:
            return KEY_END;
        case SDLK_HOME:
            return KEY_HOME;
        case SDLK_LEFT:
            return KEY_LEFT;
        case SDLK_UP:
            return KEY_UP;
        case SDLK_RIGHT:
            return KEY_RIGHT;
        case SDLK_DOWN:
            return KEY_DOWN;
        case SDLK_SELECT:
            return KEY_SELECT;
        case SDLK_PRINTSCREEN:
            return KEY_PRINT;
        case SDLK_EXECUTE:
            return KEY_EXECUTE;
            // case XK_snapshot: return KEY_SNAPSHOT; // not supported
        case SDLK_INSERT:
            return KEY_INSERT;
        case SDLK_DELETE:
            return KEY_DELETE;
        case SDLK_HELP:
            return KEY_HELP;

        case SDLK_0:
            return KEY_NUMPAD0;
        case SDLK_1:
            return KEY_NUMPAD1;
        case SDLK_2:
            return KEY_NUMPAD2;
        case SDLK_3:
            return KEY_NUMPAD3;
        case SDLK_4:
            return KEY_NUMPAD4;
        case SDLK_5:
            return KEY_NUMPAD5;
        case SDLK_6:
            return KEY_NUMPAD6;
        case SDLK_7:
            return KEY_NUMPAD7;
        case SDLK_8:
            return KEY_NUMPAD8;
        case SDLK_9:
            return KEY_NUMPAD9;
        case SDLK_KP_MULTIPLY:
            return KEY_MULTIPLY;
        case SDLK_SEPARATOR:
            return KEY_SEPARATOR;
        case SDLK_KP_MINUS:
            return KEY_SUBTRACT;
        case SDLK_KP_DECIMAL:
            return KEY_DECIMAL;
        case SDLK_KP_DIVIDE:
            return KEY_DIVIDE;
        case SDLK_F1:
            return KEY_F1;
        case SDLK_F2:
            return KEY_F2;
        case SDLK_F3:
            return KEY_F3;
        case SDLK_F4:
            return KEY_F4;
        case SDLK_F5:
            return KEY_F5;
        case SDLK_F6:
            return KEY_F6;
        case SDLK_F7:
            return KEY_F7;
        case SDLK_F8:
            return KEY_F8;
        case SDLK_F9:
            return KEY_F9;
        case SDLK_F10:
            return KEY_F10;
        case SDLK_F11:
            return KEY_F11;
        case SDLK_F12:
            return KEY_F12;
        case SDLK_F13:
            return KEY_F13;
        case SDLK_F14:
            return KEY_F14;
        case SDLK_F15:
            return KEY_F15;
        case SDLK_F16:
            return KEY_F16;
        case SDLK_F17:
            return KEY_F17;
        case SDLK_F18:
            return KEY_F18;
        case SDLK_F19:
            return KEY_F19;
        case SDLK_F20:
            return KEY_F20;
        case SDLK_F21:
            return KEY_F21;
        case SDLK_F22:
            return KEY_F22;
        case SDLK_F23:
            return KEY_F23;
        case SDLK_F24:
            return KEY_F24;

        case SDLK_NUMLOCKCLEAR:
            return KEY_NUMLOCK;
        case SDLK_SCROLLLOCK:
            return KEY_SCROLL;

        case SDLK_KP_EQUALS:
            return KEY_NUMPAD_EQUAL;

        case SDLK_MENU:
            return KEY_RMENU;

        case SDLK_SEMICOLON:
            return KEY_SEMICOLON;
        case SDLK_PLUS:
            return KEY_PLUS;
        case SDLK_COMMA:
            return KEY_COMMA;
        case SDLK_MINUS:
            return KEY_MINUS;
        case SDLK_PERIOD:
            return KEY_PERIOD;
        case SDLK_SLASH:
            return KEY_SLASH;
        default:
            HM_WARN("Unknown keyboard event: %d", event.keysym.sym);
            return 0;
    }
}

b8 platform_pump_messages(PlatformState *platform_state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return TRUE;
            case SDL_MOUSEBUTTONDOWN:
                input_process_button(map_mouse_button(event.button), TRUE);
                break;
            case SDL_MOUSEBUTTONUP:
                input_process_button(map_mouse_button(event.button), FALSE);
                break;
            case SDL_MOUSEMOTION:
                input_process_mouse_move(event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                input_process_mouse_wheel(event.wheel.y);
                break;
            case SDL_KEYUP:
                input_process_key(map_sdl_key(event.key), FALSE);
                break;
            case SDL_KEYDOWN:
                input_process_key(map_sdl_key(event.key), TRUE);
                break;
        }
    }
    return FALSE;
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

void platform_get_required_extension_names(const char ***out_extensions) {
    darray_push(*out_extensions, &"VK_EXT_metal_surface");
}