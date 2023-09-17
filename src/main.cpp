#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>

#include "MathUtility.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "V2d.hpp"
#include "Geometry.hpp"
#include "Window.hpp"
#include "GameEngine.hpp"
#include "json.hpp"
#include "PolyCollider.hpp"
#include "Collision.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "ObjectUtility.hpp"

class BasicGameEngine: public GameEngine {
    public:
        BasicGameEngine(int width = 640, int height = 480, float fov_deg = 90.0f, const wchar_t *title = L""):
            GameEngine{width, height, fov_deg, title} {}
        ~BasicGameEngine() override {}
        
        bool OnStart() override {
            player.SetBody(ObjectUtil::GetRigidBox(0.4f, 2.0f, 0.4f));
            player.SetPos({0.5f, 3.0f, 0.5f});
            player.AddProperty(Obj::Property::gravity);
            player.AddProperty(Obj::Property::walks);
            player.AddProperty(Obj::Property::no_render);

            // m_objects.emplace_back("../resources/ramp2.obj", "../resources/brick.bmp");
            m_objects.emplace_back("../resources/scene01.obj", "../resources/grass.bmp");
            m_objects.emplace_back("../resources/scene02.obj", "../resources/box.bmp");
            m_objects.emplace_back("../resources/scene03.obj", "../resources/redbrick.bmp");
            m_objects.emplace_back("../resources/scene04.obj", "../resources/stone.bmp");
            m_objects.emplace_back("../resources/scene05.obj", "../resources/stone.bmp");
            m_objects.emplace_back("../resources/scene06.obj", "../resources/stone.bmp");
            m_objects.emplace_back("../resources/scene07.obj", "../resources/stone.bmp");
            m_objects.emplace_back("../resources/scene08.obj", "../resources/stone.bmp");
            m_objects.push_back(player);

            return true;
        }

        bool OnUpdate(const float &elapsed_time) override {
            Camera *cam = player.GetCamera();
            V3d forward = cam->look_dir * (8.0f * elapsed_time);
            if (KeyDown(VK_W)) {
                player.Move(forward);
            }
            if (KeyDown(VK_S)) {
                forward = -forward;
                player.Move(forward);
            }
            if (KeyDown(VK_A)) cam->yaw += 2.0f * elapsed_time;
            if (KeyDown(VK_D)) cam->yaw -= 2.0f * elapsed_time;
            if (KeyDown(0x51)) {
                player.SetPos(V3d{player.GetPos().x, 10.0f, player.GetPos().z});
            }

            //auto pos = player.GetPos();
            //std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

            //std::cout << 1 / elapsed_time << std::endl;
            return true;
        }
};

int main() {
    // Engine3D engine;

    // if (engine.ConstructConsole(256,240,4,4)) {
    //     engine.Start();
    // } // else error check

    auto settings = json::ReadJSON("../settings/settings.json");
    float width = settings["width"].as<float>();
    float height = settings["height"].as<float>();
    
    BasicGameEngine engine{static_cast<int>(width), static_cast<int>(height)};
    engine.Start();
}