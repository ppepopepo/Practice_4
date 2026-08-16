/*
main.c - главная программа для обработки BMP изображений.

Иванюк Агата
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "bmp.h"
#include "image_transform.h"

#define ID_BUTTON_OPEN 1001
#define ID_BUTTON_SAVE 1002
#define ID_BUTTON_BRIGHTNESS 1003
#define ID_BUTTON_SEPIA 1004
#define ID_BUTTON_DUOTONE 1005
#define ID_BUTTON_COMIC 1006
#define ID_BUTTON_TRANSPOSE 1007
#define ID_BUTTON_ROTATE180 1008
#define ID_BUTTON_PIXELATE 1009

BMPImage* current_image = NULL;
HWND hwnd_main;

void show_error(const char* message) {
    MessageBox(hwnd_main, message, "Ошибка", MB_OK | MB_ICONERROR);
}

void open_image() {
    OPENFILENAME ofn;
    char filename[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd_main;
    ofn.lpstrFilter = "BMP Files\0*.bmp\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        if (current_image) bmp_free(current_image);
        current_image = bmp_load(filename);
        if (!current_image) {
            show_error("Не удалось открыть файл");
        }
    }
}

void save_image() {
    if (!current_image) return;

    OPENFILENAME ofn;
    char filename[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd_main;
    ofn.lpstrFilter = "BMP Files\0*.bmp\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        if (!bmp_save(current_image, filename)) {
            show_error("Не удалось сохранить файл");
        }
    }
}

void apply_brightness() {
    if (!current_image) return;

    char input[32] = { 0 };
    if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(1), hwnd_main, NULL, 0) != -1) {
        BrightnessData data;
        data.percentage = (float)atof(input);
        image_apply_transform(current_image, transform_brightness, &data);
        InvalidateRect(hwnd_main, NULL, TRUE);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow("BUTTON", "Открыть", WS_VISIBLE | WS_CHILD,
            10, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_OPEN, NULL, NULL);
        CreateWindow("BUTTON", "Сохранить", WS_VISIBLE | WS_CHILD,
            120, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
        // Добавь остальные кнопки аналогично
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BUTTON_OPEN:
            open_image();
            break;
        case ID_BUTTON_SAVE:
            save_image();
            break;
        case ID_BUTTON_BRIGHTNESS:
            apply_brightness();
            break;
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "BMPProcessor";

    RegisterClass(&wc);

    hwnd_main = CreateWindow("BMPProcessor", "BMP Image Processor",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd_main, nCmdShow);
    UpdateWindow(hwnd_main);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (current_image) bmp_free(current_image);
    return msg.wParam;
}