#pragma once
#include "KeyB.h"
#include <Windows.h>
#include "XyBitMap.h"
#include <vector>
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")
#include <string>

class Game {
public:
    enum class Stages { menu, game, pause, escmenu, gameover, store, keys };
    const int SCREEN_WIDTH{ 1280 };
    const int SCREEN_HEIGHT{ 720 };
    const int SCREEN_SIZE{ SCREEN_WIDTH * SCREEN_HEIGHT };
    DWORD* pScreenMem{ nullptr };

    bool framer{ true };
    void loop();
    bool ticker{ false };
    unsigned fps{ 30u };
    KeyB* keyB{ nullptr };
    int score{0};
    Stages stage{ Stages::menu };

    Game();
    ~Game();

    struct Item {
        int price{};
        bool isBuy{ false };
    };

    Item items[3]{ {10, false},{50, false},{100, false} };
private:
    // ������� ���������
    void menu();
    void go();
    void exit();
    void gameover();
    void store();

    void drawBitMap(XyBitMap* bmp);
    void drawBitMap(BitMap* bmp, int bmpX, int bmpY);
    void menuItemChange();
    void carItemChange();

    int mItem{};
    int MenuItemY[4]{ 214, 333, 453, 573 };
    const int MenuItemX{ 425 };

    // �����������
    XyBitMap* menuMap{ nullptr };
    XyBitMap* menuItemMap{ nullptr };
    XyBitMap* escMap{ nullptr };
    XyBitMap* backMap{ nullptr };
    XyBitMap* pauseMap{ nullptr };
    BitMap* carItemMap{ nullptr };
    BitMap* redCarItemMap{ nullptr };
    BitMap* obstacleMap{ nullptr };
    BitMap* obstacleTwoMap{ nullptr };
    XyBitMap* keysMap{ nullptr };
    // ����������� ������
    BitMap* car1{ nullptr };
    BitMap* car2{ nullptr };
    BitMap* car3{ nullptr };
    BitMap* sCar1{ nullptr };
    BitMap* sCar2{ nullptr };
    BitMap* sCar3{ nullptr };

    struct BitMapXY {
        BitMap* img{ nullptr };
        int x{};
        int y{};
    };
    BitMapXY car;

    void carMove();
    const int carX{ 100 };
    int carY[3]{ 70, 310, 550 };
    int currentLane{ 1 };

    BitMapXY carSelector;
    int cItem{ 0 };
    int carItemX[2]{ SCREEN_WIDTH / 2 - 500, SCREEN_WIDTH / 2 - 100 };
    int carSelectorX[2]{ carItemX[0] - 20, carItemX[1] - 20 };
    int carSelectorY[3]{ carY[0] - 20, carY[1] - 20, carY[2] - 20 };
    bool goGame{ false }; // ������������ ��� ������ � ���� �� ��������

    int obstacleTimer{};
    int OBSTACLE_SPAWN_RATE{65};
    std::vector<BitMapXY*> obstacles;
    void generateObstacle();
    void moveObstacles();
    bool checkCollision(BitMapXY* obstacle);

    std::string collisionSoundPath;
    std::string laneChangeSoundPath;
    
};
