#include <algorithm>

#include "Window.hpp"

const Texture Window::default_texture{"../resources/default_texture.bmp"};

LRESULT CALLBACK WindowProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            break;
        case WM_CLOSE:
            DestroyWindow(wnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(wnd, uMsg, wParam, lParam);
}

Window::Window(int width, int height, const wchar_t *title): CLASS_NAME{L"Window Class"}, instance(GetModuleHandle(nullptr)), WIDTH{width}, HEIGHT{height}, depth_buffer{new float[WIDTH * HEIGHT]}, frame_buffer{new uint32_t[HEIGHT * WIDTH]} {
    ConstructWindow(title);
}

Window::~Window() {
    UnregisterClass(CLASS_NAME, instance);
    DeleteDC(back_dc);
    DeleteObject(back_bm);
    delete[] frame_buffer;
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
    SetBitmapBits(back_bm, 4 * HEIGHT * WIDTH, (const void*)frame_buffer);
    BitBlt(GetDC(wnd), 0, 0, WIDTH, HEIGHT, back_dc, 0, 0, SRCCOPY);
}

void Window::setPixel(int x, int y, const uint32_t &colour) {
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) return;
    frame_buffer[y * WIDTH + x] = colour;
}

float max(float a, float b) {
    if (a > b) return a;
    else return b;
}
float min(float a, float b) {
    if (a > b) return b;
    else return a;
}

void Window::drawTriangle(const Triangle &t, const Texture &tex, bool check_depth) {
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
    float slope_v2 = 0, slope_v3 = 0;
    V2d slope_t2 = {0,0}, slope_t3 = {0,0};

    if ((v1.y - v3.y) != 0 ) {
        slope_v3 = (v1.x - v3.x) / static_cast<float>(v1.y - v3.y);
        slope_t3 = (t1 - t3) / static_cast<float>(v1.y - v3.y);
    }

    if ((v1.y - v2.y) != 0) {
        slope_v2 = (v1.x - v2.x) / static_cast<float>(v1.y - v2.y);
        slope_t2 = (t1 - t2) / static_cast<float>(v1.y - v2.y);
        // First half of triangle
        for (int i = v1.y; i <= v2.y; ++i) {
            float di = i - v1.y;
            // x-values of start/end of triangle at each y-value
            int start = slope_v2 * max(di, 0) + v1.x;
            int end = slope_v3 * max(di, 0) + v1.x;

            V2d t_start = slope_t2 * max(di, 0) + t1;
            V2d t_end = slope_t3 * max(di, 0) + t1;

            if (end < start) {
                std::swap(start, end);
                std::swap(t_start, t_end);
            }

            float tstep = 1.0f / static_cast<float>(end - start);
            float t = 0.0f;

            // Draw line start to end
            for (int j = start; j <= end; ++j) {
                // Linear interpolation texture coordinates
                V2d t_j = t_start * (1 - t) + t_end * t;

                if (t_j.w > depth_buffer[i * WIDTH + j]) {
                    int y = (t_j.v / t_j.w) * static_cast<float>(tex.m_height - 1);
                    int x = (t_j.u / t_j.w) * static_cast<float>(tex.m_width);

                    setPixel(j, i, tex.m_image[y * tex.m_width + x]);
                    depth_buffer[i * WIDTH + j] = t_j.w;
                }

                t += tstep;
            }
        }
    }

    if ((v2.y - v3.y) != 0) {
        slope_v2 = (v2.x - v3.x) / static_cast<float>(v2.y - v3.y);
        slope_t2 = (t2 - t3) / static_cast<float>(v2.y - v3.y);
    }
    
    if (v2.y - v3.y != 0) {
        // Second half of triangle
        for (int i = v2.y; i <= v3.y; ++i) {
            float di = i - v1.y;
            float di2 = i - v2.y;
            int start = slope_v2 * max(di2, 0) + v2.x;
            int end = slope_v3 * max(di, 0) + v1.x;

            V2d t_start = slope_t2 * max(di2, 0) + t2;
            V2d t_end = slope_t3 * max(di, 0) + t1;

            if (end < start) {
                std::swap(start, end);
                std::swap(t_start, t_end);
            }

            float tstep = 1.0f / static_cast<float>(end - start);
            float t = 0.0f;

            for (int j = start; j <= end; ++j) {
                V2d t_j = t_start * (1 - t) + t_end * t;

                if (t_j.w > depth_buffer[i * WIDTH + j]) {
                    int y = (t_j.v / t_j.w) * static_cast<float>(tex.m_height - 1);
                    int x = (t_j.u / t_j.w) * static_cast<float>(tex.m_width);

                    setPixel(j, i, tex.m_image[y * tex.m_width + x]);
                    depth_buffer[i * WIDTH + j] = t_j.w;
                }

                t += tstep;
            }
        }
    }
}

void Window::clear(const uint32_t &colour) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            frame_buffer[i * WIDTH + j] = colour;
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

bool Window::KeyDown(const int &virt_key) {
    return GetKeyState(virt_key) & 0x8000;
}

void Window::drawImage(const Texture &t) {
    int scale = 4;
    for (int i = 0; i < t.m_height * scale; ++i) {
        for (int j = 0; j < t.m_width * scale; ++j) {
            setPixel(j, i, t.m_image[i / scale * t.m_height + j / scale]);
        }
    }
}

void Window::ConstructWindow(const wchar_t *title) {
    WNDCLASS wnd_class = {};
    wnd_class.lpszClassName = CLASS_NAME;
    wnd_class.hInstance = instance;
    wnd_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class.lpfnWndProc = WindowProc;

    RegisterClass(&wnd_class);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | CS_OWNDC;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + WIDTH;
    rect.bottom = rect.top + HEIGHT;

    AdjustWindowRect(&rect, style, false);

    wnd = CreateWindowEx(
        0, CLASS_NAME, title, style, 
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
        NULL, NULL, instance, NULL
    );

    memset(frame_buffer, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
    memset(depth_buffer, 0, sizeof(float) * WIDTH * HEIGHT);

    HDC dc = GetDC(wnd);
    ConstructBackBuffer(dc);

    ShowWindow(wnd, SW_SHOW);
    UpdateWindow(wnd);
}

void Window::ConstructBackBuffer(HDC &dc) {
    // BITMAPINFO bmi;
    // bmi.bmiHeader.biSize = sizeof(BITMAPCOREHEADER);
    // bmi.bmiHeader.biWidth = WIDTH;
    // bmi.bmiHeader.biWidth = -HEIGHT;
    // bmi.bmiHeader.biPlanes = 1;
    // bmi.bmiHeader.biBitCount = 24;
    // bmi.bmiHeader.biCompression = BI_RGB;

    back_dc = CreateCompatibleDC(dc);
    back_bm = CreateCompatibleBitmap(dc, WIDTH, HEIGHT);

    SetBitmapBits(back_bm, 4 * HEIGHT * WIDTH, (const void*)frame_buffer);

    SelectObject(back_dc, back_bm);
    ReleaseDC(wnd, dc);
}
