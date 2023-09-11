#include <list>
#include <chrono>
#include <cmath>
#include "GameEngine.hpp"
#include "MathUtility.hpp"
#include "Triangle.hpp"
#include "Geometry.hpp"

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
        if (!OnUpdate(elapsed_time.count())) return;
        time_prev = time_now;

        Render();
    }
}

bool GameEngine::KeyDown(const int &virt_key) {
    return m_window.KeyDown(virt_key);
}

/*
bool GameEngine::PhysicsStep(const float &elapsed_time) {
    for (Object &o : m_objects) {
        Rigidbody *body;
        if (body = dynamic_cast<Rigidbody*>(o.GetBody())) {
            // V3d velo = body->GetVelocity();
            // body->SetVelocity(V3d{velo.x * 0.9f * elapsed_time, velo.y, velo.z * 0.9f * elapsed_time});
            // std::cout << velo.z << " " << elapsed_time << std::endl;

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
                body->SetVelocity(MathUtil::RemoveDirectionFrom(body->GetVelocity(), V3d::unit_y));
                body->SetVelocity(body->GetVelocity() * 0.9f);
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
                Rigidbody *a_body = dynamic_cast<Rigidbody*>(a.GetBody());
                Rigidbody *b_body = dynamic_cast<Rigidbody*>(b.GetBody());

                float a_mass = a_body ? (1.0f / a_body->GetMass()) : 1.0f;
                float b_mass = b_body ? (1.0f / b_body->GetMass()) : 1.0f;

                // Position Solver
                
                const float percent = 0.8f;
                const float slop = 0.001f;
                
                V3d correction = norm * percent * static_cast<float>(fmax(norm.length() - slop, 0.0f)) / (a_mass + b_mass);

                V3d deltaA, deltaB;

                if (a_body) {
                    deltaA = correction * a_mass;
                    a.Move(deltaA);
                }
                if (b_body) {
                    deltaB = correction * b_mass;
                    b.Move(-deltaB);
                }

                // Impulse Solver

                V3d a_velo = a_body ? a_body->GetVelocity() : V3d::origin;
                V3d b_velo = b_body ? b_body->GetVelocity() : V3d::origin;
                V3d r_velo = b_velo - a_velo;
                float spd = r_velo.dot(norm);

                if (spd >= 0) continue;

                // restitution
                float e = 1.0f; (a_body ? a_body->GetRestitution() : 1.0f) * ( b_body ? b_body->GetRestitution() : 1.0f );
                
                float j = -(1.0f + e) * spd / (a_mass + b_mass);

                V3d impulse = norm * j;
                if (a_body) a_velo = a_velo - (impulse * a_mass);
                if (b_body) b_velo = b_velo + (impulse * b_mass);

                // Friction

                r_velo = b_velo - a_velo;
                spd = r_velo.dot(norm);

                V3d tangent = r_velo - norm * spd;

                if (tangent.length() > 0.0001f) tangent = tangent.normalize();

                float f_velo = r_velo.dot(tangent);

                // Static/Dynamic Friction
                float a_sf = 0.5f;
                float b_sf = 0.5f;
                float a_df = 0.5f;
                float b_df = 0.5f;
                float mu = V3d{a_sf, b_sf, 0.0f}.length();

                float f = -f_velo / (a_mass + b_mass);

                V3d friction;
                if (abs(f) < j * mu) friction = tangent * f;
                else {
                    mu = V3d{a_df, b_df, 0.0f}.length();
                    friction = tangent * -j * mu;
                }

                if (a_body) a_body->SetVelocity(a_velo - friction * a_mass);
                if (b_body) b_body->SetVelocity(b_velo + friction * b_mass);

                // if (a_body && b_body) norm = norm / 2.0f;

                // auto move = [](Object &o, Rigidbody *body, Body *other_body, const V3d &norm) {
                //     float max_step = -0.2f;
                //     if (o.ContainsProperty(Obj::Property::walks)) {
                //         V3d diff = body->FurthestPointIn(-V3d::unit_y) - other_body->FurthestPointIn(V3d::unit_y);
                //         if (diff.y < 0 && diff.y > max_step) {
                //             o.Move(V3d{0.0f, -diff.y + 0.001f, 0.0f});
                //             return;
                //         }
                //     }
                //     o.Move(norm);
                //     V3d velo = MathUtil::RemoveDirectionFrom(body->GetVelocity(), norm);
                //     velo.x *= 0.3f;
                //     velo.z *= 0.3f;
                //     body->SetVelocity(velo);
                // };

                // if (a_body) move(a, a_body, b.GetBody(), norm);
                // if (b_body) move(b, b_body, a.GetBody(), -norm);
            }
        }
    }
    return true;
}
*/

void GameEngine::Render() {
    m_window.clear(0x00000000);
    m_window.clear_depth_buffer();

    std::list<Triangle> clipped = Render::GetClippedTriangles(*m_objects[0].GetMesh(), *player.GetCamera(), m_window.getWidth(), m_window.getHeight());
    for (Triangle &t : clipped) m_window.drawTriangle(t, m_temp);
    
    m_window.update();
}