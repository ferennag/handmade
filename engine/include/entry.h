#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/memory.h"
#include "game_types.h"

/// Externally defined function to create a game
extern b8 create_game(Game *out_game);

/**
 * The main entry point of the application.
 */
int main(void) {
    init_memory();

    Game game_instance;
    if (!create_game(&game_instance)) {
        HM_FATAL("Failed to create the game instance!");
        return -1;
    }

    if (!game_instance.render || !game_instance.update || !game_instance.on_resize || !game_instance.initialize) {
        HM_FATAL("The game object's function pointers were not set correctly!");
        return -2;
    }

    if (!application_create(&game_instance)) {
        HM_FATAL("Application failed to create!");
        return -3;
    }

    // Begin the game loop
    if (!application_run()) {
        HM_FATAL("Application encountered an error while running!");
        return -4;
    }

    shutdown_memory();
    return 0;
}
