#include "..\head\game.h"
#include <cstdlib> // rand()
#include <ctime>   // time()
#include <cstring> // memset() функций работы с памятью

int selectCarSkin() {
    const auto& skins = CarSkin::getAllSkins();
    int choice = 0;
    std::cout << "skin:" << std::endl;
    for (size_t i = 0; i < skins.size(); ++i) {
        std::cout << i + 1 << ". " << skins[i].getName() << std::endl;
    }
    std::cin >> choice;
    if (choice < 1 || choice > skins.size()) choice = 1;
    return choice - 1;
}

Game::Game()
{
    pScreenMem = new DWORD[SCREEN_SIZE];
    memset(pScreenMem, 0, sizeof(DWORD) * SCREEN_SIZE);
    keyB = new KeyB();
    menuMap = new XyBitMap();
    menuMap->loadBitMap("img\\menu.bmp");
    menuItemMap = new XyBitMap();
    menuItemMap->loadBitMap("img\\menuItem.bmp");
    menuItemMap->x = 425;
    menuItemMap->y = 270;
    backMap = new XyBitMap();
    backMap->loadBitMap("img\\back.bmp");
    pauseMap = new XyBitMap();
    pauseMap->loadBitMap("img\\pause.bmp");
    escMap = new XyBitMap();
    escMap->loadBitMap("img\\esc.bmp");

    //choose car skin
    selectedSkinIndex = selectCarSkin();
    std::string carImagePath = CarSkin::getAllSkins()[selectedSkinIndex].getImagePath();

    car = new XyBitMap;
    car->loadBitMap(carImagePath.c_str());
    car->x = 100;
    car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;

    collisionSoundPath = "sound\\collision.wav";  // Инициализировать путь к файлу звукового эффекта

}

Game::~Game()
{
    if (pScreenMem)
    {
        delete[] pScreenMem;
        pScreenMem = nullptr;
    }
    if (keyB)
    {
        delete keyB;
        keyB = nullptr;
    }
    if (menuMap)
    {
        delete menuMap;
        menuMap = nullptr;
    }
    if (menuItemMap)
    {
        delete menuItemMap;
        menuItemMap = nullptr;
    }
    if (backMap)
    {
        delete backMap;
        backMap = nullptr;
    }
    if (pauseMap)
    {
        delete pauseMap;
        pauseMap = nullptr;
    }
    if (escMap)
    {
        delete escMap;
        escMap = nullptr;
    }
}

void Game::loop()
{
    if (stage == 0)
    { // menu
        menu();
        return;
    }
    if (stage == 1)
    { // game
        if (keyB->isKeyDown(VK_SPACE))
        {
            keyB->keyUp(VK_SPACE);
            stage = 2;
            return;
        }
        if (keyB->isKeyDown(VK_ESCAPE))
        {
            stage = 3;
            return;
        }
        if (keyB->isKeyDown(VK_UP))
        {
            keyB->keyUp(VK_UP);
            if (currentLane > 0)
            {
                currentLane--;
                car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;
            }
        }
        if (keyB->isKeyDown(VK_DOWN))
        {
            keyB->keyUp(VK_DOWN);
            if (currentLane < 2)
            {
                currentLane++;
                car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;
            }
        }

        logic();
        return;
    }
    if (stage == 2)
    { // pause
        drawXyBitMap(pauseMap);
        if (keyB->isKeyDown(VK_SPACE))
        {
            keyB->keyUp(VK_SPACE);
            stage = 1;
            return;
        }
        return;
    }
    if (stage == 3)
    { // escpause
        exit();
        return;
    }
    if (stage == 4)
    { // gameover
        gameover();
        return;
    }
    if (stage == 5)
    { // 
        drawCarSelectionMenu();
        handleCarSelectionInput();
        return;
    }
}


