#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>

#include "olcConsoleGameEngine.h"

#include "MathUtility.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"
#include "V2d.hpp"

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

class Engine3D : public olcConsoleGameEngine {
    public:
        Engine3D() { m_sAppName = L"Demo"; }

        bool OnUserCreate() override {
            //mesh_cube.LoadObject("../resources/cube.obj");
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
            mat_proj = MathUtil::GetProjMat(90.0f, ((float) ScreenHeight()) / ((float) ScreenWidth()), 0.1f, 1000.0f);

            return true;
        }
        bool OnUserUpdate(float fElapsedTime) override {

            if (GetKey(VK_UP).bHeld) cam.y += 8.0f * fElapsedTime;
            if (GetKey(VK_DOWN).bHeld) cam.y -= 8.0f * fElapsedTime;
            if (GetKey(VK_LEFT).bHeld) cam.x -= 8.0f * fElapsedTime;
            if (GetKey(VK_RIGHT).bHeld) cam.x += 8.0f * fElapsedTime;
            V3d forward = look_dir * (8.0f * fElapsedTime);
            if (GetKey(L'W').bHeld) cam = cam + forward;
            if (GetKey(L'S').bHeld) cam = cam - forward;
            if (GetKey(L'A').bHeld) yaw += 2.0f * fElapsedTime;
            if (GetKey(L'D').bHeld) yaw -= 2.0f * fElapsedTime;

            // Clear screen
            Fill(0,0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

            // Clear Depth buffer
            for (int i = 0; i < ScreenWidth() * ScreenHeight(); ++i) pDebthBuffer[i] = 0.0f;

            // Rotation matrices
            M4x4 mat_rot_z, mat_rot_x;
            //theta += 1.0f * fElapsedTime;

            mat_rot_z = MathUtil::GetMatRotZ(theta * 0.5f);
            mat_rot_x = MathUtil::GetMatRotX(theta);

            // Translate matrix
            M4x4 mat_translate = MathUtil::MatTranslate(0.0f, 0.0f, 8.0f);
            // World matrix
            M4x4 mat_world = (mat_rot_z * mat_rot_x) * mat_translate;

            V3d up = { 0,1,0 };
            V3d target = { 0,0,1 };
            M4x4 mat_cam_rot = MathUtil::GetMatRotY(yaw);
            look_dir = mat_cam_rot * target;
            target = cam + look_dir;
            M4x4 mat_cam = MathUtil::MatPointAt(cam, target, up);
            M4x4 mat_view = MathUtil::InvertRotTransMat(mat_cam);

            std::vector<Triangle> triangles_to_raster;

            // Draw triangles
            for (Triangle t : mesh_cube.triangles) {
                Triangle t_proj, t_transform, t_viewed;

                // Transform with world matrix
                t_transform.copyPoints(t.multPoints(mat_world));
                t_transform.copyTexture(t);

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

                    // World space to view space
                    t_viewed.copyPoints(t_transform.multPoints(mat_view));
                    t_viewed.sym = t_transform.sym;
                    t_viewed.col = t_transform.col;
                    t_viewed.copyTexture(t_transform);

                    // Clip against near plane (at most 2 triangles)
                    int clipped_triangle_count = 0;
                    Triangle clipped[2];
                    clipped_triangle_count = MathUtil::TriangleClipPlane({ 0.0f, 0.0f, 0.1f }, {0.0f, 0.0f, 1.0f }, t_viewed, clipped[0], clipped[1]);

                    for (int i = 0; i < clipped_triangle_count; ++i) {
                        // Project into screen space
                        t_proj.copyPoints(clipped[i].multPoints(mat_proj));
                        t_proj.col = clipped[i].col;
                        t_proj.sym = clipped[i].sym;
                        t_proj.copyTexture(clipped[i]);

                        // Fix distortion of textures caused by perspective
                        for (int i = 0; i < 3; ++i) {
                            t_proj.t[i].u = t_proj.t[i].u / t_proj.p[i].w;
                            t_proj.t[i].v = t_proj.t[i].v / t_proj.p[i].w;
                            t_proj.t[i].w = 1.0f / t_proj.p[i].w;
                        }

                        // Normalize
                        t_proj.normalizePoints();

                        // Invert y axis
                        t_proj.p[0].y *= -1.0f;
                        t_proj.p[1].y *= -1.0f;
                        t_proj.p[2].y *= -1.0f;

                        // Offset vertices into visible normalized space
                        V3d view_offset = { 1, 1, 0 };
                        t_proj.copyPoints(t_proj.addPoints(view_offset));
                        t_proj.p[0].x *= 0.5f * (float) ScreenWidth();
                        t_proj.p[0].y *= 0.5f * (float) ScreenHeight();
                        t_proj.p[1].x *= 0.5f * (float) ScreenWidth();
                        t_proj.p[1].y *= 0.5f * (float) ScreenHeight();
                        t_proj.p[2].x *= 0.5f * (float) ScreenWidth();
                        t_proj.p[2].y *= 0.5f * (float) ScreenHeight();

                        // Store triangles for sorting
                        triangles_to_raster.push_back(t_proj);
                    }
                }
            }

            // Sort triangles back to front
            // sort(triangles_to_raster.begin(), triangles_to_raster.end(), [](Triangle &t1, Triangle &t2) {
            //     float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
            //     float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
            //     return z1 > z2;
            // });

            for (Triangle &t: triangles_to_raster) {
                // Clip triangles against screen edges
                Triangle clipped[2];
                std::list<Triangle> t_list;
                t_list.push_back(t);
                int new_triangles = 1;

                for (int p = 0; p < 4; ++p) {
                    int triangles_to_add = 0;
                    while (new_triangles > 0) {
                        Triangle clip = t_list.front();
                        t_list.pop_front();
                        --new_triangles;

                        // Clip triangle against each plane. New triangles will not clip against earlier planes
                        switch (p) {
                            case 0:
                                triangles_to_add = MathUtil::TriangleClipPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, clip, clipped[0], clipped[1]);
                                break;
                            case 1:
                                triangles_to_add = MathUtil::TriangleClipPlane({0.0f, ((float) ScreenHeight()) - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, clip, clipped[0], clipped[1]);
                                break;
                            case 2:
                                triangles_to_add = MathUtil::TriangleClipPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, clip, clipped[0], clipped[1]);
                                break;
                            case 3:
                                triangles_to_add = MathUtil::TriangleClipPlane({ ((float) ScreenWidth()) - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, clip, clipped[0], clipped[1]);
                                break;
                        }

                        // Add new triangles to queue
                        for (int i = 0; i < triangles_to_add; ++i) t_list.push_back(clipped[i]);
                    }
                    new_triangles = t_list.size();
                }

                for (Triangle &t : t_list) {
                    TextureTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
                        t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
                        t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w,
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
            }
            return true;
        }
    private:
        Mesh mesh_cube;
        M4x4 mat_proj;
        V3d cam;
        V3d look_dir;
        float theta = 0;
        float yaw = 0;

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

        void TextureTriangle(int x1, int y1, float u1, float v1, float w1,
            int x2, int y2, float u2, float v2, float w2,
            int x3, int y3, float u3, float v3, float w3,
            olcSprite *tex) {
                if (y2 < y1) {
                    std::swap(y1, y2);
                    std::swap(x1, x2);
                    std::swap(u1, u2);
                    std::swap(v1, v2);
                    std::swap(w1, w2);
                }
                if (y3 < y1) {
                    std::swap(y1, y3);
                    std::swap(x1, x3);
                    std::swap(u1, u3);
                    std::swap(v1, v3);
                    std::swap(w1, w3);
                }
                if (y3 < y2) {
                    std::swap(y2, y3);
                    std::swap(x2, x3);
                    std::swap(u2, u3);
                    std::swap(v2, v3);
                    std::swap(w2, w3);
                }
                int dy1 = y2 - y1;
                int dx1 = x2 - x1;
                float dv1 = v2 - v1;
                float du1 = u2 - u1;
                float dw1 = w2 - w1;

                int dy2 = y3 - y1;
                int dx2 = x3 - x1;
                float dv2 = v3 - v1;
                float du2 = u3 - u1;
                float dw2 = w3 - w1;

                float tex_u, tex_v, tex_w;

                float dax_step = 0, dbx_step = 0,
                du1_step = 0, dv1_step = 0,
                du2_step = 0, dv2_step = 0,
                dw1_step = 0, dw2_step = 0;

                if (dy1) dax_step = dx1 / ((float) abs(dy1));
                if (dy2) dbx_step = dx2 / ((float) abs(dy2));

                if (dy1) du1_step = du1 / ((float) abs(dy1));
                if (dy1) dv1_step = dv1 / ((float) abs(dy1));
                if (dy1) dw1_step = dw1 / ((float) abs(dy1));

                if (dy2) du2_step = du2 / ((float) abs(dy2));
                if (dy2) dv2_step = dv2 / ((float) abs(dy2));
                if (dy2) dw2_step = dw2 / ((float) abs(dy2));

                if (dy1) { // Top half of triangle (drawing from 1 vertex to 2 vertices)
                    for (int i = y1; i <= y2; ++i) {
                        int ax = x1 + ((float)(i - y1)) * dax_step;
                        int bx = x1 + ((float)(i - y1)) * dbx_step;

                        float tex_su = u1 + ((float)(i - y1)) * du1_step;
                        float tex_sv = v1 + ((float)(i - y1)) * dv1_step;
                        float tex_sw = w1 + ((float)(i - y1)) * dw1_step;

                        float tex_eu = u1 + ((float)(i - y1)) * du2_step;
                        float tex_ev = v1 + ((float)(i - y1)) * dv2_step;
                        float tex_ew = w1 + ((float)(i - y1)) * dw2_step;

                        if (ax > bx) {
                            std::swap(ax, bx);
                            std::swap(tex_su, tex_eu);
                            std::swap(tex_sv, tex_ev);
                            std::swap(tex_sw, tex_ew);
                        }

                        tex_u = tex_su;
                        tex_v = tex_sv;
                        tex_w = tex_sw;

                        float tstep = 1.0f / ((float) (bx - ax));
                        float t = 0.0f;

                        for (int j = ax; j < bx; ++j) {
                            tex_u = (1.0f - t) * tex_su + t * tex_eu;
                            tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                            tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                            // Only draw if greater than depth buffer (in front of whats on screen)
                            if (tex_w > pDebthBuffer[i * ScreenWidth() + j]) {
                                Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
                                pDebthBuffer[i * ScreenWidth() + j] = tex_w;
                            }

                            t += tstep;
                        }
                    }
                }

                dy1 = y3 - y2;
                dx1 = x3 - x2;
                dv1 = v3 - v2;
                du1 = u3 - u2;
                dw1 = w3 - w2;

                if (dy1) dax_step = dx1 / ((float) abs(dy1));
                if (dy2) dbx_step = dx2 / ((float) abs(dy2));
                
                du1_step = 0, dv1_step = 0;
                if (dy1) du1_step = du1 / ((float) abs(dy1));
                if (dy1) dv1_step = dv1 / ((float) abs(dy1));
                if (dy1) dw1_step = dw1 / ((float) abs(dy1));

                for (int i = y2; i <= y3; ++i) { // bottom half of triangle
                    int ax = x2 + ((float) (i - y2)) * dax_step;
                    int bx = x1 + ((float) (i - y1)) * dbx_step;

                    float tex_su = u2 + ((float) (i - y2)) * du1_step;
                    float tex_sv = v2 + ((float) (i - y2)) * dv1_step;
                    float tex_sw = w2 + ((float) (i - y2)) * dw1_step;

                    float tex_eu = u1 + ((float) (i - y1)) * du2_step;
                    float tex_ev = v1 + ((float) (i - y1)) * dv2_step;
                    float tex_ew = w1 + ((float) (i - y1)) * dw2_step;

                    if (ax > bx) {
                        std::swap(ax, bx);
                        std::swap(tex_su, tex_eu);
                        std::swap(tex_sv, tex_ev);
                        std::swap(tex_sw, tex_ew);
                    }

                    tex_u = tex_su;
                    tex_v = tex_sv;
                    tex_w = tex_sw;

                    float tstep = 1.0f / ((float) (bx - ax));
                    float t = 0.0f;

                    for (int j = ax; j < bx; ++j) {
                        tex_u = (1.0f - t) * tex_su + t * tex_eu;
                        tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                        tex_w = (1.0f - t) * tex_sw + t * tex_ew;
                        if (tex_w > pDebthBuffer[i * ScreenWidth() + j]) {
                            Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
                            pDebthBuffer[i * ScreenWidth() + j] = tex_w;
                        }

                        t += tstep;
                    }
                }
        }
};

int main() {
    Engine3D engine;

    if (engine.ConstructConsole(256,240,4,4)) {
        engine.Start();
    } // else error check
}