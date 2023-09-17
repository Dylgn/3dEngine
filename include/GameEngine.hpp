#pragma once

#include <vector>
#include "Camera.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "V3d.hpp"
#include "Object.hpp"
#include "Player.hpp"

class GameEngine {
    bool m_running;
    Window m_window;
    Texture m_temp;
    Texture *m_default;
    V3d m_gravity{0.0f, -9.81f, 0.0f};
    public:
        GameEngine(int width = 640, int height = 480, float fov_deg = 90.0f, const wchar_t *title = L"");
        virtual ~GameEngine();

        void Start();

        /** Gets if key is being held down or not.
         * Virtual key codes can be found {@link https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes|here}
         * @return True if held down, false otherwise
        */
        bool KeyDown(const int &virt_key);

        bool PhysicsStep(const float &elapsed_time);

        bool ResolveCollisions(const float &elapsed_time);

        /** Starts the engine */
        virtual bool OnStart() = 0;
        /** Method which is called every frame */
        virtual bool OnUpdate(const float &elapsed_time) = 0;
    protected:
        Player player;
        std::vector<Object> m_objects;
        //Object cube;
    private:
        void Render();
};