void Game::menu() {
    drawXyBitMap(menuMap);
    drawXyBitMap(menuItemMap);
    
    if (keyB->isKeyDown(VK_DOWN)) {
        keyB->keyUp(VK_DOWN);
        if (mItem < 2) {  
            mItem++;
            if (mItem == 1) menuItemMap->y = 400;  
            else if (mItem == 2) menuItemMap->y = 534;  
        }
        return;
    }
    if (keyB->isKeyDown(VK_UP)) {
        keyB->keyUp(VK_UP);
        if (mItem > 0) {
            mItem--;
            if (mItem == 0) menuItemMap->y = 270;  
            else if (mItem == 1) menuItemMap->y = 400;  
        }
        return;
    }

    if (keyB->isKeyDown(VK_RETURN)) {
        keyB->keyUp(VK_RETURN);
        if (mItem == 0) {
            stage = 1;  
        } else if (mItem == 1) {
            stage = 5; 
        } else if (mItem == 2) {
            PostQuitMessage(0); 
        }
        return;
    }
}

void Game::logic()
{
    // Отрисовка фона и разделительных линий горизонтальных дорожек
    const int LANE_HEIGHT = SCREEN_HEIGHT / 3;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            if (y < LANE_HEIGHT)
            {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(180, 180, 180);
            }
            else if (y < 2 * LANE_HEIGHT)
            {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(200, 200, 200);
            }
            else
            {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(180, 180, 180);
            }
        }
    }

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int dy = -2; dy <= 2; dy++)
        {
            int drawY = LANE_HEIGHT + dy;
            if (drawY >= 0 && drawY < SCREEN_HEIGHT)
            {
                pScreenMem[drawY * SCREEN_WIDTH + x] = RGB(255, 255, 255);
            }
        }

        for (int dy = -2; dy <= 2; dy++)
        {
            int drawY = 2 * LANE_HEIGHT + dy;
            if (drawY >= 0 && drawY < SCREEN_HEIGHT)
            {
                pScreenMem[drawY * SCREEN_WIDTH + x] = RGB(255, 255, 255);
            }
        }
    }

    // Таймер генерации препятствий
    obstacleTimer++;
    if (obstacleTimer >= OBSTACLE_SPAWN_RATE)
    {
        generateObstacle();
        obstacleTimer = 0;
    }

    drawXyBitMap(car);

    // Перемещение и отрисовка всех препятствий
    moveObstacles();

    // Проверка столкновений
    for (auto obstacle : obstacles)
    {
        if (checkCollision(car, obstacle))
        {
            playCollisionSound();  // Воспроизведение звуковых эффектов при столкновении
            stage = 4;
            return;
        }
    }

    // Отрисовка счета
    char scoreText[20];
    sprintf_s(scoreText, "Score: %d", score);
    // drawText(scoreText, 10, 10, RGB(255, 255, 255));
}

void Game::exit()
{
    if (keyB->isKeyDown(VK_DOWN))
    {
        keyB->keyUp(VK_DOWN);
        if (mItem < 2)
        {
            mItem++;
            if (mItem == 1)
                menuItemMap->y = 400;
            else if (mItem == 2)
                menuItemMap->y = 534;
        }
        return;
    }
    if (keyB->isKeyDown(VK_UP))
    {
        keyB->keyUp(VK_UP);
        if (mItem > 0)
        {
            mItem--;
            if (mItem == 0)
                menuItemMap->y = 270;
            else if (mItem == 1)
                menuItemMap->y = 400;
        }
        return;
    }
    if (keyB->isKeyDown(VK_RETURN))
    {
        if (mItem == 0)
        {
            stage = 1;
        }
        else if (mItem == 1)
        {
            // магазин
        }
        else if (mItem == 2)
        {
            PostQuitMessage(0);
        }
        return;
    }
    drawXyBitMap(escMap);
    drawXyBitMap(menuItemMap);
}

// Генерация препятствия
void Game::generateObstacle()
{
    int lane = rand() % 3;
    XyBitMap *obstacle = new XyBitMap;

    obstacle->x = SCREEN_WIDTH;
    obstacle->y = lane * LANE_HEIGHT + (LANE_HEIGHT - OBSTACLE_HEIGHT) / 2;

    obstacle->setDimensions(OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
    obstacle->allocateBuffer();
    /*
    // Заполнение цвета препятствия (красный)
    DWORD *buffer = obstacle->getBuffer();
    for (int i = 0; i < obstacle->getSize(); i++)
    {
        buffer[i] = 0xFFFF0000;
    }
    */
    obstacle->loadBitMap("img\\heap.bmp");
    obstacles.push_back(obstacle);
}

void Game::gameover()
{
    char scoreText[50];
    sprintf_s(scoreText, "Game over! Score: %d", score);
    MessageBoxA(nullptr, scoreText, "Game over", MB_OK | MB_ICONINFORMATION);
    PostQuitMessage(0);
}

// Функция отрисовки битмапа в буфер экрана
void Game::drawXyBitMap(XyBitMap *bmp)
{
    int w = bmp->getWidth();
    int h = bmp->getHeight();
    DWORD *img = bmp->getBuffer();

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int screenX = bmp->x + x;
            int screenY = bmp->y + y;
            if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT)
            {
                if ((BYTE)(((DWORD_PTR)(img[y * w + x]) >> 24)) & 0xff)
                {
                    pScreenMem[screenY * SCREEN_WIDTH + screenX] = img[y * w + x];
                }
            }
        }
    }
}

