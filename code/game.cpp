#include "..\head\game.h"

Game::Game() {
    pScreenMem = new DWORD[SCREEN_SIZE];
    memset(pScreenMem, 0, sizeof(DWORD) * SCREEN_SIZE);
}

Game::~Game() {
    if (pScreenMem) {
        delete[] pScreenMem;
        pScreenMem = nullptr;
    }
}

void Game::init() {
    car = new XyBitMap;
    car->loadBitMap("img\\car.bmp");
    car->x = 0;
}

void Game::loop() {
    //MessageBoxA(nullptr, "The image has been loaded and drawn to the buffer", "Game Loop", MB_OK);
    car->x += 1;
    drawXyBitMap(car);
}

void Game::exit() {
    //MessageBoxA(nullptr, "Game resources have been cleaned up", "Exit", MB_OK);
}

/*
void Game::drawBufferToScreen() {
    // TODO
}
*/

void Game::drawXyBitMap(XyBitMap *bmp) {
    int w = bmp->getWidth();
    int h = bmp->getHeight();
    DWORD *img = bmp->getBuffer();

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int screenX = bmp->x + x;
            int screenY = bmp->y + y;
            if (screenX < SCREEN_WIDTH && screenY < SCREEN_HEIGHT) {
                if ((BYTE)(((DWORD_PTR)((img[y * w + x]) >> 24)) & 0xff) == 0) {
                    pScreenMem[screenY * SCREEN_WIDTH + screenX] = img[y * w + x];
                }
            }
        }
    }
}