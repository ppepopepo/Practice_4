/*
test_lib.c - тесты для библиотеки обработки изображений.

Иванюк Агата
МК-101
*/

#include <stdio.h>
#include <assert.h>
#include "bmp.h"
#include "image_transform.h"

void test_brightness() {
    Pixel pixel = { 100, 100, 100 };
    BrightnessData data = { 50.0f };
    Pixel result = transform_brightness(pixel, &data);
    assert(result.red == 150);
    assert(result.green == 150);
    assert(result.blue == 150);
}

int main() {
    test_brightness();
    printf("Все тесты пройдены!\n");
    return 0;
}