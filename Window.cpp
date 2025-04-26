#include "Window.h"

Window::Window(HINSTANCE hInstance) :
    m_hInstance(hInstance),
    m_hwnd(nullptr)
{
}

Window::~Window()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }
}

bool Window::Create() {
    WNDCLASSW wc = {}; 
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = m_className;

    if (!RegisterClassW(&wc)) {
        return false;
    }

    m_hwnd = CreateWindowExW(
        0, m_className, L"My Book",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, m_hInstance, this
    );

    return m_hwnd != nullptr;
}

void Window::Show(int nCmdShow) {
    if (m_hwnd) {
        ShowWindow(m_hwnd, nCmdShow);
        UpdateWindow(m_hwnd);
    }
}

HWND Window::GetHandle() const {
    return m_hwnd;
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}