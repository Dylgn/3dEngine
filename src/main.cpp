#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>

#include "olcConsoleGameEngine.h"

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

class Engine3D : public olcConsoleGameEngine {
    public:
        Engine3D() { m_sAppName = L"Demo"; }

        bool OnUserCreate() override {
            //mesh_cube.LoadObject("../resources/brick_cube.obj");
            mesh_cube.triangles = {
    		    // SOUTH
    		    { 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f}, 
    		    { 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // EAST           																			   
    		    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // NORTH           																			   
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // WEST            																			   
    		    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // TOP             																			   
    		    { 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // BOTTOM          																			  
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},
    		};
            spr_tex1 = new olcSprite(L"../resources/Sword.spr");

            pDebthBuffer = new float[ScreenWidth() * ScreenHeight()];

            // Projection Matrix
            cam.mat_proj = MathUtil::GetProjMat(90.0f, ((float) ScreenHeight()) / ((float) ScreenWidth()), 0.1f, 1000.0f);

            return true;
        }
        bool OnUserUpdate(float fElapsedTime) override {

            if (GetKey(VK_UP).bHeld) cam.pos.y += 8.0f * fElapsedTime;
            if (GetKey(VK_DOWN).bHeld) cam.pos.y -= 8.0f * fElapsedTime;
            if (GetKey(VK_LEFT).bHeld) cam.pos.x -= 8.0f * fElapsedTime;
            if (GetKey(VK_RIGHT).bHeld) cam.pos.x += 8.0f * fElapsedTime;
            V3d forward = cam.look_dir * (8.0f * fElapsedTime);
            if (GetKey(L'W').bHeld) cam.pos = cam.pos + forward;
            if (GetKey(L'S').bHeld) cam.pos = cam.pos - forward;
            if (GetKey(L'A').bHeld) cam.yaw += 2.0f * fElapsedTime;
            if (GetKey(L'D').bHeld) cam.yaw -= 2.0f * fElapsedTime;

            // Clear screen
            Fill(0,0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

            // Clear Depth buffer
            for (int i = 0; i < ScreenWidth() * ScreenHeight(); ++i) pDebthBuffer[i] = 0.0f;

            std::list<Triangle> triangles = Render::GetClippedTriangles(mesh_cube, cam, ScreenWidth(), ScreenHeight());

            for (Triangle &t : triangles) {
                TextureTriangle(t.p[0], t.t[0],
                    t.p[1], t.t[1],
                    t.p[2], t.t[2],
                    spr_tex1
                );
                // FillTriangle(
                //     t.p[0].x, t.p[0].y,
                //     t.p[1].x, t.p[1].y,
                //     t.p[2].x, t.p[2].y,
                //     t.sym, t.col
                // );
                // DrawTriangle(
                //     t.p[0].x, t.p[0].y,
                //     t.p[1].x, t.p[1].y,
                //     t.p[2].x, t.p[2].y,
                //     PIXEL_SOLID, FG_WHITE
                // );
            }
            return true;
        }
    private:
        Mesh mesh_cube;
        Camera cam;
        // M4x4 mat_proj;
        // V3d cam;
        // V3d look_dir;
        // float theta = 0;
        // float yaw = 0;

        olcSprite *spr_tex1;
        float *pDebthBuffer = nullptr;

        CHAR_INFO GetColour(float lum) {
	    	short bg_col, fg_col;
	    	wchar_t sym;
	    	int pixel_bw = (int)(13.0f*lum);
	    	switch (pixel_bw) {
	    	case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

	    	case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
	    	case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
	    	case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
	    	case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

	    	case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
	    	case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
	    	case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
	    	case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

	    	case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
	    	case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
	    	case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
	    	case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
	    	default:
	    		bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
	    	}

	    	CHAR_INFO c;
	    	c.Attributes = bg_col | fg_col;
	    	c.Char.UnicodeChar = sym;
	    	return c;
	    }

        void TextureTriangle(V3d p1, V2d t1, V3d p2, V2d t2, V3d p3, V2d t3, olcSprite *tex) {
            if (p2.y < p1.y) {
                std::swap(p1, p2);
                std::swap(t1, t2);
            }
            if (p3.y < p1.y) {
                std::swap(p1, p3);
                std::swap(t1, t3);
            }
            if (p3.y < p2.y) {
                std::swap(p2, p3);
                std::swap(t2, t3);
            }

            V2d dt1{ t2.u - t1.u, t2.v - t1.v, t2.w - t1.w};
            V2d dt2{ t3.u - t1.u, t3.v - t1.v, t3.w - t1.w};

            int dx[] = { ((int) (p2.x - p1.x)), ((int) (p3.x - p1.x)) };
            int dy[] = { ((int) (p2.y - p1.y)), ((int) (p3.y - p1.y)) };

            V2d tex_cur;

            V2d dt1_step;
            V2d dt2_step;
            float dax_step = 0, dbx_step = 0;

            if (dy[0]) {
                dax_step = dx[0] / ((float) abs(dy[0]));
                dt1_step = {
                    dt1.u / ((float) abs(dy[0])),
                    dt1.v / ((float) abs(dy[0])),
                    dt1.w / ((float) abs(dy[0]))
                };
            }
            if (dy[1]) {
                dbx_step = dx[1] / ((float) abs(dy[1]));
                dt2_step = {
                    dt2.u / ((float) abs(dy[1])),
                    dt2.v / ((float) abs(dy[1])),
                    dt2.w / ((float) abs(dy[1]))
                };
            }

            if (dy[0]) { // Top half of triangle (drawing from 1 vertex to 2 vertices)
                for (int i = p1.y; i <= p2.y; ++i) {
                    float di = i - p1.y;
                    int ax = p1.x + di * dax_step;
                    int bx = p1.x + di * dbx_step;

                    V2d tex_start{
                        t1.u + di * dt1_step.u,
                        t1.v + di * dt1_step.v,
                        t1.w + di * dt1_step.w
                    };

                    V2d tex_end{
                        t1.u + di * dt2_step.u,
                        t1.v + di * dt2_step.v,
                        t1.w + di * dt2_step.w
                    };

                    if (ax > bx) {
                        std::swap(ax, bx);
                        std::swap(tex_start, tex_end);
                    }

                    tex_cur = { tex_start.u, tex_start.v, tex_start.w };

                    float tstep = 1.0f / ((float) (bx - ax));
                    float t = 0.0f;

                    for (int j = ax; j < bx; ++j) {
                        tex_cur = {
                            (1.0f - t) * tex_start.u + t * tex_end.u,
                            (1.0f - t) * tex_start.v + t * tex_end.v,
                            (1.0f - t) * tex_start.w + t * tex_end.w
                        };
                        // Only draw if greater than depth buffer (in front of whats on screen)
                        if (tex_cur.w > pDebthBuffer[i * ScreenWidth() + j]) {
                            //Draw(j, i, BG_GREY | FG_WHITE, PIXEL_SOLID);
                            Draw(j, i, tex->SampleGlyph(tex_cur.u / tex_cur.w, tex_cur.v / tex_cur.w), tex->SampleColour(tex_cur.u / tex_cur.w, tex_cur.v / tex_cur.w));
                            pDebthBuffer[i * ScreenWidth() + j] = tex_cur.w;
                        }

                        t += tstep;
                    }
                }
            }

            dx[0] = p3.x - p2.x;
            dy[0] = p3.y - p2.y;
            dt1 = { t3.u - t2.u, t3.v - t2.v, t3.w - t2.w };

            if (dy[0]) {
                dax_step = dx[0] / ((float) abs(dy[0]));
                dt1_step = {
                    dt1.u / ((float) abs(dy[0])),
                    dt1.v / ((float) abs(dy[0])),
                    dt1.w / ((float) abs(dy[0]))
                };
            }
            if (dy[1]) dbx_step = dx[1] / ((float) abs(dy[1]));

            for (int i = p2.y; i <= p3.y; ++i) { // bottom half of triangle
                float di1 = i - p1.y;
                float di2 = i - p2.y;
                int ax = p2.x + di2 * dax_step;
                int bx = p1.x + di1 * dbx_step;

                V2d tex_start{
                    t2.u + di2 * dt1_step.u,
                    t2.v + di2 * dt1_step.v,
                    t2.w + di2 * dt1_step.w
                };

                V2d tex_end{
                    t1.u + di1 * dt2_step.u,
                    t1.v + di1 * dt2_step.v,
                    t1.w + di1 * dt2_step.w
                };

                if (ax > bx) {
                    std::swap(ax, bx);
                    std::swap(tex_start, tex_end);
                }

                tex_cur = { tex_start.u, tex_start.v, tex_start.w };

                float tstep = 1.0f / ((float) (bx - ax));
                float t = 0.0f;

                for (int j = ax; j < bx; ++j) {
                    tex_cur = {
                        (1.0f - t) * tex_start.u + t * tex_end.u,
                        (1.0f - t) * tex_start.v + t * tex_end.v,
                        (1.0f - t) * tex_start.w + t * tex_end.w
                    };
                    if (tex_cur.w > pDebthBuffer[i * ScreenWidth() + j]) {
                        //Draw(j, i, BG_GREY | FG_WHITE, PIXEL_SOLID);
                        Draw(j, i, tex->SampleGlyph(tex_cur.u / tex_cur.w, tex_cur.v / tex_cur.w), tex->SampleColour(tex_cur.u / tex_cur.w, tex_cur.v / tex_cur.w));
                        pDebthBuffer[i * ScreenWidth() + j] = tex_cur.w;
                    }

                    t += tstep;
                }
            }
        }
};

class BasicGameEngine: public GameEngine {
    public:
        BasicGameEngine(int width = 640, int height = 480, float fov_deg = 90.0f, const wchar_t *title = L""):
            GameEngine{width, height, fov_deg, title} {}
        ~BasicGameEngine() override {}
        Collider *cube_collider;
        Collider *player_collider;
        
        bool onStart() override {
            // Mesh cube;
            // cube.LoadObject("../resources/brick_cube.obj");
            // meshes.push_back(cube);
            Mesh mesh_cube;
            //mesh_cube.LoadObject("../resources/brick_cube.obj");
            mesh_cube.triangles = {
    		    // SOUTH
    		    { 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f}, 
    		    { 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // EAST           																			   
    		    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // NORTH           																			   
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // WEST            																			   
    		    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // TOP             																			   
    		    { 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},

    		    // BOTTOM          																			  
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f},
    		    { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f},
    		};
            meshes.push_back(mesh_cube);
            cube_collider = new PolyCollider({V3d{0,0,0}, V3d{0,0,1}, V3d{1,0,1}, V3d{1,0,0}, V3d{0,1,0}, V3d{0,1,1}, V3d{1,1,1}, V3d{1,1,0}});
            player_collider = new PolyCollider({V3d{-0.2f, 0.5f, -1.2f}, V3d{-0.2f, 0.5f, -0.8f}, V3d{0.2f, 0.5f, -0.8f}, V3d{0.2f, 0.5f, -1.2f},
                V3d{-0.2f, 1.5f, -1.2f}, V3d{-0.2f, 1.5f, -0.8f}, V3d{0.2f, 1.5f, -0.8f}, V3d{0.2f, 1.5f, -1.2f}});
            cam.pos = {0, 1, -1};

            return true;
        }
        bool onUpdate(const float &elapsed_time) override {
            V3d forward = cam.look_dir * (8.0f * elapsed_time);
            if (KeyDown(VK_W)) {
                cam.pos = cam.pos + forward;
                player_collider->move(forward);
            }
            if (KeyDown(VK_S)) {
                forward = forward.opposite();
                cam.pos = cam.pos + forward;
                player_collider->move(forward);
            }
            if (KeyDown(VK_A)) cam.yaw += 2.0f * elapsed_time;
            if (KeyDown(VK_D)) cam.yaw -= 2.0f * elapsed_time;
            if (KeyDown(VK_UP)) cam.pos.y += 8.0f * elapsed_time;
            if (KeyDown(VK_DOWN)) cam.pos.y -= 8.0f * elapsed_time;
            if (KeyDown(VK_LEFT)) cam.pos.x -= 8.0f * elapsed_time;
            if (KeyDown(VK_RIGHT)) cam.pos.x += 8.0f * elapsed_time;

            if (Collision::GJK(cube_collider, player_collider)) {
                cam.pos = cam.pos - forward;
                player_collider->move(forward.opposite());
            }

            std::cout << 1 / elapsed_time << std::endl;
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