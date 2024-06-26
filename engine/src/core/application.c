#include "game_types.h"
#include "core/application.h"
#include "core/logger.h"
#include "platform/platform.h"
#include "core/memory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"
#include "renderer/renderer_frontend.h"

typedef struct application_state {
    Game *game_inst;
    b8 is_running;
    b8 is_suspended;
    PlatformState platform;
    i16 width;
    i16 height;
    Clock clock;
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

    if (!event_init()) {
        HM_ERROR("Failed to initialize the event subsystem!");
        return FALSE;
    }

    if (!input_init()) {
        HM_ERROR("Failed to initialize the input subsystem!");
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

    if (!renderer_init(app_state.game_inst->application_config.name, &app_state.platform)) {
        HM_ERROR("Failed to initialize the renderer subsystem!");
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
    f64 target_fps = 1.0f / 60.0f;
    clock_start(&app_state.clock, target_fps);
    clock_update(&app_state.clock);

    HM_INFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            clock_update(&app_state.clock);
            clock_start_frame(&app_state.clock);
            f64 delta = clock_delta_time(&app_state.clock);

            if (!app_state.game_inst->update(app_state.game_inst, delta)) {
                HM_FATAL("Game update failed!");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, delta)) {
                HM_FATAL("Game render failed!");
                app_state.is_running = FALSE;
                break;
            }

            RenderPacket packet = {.delta_time = delta};
            renderer_draw_frame(&packet);

            clock_end_frame(&app_state.clock);

            // Input is checked at the end of the frame, and used in the next frame
            // TODO pass delta time
            input_update(delta);
        }
    }

    app_state.is_running = FALSE;
    clock_stop(&app_state.clock);

    input_shutdown();
    event_shutdown();
    shutdown_logging();

    renderer_shutdown();

    platform_shutdown(&app_state.platform);
    return TRUE;
}