#pragma once

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window();
        ~Window();

        bool ProcessMessages();

        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;
    private:
        const wchar_t *CLASS_NAME;
        HINSTANCE hInstance;
        HWND hWnd;
};