// Перемещение и отрисовка всех препятствий
void Game::moveObstacles()
{
    for (auto it = obstacles.begin(); it != obstacles.end();)
    {
        XyBitMap *obstacle = *it;
        obstacle->x -= 7; // Движение препятствия влево (скорость 5 пикселей/кадр)

        drawXyBitMap(obstacle);

        if (obstacle->x + obstacle->getWidth() < 0)
        {
            delete obstacle;
            it = obstacles.erase(it);
            score += 10; // Увеличение счета (успешно обошли препятствие)
        }
        else
        {
            ++it;
        }
    }
}

// Функция проверки столкновений (прямоугольные коллизии)
bool Game::checkCollision(XyBitMap *car, XyBitMap *obstacle)
{
    int carW = car->getWidth();
    int carH = car->getHeight();
    int obsW = obstacle->getWidth();
    int obsH = obstacle->getHeight();

    return car->x < obstacle->x + obsW &&
           car->x + carW > obstacle->x &&
           car->y < obstacle->y + obsH &&
           car->y + carH > obstacle->y;
}

// Функция воспроизведения звуковых эффектов столкновения
void Game::playCollisionSound()
{
    PlaySoundA(collisionSoundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void Game::handleCarSelectionInput() {
    const auto& skins = CarSkin::getAllSkins();
    if (keyB->isKeyDown(VK_UP)) {
        keyB->keyUp(VK_UP);
        if (carSelectIndex > 0) carSelectIndex--;
    }
    if (keyB->isKeyDown(VK_DOWN)) {
        keyB->keyUp(VK_DOWN);
        if (carSelectIndex < skins.size() - 1) carSelectIndex++;
    }
    if (keyB->isKeyDown(VK_RETURN)) {
        keyB->keyUp(VK_RETURN);
        std::string carPath = skins[carSelectIndex].getImagePath();
        car->loadBitMap(carPath.c_str());
        stage = 0;
    }
    if (keyB->isKeyDown(VK_ESCAPE)) {
        keyB->keyUp(VK_ESCAPE);
        stage = 0;
    }
}

void Game::drawCarSelectionMenu() {
    for (int i = 0; i < SCREEN_SIZE; i++) {
        pScreenMem[i] = RGB(128, 128, 128);
    }
    
    const auto& skins = CarSkin::getAllSkins();
    for (size_t i = 0; i < skins.size(); i++) {
        XyBitMap tempCar;
        tempCar.loadBitMap(skins[i].getImagePath().c_str());
        tempCar.x = SCREEN_WIDTH / 2 - tempCar.getWidth() / 2;
        tempCar.y = 200 + i * 150;
        drawXyBitMap(&tempCar);
        
        if (i == carSelectIndex) {
            int x = tempCar.x - 5;
            int y = tempCar.y - 5;
            int w = tempCar.getWidth() + 10;
            int h = tempCar.getHeight() + 10;
            for (int px = x; px < x + w; px++) {
                if (px >=0 && px < SCREEN_WIDTH) {
                    pScreenMem[y * SCREEN_WIDTH + px] = RGB(255, 0, 0); //
                    pScreenMem[(y + h) * SCREEN_WIDTH + px] = RGB(255, 0, 0); // 
                }
            }
            for (int py = y; py < y + h; py++) {
                if (py >=0 && py < SCREEN_HEIGHT) {
                    pScreenMem[py * SCREEN_WIDTH + x] = RGB(255, 0, 0); // 
                    pScreenMem[py * SCREEN_WIDTH + (x + w)] = RGB(255, 0, 0); // 
                }
            }
        }
    }
}
