#include "Window.hpp"
#include <GL/gl.h>

#include <iostream>

// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

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
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hContext);
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

void Window::GLSwapBuffers() {
    SwapBuffers(hDC);
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

    ConstructGLContext();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

void Window::ConstructGLContext() {
    ConstructGLExtensions();
    
    int pfn;
    UINT num_formats;

    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    wglChoosePixelFormatARB(hDC, pixel_format_attribs, 0, 1, &pfn, &num_formats);

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(hDC, pfn, sizeof(pfd), &pfd);
    SetPixelFormat(hDC, pfn, &pfd);

    int gl_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    hContext = wglCreateContextAttribsARB(hDC, 0, gl_attribs);
    wglMakeCurrent(hDC, hContext);
}

void Window::ConstructGLExtensions() {
    // Load extensions using temp opengl context
    WNDCLASSA wnd_class = {};
    wnd_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wnd_class.lpfnWndProc = DefWindowProcA;
    wnd_class.hInstance = GetModuleHandle(0);
    wnd_class.lpszClassName = "TEMPORARY CLASS (3dEngine)";

    RegisterClassA(&wnd_class);

    HWND temp_window = CreateWindowExA(
        0, wnd_class.lpszClassName, "Temporary OpenGL Window", 0, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, wnd_class.hInstance, 0
    );

    HDC temp_dc = GetDC(temp_window);

    PIXELFORMATDESCRIPTOR pfd = {
	    sizeof(PIXELFORMATDESCRIPTOR),
	    1,
	    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
	    PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
	    32,                   // Colordepth of the framebuffer.
	    0, 0, 0, 0, 0, 0,
	    0,
	    0,
	    0,
	    0, 0, 0, 0,
	    0,                   // Number of bits for the depthbuffer
	    8,                    // Number of bits for the stencilbuffer
	    0,                    // Number of Aux buffers in the framebuffer.
	    PFD_MAIN_PLANE,
	    0,
	    0, 0, 0
    };

    int pfn = ChoosePixelFormat(temp_dc, &pfd);
    SetPixelFormat(temp_dc, pfn, &pfd);

    HGLRC temp_context = wglCreateContext(temp_dc);

    wglMakeCurrent(temp_dc, temp_context);

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*) wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*) wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(temp_dc, 0);
    wglDeleteContext(temp_context);
    ReleaseDC(temp_window, temp_dc);
    DestroyWindow(temp_window);
}