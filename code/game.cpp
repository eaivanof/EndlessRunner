#include "..\head\game.h"

Game::Game() {
    pScreenMem = new DWORD[SCREEN_SIZE]; 
}

Game::~Game() {
    delete[] pScreenMem;
    pScreenMem = nullptr;
}

void Game::init() {

} 

void Game::loop() {

}

void Game::exit() {

}
