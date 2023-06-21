#include <algorithm>

#include "Window.hpp"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            break;
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
    DeleteDC(back_dc);
    DeleteObject(back_bm);
    delete[] screen_buffer;
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

void Window::update() {
    SetBitmapBits(back_bm, 4 * HEIGHT * WIDTH, (const void*)screen_buffer);
    BitBlt(GetDC(hWnd), 0, 0, WIDTH, HEIGHT, back_dc, 0, 0, SRCCOPY);
}

void Window::setPixel(int x, int y, DWORD colour) {
    screen_buffer[y * WIDTH + x] = colour;
}

void Window::drawTriangle(V2d v1, V2d v2, V2d v3) {
    // Order vertices so that vertex 1 is highest on screen & vertex 3 is lowest
    if (v2.v < v1.v) std::swap(v1,v2);
    if (v3.v < v1.v) std::swap(v1,v3);
    if (v3.v < v2.v) std::swap(v2,v3);

    // Slopes of line segments from v1 to v2 & v1 to v3
    float slope_v2 = (v1.u - v2.u) / (v1.v - v2.v);
    float slope_v3 = (v1.u - v3.u) / (v1.v - v3.v);
    if (v1.v - v2.v != 0) {
        // First half of triangle
        for (int i = v1.v; i <= v2.v; ++i) {
            // x-values of start/end of triangle at each y-value
            float start = slope_v2 * (i - v1.v) + v1.u;
            float end = slope_v3 * (i - v1.v) + v1.u;
            if (end < start) std::swap(start, end);

            // Draw line start to end
            for (int j = start; j <= end; ++j) {
                setPixel(j, i, 0x00FF0000);
            }
        }
    }

    slope_v2 = (v2.u - v3.u) / (v2.v - v3.v);
    // Second half of triangle
    for (int i = v2.v; i <= v3.v; ++i) {
        float start = slope_v2 * (i - v2.v) + v2.u;
        float end = slope_v3 * (i - v1.v) + v1.u;
        if (end < start) std::swap(start, end);
        for (int j = start; j <= end; ++j) {
            setPixel(j, i, 0x00FF0000);
        }
    }
}

void Window::clear(DWORD colour) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            screen_buffer[i * WIDTH + j] = colour;
        }
    }
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

    WIDTH = width;
    HEIGHT = height;

    AdjustWindowRect(&rect, style, false);

    hWnd = CreateWindowEx(
        0, CLASS_NAME, title, style, 
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
        NULL, NULL, hInstance, NULL
    );

    hDC = GetDC(hWnd);

    screen_buffer = new DWORD[WIDTH * HEIGHT];
    memset(screen_buffer, 0, sizeof(DWORD) * WIDTH * HEIGHT);
    ConstructBackBuffer();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

void Window::ConstructBackBuffer() {
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPCOREHEADER);
    bmi.bmiHeader.biWidth = WIDTH;
    bmi.bmiHeader.biWidth = -HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    back_dc = CreateCompatibleDC(hDC);
    back_bm = CreateCompatibleBitmap(hDC, WIDTH, HEIGHT);

    SetBitmapBits(back_bm, 4 * HEIGHT * WIDTH, (const void*)screen_buffer);

    SelectObject(back_dc, back_bm);
    ReleaseDC(hWnd, hDC);
}
