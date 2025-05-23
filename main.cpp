﻿#include "Window.h"

int APIENTRY wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow)
{
    Window window(hInstance);

    if (!window.Create()) {
        MessageBoxW(nullptr, L"Ошибка создания окна", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    window.Show(nCmdShow);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}