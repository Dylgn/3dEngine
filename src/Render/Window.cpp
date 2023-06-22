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
    depth_buffer = new float[width * height];
}

Window::~Window() {
    UnregisterClass(CLASS_NAME, hInstance);
    DeleteDC(back_dc);
    DeleteObject(back_bm);
    delete[] screen_buffer;
    delete[] depth_buffer;
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

void Window::drawTriangle(Triangle t, bool check_depth) {
    V3d v1 = t.p[0];
    V3d v2 = t.p[1];
    V3d v3 = t.p[2];
    V2d t1 = t.t[0];
    V2d t2 = t.t[1];
    V2d t3 = t.t[2];
    // Order vertices so that vertex 1 is highest on screen & vertex 3 is lowest
    if (v2.y < v1.y) {
        std::swap(v1, v2);
        std::swap(t1, t2);
    }
    if (v3.y < v1.y) {
        std::swap(v1, v3);
        std::swap(t1, t3);
    }
    if (v3.y < v2.y) {
        std::swap(v2, v3);
        std::swap(t2, t3);
    }
    // Slopes of line segments from v1 to v2 & v1 to v3
    float slope_v2 = (v1.x - v2.x) / (v1.y - v2.y);
    float slope_v3 = (v1.x - v3.x) / (v1.y - v3.y);

    //float slope_t2 = (v1.x - v2.x) / (t1.v - t2.v);
    //float slope_t3 = (v1.x - v3.x) / (t1.v - t3.v);
    if (v1.y - v2.y != 0) {
        // First half of triangle
        for (int i = v1.y; i <= v2.y; ++i) {
            // x-values of start/end of triangle at each y-value
            float start = slope_v2 * (i - v1.y) + v1.x;
            float end = slope_v3 * (i - v1.y) + v1.x;

            //float t_start = slope_t2 * (i - t1.v) + v1.x;
            //float t_end = slope_t2 * (i - t1.v) + v1.x;

            if (end < start) std::swap(start, end);

            // Draw line start to end
            for (int j = start; j <= end; ++j) {
                setPixel(j, i, 0x00FF0000);
            }
        }
    }

    slope_v2 = (v2.x - v3.x) / (v2.y - v3.y);
    if (v2.y - v3.y != 0) {
        // Second half of triangle
        for (int i = v2.y; i <= v3.y; ++i) {
            float start = slope_v2 * (i - v2.y) + v2.x;
            float end = slope_v3 * (i - v1.y) + v1.x;
            if (end < start) std::swap(start, end);
            for (int j = start; j <= end; ++j) {
                setPixel(j, i, 0x00FF0000);
            }
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

void Window::clear_depth_buffer() {
    for (int i = 0; i < WIDTH * HEIGHT; ++i) depth_buffer[i] = 0.0f;
}

int Window::getWidth() {
    return WIDTH;
}

int Window::getHeight() {
    return HEIGHT;
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
