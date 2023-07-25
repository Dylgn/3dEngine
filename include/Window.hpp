#pragma once

#include <Windows.h>
#include <cstdint>
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
        /** Set pixel at given coordinates to given colour in 0xRRGGBB format */
        void setPixel(int x, int y, const uint32_t &colour);
        /** Draw triangle with texture */
        void drawTriangle(const Triangle &t, const Texture &tex = default_texture, bool check_depth = false);
        /** Clears screen with given colour */
        void clear(const uint32_t &colour);
        /** Clears the depth buffer */
        void clear_depth_buffer();

        /** Gets width of screen */
        int getWidth();
        /** Gets height of screen */
        int getHeight();

        /** Gets if key is being held down or not.
         * Virtual key codes can be found {@link https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes|here}
         * @return True if held down, false otherwise
        */
        bool KeyDown(const int &virt_key);
        /** Draw texture to screen */
        void drawImage(const Texture &t);

        static const Texture default_texture;

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