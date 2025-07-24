#include "..\head\game.h"
#include <ctime>

Game::Game() {
    srand(time(0));
    // создаём буфер экрана и обработчик клавиатуры
    pScreenMem = new DWORD[SCREEN_SIZE];
    keyB = new KeyB();

    // загрузка изображений
    menuMap = new XyBitMap();
    menuMap->loadBitMap("img\\menu.bmp");
    menuItemMap = new XyBitMap();
    menuItemMap->loadBitMap("img\\menuItem.bmp");
    escMap = new XyBitMap();
    escMap->loadBitMap("img\\esc.bmp");
    backMap = new XyBitMap();
    backMap->loadBitMap("img\\back.bmp");
    pauseMap = new XyBitMap();
    pauseMap->loadBitMap("img\\pause.bmp");
    car1 = new BitMap;
    car1->loadBitMap("img\\car1.bmp");
    car2 = new BitMap;
    car2->loadBitMap("img\\car2.bmp");
    car3 = new BitMap;
    car3->loadBitMap("img\\car3.bmp");
    carItemMap = new BitMap();
    carItemMap->loadBitMap("img\\carItem.bmp");
    redCarItemMap = new BitMap();
    redCarItemMap->loadBitMap("img\\redcarItem.bmp");
    sCar1 = new BitMap;
    sCar1->loadBitMap("img\\Scar1.bmp");
    sCar2 = new BitMap;
    sCar2->loadBitMap("img\\Scar2.bmp");
    sCar3 = new BitMap;
    sCar3->loadBitMap("img\\Scar3.bmp");
    obstacleMap = new BitMap;
    obstacleMap->loadBitMap("img\\heap.bmp");
    obstacleTwoMap = new BitMap;
    obstacleTwoMap->loadBitMap("img\\heap2.bmp");
    keysMap = new XyBitMap;
    keysMap->loadBitMap("img\\keys.bmp");

    // инициализация полей
    menuItemMap->x = MenuItemX;
    menuItemMap->y = MenuItemY[0];

    car.img = car1;
    car.x = carX;
    car.y = carY[1];

    carSelector.img = carItemMap;
    carSelector.x = carSelectorX[0];
    carSelector.y = carSelectorY[0];

    collisionSoundPath = "sound\\collision.wav";
    laneChangeSoundPath = "sound\\lane_change.wav";
}

Game::~Game() {
    if (pScreenMem) {
        delete[] pScreenMem;
        pScreenMem = nullptr;
    }
    if (keyB) {
        delete keyB;
        keyB = nullptr;
    }
    if (menuMap) {
        delete menuMap;
        menuMap = nullptr;
    }
    if (menuItemMap) {
        delete menuItemMap;
        menuItemMap = nullptr;
    }
    if (escMap) {
        delete escMap;
        escMap = nullptr;
    }
    if (backMap) {
        delete backMap;
        backMap = nullptr;
    }
    if (pauseMap) {
        delete pauseMap;
        pauseMap = nullptr;
    }
    if (car1) {
        delete car1;
        car1 = nullptr;
    }
    if (car2) {
        delete car2;
        car2 = nullptr;
    }
    if (car3) {
        delete car3;
        car3 = nullptr;
    }
    if (carItemMap) {
        delete carItemMap;
        carItemMap = nullptr;
    }
    if (redCarItemMap) {
        delete redCarItemMap;
        redCarItemMap = nullptr;
    }
    if (sCar1) {
        delete sCar1;
        sCar1 = nullptr;
    }
    if (sCar2) {
        delete sCar2;
        sCar2 = nullptr;
    }
    if (sCar3) {
        delete sCar3;
        sCar3 = nullptr;
    }
    if (obstacleMap) {
        delete obstacleMap;
        obstacleMap = nullptr;
    }
    if (obstacleTwoMap) {
        delete obstacleTwoMap;
        obstacleTwoMap = nullptr;
    }
    if (keysMap) {
        delete keysMap;
        keysMap = nullptr;
    }
}

void Game::loop() {
    if (stage == Stages::menu) {
        menu();
        return;
    }
    if (stage == Stages::game) {
        go();
        return;
    }
    if (stage == Stages::pause) {
        drawBitMap(pauseMap);
        if (keyB->isKeyDown(VK_SPACE)) {
            keyB->keyUp(VK_SPACE);
            stage = Stages::game;
        }
        return;
    }
    if (stage == Stages::escmenu) {
        exit();
        return;
    }
    if (stage == Stages::gameover) {
        gameover();
        return;
    }
    if (stage == Stages::store) {
        store();
        return;
    }
    if (stage == Stages::keys) {
        drawBitMap(keysMap);
        if (keyB->isKeyDown(VK_ESCAPE)) {
            keyB->keyUp(VK_ESCAPE);
            if (goGame) {
                stage = Stages::escmenu;
            }
            else {
                stage = Stages::menu;
            }
        }
        return;
    }
}

