#pragma once
#include <windows.h>
#include <string>

class Window {
public:
    explicit Window(HINSTANCE hInstance);
    ~Window();

    bool Create();
    void Show(int nCmdShow);

private:
    void CreateControls();
    void OpenFile();
    void SaveFile();
    std::wstring ShowFileDialog(bool save);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    HWND m_hwnd;
    HWND m_hEdit;
    HFONT m_hFont; 
    std::wstring m_currentFile;
    const wchar_t* m_className = L"TextEditorClass";
};