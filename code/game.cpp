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
    BitMap bmp = BitMap();
    bmp.loadBitMap("img\\car.bmp");

    int w = bmp.getWidth();
    int h = bmp.getHeight();
    DWORD* img = bmp.getBuffer();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int screenX = 100 + x;
            int screenY = 100 + y;
            if (screenX < SCREEN_WIDTH && screenY < SCREEN_HEIGHT) {
                pScreenMem[screenY * SCREEN_WIDTH + screenX] = img[y * w + x];
            }
        }
    }
}

void Game::loop() {
    //MessageBoxA(nullptr, "The image has been loaded and drawn to the buffer", "Game Loop", MB_OK);
}

void Game::exit() {
    MessageBoxA(nullptr, "Game resources have been cleaned up", "Exit", MB_OK);
}

/*
void Game::drawBufferToScreen() {
    // TODO
}
*/