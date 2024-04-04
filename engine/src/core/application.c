#include "core/application.h"
#include "core/logger.h"
#include "platform/platform.h"

typedef struct application_state {
    b8 is_running;
    b8 is_suspended;
    PlatformState platform;
    i16 width;
    i16 height;
    f64 last_time;
} ApplicationState;

static b8 initialized = FALSE;
static ApplicationState app_state = {0};

b8 application_create(ApplicationConfig *config) {
    if (initialized) {
        HM_ERROR("application_create called more than once!");
        return FALSE;
    }

    HM_INFO("Starting the application...");

    // Initialize subsystems
    if (!init_logging()) {
        HM_ERROR("Failed to initialize the logging subsystem!");
        return FALSE;
    }

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!platform_init(&app_state.platform, config->name, config->start_pos_x, config->start_pos_y, config->start_width, config->start_height)) {
        HM_ERROR("Failed to initialize the platform!");
        return FALSE;
    }

    initialized = TRUE;
    return TRUE;
}

b8 application_run() {
    while(app_state.is_running) {
        if (platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }
    }

    app_state.is_running = FALSE;
    platform_shutdown(&app_state.platform);
    return TRUE;
}