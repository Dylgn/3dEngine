#pragma once

#include <vector>
#include "Camera.hpp"
#include "Window.hpp"
#include "Mesh.hpp"

class GameEngine {
    bool running;
    Window window;
    Texture temp;
    public:
        GameEngine(int width = 640, int height = 480, float fov_deg = 90.0f, const wchar_t *title = L"");
        virtual ~GameEngine();

        void Start();

        /** Gets if key is being held down or not.
         * Virtual key codes can be found {@link https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes|here}
         * @return True if held down, false otherwise
        */
        bool KeyDown(const int &virt_key);

        /** Starts the engine */
        virtual bool onStart() = 0;
        /** Method which is called every frame */
        virtual bool onUpdate(const float &elapsed_time) = 0;
    protected:
        Camera cam;
        std::vector<Mesh> meshes;
    private:
        void Render();
};