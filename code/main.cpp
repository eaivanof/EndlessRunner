#define WIN32_LEAN_AND_MEAN
#include "..\head\Game.h"

struct StateInfo {
    bool* ticker{ nullptr };
    Game* pGame{ nullptr };
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    bool ticker = false;
    bool framer = true;
    Game* game = new Game();

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    RegisterClass(&wc);

    StateInfo* pState = new StateInfo;
    pState->ticker = &ticker;
    pState->pGame = game;

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        L"EndlessRunner",
        WS_SYSMENU,
        0, 0,
        game->SCREEN_WIDTH,
        game->SCREEN_HEIGHT,
        nullptr,
        nullptr,
        hInst,
        pState);

    RECT rect;
    GetClientRect(hwnd, &rect);
    int border_width = game->SCREEN_WIDTH - (rect.right - rect.left);
    int border_height = game->SCREEN_HEIGHT - (rect.bottom - rect.top);
    SetWindowPos(hwnd, NULL, 0, 0,
        game->SCREEN_WIDTH + border_width,
        game->SCREEN_HEIGHT + border_height,
        0);
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            if (framer) {
                game->loop();
                framer = false;
            }

            if (ticker) {
                RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
                ticker = false;
                framer = true;
            }
        }
    }

    delete game;
    delete pState;
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    StateInfo* pState = reinterpret_cast<StateInfo*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pState);
        SetTimer(hWnd, 1000, 1000 / 60, NULL);
        return 0;
    }
    if (pState == nullptr) return DefWindowProc(hWnd, msg, wParam, lParam);

    if (msg == WM_KEYDOWN) {
        pState->pGame->keyB->keyDown((unsigned int)wParam);
        return 0;
    }
    if (msg == WM_KEYUP) {
        pState->pGame->keyB->keyUp((unsigned int)wParam);
        return 0;
    }

    if (msg == WM_TIMER) {
        *(pState->ticker) = true;
        return 0;
    }

    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP screen = CreateCompatibleBitmap(hdc, pState->pGame->SCREEN_WIDTH, pState->pGame->SCREEN_HEIGHT);
        SelectObject(hMemDC, screen);

        SetBitmapBits(screen, pState->pGame->SCREEN_SIZE * 4, pState->pGame->pScreenMem);
        BitBlt(hdc, 0, 0, pState->pGame->SCREEN_WIDTH, pState->pGame->SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

        DeleteObject((HGDIOBJ)(HBITMAP)screen);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
