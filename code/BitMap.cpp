#include "..\head\BitMap.h"

void BitMap::loadBitMap() {

}

COLORREF BitMap::RGBcolor(int r, int g, int b) {
    return (COLORREF)(((BYTE)(b)) | ((WORD)((BYTE)(g)) << 8) | (((DWORD)(BYTE)(r) << 16)));
}