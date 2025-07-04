#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\head\game.h"

int main() {
    Game* game = new Game(); 
    game->init();
    game->loop();
    game->exit();
    delete game;
    return 0;
}
