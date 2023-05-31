#include "olcConsoleGameEngine.h"

struct V3 { float x,y,z; };
struct Triangle { V3 p[3]; };
struct Mesh { std::vector<Triangle> triangles; };
struct M4x4 { float m[4][4] = {0}; };

class Engine3D : public olcConsoleGameEngine {
    public:
        Engine3D() { m_sAppName = L"Demo"; }

        bool OnUserCreate() override {
            mesh_cube.triangles = {
                // SOOUTH 
                { 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f },
                // EAST
                { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f },
                // NORTH
                { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f },
                // WEST
                { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f },
                // TOP
                { 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
                // BOTTOM
                { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f }
            };

            // Projection Matrix
            float fNear = 0.f;
            float fFar = 1000.0f;
            float fFov = 90.0f;
            float fAspectRatio = (float) ScreenHeight() / (float) ScreenWidth();
            float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

            mat_proj.m[0][0] = fAspectRatio * fFovRad;
            mat_proj.m[1][1] = fFovRad;
            mat_proj.m[2][2] = fFar / (fFar - fNear);
            mat_proj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
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
            mat_rot_z.m[2][2] = 1.0f;
            mat_rot_z.m[3][3] = 1.0f;
            // X Rotation
            mat_rot_x.m[0][0] = 1.0f;
            mat_rot_x.m[1][1] = cosf(fTheta * 0.5f);
            mat_rot_x.m[1][2] = sinf(fTheta * 0.5f);
            mat_rot_x.m[2][1] = -sinf(fTheta * 0.5f);
            mat_rot_x.m[2][2] = cosf(fTheta * 0.5f);
            mat_rot_x.m[3][3] = 1.0f;

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
                t_translate.p[0].z = t_rot_zx.p[0].z + 3.0f;
                t_translate.p[1].z = t_rot_zx.p[1].z + 3.0f;
                t_translate.p[2].z = t_rot_zx.p[2].z + 3.0f;

                MultMatrixVector(t_translate.p[0], t_proj.p[0], mat_proj);
                MultMatrixVector(t_translate.p[1], t_proj.p[1], mat_proj);
                MultMatrixVector(t_translate.p[2], t_proj.p[2], mat_proj);

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

                DrawTriangle(
                    t_proj.p[0].x, t_proj.p[0].y,
                    t_proj.p[1].x, t_proj.p[1].y,
                    t_proj.p[2].x, t_proj.p[2].y,
                    PIXEL_SOLID, FG_WHITE
                );
            }
            return true;
        }
    private:
        Mesh mesh_cube;
        M4x4 mat_proj;
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
};

int main() {
    Engine3D engine;

    if (engine.ConstructConsole(256,240,4,4)) {
        engine.Start();
    } // else error check
}