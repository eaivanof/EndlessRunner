#include "../head/BitMap.h"
#include <cstdio>

BitMap::BitMap() : pBitMap(nullptr), width(0), height(0), size(0) {}

BitMap::~BitMap()
{
    if (pBitMap)
    {
        delete[] pBitMap;
    }
}

void BitMap::setDimensions(int w, int h)
{
    width = w;
    height = h;
    size = width * height;
}

void BitMap::allocateBuffer()
{
    if (pBitMap)
    {
        delete[] pBitMap;
    }
    pBitMap = new DWORD[size];
    memset(pBitMap, 0, size * sizeof(DWORD));
}

void BitMap::loadBitMap(const char *filename)
{
    FILE *file = nullptr;
    fopen_s(&file, filename, "rb");
    if (!file)
        return;

    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    if (fileHeader.bfType != 0x4D42)
    { // 'BM'
        fclose(file);
        return;
    }

    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    if (infoHeader.biBitCount != 32)
    {
        fclose(file);
        return;
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    size = width * height;

    if (pBitMap)
        delete[] pBitMap;
    pBitMap = new DWORD[size];

    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    BYTE pixel[4];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            fread(pixel, 1, 4, file); // BGRA
            int index = (height - 1 - y) * width + x;
            pBitMap[index] = RGBcolor(pixel[3], pixel[2], pixel[1], pixel[0]);
        }
    }

    fclose(file);
}

void BitMap::drawXyBitMap(HDC hdc, int x, int y)
{
    if (!pBitMap)
        return;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            DWORD color = pBitMap[i * width + j];
            BYTE a = (color >> 24) & 0xFF;
            BYTE r = (color >> 16) & 0xFF;
            BYTE g = (color >> 8) & 0xFF;
            BYTE b = color & 0xFF;

            if (a > 0)
            {
                SetPixel(hdc, x + j, y + i, RGB(r, g, b));
            }
        }
    }
}
