#pragma once

#include <vector>
#include <list>

#include "Mesh.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"
#include "Camera.hpp"

namespace Render {
    /** Get all visible triangles from mesh in view space */
    std::vector<Triangle> GetTriangles(Mesh mesh, Camera &cam, int width, int height);

    /** Clip given triangles against the screen */
    std::list<Triangle> ClipTriangles(std::vector<Triangle> triangles, int width, int height);

    /** Gets all visible triangles from mesh and clips them against the screen */
    std::list<Triangle> GetClippedTriangles(Mesh mesh, Camera &cam, float width, float height);

    /** Sorts triangles back to front by depth (z-value) */
    std::vector<Triangle> SortDepth(std::vector<Triangle> triangles);
}