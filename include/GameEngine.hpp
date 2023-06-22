#pragma once

#include <vector>

#include "Camera.hpp"
#include "Window.hpp"
#include "Mesh.hpp"

class GameEngine {
    bool running;
    Window window;
    Camera cam;
    public:
        GameEngine(int width = 640, int height = 480, float fov_deg = 90.0f, const wchar_t *title = L"");
        virtual ~GameEngine();
        void Start();
        virtual bool onStart() = 0;
        virtual bool onUpdate(float elapsed_time) = 0;
    protected:
        std::vector<Mesh> meshes;
    private:
        void Render();
};