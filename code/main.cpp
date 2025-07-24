#define WIN32_LEAN_AND_MEAN
#include "..\head\Game.h"
#include <Windows.h>
#include <string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
const int BPP{ 32 }; // глубина цвета
enum class ERRORS { window = 1, display, change }; // коды ошибок

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    Game game;

    // создание окна
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        L"EndlessRunner",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        game.SCREEN_WIDTH, game.SCREEN_HEIGHT,
        nullptr,
        nullptr,
        hInst,
        &game);

    if (hwnd == NULL) {
        MessageBox(hwnd, L"Окно не создано", L"Ошибка", MB_OK);
        return static_cast<int>(ERRORS::window);
    }

    // проверка поддержки монитором необходимого режима
    bool monitor{ false };
    DEVMODE dm {};
    int i{};
    while (EnumDisplaySettings(NULL, i, &dm)) {
        dm.dmPelsWidth;
        dm.dmPelsHeight;
        dm.dmBitsPerPel;
        i++;
        if (dm.dmPelsWidth == game.SCREEN_WIDTH && dm.dmPelsHeight == game.SCREEN_HEIGHT && dm.dmBitsPerPel == BPP) {
            monitor = true;
        }
    }
    if (!monitor) {
        MessageBox(hwnd, L"Экран не поддерживает необходимый режим", L"Ошибка", MB_OK);
        return static_cast<int>(ERRORS::display);
    }

    // переходим в полноэкранный режим
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = game.SCREEN_WIDTH;
    dm.dmPelsHeight = game.SCREEN_HEIGHT;
    dm.dmBitsPerPel = BPP;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    LONG result = ChangeDisplaySettings(&dm, 0);
    if (result != DISP_CHANGE_SUCCESSFUL) {
        MessageBox(hwnd, L"Параметры экрана не были изменены", L"Ошибка", MB_OK);
        return static_cast<int>(ERRORS::change);
    }
    ShowWindow(hwnd, nCmdShow);
    ShowCursor(false);

    // Игровой цикл
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            // Построение игрового кадра
            if (game.framer) {
                game.loop();
                game.framer = false;
            }

            // Отрисовка кадра
            if (game.ticker) {
                RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                game.ticker = false;
                game.framer = true;
            }
        }
    }
    ChangeDisplaySettings(NULL, 0);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    Game* pGame = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pGame = reinterpret_cast<Game*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pGame);
        SetTimer(hWnd, 1000, 1000 / pGame->fps, NULL);
        return 0;
    }
    if (pGame == nullptr)
        return DefWindowProc(hWnd, msg, wParam, lParam);

    if (msg == WM_KEYDOWN) {
        pGame->keyB->keyDown((unsigned int)wParam);
        return 0;
    }
    if (msg == WM_KEYUP) {
        pGame->keyB->keyUp((unsigned int)wParam);
        return 0;
    }

    // таймер для отрисовки кадров
    if (msg == WM_TIMER) {
        pGame->ticker = true;
        return 0;
    }

    // отрисовка кадра
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP screen = CreateCompatibleBitmap(hdc, pGame->SCREEN_WIDTH, pGame->SCREEN_HEIGHT);
        SelectObject(hMemDC, screen);

        SetBitmapBits(screen, pGame->SCREEN_SIZE * sizeof(DWORD), pGame->pScreenMem);
        BitBlt(hdc, 0, 0, pGame->SCREEN_WIDTH, pGame->SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

        // отрисовка игрового счёта
        if (pGame->stage != Game::Stages::menu && pGame->stage != Game::Stages::keys) {
            std::wstring ss = std::to_wstring(pGame->score);
            RECT rect = { 10, 10, 200, 50 };
            DrawTextW(hdc, (L"Score: " + ss).c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
        }
        // отрисовка цены
        if (pGame->stage == Game::Stages::store) {
            std::wstring ss1 = std::to_wstring(pGame->items[0].price);
            std::wstring ss2 = std::to_wstring(pGame->items[1].price);
            std::wstring ss3 = std::to_wstring(pGame->items[2].price);
            if (!pGame->items[0].isBuy) {
                RECT rect = { 600, 200, 800, 240 };
                DrawTextW(hdc, (L"Price: " + ss1).c_str(), -1, &rect, DT_SINGLELINE);
            }
            if (!pGame->items[1].isBuy) {
                RECT rect = { 600, 440, 800, 480 };
                DrawTextW(hdc, (L"Price: " + ss2).c_str(), -1, &rect, DT_SINGLELINE);
            }
            if (!pGame->items[2].isBuy) {
                RECT rect = { 600, 680, 800, 720 };
                DrawTextW(hdc, (L"Price: " + ss3).c_str(), -1, &rect, DT_SINGLELINE);
            }
        }
        DeleteObject((HGDIOBJ)(HBITMAP)screen);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
