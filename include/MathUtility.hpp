#pragma once

#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"

namespace MathUtil {
    /** Multiplies 4x4 matrix by vector */
    V3d Mat4x4VecMult(const M4x4 &m, const V3d &v);

    float DotProd(const V3d &v1, const V3d &v2);

    V3d CrossProd(const V3d &v1, const V3d &v2);

    bool SameDirection(const V3d &a, const V3d &b);

    /** Creates a vector in the specified direction with the given length */
    V3d AdjustToLength(const V3d &dir, const float &length);

    /** Gets the point where the given line segment intersects the given plane */
    V3d LineIntersectPlane(const V3d &plane_point, V3d &plane_norm, const V3d &line_start, const V3d &line_end, float &t);

    M4x4 GetIdentityMat();

    M4x4 GetMatRotX(const float &angle);

    M4x4 GetMatRotY(const float &angle);

    M4x4 GetMatRotZ(const float &angle);

    M4x4 GetProjMat(const float &fov_deg, const float &aspect_ratio, const float &view_near, const float &view_far);

    M4x4 MatTranslate(const float &x, const float &y, const float &z);
    
    M4x4 MatPointAt(const V3d &pos, const V3d &target, const V3d &up);

    /** Invert rotation/translation matrices */
    M4x4 InvertRotTransMat(const M4x4 &m);

    /** Clip in_triangle against plane, output clipped triangles in out_triangle1 and out_triangle2.
     * @return Number of new triangles clipped
     */
    int TriangleClipPlane(const V3d &plane_point, V3d plane_norm, Triangle &in_triangle, Triangle &out_triangle1, Triangle &out_triangle2);
}