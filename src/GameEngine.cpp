#include <list>
#include <chrono>
#include "GameEngine.hpp"
#include "MathUtility.hpp"
#include "Triangle.hpp"
#include "Geometry.hpp"

GameEngine::GameEngine(int width, int height, float fov_deg, const wchar_t *title): 
    running{false}, window{width, height, title} {
    cam = {
        MathUtil::GetProjMat(fov_deg, ((float) height) / ((float) width), 0.1f, 1000.0f),
        {0,0,0},
        {0,0,0},
        0, 0
    };
}

GameEngine::~GameEngine() {}

void GameEngine::Start() {
    if (running) return;
    else running = true;

    onStart();

    auto time_prev = std::chrono::system_clock::now();

    while (running) {
        if (!window.ProcessMessages()) {
            running = false;
            break;
        }
        
        // Calculate elapsed time since last call to onUpdate()
        auto time_now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_time = time_now - time_prev;
        onUpdate(elapsed_time.count());
        time_prev = time_now;
        
        Render();
    }
}

bool GameEngine::KeyDown(int virt_key) {
    return window.KeyDown(virt_key);
}

void GameEngine::Render() {
    window.clear(0x00000000);
    window.clear_depth_buffer();
    int WIDTH = window.getWidth();
    int HEIGHT = window.getHeight();

    std::list<Triangle> triangles;

    for (Mesh &mesh : meshes) {
        std::list<Triangle> clipped = GetClippedTriangles(mesh, cam, WIDTH, HEIGHT);
        for (Triangle &t : clipped) triangles.push_back(t);
    }
    for (Triangle &t : triangles) {
        window.drawTriangle(t);
    }
    window.update();
}