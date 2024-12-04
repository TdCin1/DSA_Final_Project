#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "TextureManager.h"

Game *game = nullptr;

using namespace std;

int main() {



    game = new Game();
    game->init("Display",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,1200,false);


    while(game->running()) {

        game->handleEvents();
        game->update();
        game->render();
    }



    return 0;
}

