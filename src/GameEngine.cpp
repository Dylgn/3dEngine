#include <list>
#include <chrono>
#include "GameEngine.hpp"
#include "MathUtility.hpp"
#include "Triangle.hpp"
#include "Geometry.hpp"
#include "Rigidbody.hpp"

GameEngine::GameEngine(int width, int height, float fov_deg, const wchar_t *title): 
    m_running{false}, m_window{width, height, title} {
    player = {
        MathUtil::GetProjMat(fov_deg, ((float) height) / ((float) width), 0.1f, 1000.0f),
        {0,0,0},
        {0,0,0},
        0, 0, 0
    };
}

GameEngine::~GameEngine() {}

void GameEngine::Start() {
    if (m_running) return;
    else m_running = true;

    if (!OnStart()) return;

    auto time_prev = std::chrono::system_clock::now();

    m_temp.LoadTexture("../resources/brick.bmp");
    if (!m_temp.m_image) return;

    while (m_running) {
        if (!m_window.ProcessMessages()) {
            m_running = false;
            break;
        }
        
        // Calculate elapsed time since last call to onUpdate()
        auto time_now = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_time = time_now - time_prev;
        if (!PhysicsStep(elapsed_time.count())) return;
        if (!OnUpdate(elapsed_time.count())) return;
        time_prev = time_now;

        Render();
    }
}

bool GameEngine::KeyDown(const int &virt_key) {
    return m_window.KeyDown(virt_key);
}

bool GameEngine::PhysicsStep(const float &elapsed_time) {
    for (auto &o : m_objects) {
        Rigidbody *body;
        if (body = dynamic_cast<Rigidbody*>(o.GetBody())) {
            body->Accelerate(m_gravity);
            o.Move(body->CalculateVelocity(elapsed_time) * elapsed_time);
            body->SetForce(V3d::origin);

            float min_height = 1.0f;

            if (o.GetPos().y < min_height) {
                body->SetVelocity(V3d::origin);
                o.Move({0.0f, min_height - o.GetPos().y, 0.0f});
            }
        }
    }
    return true;
}

void GameEngine::Render()
{
    m_window.clear(0x00000000);
    m_window.clear_depth_buffer();

    std::list<Triangle> clipped = Render::GetClippedTriangles(*m_objects[0].GetMesh(), *player.GetCamera(), m_window.getWidth(), m_window.getHeight());
    for (Triangle &t : clipped) m_window.drawTriangle(t, m_temp);
    
    m_window.update();
}