#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\head\game.h"

struct StateInfo {
    DWORD *pScreenMem{nullptr};
    bool *ticker{nullptr};
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int SCREEN_SIZE;
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
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);
    StateInfo *pState = new StateInfo;
    pState->pScreenMem = game->pScreenMem;
    pState->ticker = &ticker;
    pState->SCREEN_WIDTH = game->SCREEN_WIDTH;
    pState->SCREEN_HEIGHT = game->SCREEN_HEIGHT;
    pState->SCREEN_SIZE = game->SCREEN_SIZE;
    HWND hwnd = CreateWindow(L"myWindowClass", L"EndlessRunner", WS_SYSMENU,
                             0, 0, game->SCREEN_WIDTH, game->SCREEN_HEIGHT, nullptr, nullptr, hInst, pState);
    RECT rect;
    GetClientRect(hwnd, &rect);
    int border_width = game->SCREEN_WIDTH - (rect.right - rect.left);
    int border_height = game->SCREEN_HEIGHT - (rect.bottom - rect.top);
    SetWindowPos(hwnd, NULL, rect.left, rect.top, game->SCREEN_WIDTH + border_width, game->SCREEN_HEIGHT + border_height, SWP_NOMOVE | SWP_NOZORDER);
    ShowWindow(hwnd, nCmdShow);

    game->init();
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
    game->exit();
    delete game;
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    if (msg == WM_KEYDOWN && (unsigned int)wParam == VK_ESCAPE) {
        PostQuitMessage(0);
        return 0;
    }
    StateInfo *pState;
    if (msg == WM_CREATE) {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pState = reinterpret_cast<StateInfo *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pState);
        SetTimer(hWnd, 1000, 1000 / 60, NULL);
        return 0;
    }
    pState = reinterpret_cast<StateInfo *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_TIMER) {
        *(pState->ticker) = true;
        return 0;
    }
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP screen = CreateCompatibleBitmap(hdc, pState->SCREEN_WIDTH, pState->SCREEN_HEIGHT);
        SelectObject(hMemDC, screen);
        SetBitmapBits(screen, pState->SCREEN_SIZE * 4, pState->pScreenMem);
        BitBlt(hdc, 0, 0, pState->SCREEN_WIDTH, pState->SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);
        DeleteObject((HGDIOBJ)(HBITMAP)screen);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
