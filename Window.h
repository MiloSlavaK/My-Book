#pragma once
#include <windows.h>

class Window {
public:
    explicit Window(HINSTANCE hInstance);
    ~Window();

    bool Create();
    void Show(int nCmdShow);
    HWND GetHandle() const;

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    HWND m_hwnd;
    const wchar_t* m_className = L"MyWindowClass";
};