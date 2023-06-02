#include <cmath>
#include "MathUtility.hpp"

namespace MathUtil {
    V3d Mat4x4VecMult(M4x4 &m, V3d &v) {
        return {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
            v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]
        };
    }

    float DotProd(V3d &v1, V3d &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    V3d CrossProd(V3d &v1, V3d &v2) {
        return {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        };
    }

    M4x4 GetIdentityMat() {
        M4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = 1.0f;
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotX(float angle) {
        M4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = cosf(angle);
        m.m[1][2] = sinf(angle);
        m.m[2][1] = -sinf(angle);
        m.m[2][2] = cosf(angle);
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotY(float angle) {
        M4x4 m;
        m.m[0][0] = cosf(angle);
        m.m[0][2] = sinf(angle);
        m.m[2][0] = -sinf(angle);
        m.m[1][1] = 1.0f;
        m.m[2][2] = cosf(angle);
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotZ(float angle) {
        M4x4 m;
        m.m[0][0] = cosf(angle);
        m.m[0][1] = sinf(angle);
        m.m[1][0] = -sinf(angle);
        m.m[1][1] = cosf(angle);
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetProjMat(float fov_deg, float aspect_ratio, float view_near, float view_far) {
        float fov_rad = 1.0f / tanf(fov_deg * 0.5f / 180.0f * 3.14159f);
        M4x4 m;
        m.m[0][0] = aspect_ratio * fov_rad;
        m.m[1][1] = fov_rad;
        m.m[2][2] = view_far / (view_far - view_near);
        m.m[3][2] = (-view_far * view_near) / (view_far - view_near);
        m.m[2][3] = 1.0f;
        m.m[3][3] = 0.0f;
        return m;
    }

    M4x4 MatTranslate(float x, float y, float z) {
        M4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = 1.0f;
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        m.m[3][0] = x;
        m.m[3][1] = y;
        m.m[3][2] = z;
        return m;
    }

    float max(float a, float b) {
        if (a >= b) return a;
        else return b;
    }

    float min(float a, float b) {
        if (a >= b) return b;
        else return a;
    }
}