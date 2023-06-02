#pragma once

#include "M4x4.hpp"
#include "V3d.hpp"

namespace MathUtil {
    V3d Mat4x4VecMult(M4x4 &m, V3d &v);

    float DotProd(V3d &v1, V3d &v2);

    V3d CrossProd(V3d &v1, V3d &v2);

    M4x4 GetIdentityMat();

    M4x4 GetMatRotX(float angle);

    M4x4 GetMatRotY(float angle);

    M4x4 GetMatRotZ(float angle);

    M4x4 GetProjMat(float fov_deg, float aspect_ratio, float view_near, float view_far);

    M4x4 MatTranslate(float x, float y, float z);

    float max(float a, float b);

    float min(float a, float b);
}