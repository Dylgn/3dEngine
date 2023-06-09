#pragma once

#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"

namespace MathUtil {
    V3d Mat4x4VecMult(M4x4 &m, V3d &v);

    float DotProd(V3d &v1, V3d &v2);

    V3d CrossProd(V3d &v1, V3d &v2);

    V3d LineIntersectPlane(V3d &plane_point, V3d &plane_norm, V3d &line_start, V3d &line_end, float &t);

    M4x4 GetIdentityMat();

    M4x4 GetMatRotX(float angle);

    M4x4 GetMatRotY(float angle);

    M4x4 GetMatRotZ(float angle);

    M4x4 GetProjMat(float fov_deg, float aspect_ratio, float view_near, float view_far);

    M4x4 MatTranslate(float x, float y, float z);

    M4x4 MatPointAt(V3d &pos, V3d &target, V3d &up);

    M4x4 InvertRotTransMat(M4x4 &m);

    int TriangleClipPlane(V3d plane_point, V3d plane_norm, Triangle &in_triangle, Triangle &out_triangle1, Triangle &out_triangle2);

    float max(float a, float b);

    float min(float a, float b);
}