void Game::menu() {
    drawBitMap(menuMap);
    drawBitMap(menuItemMap);

    menuItemChange();

    if (keyB->isKeyDown(VK_RETURN)) {
        keyB->keyUp(VK_RETURN);
        if (mItem == 0) {
            stage = Stages::game;
            carSelector.x = carSelectorX[0];
            goGame = true; // выход из магазина в escmenu
        }
        else if (mItem == 1) {
            stage = Stages::store;
        }
        else if (mItem == 2) {
            stage = Stages::keys;
        }
        else if (mItem == 3) {
            PostQuitMessage(0);
        }
        return;
    }
}

void Game::go() {
    if (keyB->isKeyDown(VK_SPACE)) {
        keyB->keyUp(VK_SPACE);
        stage = Stages::pause;
        return;
    }
    if (keyB->isKeyDown(VK_ESCAPE)) {
        stage = Stages::escmenu;
        return;
    }

    // Перемещения объектов
    carMove();
    obstacleTimer++;
    if (obstacleTimer >= OBSTACLE_SPAWN_RATE) {
        generateObstacle();
        obstacleTimer = 0;
    }
    moveObstacles();

    // Отрисовка
    drawBitMap(backMap);
    drawBitMap(car.img, car.x, car.y);
    for (auto obstacle : obstacles) {
        drawBitMap(obstacle->img, obstacle->x, obstacle->y);
    }

    // Проверки столкновений
    for (auto obstacle : obstacles) {
        if (checkCollision(obstacle)) {
            PlaySoundA(collisionSoundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
            stage = Stages::gameover;
            return;
        }
    }
}

void Game::exit() {
    drawBitMap(escMap);
    drawBitMap(menuItemMap);

    menuItemChange();

    if (keyB->isKeyDown(VK_RETURN)) {
        keyB->keyUp(VK_RETURN);
        if (mItem == 0) {
            stage = Stages::game;
        }
        else if (mItem == 1) {
            stage = Stages::store;
        }
        else if (mItem == 2) {
            stage = Stages::keys;
        }
        else if (mItem == 3) {
            PostQuitMessage(0);
        }
        return;
    }
}

void Game::gameover() {
    char scoreText[50];
    sprintf_s(scoreText, "Game over! Score: %d", score);
    MessageBoxA(nullptr, scoreText, "Game over", MB_OK | MB_ICONINFORMATION);
    score = 0;
    for (int i{}; i < 3;++i) {
        items[i].isBuy = false;
    }
    obstacles.clear();
    menuItemMap->y = MenuItemY[0];
    currentLane = 1;
    car.y = carY[1];
    car.img = car1;
    cItem = 0;
    carSelector.x = carSelectorX[1];
    carSelector.y = carSelectorY[0];
    stage = Stages::menu;
    goGame = false; // выход из магазина в menu
}

void Game::store() {
    if (keyB->isKeyDown(VK_ESCAPE)) {
        keyB->keyUp(VK_ESCAPE);
        if (goGame) {
            stage = Stages::escmenu;
        }
        else {
            stage = Stages::menu;
        }
        return;
    }

    drawBitMap(backMap);
    drawBitMap(car1, carItemX[0], carY[0]);
    drawBitMap(car2, carItemX[0], carY[1]);
    drawBitMap(car3, carItemX[0], carY[2]);
    drawBitMap(sCar1, carItemX[1], carY[0]);
    drawBitMap(sCar2, carItemX[1], carY[1]);
    drawBitMap(sCar3, carItemX[1], carY[2]);
    drawBitMap(carSelector.img, carSelector.x, carSelector.y);

    carItemChange();
    if ((carSelector.x == carSelectorX[1]) &&
        (!items[cItem].isBuy) &&
        (items[cItem].price > score)) {
        carSelector.img = redCarItemMap;
    }
    else {
        carSelector.img = carItemMap;
    }

    if (keyB->isKeyDown(VK_RETURN)) {
        keyB->keyUp(VK_RETURN);
        if (carSelector.x == carSelectorX[0]) {
            if (cItem == 0) {
                car.img = car1;
            }
            else if (cItem == 1) {
                car.img = car2;
            }
            else if (cItem == 2) {
                car.img = car3;
            }
            if (goGame) {
                stage = Stages::escmenu;
            }
            else {
                stage = Stages::menu;
            }
        }
        else {
            if (items[cItem].isBuy) {
                if (cItem == 0) {
                    car.img = sCar1;
                }
                else if (cItem == 1) {
                    car.img = sCar2;
                }
                else if (cItem == 2) {
                    car.img = sCar3;
                }
                if (goGame) {
                    stage = Stages::escmenu;
                }
                else {
                    stage = Stages::menu;
                }
            }
            else if (items[cItem].price <= score) {
                if (cItem == 0) {
                    score -= items[0].price;
                    items[0].isBuy = true;
                }
                else if (cItem == 1) {
                    score -= items[1].price;
                    items[1].isBuy = true;
                }
                else if (cItem == 2) {
                    score -= items[2].price;
                    items[2].isBuy = true;
                }
            }
        }
        return;
    }
}

// выбор пункта меню
void Game::menuItemChange() {
    if (keyB->isKeyDown(VK_DOWN)) {
        keyB->keyUp(VK_DOWN);
        if (mItem < 3) {
            menuItemMap->y = MenuItemY[++mItem];
        }
        return;
    }
    if (keyB->isKeyDown(VK_UP)) {
        keyB->keyUp(VK_UP);
        if (mItem > 0) {
            menuItemMap->y = MenuItemY[--mItem];
        }
        return;
    }
}

// передвижение игрока 
void Game::carMove() {
    if (keyB->isKeyDown(VK_UP)) {
        keyB->keyUp(VK_UP);
        if (currentLane > 0) {
            car.y = carY[--currentLane];
            PlaySoundA(laneChangeSoundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
    if (keyB->isKeyDown(VK_DOWN)) {
        keyB->keyUp(VK_DOWN);
        if (currentLane < 2) {
            car.y = carY[++currentLane];
            PlaySoundA(laneChangeSoundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
        }
    }
}

// выбор изображения машины
void Game::carItemChange() {
    if (keyB->isKeyDown(VK_DOWN)) {
        keyB->keyUp(VK_DOWN);
        if (cItem < 2) {
            carSelector.y = carSelectorY[++cItem];
        }
        return;
    }
    if (keyB->isKeyDown(VK_UP)) {
        keyB->keyUp(VK_UP);
        if (cItem > 0) {
            carSelector.y = carSelectorY[--cItem];
        }
        return;
    }
    if (keyB->isKeyDown(VK_RIGHT)) {
        keyB->keyUp(VK_RIGHT);
        carSelector.x = carSelectorX[1];
        return;
    }
    if (keyB->isKeyDown(VK_LEFT)) {
        keyB->keyUp(VK_LEFT);
        carSelector.x = carSelectorX[0];
        return;
    } 
}

// генерация препятствий
void Game::generateObstacle() {
    BitMapXY* obstacle = new BitMapXY;

    obstacle->x = SCREEN_WIDTH;
    obstacle->y = carY[rand() % 3];

    int tmp_generateObstacle = rand() % 2;
    if (tmp_generateObstacle) {
        obstacle->img = obstacleTwoMap;
    }
    else {
        obstacle->img = obstacleMap;
    }

    obstacles.push_back(obstacle);
}

// перемещение и орисовка препятствий 
void Game::moveObstacles() {
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        BitMapXY* obstacle = *it;
        obstacle->x -= 7;

        if (obstacle->x + obstacleMap->getWidth() < 0) {
            delete obstacle;
            it = obstacles.erase(it);
            score += 10;
        }
        else {
            ++it;
        }
    }
}

// Проверка столкновений
bool Game::checkCollision(BitMapXY* obstacle) {
    int carW = car.img->getWidth();
    int carH = car.img->getHeight();
    int obsW = obstacleMap->getWidth();
    int obsH = obstacleMap->getHeight();

    return car.x < obstacle->x + obsW &&
        car.x + carW > obstacle->x &&
        car.y < obstacle->y + obsH &&
        car.y + carH > obstacle->y;
}

// Функция отрисовки битмапа содержащего координаты в буфер экрана
void Game::drawBitMap(XyBitMap* bmp) {
    int w = bmp->getWidth();
    int h = bmp->getHeight();
    DWORD* img = bmp->getBuffer();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int screenX = bmp->x + x;
            int screenY = bmp->y + y;
            if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
                if ((BYTE)(((DWORD_PTR)(img[y * w + x]) >> 24)) & 0xff) {
                    pScreenMem[screenY * SCREEN_WIDTH + screenX] = img[y * w + x];
                }
            }
        }
    }
}

// Функция отрисовки битмапа c заданием координат в буфер экрана
void Game::drawBitMap(BitMap* bmp, int bmpX, int bmpY) {
    int w = bmp->getWidth();
    int h = bmp->getHeight();
    DWORD* img = bmp->getBuffer();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int screenX = bmpX + x;
            int screenY = bmpY + y;
            if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
                if ((BYTE)(((DWORD_PTR)(img[y * w + x]) >> 24)) & 0xff) {
                    pScreenMem[screenY * SCREEN_WIDTH + screenX] = img[y * w + x];
                }
            }
        }
    }
}
