#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\head\game.h"

int main() {
    Game* game = new Game(); 
    game->init();
    // loop будет выполняться в цикле
    game->loop();
    //
    game->exit();
    return 0;
}