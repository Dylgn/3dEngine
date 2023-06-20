#pragma once

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window(int width = 640, int height = 480, const wchar_t *title = L"");
        ~Window();

        bool ProcessMessages();

        void update();
        void setPixel(int x, int y, DWORD colour);
        void clear(DWORD colour);

        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;
    private:
        const wchar_t *CLASS_NAME;
        HINSTANCE hInstance;
        HWND hWnd;
        HDC hDC;
        int WIDTH;
        int HEIGHT;

        DWORD *screen_buffer;

        HDC back_dc;
        HBITMAP back_bm;

        void ConstructWindow(int width, int height, const wchar_t *title);
        void ConstructBackBuffer();
};