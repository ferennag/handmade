#include "game_types.h"
#include "core/application.h"
#include "core/logger.h"
#include "platform/platform.h"

typedef struct application_state {
    Game *game_inst;
    b8 is_running;
    b8 is_suspended;
    PlatformState platform;
    i16 width;
    i16 height;
    f64 last_time;
} ApplicationState;

static b8 initialized = FALSE;
static ApplicationState app_state = {0};

b8 application_create(Game *game_inst) {
    if (initialized) {
        HM_ERROR("application_create called more than once!");
        return FALSE;
    }

    HM_INFO("Starting the application...");

    app_state.game_inst = game_inst;

    // Initialize subsystems
    if (!init_logging()) {
        HM_ERROR("Failed to initialize the logging subsystem!");
        return FALSE;
    }

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!platform_init(
            &app_state.platform,
            game_inst->application_config.name,
            game_inst->application_config.start_pos_x,
            game_inst->application_config.start_pos_y,
            game_inst->application_config.start_width,
            game_inst->application_config.start_height)) {
        return FALSE;
    }

    if (!app_state.game_inst->initialize(app_state.game_inst)) {
        HM_FATAL("Failed to initialize the game");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialized = TRUE;
    return TRUE;
}

b8 application_run() {
    while (app_state.is_running) {
        if (platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, 0)) {
                HM_FATAL("Game update failed!");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, 0)) {
                HM_FATAL("Game render failed!");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;
    platform_shutdown(&app_state.platform);
    return TRUE;
}