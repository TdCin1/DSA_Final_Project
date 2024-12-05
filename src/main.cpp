#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Game.h"
#include "TUI.h"

int main(int argc, char** argv) {
    if(argc > 1 && strcmp(argv[1], "--tui") == 0) {
        TUI::run();
    } else {
        Game *game = new Game();
        game->init("Display", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1200, false);
        while(game->running()) {
            game->handleEvents();
            game->update();
            game->render();
        }
    }

    return 0;
}
