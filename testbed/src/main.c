#include <stdlib.h>
#include <core/application.h>

int main() {
    ApplicationConfig config = {
            .start_pos_x = 100,
            .start_pos_y = 100,
            .start_width = 1280,
            .start_height = 760,
            .name = "Handmade Hero"
    };

    if (!application_create(&config)) {
        exit(-1);
    }

    application_run();
    return 0;
}
