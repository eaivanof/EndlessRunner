#define WIN32_LEAN_AND_MEAN
#include "..\head\Game.h"

/*
struct StateInfo {
    bool* ticker{ nullptr };
    Game* pGame{ nullptr };
};
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

    if (hwnd == NULL)
    {
        MessageBox(hwnd, L"Окно не создано", L"Ошибка", MB_OK);
        return 1;
    }

    // проверка поддержки монитором необходимого режима
    bool monitor = false;
    DEVMODE dm = {};
    int i{};
    while (EnumDisplaySettings(NULL, i, &dm))
    {
        dm.dmPelsWidth;
        dm.dmPelsHeight;
        dm.dmBitsPerPel;
        i++;
        if (dm.dmPelsWidth == game.SCREEN_WIDTH && dm.dmPelsHeight == game.SCREEN_HEIGHT && dm.dmBitsPerPel == 32)
        {
            monitor = true;
        }
    }
    if (!monitor)
    {
        MessageBox(hwnd, L"Экран не поддерживает необходимый режим", L"Ошибка", MB_OK);
        return 1;
    }

    // переходим в полноэкранный режим
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = game.SCREEN_WIDTH;
    dm.dmPelsHeight = game.SCREEN_HEIGHT;
    dm.dmBitsPerPel = 32;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    LONG result = ChangeDisplaySettings(&dm, 0);
    if (result != DISP_CHANGE_SUCCESSFUL)
    {
        MessageBox(hwnd, L"Параметры экрана не были изменены", L"Ошибка", MB_OK);
        return 1;
    }
    ShowWindow(hwnd, nCmdShow);
    ShowCursor(false);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            if (game.framer) {
                game.loop();
                game.framer = false;
            }

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

    Game *pGame = reinterpret_cast<Game *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_CREATE)
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pGame = reinterpret_cast<Game *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pGame);
        SetTimer(hWnd, 1000, 1000 / pGame->fps, NULL);
        return 0;
    }
    if (pGame == nullptr)
        return DefWindowProc(hWnd, msg, wParam, lParam);

    if (msg == WM_KEYDOWN)
    {
        pGame->keyB->keyDown((unsigned int)wParam);
        return 0;
    }
    if (msg == WM_KEYUP)
    {
        pGame->keyB->keyUp((unsigned int)wParam);
        return 0;
    }

    if (msg == WM_TIMER)
    {
        pGame->ticker = true;
        return 0;
    }

    if (msg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP screen = CreateCompatibleBitmap(hdc, pGame->SCREEN_WIDTH, pGame->SCREEN_HEIGHT);
        SelectObject(hMemDC, screen);

        SetBitmapBits(screen, pGame->SCREEN_SIZE * sizeof(DWORD), pGame->pScreenMem);
        BitBlt(hdc, 0, 0, pGame->SCREEN_WIDTH, pGame->SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

        if (pGame->stage != 0) {
            std::wstring ss = std::to_wstring(pGame->score);
            RECT rect = {10, 10, 200, 50};
            DrawText(hdc, (L"Score: " + ss).c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
        }
        DeleteObject((HGDIOBJ)(HBITMAP)screen);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);


        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
