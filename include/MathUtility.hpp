#pragma once

#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"

namespace MathUtil {
    /** Multiplies 4x4 matrix by vector */
    V3d Mat4x4VecMult(M4x4 &m, V3d &v);

    float DotProd(V3d &v1, V3d &v2);

    V3d CrossProd(V3d &v1, V3d &v2);

    bool SameDirection(V3d a, V3d b);

    /** Gets the point where the given line segment intersects the given plane */
    V3d LineIntersectPlane(V3d plane_point, V3d &plane_norm, V3d line_start, V3d line_end, float &t);

    M4x4 GetIdentityMat();

    M4x4 GetMatRotX(float angle);

    M4x4 GetMatRotY(float angle);

    M4x4 GetMatRotZ(float angle);

    M4x4 GetProjMat(float fov_deg, float aspect_ratio, float view_near, float view_far);

    M4x4 MatTranslate(float x, float y, float z);
    
    M4x4 MatPointAt(V3d &pos, V3d &target, V3d &up);

    /** Invert rotation/translation matrices */
    M4x4 InvertRotTransMat(M4x4 &m);

    /** Clip in_triangle against plane, output clipped triangles in out_triangle1 and out_triangle2.
     * @return Number of new triangles clipped
     */
    int TriangleClipPlane(V3d plane_point, V3d plane_norm, Triangle &in_triangle, Triangle &out_triangle1, Triangle &out_triangle2);

    float max(float a, float b);

    float min(float a, float b);
}