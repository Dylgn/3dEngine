#pragma once

#include <Windows.h>
#include "Triangle.hpp"
#include "Texture.hpp"

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

LRESULT CALLBACK WindowProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window(int width = 640, int height = 480, const wchar_t *title = L"");
        ~Window();

        bool ProcessMessages();

        void update();
        void setPixel(int x, int y, uint32_t colour);
        void drawTriangle(Triangle t, Texture tex, bool check_depth = false);
        void clear(unsigned int colour);
        void clear_depth_buffer();

        int getWidth();
        int getHeight();

        bool KeyDown(int virt_key);

        void drawImage(Texture t);

        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;
    private:
        const wchar_t *CLASS_NAME;
        HINSTANCE instance;
        HWND wnd;

        const int WIDTH;
        const int HEIGHT;

        uint32_t *frame_buffer;
        float *depth_buffer = nullptr;

        HDC back_dc;
        HBITMAP back_bm;

        void ConstructWindow(const wchar_t *title);
        void ConstructBackBuffer(HDC &dc);
};