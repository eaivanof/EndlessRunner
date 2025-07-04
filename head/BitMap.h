#pragma once
#include <Windows.h>
#include <string>

class BitMap {
public:
    BitMap();
    ~BitMap();
    void loadBitMap(const char* filename);
    DWORD* getBuffer() const;
    int getWidth() const;
    int getHeight() const;

protected:
    DWORD* pBitMap = nullptr;
    int width = 0;
    int height = 0;
    int size = 0;

    COLORREF RGBcolor(int r, int g, int b);
};
