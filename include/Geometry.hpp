#pragma once

#include <vector>
#include <list>

#include "Mesh.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"

std::vector<Triangle> GetTriangles(Mesh mesh, M4x4 mat_proj, V3d &cam, V3d &look_dir, float theta, float yaw, int width, int height);
std::list<Triangle> ClipTriangles(std::vector<Triangle> triangles, int width, int height);
std::list<Triangle> GetClippedTriangles(Mesh mesh, M4x4 mat_proj, V3d &cam, V3d &look_dir, float theta, float yaw, float width, float height);
std::vector<Triangle> SortDepth(std::vector<Triangle> triangles);