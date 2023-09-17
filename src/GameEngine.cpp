#include <list>
#include <chrono>
#include <cmath>
#include "GameEngine.hpp"
#include "MathUtility.hpp"
#include "Triangle.hpp"
#include "Geometry.hpp"

GameEngine::GameEngine(int width, int height, float fov_deg, const wchar_t *title): 
    m_running{false}, m_window{width, height, title}, m_default{new Texture{"../resources/default_texture.bmp"}} {
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
        if (!ResolveCollisions(elapsed_time.count())) return;
        time_prev = time_now;

        Render();
    }
}

bool GameEngine::KeyDown(const int &virt_key) {
    return m_window.KeyDown(virt_key);
}

bool GameEngine::PhysicsStep(const float &elapsed_time) {
    for (auto &o : m_objects) {
        Rbody *body;
        if (body = dynamic_cast<Rbody*>(o.GetBody())) {
            // Apply gravity and move body
            if (o.ContainsProperty(Obj::Property::gravity)) {
                body->Accelerate(m_gravity);
                o.Move(body->CalculateVelocity(elapsed_time) * elapsed_time);
                body->SetForce(V3d::origin);
            }

            // Dont let objects fall below a certain height
            float min_height = 0.0f;
            float cur_height = o.GetBody()->FurthestPointIn(-V3d::unit_y).y;
            if (cur_height < min_height) {
                body->SetVelocity(V3d::origin);
                o.Move({0.0f, min_height - cur_height, 0.0f});
            }
        }
    }
    return true;
}

bool GameEngine::ResolveCollisions(const float &elapsed_time) {
    for (Object &a : m_objects) {
        for (Object &b : m_objects) {
            if (a == b) break;
            else if (!a.GetBody() || !b.GetBody()) continue;
            V3d norm = a.GetCollisionNormal(b);
            if (norm) {
                // Call OnCollision
                a.OnCollision(b);
                b.OnCollision(a);
                // Move bodies if they're rigid
                Rbody *a_body = dynamic_cast<Rbody*>(a.GetBody());
                Rbody *b_body = dynamic_cast<Rbody*>(b.GetBody());
                if (a_body && b_body) norm = norm / 2.0f;

                auto move = [](Object &o, Rbody *body, Body *other_body, const V3d &norm) {

                    float max_step = -0.5f;
                    if (o.ContainsProperty(Obj::Property::walks)) {
                        V3d diff = body->FurthestPointIn(-V3d::unit_y) - other_body->FurthestPointIn(V3d::unit_y);
                        if (diff.y < 0 && diff.y > max_step) {
                            o.Move(V3d{0.0f, -diff.y + 0.001f, 0.0f});
                            return;
                        }
                    }
                    o.Move(norm);
                    body->SetVelocity(V3d::origin);
                };
                if (a_body) move(a, a_body, b.GetBody(), norm);
                if (b_body) move(b, b_body, a.GetBody(), -norm);
            }
        }
    }
    return true;
}

void GameEngine::Render() {
    m_window.clear(0x00000000);
    m_window.clear_depth_buffer();
    
    for (Object &o : m_objects) {
        if (o.ContainsProperty(Obj::Property::no_render)) continue;
        std::list<Triangle> clipped = Render::GetClippedTriangles(*o.GetMesh(), *player.GetCamera(), m_window.getWidth(), m_window.getHeight());
        Texture *texture = o.GetTexture();
        if (!texture) texture = m_default;
        for (Triangle &t : clipped) m_window.drawTriangle(t, *texture);
    }
    // std::list<Triangle> clipped = Render::GetClippedTriangles(*m_objects[0].GetMesh(), *player.GetCamera(), m_window.getWidth(), m_window.getHeight());
    // for (Triangle &t : clipped) m_window.drawTriangle(t, m_temp);
    
    m_window.update();
}