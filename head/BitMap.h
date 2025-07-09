#pragma once
#include <Windows.h>
#include <string>

class BitMap
{
public:
    BitMap();
    ~BitMap();

    void loadBitMap(const char *filename);
    void drawXyBitMap(HDC hdc, int x, int y);
    void setDimensions(int w, int h);
    void allocateBuffer();

    DWORD *getBuffer() const { return pBitMap; }
    int getSize() const { return size; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    DWORD *pBitMap;
    int width;
    int height;
    int size;

    inline DWORD RGBcolor(BYTE a, BYTE r, BYTE g, BYTE b)
    {
        return ((DWORD)a << 24) | ((DWORD)r << 16) | ((DWORD)g << 8) | b;
    }
};
