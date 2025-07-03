#pragma once
#include <Windows.h>

class Game {
public:
    Game();
    ~Game();
    void init();
    void loop();
    void exit();
    DWORD* pScreenMem = nullptr;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
};
