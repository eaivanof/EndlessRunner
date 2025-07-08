#pragma once
#include <Windows.h>
#include "BitMap.h"   
#include "xyBitMap.h"
#include <vector>  
#include <string> 

class Game {
public:
    Game();
    ~Game();
    void init();
    void loop();
    void exit();
    void handleInput(WPARAM wParam);
    void drawText(const char*, int, int, COLORREF);
    bool isGameOver() const { return gameOver; } 
    
    
    DWORD* pScreenMem = nullptr;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

private:
    void drawXyBitMap(XyBitMap *XYimg);
    void generateObstacle();
    void moveObstacles();
    bool checkCollision(XyBitMap* car, XyBitMap* obstacle);
    
    XyBitMap* car = nullptr;
    std::vector<XyBitMap*> obstacles;
    int currentLane = 1;
    const int LANE_HEIGHT = 240;
    const int CAR_WIDTH = 80;
    const int CAR_HEIGHT = 120;
    const int OBSTACLE_WIDTH = 60;
    const int OBSTACLE_HEIGHT = 40;
    bool gameOver = false;
    int obstacleTimer = 0;
    const int OBSTACLE_SPAWN_RATE = 60;
    int score = 0;
};
