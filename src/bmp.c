/*
bmp.c - пеализация функций для работы с BMP файлами.

Иванюк Агата
МК-101
*/

#include "bmp.h"
#include <stdlib.h>
#include <string.h>

BMPImage* bmp_load(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    BMPImage* image = (BMPImage*)malloc(sizeof(BMPImage));
    if (!image) {
        fclose(file);
        return NULL;
    }

    fread(&image->fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&image->infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    if (image->fileHeader.bfType != 0x4D42) {
        free(image);
        fclose(file);
        return NULL;
    }

    image->width = image->infoHeader.biWidth;
    image->height = image->infoHeader.biHeight;

    if (image->height < 0) {
        image->height = -image->height;
    }

    int padding = (4 - (image->width * sizeof(Pixel)) % 4) % 4;
    image->pixels = (Pixel*)malloc(sizeof(Pixel) * image->width * image->height);

    fseek(file, image->fileHeader.bfOffBits, SEEK_SET);

    for (int y = 0; y < image->height; y++) {
        fread(&image->pixels[y * image->width], sizeof(Pixel), image->width, file);
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    return image;
}

int bmp_save(const BMPImage* image, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return 0;

    fwrite(&image->fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&image->infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    int padding = (4 - (image->width * sizeof(Pixel)) % 4) % 4;
    uint8_t pad_data[3] = { 0, 0, 0 };

    fseek(file, image->fileHeader.bfOffBits, SEEK_SET);

    for (int y = 0; y < image->height; y++) {
        fwrite(&image->pixels[y * image->width], sizeof(Pixel), image->width, file);
        fwrite(pad_data, padding, 1, file);
    }

    fclose(file);
    return 1;
}

void bmp_free(BMPImage* image) {
    if (image) {
        free(image->pixels);
        free(image);
    }
}