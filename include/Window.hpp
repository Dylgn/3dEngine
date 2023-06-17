#pragma once

#include <Windows.h>

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
        const int *attribList);
typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window(int width = 640, int height = 480, const wchar_t *title = L"");
        ~Window();

        bool ProcessMessages();

        void GLSwapBuffers();

        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;
    private:
        const wchar_t *CLASS_NAME;
        HINSTANCE hInstance;
        HWND hWnd;
        HDC hDC;
        HGLRC hContext;

        void ConstructWindow(int width, int height, const wchar_t *title);
        void ConstructGLContext();
        void ConstructGLExtensions();

        wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;
        wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
};