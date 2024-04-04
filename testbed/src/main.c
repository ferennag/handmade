#include <core/logger.h>
#include <platform/platform.h>

int main() {
    HM_INFO("Starting the application...");

    PlatformState state;
    if (platform_init(&state, "Handmade Hero", 100, 100, 1280, 720)) {
        while(!platform_pump_messages(&state)) {
        }
    }
    platform_shutdown(&state);

    HM_INFO("Shutting down the application... Bye");
    return 0;
}
