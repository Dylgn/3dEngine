#include <fstream>
#include <sstream>
#include <algorithm>

#include "olcConsoleGameEngine.h"

#include "MathUtility.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"

// struct V3 { float x,y,z; };
struct Triangle { 
    V3d p[3];
    wchar_t sym;
	short col;
};
struct Mesh { 
    std::vector<Triangle> triangles;

    bool LoadObject(std::string file_name) {
        std::ifstream f(file_name);
        if (!f.is_open()) return false;

        // Temp storage of vertices
        std::vector<V3d> vertices;

        while (!f.eof()) {
            char line[128];
            f.getline(line, 128);

            std::stringstream s;
            s << line;

            char type;
            if (line[0] == 'v') {
                V3d v;
                s >> type >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            } else if (line[0] == 'f') {
                int f[3];
                s >> type >> f[0] >> f[1] >> f[2];
                triangles.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
            }
        }

        return true;
    }
};
// struct M4x4 { float m[4][4] = {0}; };

class Engine3D : public olcConsoleGameEngine {
    public:
        Engine3D() { m_sAppName = L"Demo"; }

        bool OnUserCreate() override {
            mesh_cube.LoadObject("../resources/test.obj");

            // Projection Matrix
            mat_proj = MathUtil::GetProjMat(90.0f, ((float) ScreenHeight()) / ((float) ScreenWidth()), 0.1f, 1000.0f);

            return true;
        }
        bool OnUserUpdate(float fElapsedTime) override {
            // Clear screen
            Fill(0,0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

            // Rotation matrices
            M4x4 mat_rot_z, mat_rot_x;
            theta += 1.0f * fElapsedTime;
            mat_rot_z = MathUtil::GetMatRotZ(theta * 0.5f);
            mat_rot_x = MathUtil::GetMatRotX(theta);

            // Translate matrix
            M4x4 mat_translate = MathUtil::MatTranslate(0.0f, 0.0f, 5.0f);
            // World matrix
            M4x4 mat_world = (mat_rot_z * mat_rot_x) * mat_translate;

            std::vector<Triangle> triangles_to_render;

            // Draw triangles
            for (Triangle t : mesh_cube.triangles) {
                Triangle t_proj, t_transform;

                // Transform with world matrix
                t_transform.p[0] = mat_world * t.p[0];
                t_transform.p[1] = mat_world * t.p[1];
                t_transform.p[2] = mat_world * t.p[2];

                // Calculate normal of t
                V3d norm, line1, line2;
                // Get 2 sides of triangle
                line1 = t_transform.p[1] - t_transform.p[0];
                line2 = t_transform.p[2] - t_transform.p[0];
                // Get normal from cross product
                norm = line1.crossProd(line2);
                norm = norm.normalize();

                // Ray from triangle to camera
                V3d cam_ray = t_transform.p[0] - cam;
                // t visible if ray aligned with normal
                if (norm.dotProd(cam_ray) < 0.0f) {
                    // Illumination
                    V3d light_dir = { 0.0f, 1.0f, -1.0f };
                    light_dir = light_dir.normalize();
                    // Alignment of light direction & normal
                    float dp = MathUtil::max(0.1f, light_dir.dotProd(norm));

				    // Choose console colours as required (much easier with RGB)
				    CHAR_INFO c = GetColour(dp);
				    t_transform.col = c.Attributes;
				    t_transform.sym = c.Char.UnicodeChar;

                    // Project into screen space
                    t_proj.p[0] = mat_proj * t_transform.p[0];
                    t_proj.p[1] = mat_proj * t_transform.p[1];
                    t_proj.p[2] = mat_proj * t_transform.p[2];
                    t_proj.col = t_transform.col;
                    t_proj.sym = t_transform.sym;

                    // Normalize
                    t_proj.p[0] = t_proj.p[0] / t_proj.p[0].w;
                    t_proj.p[1] = t_proj.p[1] / t_proj.p[1].w;
                    t_proj.p[2] = t_proj.p[2] / t_proj.p[2].w;

                    // Offset vertices into visible normalized space
                    V3d view_offset = { 1, 1, 0 };
                    t_proj.p[0] = t_proj.p[0] + view_offset;
                    t_proj.p[1] = t_proj.p[1] + view_offset;
                    t_proj.p[2] = t_proj.p[2] + view_offset;
                    t_proj.p[0].x *= 0.5f * (float) ScreenWidth();
                    t_proj.p[0].y *= 0.5f * (float) ScreenHeight();
                    t_proj.p[1].x *= 0.5f * (float) ScreenWidth();
                    t_proj.p[1].y *= 0.5f * (float) ScreenHeight();
                    t_proj.p[2].x *= 0.5f * (float) ScreenWidth();
                    t_proj.p[2].y *= 0.5f * (float) ScreenHeight();

                    // Store triangles for sorting
                    triangles_to_render.push_back(t_proj);
                }
            }

            // Sort triangles back to front
            sort(triangles_to_render.begin(), triangles_to_render.end(), [](Triangle &t1, Triangle &t2) {
                float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
                float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
                return z1 > z2;
            });

            for (Triangle &t: triangles_to_render) {
                FillTriangle(
                    t.p[0].x, t.p[0].y,
                    t.p[1].x, t.p[1].y,
                    t.p[2].x, t.p[2].y,
                    t.sym, t.col);
                // DrawTriangle(
                //     t.p[0].x, t.p[0].y,
                //     t.p[1].x, t.p[1].y,
                //     t.p[2].x, t.p[2].y,
                //     PIXEL_SOLID, FG_BLACK);
            }
            return true;
        }
    private:
        Mesh mesh_cube;
        M4x4 mat_proj;
        V3d cam;
        float theta;

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
};

int main() {
    Engine3D engine;

    if (engine.ConstructConsole(256,240,4,4)) {
        engine.Start();
    } // else error check
}