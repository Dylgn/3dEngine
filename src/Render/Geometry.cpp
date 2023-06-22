#include <algorithm>

#include "Geometry.hpp"
#include "MathUtility.hpp"

std::vector<Triangle> GetTriangles(Mesh mesh, M4x4 mat_proj, Camera &cam, float theta, float yaw, int width, int height);
std::list<Triangle> ClipTriangles(std::vector<Triangle> triangles, int width, int height);


std::list<Triangle> GetClippedTriangles(Mesh mesh, Camera &cam, float width, float height) {
    auto triangles = GetTriangles(mesh, cam, width, height);
    return ClipTriangles(triangles, width, height);
}

std::vector<Triangle> GetTriangles(Mesh mesh, Camera &cam, int width, int height) {
    // Rotation matrices
    M4x4 mat_rot_z, mat_rot_x;
    //theta += 1.0f * fElapsedTime;
    mat_rot_z = MathUtil::GetMatRotZ(cam.theta * 0.5f);
    mat_rot_x = MathUtil::GetMatRotX(cam.theta);

    // Translate matrix
    M4x4 mat_translate = MathUtil::MatTranslate(0.0f, 0.0f, 8.0f);

    // World matrix
    M4x4 mat_world = (mat_rot_z * mat_rot_x) * mat_translate;

    // View matrix
    V3d up = { 0,1,0 };
    V3d target = { 0,0,1 };
    M4x4 mat_cam_rot = MathUtil::GetMatRotY(cam.yaw);
    cam.look_dir = mat_cam_rot * target;
    target = cam.pos + cam.look_dir;
    M4x4 mat_cam = MathUtil::MatPointAt(cam.pos, target, up);
    M4x4 mat_view = MathUtil::InvertRotTransMat(mat_cam);

    std::vector<Triangle> triangles;
    // Get triangles
    for (Triangle t : mesh.triangles) {
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
        V3d cam_ray = t_transform.p[0] - cam.pos;
        // t visible if ray aligned with normal
        if (norm.dotProd(cam_ray) < 0.0f) {
            /*
            // Illumination
            V3d light_dir = { 0.0f, 1.0f, -1.0f };
            light_dir = light_dir.normalize();
            // Alignment of light direction & normal
            float dp = MathUtil::max(0.1f, light_dir.dotProd(norm));

			CHAR_INFO c = GetColour(dp);
			t_transform.col = c.Attributes;
			t_transform.sym = c.Char.UnicodeChar;
            */

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
                t_proj.copyPoints(clipped[i].multPoints(cam.mat_proj));
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
                t_proj.p[0].x *= 0.5f * (float) width;
                t_proj.p[0].y *= 0.5f * (float) height;
                t_proj.p[1].x *= 0.5f * (float) width;
                t_proj.p[1].y *= 0.5f * (float) height;
                t_proj.p[2].x *= 0.5f * (float) width;
                t_proj.p[2].y *= 0.5f * (float) height;

                // Store triangles for int
                triangles.push_back(t_proj);
            }
        }
    }

    return triangles;
}

std::list<Triangle> ClipTriangles(std::vector<Triangle> triangles, int width, int height) {
    std::list<Triangle> output;
    for (Triangle &t : triangles) {
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
                        triangles_to_add = MathUtil::TriangleClipPlane({0.0f, ((float) height) - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, clip, clipped[0], clipped[1]);
                        break;
                    case 2:
                        triangles_to_add = MathUtil::TriangleClipPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, clip, clipped[0], clipped[1]);
                        break;
                    case 3:
                        triangles_to_add = MathUtil::TriangleClipPlane({ ((float) width) - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, clip, clipped[0], clipped[1]);
                        break;
                }

                // Add new triangles to queue
                for (int i = 0; i < triangles_to_add; ++i) t_list.push_back(clipped[i]);
            }
            new_triangles = t_list.size();
        }

        for (Triangle &t2 : t_list) output.push_back(t2);
    }

    return output;
}

std::vector<Triangle> SortDepth(std::vector<Triangle> triangles) {
    // Sort triangles back to front
    sort(triangles.begin(), triangles.end(), [](Triangle &t1, Triangle &t2) {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });
    return triangles;
}