/*
bmp.h - заголовочный файл для работы с BMP изображениями.

Иванюк Агата
МК-101
*/

#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    Pixel* pixels;
    int width;
    int height;
} BMPImage;

BMPImage* bmp_load(const char* filename);
int bmp_save(const BMPImage* image, const char* filename);
void bmp_free(BMPImage* image);

#endif