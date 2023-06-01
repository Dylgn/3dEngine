#include <fstream>
#include <sstream>
#include <algorithm>
#include "olcConsoleGameEngine.h"

struct V3 { float x,y,z; };
struct Triangle { 
    V3 p[3];
    wchar_t sym;
	short col;
};
struct Mesh { 
    std::vector<Triangle> triangles;

    bool LoadObject(std::string file_name) {
        std::ifstream f(file_name);
        if (!f.is_open()) return false;

        // Temp storage of vertices
        std::vector<V3> vertices;

        while (!f.eof()) {
            char line[128];
            f.getline(line, 128);

            std::stringstream s;
            s << line;

            char type;
            if (line[0] == 'v') {
                V3 v;
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
struct M4x4 { float m[4][4] = {0}; };

class Engine3D : public olcConsoleGameEngine {
    public:
        Engine3D() { m_sAppName = L"Demo"; }

        bool OnUserCreate() override {
            // mesh_cube.triangles = {
            //     // SOOUTH 
            //     { 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f },
            //     { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f },
            //     // EAST
            //     { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f },
            //     { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f },
            //     // NORTH
            //     { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f },
            //     { 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f },
            //     // WEST
            //     { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f },
            //     { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f },
            //     // TOP
            //     { 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f },
            //     { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
            //     // BOTTOM
            //     { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f },
            //     { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f }
            // };
            mesh_cube.LoadObject("../resources/test.obj");

            // Projection Matrix
            float fNear = 0.1f;
            float fFar = 1000.0f;
            float fFov = 90.0f;
            float fAspectRatio = (float) ScreenHeight() / (float) ScreenWidth();
            float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

            // Camera
            v_cam = {0,0,0};

            mat_proj.m[0][0] = fAspectRatio * fFovRad;
            mat_proj.m[1][1] = fFovRad;
            mat_proj.m[2][2] = fFar / (fFar - fNear);
            mat_proj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            mat_proj.m[2][3] = 1.0f;
            mat_proj.m[3][3] = 0.0f;
            return true;
        }
        bool OnUserUpdate(float fElapsedTime) override {
            // Clear screen
            Fill(0,0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

            // Rotation matrices
            M4x4 mat_rot_z, mat_rot_x;
            fTheta += 1.0f * fElapsedTime;
            // Z Rotation
            mat_rot_z.m[0][0] = cosf(fTheta);
            mat_rot_z.m[0][1] = sinf(fTheta);
            mat_rot_z.m[1][0] = -sinf(fTheta);
            mat_rot_z.m[1][1] = cosf(fTheta);
            mat_rot_z.m[2][2] = 1;
            mat_rot_z.m[3][3] = 1;
            // X Rotation
            mat_rot_x.m[0][0] = 1;
            mat_rot_x.m[1][1] = cosf(fTheta * 0.5f);
            mat_rot_x.m[1][2] = sinf(fTheta * 0.5f);
            mat_rot_x.m[2][1] = -sinf(fTheta * 0.5f);
            mat_rot_x.m[2][2] = cosf(fTheta * 0.5f);
            mat_rot_x.m[3][3] = 1;

            std::vector<Triangle> triangles_to_render;

            // Draw triangles
            for (Triangle t : mesh_cube.triangles) {
                Triangle t_proj, t_translate, t_rot_z, t_rot_zx;

                // Z Rotation
                MultMatrixVector(t.p[0], t_rot_z.p[0], mat_rot_z);
                MultMatrixVector(t.p[1], t_rot_z.p[1], mat_rot_z);
                MultMatrixVector(t.p[2], t_rot_z.p[2], mat_rot_z);
                // X Rotation
                MultMatrixVector(t_rot_z.p[0], t_rot_zx.p[0], mat_rot_x);
                MultMatrixVector(t_rot_z.p[1], t_rot_zx.p[1], mat_rot_x);
                MultMatrixVector(t_rot_z.p[2], t_rot_zx.p[2], mat_rot_x);

                // Offset objects 3 units so they are correctly in view
                t_translate = t_rot_zx;
                t_translate.p[0].z = t_rot_zx.p[0].z + 8.0f;
                t_translate.p[1].z = t_rot_zx.p[1].z + 8.0f;
                t_translate.p[2].z = t_rot_zx.p[2].z + 8.0f;

                // Get normals of triangle
                V3 norm, line1, line2;
                line1.x = t_translate.p[1].x - t_translate.p[0].x;
                line1.y = t_translate.p[1].y - t_translate.p[0].y;
                line1.z = t_translate.p[1].z - t_translate.p[0].z;

                line2.x = t_translate.p[2].x - t_translate.p[0].x;
                line2.y = t_translate.p[2].y - t_translate.p[0].y;
                line2.z = t_translate.p[2].z - t_translate.p[0].z;
                // Cross product
                norm.x = line1.y * line2.z - line1.z * line2.y;
                norm.y = line1.z * line2.x - line1.x * line2.z;
                norm.z = line1.x * line2.y - line1.y * line2.x;

                float l = sqrtf(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
                norm.x /= l; norm.y /= l; norm.z /= l;

                //if (norm.z < 0.0f) {
                if (norm.x * (t_translate.p[0].x - v_cam.x) +
                    norm.y * (t_translate.p[0].y - v_cam.y) +
                    norm.z * (t_translate.p[0].z - v_cam.z) < 0.0f) {
                    
                    // Illumination
                    V3 light_dir = {0.0f, 0.0f, -1.0f};
                    float l = sqrtf(light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z);
                    light_dir.x /= l; light_dir.y /= l; light_dir.z /= l;
                    // Dot Product
                    float dp = norm.x * light_dir.x + norm.y * light_dir.y + norm.z * light_dir.z;

				    // Choose console colours as required (much easier with RGB)
				    CHAR_INFO c = GetColour(dp);
				    t_translate.col = c.Attributes;
				    t_translate.sym = c.Char.UnicodeChar;

                    // Project into screen space
                    MultMatrixVector(t_translate.p[0], t_proj.p[0], mat_proj);
                    MultMatrixVector(t_translate.p[1], t_proj.p[1], mat_proj);
                    MultMatrixVector(t_translate.p[2], t_proj.p[2], mat_proj);
                    t_proj.col = t_translate.col;
				    t_proj.sym = t_translate.sym;

                    // Scale to view
                    t_proj.p[0].x += 1.0f; t_proj.p[0].y += 1.0f;
                    t_proj.p[1].x += 1.0f; t_proj.p[1].y += 1.0f;
                    t_proj.p[2].x += 1.0f; t_proj.p[2].y += 1.0f;

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
        V3 v_cam;
        float fTheta;

        void MultMatrixVector(V3 &i, V3 &o, M4x4 &m) {
            o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
            o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
            o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
            float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

            if (w != 0.0f) {
                o.x /= w;
                o.y /= w;
                o.z /= w;
            }
        }

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