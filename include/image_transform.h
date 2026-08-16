/*
image_transform.h - заголовочный файл для функций преобразования изображений.

Иванюк Агата
МК-101
*/

#ifndef IMAGE_TRANSFORM_H
#define IMAGE_TRANSFORM_H

#include "bmp.h"

typedef Pixel(*PixelTransformFunc)(Pixel pixel, void* user_data);

int image_apply_transform(BMPImage* image, PixelTransformFunc transform, void* user_data);

Pixel transform_brightness(Pixel pixel, void* user_data);
Pixel transform_sepia(Pixel pixel, void* user_data);
Pixel transform_duotone(Pixel pixel, void* user_data);
Pixel transform_comic(Pixel pixel, void* user_data);

int image_transpose(BMPImage* image);
int image_rotate_180(BMPImage* image);
int image_pixelate(BMPImage* image, int block_size);

#endif