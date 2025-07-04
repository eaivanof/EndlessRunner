#include "..\head\BitMap.h"
#include <cstdio>

BitMap::BitMap() {}

BitMap::~BitMap() {
    if (pBitMap) {
        delete[] pBitMap;
        pBitMap = nullptr;
    }
}

// Загрузка BMP-файла и сохранение пикселей в буфере pBitMap
void BitMap::loadBitMap(const char* filename) {
    FILE* file = nullptr;

    // Открываем файл в двоичном режиме
    fopen_s(&file, filename, "rb");
    if (!file) {
        MessageBoxA(nullptr, "Unable to open BMP file", filename, MB_OK | MB_ICONERROR);
        return;
    }

    // Читаем заголовок BMP-файла (14 байт)
    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    // Проверяем сигнатуру "BM" для подтверждения BMP-файла
    if (fileHeader.bfType != 0x4D42) {
        MessageBoxA(nullptr, "Not a valid BMP file", filename, MB_OK | MB_ICONERROR);
        fclose(file);
        return;
    }

    // Читаем заголовок информации BMP (40 байт)
    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    // Получаем ширину и высоту изображения
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    size = width * height;

    // Освобождаем старые данные, если они есть
    if (pBitMap) delete[] pBitMap;

    // Выделяем память под пиксели (по одному DWORD на пиксель)
    pBitMap = new DWORD[size];

    // Вычисляем выравнивание (padding), чтобы длина строки была кратна 4 байтам
    int padding = (4 - (width * 3) % 4) % 4;
    BYTE rgb[3]; // Временный массив для хранения BGR-значений одного пикселя

    // Читаем данные построчно снизу вверх, так как BMP хранит строки в обратном порядке
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Считываем 3 байта на пиксель: синий, зелёный, красный
            fread(rgb, sizeof(BYTE), 3, file);

            // Вычисляем индекс в буфере (с инверсией по вертикали)
            int index = (height - y - 1) * width + x;

            // Преобразуем BGR в DWORD формат и записываем в буфер
            pBitMap[index] = RGBcolor(rgb[2], rgb[1], rgb[0]);
        }
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
}

// Преобразует компоненты R, G, B в COLORREF (DWORD) цвет
COLORREF BitMap::RGBcolor(int r, int g, int b) {
    return (COLORREF)(((BYTE)(b)) | ((WORD)((BYTE)(g)) << 8) | (((DWORD)(BYTE)(r) << 16)));
}

DWORD* BitMap::getBuffer() const { // Возвращает указатель на буфер пикселей
    return pBitMap;
}

int BitMap::getWidth() const { // Возвращает ширину изображения в пикселях
    return width;
}

int BitMap::getHeight() const { // Возвращает высоту
    return height;
}
