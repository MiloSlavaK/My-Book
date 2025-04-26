#include "Window.h"
#include "resources/resources.h"
#include <commdlg.h>
#include <fstream>
#include <string>

Window::Window(HINSTANCE hInstance)
    : m_hInstance(hInstance),
    m_hwnd(nullptr),
    m_hEdit(nullptr),
    m_hFont(nullptr) {
}

Window::~Window() {
    if (m_hFont) {
        DeleteObject(m_hFont);
    }
    if (m_hwnd && IsWindow(m_hwnd)) {
        DestroyWindow(m_hwnd);
    }
}

void Window::CreateControls() {
    m_hEdit = CreateWindowExW(
        0, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN,
        0, 0, 0, 0,
        m_hwnd, (HMENU)IDC_MAIN_EDIT, m_hInstance, nullptr
    );

    m_hFont = CreateFontW(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, L"Consolas"
    );
    SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)m_hFont, TRUE);
}

bool Window::Create() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = m_className;
    wc.hIcon = LoadIconW(m_hInstance, MAKEINTRESOURCEW(IDI_APP_ICON));

    if (!RegisterClassW(&wc)) return false;

    m_hwnd = CreateWindowExW(
        0, m_className, L"MyBook",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, LoadMenuW(m_hInstance, MAKEINTRESOURCEW(IDM_MAIN_MENU)),
        m_hInstance, this
    );

    return m_hwnd != nullptr;
}

void Window::Show(int nCmdShow) {
    if (m_hwnd) {
        ShowWindow(m_hwnd, nCmdShow);
        UpdateWindow(m_hwnd);
    }
}

std::wstring Window::ShowFileDialog(bool save) {
    OPENFILENAMEW ofn = {};
    wchar_t fileName[MAX_PATH] = {};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"txt";

    if (save) {
        ofn.Flags |= OFN_OVERWRITEPROMPT;
        if (!GetSaveFileNameW(&ofn)) return L"";
    }
    else {
        if (!GetOpenFileNameW(&ofn)) return L"";
    }

    return fileName;
}

void Window::OpenFile() {
    std::wstring filePath = ShowFileDialog(false);
    if (filePath.empty()) return;

    std::wifstream file(filePath);
    if (!file.is_open()) {
        MessageBoxW(m_hwnd, L"File do not open", L"Error", MB_ICONERROR);
        return;
    }

    // Простое чтение wide-строк
    std::wstring content(
        (std::istreambuf_iterator<wchar_t>(file)),
        std::istreambuf_iterator<wchar_t>()
    );

    SetWindowTextW(m_hEdit, content.c_str());
    m_currentFile = filePath;
    SetWindowTextW(m_hwnd, (L"MyBook - " + filePath).c_str());
}

void Window::SaveFile() {
    if (m_currentFile.empty()) {
        m_currentFile = ShowFileDialog(true);
        if (m_currentFile.empty()) return;
    }

    int length = GetWindowTextLengthW(m_hEdit) + 1;
    std::wstring content;
    content.resize(length);
    GetWindowTextW(m_hEdit, &content[0], length);

    std::wofstream file(m_currentFile);
    if (!file.is_open()) {
        MessageBoxW(m_hwnd, L"Do not save", L"Error", MB_ICONERROR);
        return;
    }

    file << content.c_str();
    SetWindowTextW(m_hwnd, (L"MyBook - " + m_currentFile).c_str());
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (uMsg) {
        case WM_CREATE:
            pThis->CreateControls();
            return 0;

        case WM_SIZE:
            if (pThis->m_hEdit) {
                MoveWindow(pThis->m_hEdit, 0, 0,
                    LOWORD(lParam), HIWORD(lParam), TRUE);
            }
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case IDM_FILE_NEW:
                SetWindowTextW(pThis->m_hEdit, L"");
                pThis->m_currentFile.clear();
                SetWindowTextW(hwnd, L"MyBook");
                return 0;

            case IDM_FILE_OPEN:
                pThis->OpenFile();
                return 0;

            case IDM_FILE_SAVE:
            case IDM_FILE_SAVEAS:
                pThis->SaveFile();
                return 0;

            case IDM_FILE_EXIT:
                PostQuitMessage(0);
                return 0;

            case IDM_EDIT_UNDO:
                SendMessageW(pThis->m_hEdit, WM_UNDO, 0, 0);
                return 0;

            case IDM_EDIT_CUT:
                SendMessageW(pThis->m_hEdit, WM_CUT, 0, 0);
                return 0;

            case IDM_EDIT_COPY:
                SendMessageW(pThis->m_hEdit, WM_COPY, 0, 0);
                return 0;

            case IDM_EDIT_PASTE:
                SendMessageW(pThis->m_hEdit, WM_PASTE, 0, 0);
                return 0;

            case IDM_EDIT_DELETE:
                SendMessageW(pThis->m_hEdit, WM_CLEAR, 0, 0);
                return 0;

            case IDM_HELP_ABOUT:
                MessageBoxW(hwnd, L"MyBook\nver 1.0",
                    L"Help", MB_OK | MB_ICONINFORMATION);
                return 0;
            }
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}