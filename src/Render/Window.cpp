#include "Window.hpp"

#include <iostream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window(int width, int height, const wchar_t *title): CLASS_NAME{L"Window Class"}, hInstance(GetModuleHandle(nullptr)) {
    ConstructWindow(width, height, title);
}

Window::~Window() {
    UnregisterClass(CLASS_NAME, hInstance);
}

bool Window::ProcessMessages() {
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Window::ConstructWindow(int width, int height, const wchar_t *title) {
    WNDCLASS wnd_class = {};
    wnd_class.lpszClassName = CLASS_NAME;
    wnd_class.hInstance = hInstance;
    wnd_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class.lpfnWndProc = WindowProc;

    RegisterClass(&wnd_class);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | CS_OWNDC;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    hWnd = CreateWindowEx(
        0, CLASS_NAME, title, style, 
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
        NULL, NULL, hInstance, NULL
    );

    hDC = GetDC(hWnd);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}