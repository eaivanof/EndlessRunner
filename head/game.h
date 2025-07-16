#pragma once
#include "KeyB.h"
#include "XyBitMap.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <mmsystem.h>  // Добавлена система mmsystem.h файл заголовка 
#pragma comment(lib, "winmm.lib")  // Ссылка на библиотеку winmm.lib 

class Game
{
public:
    Game();
    ~Game();
    void loop();

    bool ticker{false};
    bool framer{true};
    unsigned fps{30u};

    DWORD *pScreenMem = nullptr;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

    KeyB *keyB = nullptr;

private:
    int stage = 0; // start with menu
    int mItem = 0;

    void menu();
    void logic();
    void exit();
    void gameover();

    XyBitMap *menuMap = nullptr;
    XyBitMap *menuItemMap = nullptr;
    XyBitMap *backMap = nullptr;
    XyBitMap *pauseMap = nullptr;
    XyBitMap *escMap = nullptr;
    XyBitMap *car = nullptr;
    void drawXyBitMap(XyBitMap *XYimg);

    int currentLane = 1;
    const int LANE_HEIGHT = 240;
    const int CAR_WIDTH = 80;
    const int CAR_HEIGHT = 120;

    void generateObstacle();
    std::vector<XyBitMap *> obstacles;
    const int OBSTACLE_WIDTH = 60;
    const int OBSTACLE_HEIGHT = 40;
    int obstacleTimer = 0;
    const int OBSTACLE_SPAWN_RATE = 60;

    bool gameOver = false;
    int score = 0;

    void moveObstacles();
    bool checkCollision(XyBitMap *car, XyBitMap *obstacle);

    void playCollisionSound();  // Функция воспроизведения звуковых эффектов
    std::string collisionSoundPath;  // Путь к звуковому файлу
};
