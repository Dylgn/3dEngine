#include "MathUtility.hpp"

namespace MathUtil {
    V3d Mat4x4VecMult(const M4x4 &m, const V3d &v) {
        return {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
            v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]
        };
    }

    float DotProd(const V3d &v1, const V3d &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    V3d CrossProd(const V3d &v1, const V3d &v2) {
        return {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        };
    }

    bool SameDirection(const V3d &a, const V3d &b) {
        return a.dot(b) > 0.0f;
    }

    V3d RemoveDirectionFrom(const V3d &vec, const V3d &dir) {
        return vec - (dir * vec.dot(dir));
    }

    V3d AdjustToLength(const V3d &dir, const float &length) {
        return dir * (length / dir.length());
    }

    V3d OrthonormalBasis(V3d &a, V3d &b) {
        // Get new b and c vectors
        V3d c = a.cross(b);
        if (c.length() == 0) return V3d::origin;
        else b = c.cross(a);
        // Normalize vectors
        a = a.normalize();
        b = b.normalize();
        return c.normalize();
    }

    V3d LineIntersectPlane(const V3d &plane_point, V3d &plane_norm, const V3d &line_start, const V3d &line_end, float &t)
    {
        plane_norm = plane_norm.normalize();
        float plane_d = -plane_norm.dot(plane_point);
        float ad = line_start.dot(plane_norm);
        float bd = line_end.dot(plane_norm);
        t = (-plane_d - ad) / (bd - ad);
        V3d line_slope = line_end - line_start;
        V3d line_to_intersect = line_slope * t;
        return line_start + line_to_intersect;
    }

    M4x4 GetIdentityMat() {
        M4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = 1.0f;
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotX(const float &angle) {
        M4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = cosf(angle);
        m.m[1][2] = sinf(angle);
        m.m[2][1] = -sinf(angle);
        m.m[2][2] = cosf(angle);
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotY(const float &angle) {
        M4x4 m;
        m.m[0][0] = cosf(angle);
        m.m[0][2] = sinf(angle);
        m.m[2][0] = -sinf(angle);
        m.m[1][1] = 1.0f;
        m.m[2][2] = cosf(angle);
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetMatRotZ(const float &angle) {
        M4x4 m;
        m.m[0][0] = cosf(angle);
        m.m[0][1] = sinf(angle);
        m.m[1][0] = -sinf(angle);
        m.m[1][1] = cosf(angle);
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        return m;
    }

    M4x4 GetProjMat(const float &fov_deg, const float &aspect_ratio, const float &view_near, const float &view_far) {
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

    M4x4 MatTranslate(const float &x, const float &y, const float &z) {
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

    M4x4 MatPointAt(const V3d &pos, const V3d &target, const V3d &up) {
        V3d new_forward = target - pos;
        new_forward.normalize();

        V3d new_up = up - (new_forward * up.dot(new_forward));
        new_up.normalize();

        V3d new_right = new_up.cross(new_forward);

        // Dimensioning/translation matrix
        M4x4 m;
        m.m[0][0] = new_right.x;
        m.m[0][1] = new_right.y;
        m.m[0][2] = new_right.z;
        m.m[0][3] = 0.0f;
        m.m[1][0] = new_up.x;
        m.m[1][1] = new_up.y;
        m.m[1][2] = new_up.z;
        m.m[1][3] = 0.0f;
        m.m[2][0] = new_forward.x;
        m.m[2][1] = new_forward.y;
        m.m[2][2] = new_forward.z;
        m.m[2][3] = 0.0f;
        m.m[3][0] = pos.x;
        m.m[3][1] = pos.y;
        m.m[3][2] = pos.z;
        m.m[3][3] = 1.0f;
        return m;
    }

    int TriangleClipPlane(const V3d &plane_point, V3d plane_norm, Triangle &in_triangle, Triangle &out_triangle1, Triangle &out_triangle2) {
        plane_norm = plane_norm.normalize();

        // Distance from point to plane (signed)
        auto dist = [&](V3d &p) {
            V3d n = p.normalize();
            return (plane_norm.x * p.x + plane_norm.y * p.y + plane_norm.z * p.z - plane_norm.dot(plane_point));
        };

        // Points inside/outside of plane
        // Inside if distance is positive
        V3d *in_points[3];
        V3d *out_points[3];
        int in_count = 0;
        int out_count = 0;
        // Texture coords
        V2d *in_tex[3];
        V2d *out_tex[3];
        int in_tex_count = 0;
        int out_tex_count = 0;

        for (int i = 0; i < 3; ++i) {
            float d = dist(in_triangle.p[i]);
            if (d >= 0) {
                in_points[in_count++] = &in_triangle.p[i];
                in_tex[in_tex_count++] = &in_triangle.t[i];
            } else {
                out_points[out_count++] = &in_triangle.p[i];
                out_tex[out_tex_count++] = &in_triangle.t[i];
            }
        }

        float t;

        // Return # of triangles *potentially* inside screen
        switch (in_count) {
            case 0:
                return 0; // All points outside screen, do not render
            case 1:
                out_triangle1.col = in_triangle.col;
                out_triangle1.sym = in_triangle.sym;

                out_triangle1.p[0] = *in_points[0]; // 1 point inside
                out_triangle1.t[0] = *in_tex[0];

                // New points at intersection of triangle sides & plane
                out_triangle1.p[1] = LineIntersectPlane(plane_point, plane_norm, *in_points[0], *out_points[0], t);
                out_triangle1.t[1].u = t * (out_tex[0]->u - in_tex[0]->u) + in_tex[0]->u;
                out_triangle1.t[1].v = t * (out_tex[0]->v - in_tex[0]->v) + in_tex[0]->v;
                out_triangle1.t[1].w = t * (out_tex[0]->w - in_tex[0]->w) + in_tex[0]->w;
                out_triangle1.p[2] = LineIntersectPlane(plane_point, plane_norm, *in_points[0], *out_points[1], t);
                out_triangle1.t[2].u = t * (out_tex[1]->u - in_tex[0]->u) + in_tex[0]->u;
                out_triangle1.t[2].v = t * (out_tex[1]->v - in_tex[0]->v) + in_tex[0]->v;
                out_triangle1.t[2].w = t * (out_tex[1]->w - in_tex[0]->w) + in_tex[0]->w;
                
                return 1;
            case 2:
                out_triangle1.col = in_triangle.col;
                out_triangle1.sym = in_triangle.sym;
                out_triangle2.col = in_triangle.col;
                out_triangle2.sym = in_triangle.sym;

                // 1 outside point causes 2 new triangles
                out_triangle1.p[0] = *in_points[0];
                out_triangle1.p[1] = *in_points[1];
                out_triangle1.t[0] = *in_tex[0];
                out_triangle1.t[1] = *in_tex[1];
                out_triangle1.p[2] = LineIntersectPlane(plane_point, plane_norm, *in_points[0], *out_points[0], t);
                out_triangle1.t[2].u = t * (out_tex[0]->u - in_tex[0]->u) + in_tex[0]->u;
                out_triangle1.t[2].v = t * (out_tex[0]->v - in_tex[0]->v) + in_tex[0]->v;
                out_triangle1.t[2].w = t * (out_tex[0]->w - in_tex[0]->w) + in_tex[0]->w;

                out_triangle2.p[0] = *in_points[1];
                out_triangle2.p[1] = out_triangle1.p[2];
                out_triangle2.t[0] = *in_tex[1];
                out_triangle2.t[1] = out_triangle1.t[2];
                out_triangle2.p[2] = LineIntersectPlane(plane_point, plane_norm, *in_points[1], *out_points[0], t);
                out_triangle2.t[2].u = t * (out_tex[0]->u - in_tex[1]->u) + in_tex[1]->u;
                out_triangle2.t[2].v = t * (out_tex[0]->v - in_tex[1]->v) + in_tex[1]->v;
                out_triangle2.t[2].w = t * (out_tex[0]->w - in_tex[1]->w) + in_tex[1]->w;

                return 2; // 2 new triangles formed
            case 3:
                out_triangle1 = in_triangle;
                return 1; // All points inside screen, return single valid triangle
        }

        return 0;
    }

    float clamp(float num, float min, float max)
    {
        return std::min(std::max(num, min), max);
    }

    M4x4 InvertRotTransMat(const M4x4 &m) {
        // Invert rotation/translation matrices
        M4x4 ret;
        ret.m[0][0] = m.m[0][0];
        ret.m[0][1] = m.m[1][0];
        ret.m[0][2] = m.m[2][0];
        ret.m[0][3] = 0.0f;
        ret.m[1][0] = m.m[0][1];
        ret.m[1][1] = m.m[1][1];
        ret.m[1][2] = m.m[2][1];
        ret.m[1][3] = 0.0f;
        ret.m[2][0] = m.m[0][2];
        ret.m[2][1] = m.m[1][2];
        ret.m[2][2] = m.m[2][2];
        ret.m[2][3] = 0.0f;
        ret.m[3][0] = -(m.m[3][0] * ret.m[0][0] + m.m[3][1] * ret.m[1][0] + m.m[3][2] * ret.m[2][0]);
        ret.m[3][1] = -(m.m[3][0] * ret.m[0][1] + m.m[3][1] * ret.m[1][1] + m.m[3][2] * ret.m[2][1]);
        ret.m[3][2] = -(m.m[3][0] * ret.m[0][2] + m.m[3][1] * ret.m[1][2] + m.m[3][2] * ret.m[2][2]);
        ret.m[3][3] = 1.0f;
        return ret;
    }
}