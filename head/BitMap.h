#pragma once
#include <Windows.h>

class BitMap {
public:
    void loadBitMap();
protected:
    DWORD* pBitMap = nullptr;
    int width;
    int height;
    int size;
    COLORREF RGBcolor(int r, int g, int b);
};