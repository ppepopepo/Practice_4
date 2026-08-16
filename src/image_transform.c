/*
image_transform.c - реализация функций преобразования изображений.

Иванюк Агата
МК-101
*/

#include "image_transform.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    float percentage;
} BrightnessData;

typedef struct {
    int red_boost;
    int blue_reduce;
} SepiaData;

typedef struct {
    Pixel color1;
    Pixel color2;
} DuotoneData;

typedef struct {
    int step;
} ComicData;

int image_apply_transform(BMPImage* image, PixelTransformFunc transform, void* user_data) {
    if (!image || !transform) return 0;

    for (int i = 0; i < image->width * image->height; i++) {
        image->pixels[i] = transform(image->pixels[i], user_data);
    }
    return 1;
}

Pixel transform_brightness(Pixel pixel, void* user_data) {
    BrightnessData* data = (BrightnessData*)user_data;
    float factor = 1.0f + data->percentage / 100.0f;

    pixel.red = (uint8_t)min(255, (int)(pixel.red * factor));
    pixel.green = (uint8_t)min(255, (int)(pixel.green * factor));
    pixel.blue = (uint8_t)min(255, (int)(pixel.blue * factor));

    return pixel;
}

Pixel transform_sepia(Pixel pixel, void* user_data) {
    SepiaData* data = (SepiaData*)user_data;

    uint8_t gray = (uint8_t)(0.299f * pixel.red + 0.587f * pixel.green + 0.114f * pixel.blue);

    pixel.red = (uint8_t)min(255, gray + data->red_boost);
    pixel.green = gray;
    pixel.blue = (uint8_t)max(0, gray - data->blue_reduce);

    return pixel;
}

Pixel transform_duotone(Pixel pixel, void* user_data) {
    DuotoneData* data = (DuotoneData*)user_data;

    float brightness = (0.299f * pixel.red + 0.587f * pixel.green + 0.114f * pixel.blue) / 255.0f;

    pixel.red = (uint8_t)(data->color1.red + (data->color2.red - data->color1.red) * brightness);
    pixel.green = (uint8_t)(data->color1.green + (data->color2.green - data->color1.green) * brightness);
    pixel.blue = (uint8_t)(data->color1.blue + (data->color2.blue - data->color1.blue) * brightness);

    return pixel;
}

Pixel transform_comic(Pixel pixel, void* user_data) {
    ComicData* data = (ComicData*)user_data;
    int step = data->step;

    pixel.red = (pixel.red / step) * step;
    pixel.green = (pixel.green / step) * step;
    pixel.blue = (pixel.blue / step) * step;

    return pixel;
}

int image_transpose(BMPImage* image) {
    if (!image) return 0;

    Pixel* new_pixels = (Pixel*)malloc(sizeof(Pixel) * image->width * image->height);
    if (!new_pixels) return 0;

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            new_pixels[x * image->height + y] = image->pixels[y * image->width + x];
        }
    }

    int temp = image->width;
    image->width = image->height;
    image->height = temp;

    free(image->pixels);
    image->pixels = new_pixels;
    return 1;
}

int image_rotate_180(BMPImage* image) {
    if (!image) return 0;

    int total = image->width * image->height;
    for (int i = 0; i < total / 2; i++) {
        Pixel temp = image->pixels[i];
        image->pixels[i] = image->pixels[total - 1 - i];
        image->pixels[total - 1 - i] = temp;
    }
    return 1;
}

int image_pixelate(BMPImage* image, int block_size) {
    if (!image || block_size <= 0) return 0;

    for (int y = 0; y < image->height; y += block_size) {
        for (int x = 0; x < image->width; x += block_size) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int count = 0;

            for (int dy = 0; dy < block_size && y + dy < image->height; dy++) {
                for (int dx = 0; dx < block_size && x + dx < image->width; dx++) {
                    Pixel p = image->pixels[(y + dy) * image->width + (x + dx)];
                    sum_r += p.red;
                    sum_g += p.green;
                    sum_b += p.blue;
                    count++;
                }
            }

            Pixel avg;
            avg.red = sum_r / count;
            avg.green = sum_g / count;
            avg.blue = sum_b / count;

            for (int dy = 0; dy < block_size && y + dy < image->height; dy++) {
                for (int dx = 0; dx < block_size && x + dx < image->width; dx++) {
                    image->pixels[(y + dy) * image->width + (x + dx)] = avg;
                }
            }
        }
    }
    return 1;
}