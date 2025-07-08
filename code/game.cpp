#include "..\head\game.h"
#include <cstdlib>         // rand()
#include <ctime>           // time()
#include <cstring>         // memset() функций работы с памятью

// Конструктор
Game::Game() {
    pScreenMem = new DWORD[SCREEN_SIZE];
    memset(pScreenMem, 0, sizeof(DWORD) * SCREEN_SIZE);
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Деструктор
Game::~Game() {
    if (pScreenMem) {
        delete[] pScreenMem;
        pScreenMem = nullptr;
    }
    
    if (car) {
        delete car;
        car = nullptr;
    }
    
    for (auto obstacle : obstacles) {
        delete obstacle;
    }
    obstacles.clear();
}

//  Инициализация игрового состояния
void Game::init() {
    car = new XyBitMap;
    car->loadBitMap("img\\car.bmp");
    car->x = 100;
    car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;
    
    gameOver = false;
    score = 0;
    obstacleTimer = 0;
    currentLane = 1;
    obstacles.clear();
}

// Главный цикл игры
void Game::loop() {
    if (gameOver) return;

    // Отрисовка фона и разделительных линий горизонтальных дорожек
    const int LANE_HEIGHT = SCREEN_HEIGHT / 3;
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (y < LANE_HEIGHT) {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(180, 180, 180);
            } else if (y < 2 * LANE_HEIGHT) {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(200, 200, 200);
            } else {
                pScreenMem[y * SCREEN_WIDTH + x] = RGB(180, 180, 180);
            }
        }
    }

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int dy = -2; dy <= 2; dy++) {
            int drawY = LANE_HEIGHT + dy;
            if (drawY >= 0 && drawY < SCREEN_HEIGHT) {
                pScreenMem[drawY * SCREEN_WIDTH + x] = RGB(255, 255, 255);
            }
        }

        for (int dy = -2; dy <= 2; dy++) {
            int drawY = 2 * LANE_HEIGHT + dy;
            if (drawY >= 0 && drawY < SCREEN_HEIGHT) {
                pScreenMem[drawY * SCREEN_WIDTH + x] = RGB(255, 255, 255);
            }
        }
    }
    
    // Таймер генерации препятствий
    obstacleTimer++;
    if (obstacleTimer >= OBSTACLE_SPAWN_RATE) {
        generateObstacle();
        obstacleTimer = 0;
    }
    
    // Перемещение и отрисовка всех препятствий
    moveObstacles();
    
    // Проверка столкновений
    for (auto obstacle : obstacles) {
        if (checkCollision(car, obstacle)) {
            gameOver = true;
            char scoreText[50];
            sprintf_s(scoreText, "Game over! Score: %d", score);
            MessageBoxA(nullptr, scoreText, "Game over", MB_OK | MB_ICONINFORMATION);
            return;
        }
    }
    
    // Отрисовка игрока и счета
    drawXyBitMap(car);
    char scoreText[20];
    sprintf_s(scoreText, "Score: %d", score);
    drawText(scoreText, 10, 10, RGB(255, 255, 255));
}

// Функция очистки при выходе из игры
void Game::exit() {
    // Освобождение ресурсов (осуществляется в деструкторе)
}

// Обработка клавиатурного ввода
void Game::handleInput(WPARAM wParam) {
    if (gameOver) return;
    
    // Обработка клавиш направления
    switch (wParam) {
        case VK_UP:
            if (currentLane > 0) {
                currentLane--;
                car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;
            }
            break;
        case VK_DOWN:
            if (currentLane < 2) {
                currentLane++;
                car->y = currentLane * LANE_HEIGHT + (LANE_HEIGHT - CAR_HEIGHT) / 2;
            }
            break;
    }
}

// Функция отрисовки битмапа в буфер экрана
void Game::drawXyBitMap(XyBitMap *bmp) {
    int w = bmp->getWidth();
    int h = bmp->getHeight();
    DWORD *img = bmp->getBuffer();

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

// Генерация препятствия
void Game::generateObstacle() {
    int lane = rand() % 3;
    XyBitMap* obstacle = new XyBitMap;
    
    obstacle->x = SCREEN_WIDTH;
    obstacle->y = lane * LANE_HEIGHT + (LANE_HEIGHT - OBSTACLE_HEIGHT) / 2;
    
    obstacle->setDimensions(OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
    obstacle->allocateBuffer();
    
    // Заполнение цвета препятствия (красный)
    DWORD* buffer = obstacle->getBuffer();
    for (int i = 0; i < obstacle->getSize(); i++) {
        buffer[i] = 0xFFFF0000;
    }
    
    obstacles.push_back(obstacle);
}

// Перемещение и отрисовка всех препятствий
void Game::moveObstacles() {
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        XyBitMap* obstacle = *it;
        obstacle->x -= 5;  // Движение препятствия влево (скорость 5 пикселей/кадр)
        
        drawXyBitMap(obstacle);
        
        if (obstacle->x + obstacle->getWidth() < 0) {
            delete obstacle;
            it = obstacles.erase(it);
            score += 10;  // Увеличение счета (успешно обошли препятствие)
        } else {
            ++it;
        }
    }
}

// Функция проверки столкновений (прямоугольные коллизии)
bool Game::checkCollision(XyBitMap* car, XyBitMap* obstacle) {
    int carW = car->getWidth();
    int carH = car->getHeight();
    int obsW = obstacle->getWidth();
    int obsH = obstacle->getHeight();
    
    return car->x < obstacle->x + obsW &&
           car->x + carW > obstacle->x &&
           car->y < obstacle->y + obsH &&
           car->y + carH > obstacle->y;
}

// Простая функция отрисовки текста (рисование символов точками)
void Game::drawText(const char* text, int x, int y, COLORREF color) {
    int textLen = strlen(text);
    
    for (int i = 0; i < textLen; i++) {
        for (int cy = 0; cy < 10; cy++) {
            for (int cx = 0; cx < 6; cx++) {
                if (cx + cy > 3 && cx < 5 && cy < 8) {
                    int screenX = x + i * 8 + cx;
                    int screenY = y + cy;
                    if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
                        pScreenMem[screenY * SCREEN_WIDTH + screenX] = color;
                    }
                }
            }
        }
    }
}
